/*
 * Copyright (C) 2015-10-09, Tim van Elsloo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "error.h"

void kmnd_free(kmnd_t *kmnd) {
    if (kmnd->type == KMND_TYPE_OPTION)
        kmnd_option_free((kmnd_option_t *) kmnd);
    else if (kmnd->type == KMND_TYPE_USAGE)
        kmnd_usage_free((kmnd_usage_t *) kmnd);
    else if (kmnd->type == KMND_TYPE_INPUT)
        kmnd_input_free((kmnd_input_t *) kmnd);
    else if (kmnd->type == KMND_TYPE_COMMAND) {
        kmnd_command_t *command = (kmnd_command_t *) kmnd;

        size_t i;
        for (i = 0; i < command->num_commands; i ++)
            kmnd_free((kmnd_t *) command->commands[i]);

        for (i = 0; i < command->num_inputs; i ++)
            kmnd_free((kmnd_t *) command->inputs[i]);

        for (i = 0; i < command->num_options; i ++)
            kmnd_option_free(command->options[i]);

        free(command->commands);
        free(command->inputs);
        free(command->options);

        if (command->usage != NULL)
            kmnd_free((kmnd_t *) command->usage);

        kmnd_terminal_free(command->terminal);

        free(command);
    }
}

static int kmnd_process_option(kmnd_t *kmnd, const char *string) {
    assert(strlen(string) > 0);

    kmnd_command_t *command = (kmnd_command_t *) kmnd;

    /* Note that this is perfectly safe because if arg = "-" then arg[1]
     * will be \0. */
    char twice = (string[1] == '-');

    size_t setter;
    for (setter = 0; setter < strlen(string); setter ++) {
        if (string[setter] == '=')
            break;
    }

    size_t j, k = 1;
    while (command != NULL) {
        for (j = 0; j < command->num_options; j ++) {
            kmnd_option_t *option = (kmnd_option_t *) command->options[j];

            if (twice &&
                strncmp(option->core.name, string + 2, setter - 2) != 0)
                continue;

            if (twice == 0 && option->character != string[k])
                continue;

            if ((twice && setter == strlen(string)) ||
                (!twice && string[k + 1] != '='))
                kmnd_option_flag(kmnd, option);
            else {
                return kmnd_option_activate(kmnd, option, string + setter + 1);
            }

            if (twice == 1)
                return 0;
            else if (++ k == strlen(string))
                return 0;
            else {
                j = (size_t) -1;
                command = (kmnd_command_t *) kmnd;
            }
        }

        command = (kmnd_command_t *) command->super;
    }

    command = (kmnd_command_t *) kmnd;

    kmnd_error_t error;

    if (twice)
        kmnd_error_init_unknown_option(&error, string);
    else {
        char option[3] = { '-', '\0', '\0' };
        option[1] = string[k];
        kmnd_error_init_unknown_option(&error, option);
    }

    kmnd_error_print(&error, kmnd);

    if (command->usage != NULL)
        kmnd_usage_print(command->usage, command);

    return -1;
}

int kmnd_run(kmnd_t *kmnd, const int argc, const char **argv) {
    kmnd_command_t *command = (kmnd_command_t *) kmnd;

    /** Three stages: commands, options and inputs. */
    size_t stage = 0, input = 0;

    int i;
    for (i = 1; i < argc; i ++) {
        const char *arg = argv[i];

        /* Skip empty arguments. */
        if (strlen(arg) == 0)
            continue;

        /**
         * Check if this argument is an option or a command or an input. Options
         * start with one or two dashes.
         */
        if (arg[0] == '-') {
            stage = 1;

            if (command->usage != NULL &&
                (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0)) {
                kmnd_usage_print(command->usage, command);
                return 0;
            }

            int res = kmnd_process_option(kmnd, arg);

            if (res != 0)
                return res;
        }else {
            if (stage == 1)
                stage = 2;

            if (stage == 0) {
                size_t j;
                for (j = 0; j < command->num_commands; j++) {
                    if (strcmp(command->commands[j]->core.name, arg) != 0)
                        continue;

                    return kmnd_run((kmnd_t *) command->commands[j], argc - i,
                                    argv + i);
                }
            }

            unsigned char is_input = 0;

            for (; input < command->num_inputs; input ++) {
                kmnd_input_t *inp = command->inputs[input];

                int res = kmnd_input_activate(inp, kmnd, arg);

                if (res == 0) {
                    is_input = 1;

                    const kmnd_flags_t flag = KMND_FLAGS_MULTIPLE;
                    if ((inp->flags & flag) != flag)
                        input ++;

                    break;
                }else if (res == -1 &&
                          kmnd_input_required(command->inputs[input]) == 1) {
                    kmnd_error_t error;
                    kmnd_error_init_invalid_input(&error, inp->core.name, arg);
                    kmnd_error_print(&error, kmnd);

                    if (command->usage != NULL)
                        kmnd_usage_print(command->usage, command);

                    return -1;
                }
            }

            if (is_input)
                continue;

            kmnd_error_t error;
            kmnd_error_init_unknown_command(&error, arg);
            kmnd_error_print(&error, kmnd);

            if (command->usage != NULL)
                kmnd_usage_print(command->usage, command);

            return -1;
        }
    }

    size_t j;

    /**
     * We have to make sure that all options that are required, are also
     * activated.
     */
    for (j = 0; j < command->num_options; j ++) {
        if (kmnd_option_required(command->options[j]) == 0 ||
            kmnd_option_activated(command->options[j]) == 1)
            continue;

        kmnd_error_t error;
        kmnd_error_init_missing_option(&error, command->options[j]->core.name);
        kmnd_error_print(&error, kmnd);

        if (command->usage != NULL)
            kmnd_usage_print(command->usage, command);

        return -1;
    }

    /**
     * We have to make sure that all inputs that are required, are also
     * activated.
     */
    for (j = 0; j < command->num_inputs; j ++) {
        if (kmnd_input_required(command->inputs[j])  == 0 ||
            kmnd_input_activated(command->inputs[j]) == 1)
            continue;

        kmnd_error_t error;
        kmnd_error_init_missing_input(&error, command->inputs[j]->core.name);
        kmnd_error_print(&error, kmnd);

        if (command->usage != NULL)
            kmnd_usage_print(command->usage, command);

        return -1;
    }

    if (command->run)
        command->run(kmnd);
    else if (command->usage != NULL) {
        kmnd_usage_print(command->usage, command);
    }

    return 0;
}
