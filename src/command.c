/*
 * Copyright (C) 2015-11-02, Tim van Elsloo
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
#include <string.h>

#include "command.h"

kmnd_t *kmnd_new(const char *name, const char *description, kmnd_run_cb *run,
                 kmnd_t *child, ...) {
    kmnd_command_t *kmnd = malloc(sizeof(kmnd_command_t));

    if (kmnd == NULL)
        return NULL;

    memset(kmnd, 0, sizeof(kmnd_command_t));

    kmnd->core.name = name;
    kmnd->core.description = description;
    kmnd->run = run;

    kmnd->terminal = kmnd_terminal_new_default();

    /*
     * We will iterate over the arguments twice. Once to count the number of
     * subcommands, options and inputs. One more time to actually store them.
     * Therefore, we duplicate the va list.
     */
    va_list arguments = { 0 }, copy = { 0 };
    va_start(arguments, child);
    va_copy(copy, arguments);

    /*
     * We start by iterating over all arguments (except for the first, name) in
     * order to count the types and the counts for each type.
     */
    kmnd->num_commands = 0;
    kmnd->num_options = 0;
    kmnd->num_inputs = 0;

    kmnd_t *child_iterator = child;

    while (child_iterator != NULL) {
        if (child_iterator->type == KMND_TYPE_COMMAND)
            kmnd->num_commands ++;
        else if (child_iterator->type == KMND_TYPE_OPTION)
            kmnd->num_options ++;
        else if (child_iterator->type == KMND_TYPE_INPUT)
            kmnd->num_inputs ++;

        child_iterator = va_arg(arguments, kmnd_t *);
    }

    va_end(arguments);

    /* We then allocate memory for all subcommands, options and inputs. */
    kmnd->commands = calloc(kmnd->num_commands, sizeof(kmnd_command_t *));
    kmnd->options = calloc(kmnd->num_options, sizeof(kmnd_option_t *));
    kmnd->inputs = calloc(kmnd->num_inputs, sizeof(kmnd_input_t *));

    child_iterator = child;
    size_t i = 0, j = 0, k = 0;

    while (child_iterator != NULL) {
        if (child_iterator->type == KMND_TYPE_COMMAND) {
            kmnd->commands[i++] = (kmnd_command_t *) child_iterator;

            kmnd_command_t *command = (kmnd_command_t *) child_iterator;
            assert(command->super == NULL);
            command->super = (kmnd_t *) kmnd;
        }else if (child_iterator->type == KMND_TYPE_OPTION)
            kmnd->options[j ++] = (kmnd_option_t *) child_iterator;
        else if (child_iterator->type == KMND_TYPE_INPUT)
            kmnd->inputs[k ++] = (kmnd_input_t *) child_iterator;
        else if (child_iterator->type == KMND_TYPE_USAGE)
            kmnd->usage = (kmnd_usage_t *) child_iterator;

        child_iterator = va_arg(copy, kmnd_t *);
    }

    assert(i == kmnd->num_commands);
    assert(j == kmnd->num_options);
    assert(k == kmnd->num_inputs);

    va_end(copy);

    return (kmnd_t *) kmnd;
}

int kmnd_fd(kmnd_t *kmnd, const int fd) {
    kmnd_terminal_t *terminal = kmnd_terminal_new(fd);

    if (terminal == NULL)
        return -1;

    kmnd_command_t *command = (kmnd_command_t *) kmnd;

    kmnd_terminal_free(command->terminal);

    command->terminal = terminal;

    return 0;
}
