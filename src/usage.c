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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "option.h"
#include "terminal.h"
#include "usage.h"

kmnd_t *kmnd_usage_new(const char *command, const char *description) {
    kmnd_usage_t *usage = malloc(sizeof(kmnd_usage_t));

    if (usage == NULL)
        return NULL;

    memset(usage, 0, sizeof(kmnd_usage_t));

    usage->core.type = KMND_TYPE_USAGE;
    usage->command = command;
    usage->description = description;

    return (kmnd_t *) usage;
}

void kmnd_usage_free(kmnd_usage_t *usage) {
    memset(usage, 0, sizeof(kmnd_usage_t));
    free(usage);
}

#define KMND_USAGE(x) ((kmnd_usage_t *) x)

void kmnd_usage_print(kmnd_usage_t *usage, kmnd_command_t *command) {
    kmnd_terminal_text(command->terminal, "Usage:\n",
                       KMND_TERMINAL_STYLE_UNDERLINE);

    kmnd_terminal_text(command->terminal, "    $ ",
                       KMND_TERMINAL_OPTIONS_NO_NEWLINE);
    kmnd_terminal_indent(command->terminal, "    $ ",
                         KMND_TERMINAL_OPTIONS_NONE);
    kmnd_terminal_format(command->terminal, KMND_USAGE(usage)->command,
                         KMND_TERMINAL_FOREGROUND_GREEN);
    kmnd_terminal_indent(command->terminal, NULL,
                         KMND_TERMINAL_OPTIONS_NONE);

    kmnd_terminal_text(command->terminal, "\n      ",
                       KMND_TERMINAL_OPTIONS_NO_NEWLINE);

    kmnd_terminal_indent(command->terminal, "      ",
                         KMND_TERMINAL_OPTIONS_NONE);
    kmnd_terminal_format(command->terminal, KMND_USAGE(usage)->description,
                         KMND_TERMINAL_OPTIONS_NONE);
    kmnd_terminal_indent(command->terminal, NULL,
                         KMND_TERMINAL_OPTIONS_NONE);

    size_t indent_length = strlen("    -h/--help  ");

    size_t i;
    for (i = 0; i < command->num_commands; i ++) {
        const size_t current = 6 + strlen(command->commands[i]->core.name) + 2;

        if (current > indent_length)
            indent_length = current;
    }

    for (i = 0; i < command->num_options; i ++) {
        kmnd_option_t *option = command->options[i];

        size_t current = 6 + strlen(option->core.name) + 2;

        if (option->character != 0)
            current += 3;

        if (current > indent_length)
            indent_length = current;
    }

    char indent[indent_length + 1];
    indent[indent_length] = '\0';
    for (i = 0; i < indent_length; i ++)
        indent[i] = ' ';

    if (command->num_commands > 0) {
        kmnd_terminal_text(command->terminal, "\nCommands:\n",
                           KMND_TERMINAL_STYLE_UNDERLINE);

        kmnd_terminal_indent(command->terminal, indent,
                             KMND_TERMINAL_OPTIONS_NONE);

        size_t j, k;
        for (j = 0; j < command->num_commands; j ++) {
            kmnd_command_t *subcommand = command->commands[j];

            kmnd_terminal_text(command->terminal, "    + ",
                               KMND_TERMINAL_FOREGROUND_GREEN |
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);
            kmnd_terminal_text(command->terminal, subcommand->core.name,
                               KMND_TERMINAL_FOREGROUND_GREEN |
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);

            const size_t padding_length = indent_length - 6 -
                                          strlen(subcommand->core.name);
            char padding[padding_length + 1];
            for (k = 0; k < padding_length; k ++)
                padding[k] = ' ';
            padding[k] = '\0';

            kmnd_terminal_text(command->terminal, padding,
                                 KMND_TERMINAL_OPTIONS_NO_NEWLINE);
            kmnd_terminal_format(command->terminal,
                                 subcommand->core.description,
                                 KMND_TERMINAL_OPTIONS_NONE);
        }

        kmnd_terminal_indent(command->terminal, NULL,
                             KMND_TERMINAL_OPTIONS_NONE);
    }

    if (command->num_inputs > 0) {
        kmnd_terminal_text(command->terminal, "\nInput:\n",
                           KMND_TERMINAL_STYLE_UNDERLINE);

        kmnd_terminal_indent(command->terminal, indent,
                             KMND_TERMINAL_OPTIONS_NONE);

        size_t j, k;
        for (j = 0; j < command->num_inputs; j ++) {
            kmnd_input_t *input = command->inputs[j];

            kmnd_terminal_text(command->terminal, "      ",
                               KMND_TERMINAL_FOREGROUND_GREEN |
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);
            kmnd_terminal_text(command->terminal, input->core.name,
                               KMND_TERMINAL_FOREGROUND_GREEN |
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);

            const size_t padding_length = indent_length - 6 -
                                          strlen(input->core.name);
            char padding[padding_length + 1];
            for (k = 0; k < padding_length; k ++)
                padding[k] = ' ';
            padding[k] = '\0';

            kmnd_terminal_text(command->terminal, padding,
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);
            kmnd_terminal_format(command->terminal,
                                 input->core.description,
                                 KMND_TERMINAL_OPTIONS_NONE);
        }

        kmnd_terminal_indent(command->terminal, NULL,
                             KMND_TERMINAL_OPTIONS_NONE);
    }

    kmnd_terminal_text(command->terminal, "\nOptions:\n",
                       KMND_TERMINAL_STYLE_UNDERLINE);

    kmnd_terminal_indent(command->terminal, indent,
                         KMND_TERMINAL_OPTIONS_NONE);

    size_t j, k;
    for (j = 0; j < command->num_options; j ++) {
        kmnd_option_t *option = (kmnd_option_t *) command->options[j];

        if (option->character) {
            kmnd_terminal_text(command->terminal, "    -",
                               KMND_TERMINAL_FOREGROUND_BLUE |
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);
            char key[2] = { option->character, 0 };
            kmnd_terminal_text(command->terminal, key,
                               KMND_TERMINAL_FOREGROUND_BLUE |
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);
            kmnd_terminal_text(command->terminal, "/--",
                               KMND_TERMINAL_FOREGROUND_BLUE |
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);

            kmnd_terminal_text(command->terminal, option->core.name,
                               KMND_TERMINAL_FOREGROUND_BLUE |
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);
        }else {
            kmnd_terminal_text(command->terminal, "    --",
                               KMND_TERMINAL_FOREGROUND_BLUE |
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);
            kmnd_terminal_text(command->terminal, option->core.name,
                               KMND_TERMINAL_FOREGROUND_BLUE |
                               KMND_TERMINAL_OPTIONS_NO_NEWLINE);
        }

        size_t padding_length = indent_length - 6 -
                                strlen(option->core.name);

        if (option->character != 0)
            padding_length -= 3;

        char padding[padding_length + 1];
        for (k = 0; k < padding_length; k ++)
            padding[k] = ' ';
        padding[k] = '\0';

        kmnd_terminal_text(command->terminal, padding,
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);
        kmnd_terminal_format(command->terminal,
                             option->core.description,
                             KMND_TERMINAL_OPTIONS_NONE);
    }

    size_t padding_length = indent_length - 6 - 4 - 2 - 3;

    char padding[padding_length + 1];
    for (k = 0; k < padding_length; k ++)
        padding[k] = ' ';
    padding[k] = '\0';

    kmnd_terminal_text(command->terminal, "    -h/--help  ",
                       KMND_TERMINAL_FOREGROUND_BLUE |
                       KMND_TERMINAL_OPTIONS_NO_NEWLINE);
    kmnd_terminal_text(command->terminal, padding,
                       KMND_TERMINAL_OPTIONS_NO_NEWLINE);
    kmnd_terminal_text(command->terminal, "Show help banner of specified "
                       "command", KMND_TERMINAL_OPTIONS_NONE);

    kmnd_terminal_indent(command->terminal, NULL,
                         KMND_TERMINAL_OPTIONS_NONE);
}
