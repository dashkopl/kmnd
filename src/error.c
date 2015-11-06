/*
 * Copyright (C) 2015-10-10, Tim van Elsloo
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

#include <string.h>

#include "command.h"
#include "error.h"
#include "terminal.h"

static void kmnd_error_init(kmnd_error_t *error) {
    memset(error, 0, sizeof(kmnd_error_t));
}

void kmnd_error_init_unknown_command(kmnd_error_t *error, const char *string) {
    kmnd_error_init(error);

    error->type = KMND_ERROR_TYPE_UNKNOWN_COMMAND;
    error->string = string;
}

void kmnd_error_init_unknown_option(kmnd_error_t *error, const char *string) {
    kmnd_error_init(error);

    error->type = KMND_ERROR_TYPE_UNKNOWN_OPTION;
    error->string = string;
}

void kmnd_error_init_not_a_boolean(kmnd_error_t *error, const char *string) {
    kmnd_error_init(error);

    error->type = KMND_ERROR_TYPE_NOT_A_BOOLEAN;
    error->string = string;
}

void kmnd_error_init_invalid_value(kmnd_error_t *error, const char *string,
                                   const char *value) {
    kmnd_error_init(error);
    error->type = KMND_ERROR_TYPE_INVALID_VALUE;
    error->string = string;
    error->value = value;
}

void kmnd_error_init_invalid_input(kmnd_error_t *error, const char *string,
                                   const char *value) {
    kmnd_error_init(error);
    error->type = KMND_ERROR_TYPE_INVALID_INPUT;
    error->string = string;
    error->value = value;
}

void kmnd_error_init_missing_input(kmnd_error_t *error, const char *string) {
    kmnd_error_init(error);
    error->type = KMND_ERROR_TYPE_MISSING_INPUT;
    error->string = string;
}

void kmnd_error_init_missing_option(kmnd_error_t *error, const char *string) {
    kmnd_error_init(error);
    error->type = KMND_ERROR_TYPE_MISSING_OPTION;
    error->string = string;
}

void kmnd_error_print(kmnd_error_t *error, kmnd_t *kmnd) {
    kmnd_terminal_t *terminal = ((kmnd_command_t *) kmnd)->terminal;

    if (error->type == KMND_ERROR_TYPE_UNKNOWN_OPTION) {
        kmnd_terminal_text(terminal, "[!] Unknown option: `",
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, error->string,
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, "`", KMND_TERMINAL_FOREGROUND_RED);

        kmnd_terminal_text(terminal, "", KMND_TERMINAL_OPTIONS_NONE);
    }else if (error->type == KMND_ERROR_TYPE_UNKNOWN_COMMAND) {
        kmnd_terminal_text(terminal, "[!] Unknown command: `",
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, error->string,
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, "`", KMND_TERMINAL_FOREGROUND_RED);

        kmnd_terminal_text(terminal, "", KMND_TERMINAL_OPTIONS_NONE);
    }else if (error->type == KMND_ERROR_TYPE_NOT_A_BOOLEAN) {
        kmnd_terminal_text(terminal, "[!] Not a boolean option: `--",
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, error->string,
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, "`", KMND_TERMINAL_FOREGROUND_RED);

        kmnd_terminal_text(terminal, "", KMND_TERMINAL_OPTIONS_NONE);
    }else if (error->type == KMND_ERROR_TYPE_INVALID_VALUE) {
        kmnd_terminal_text(terminal, "[!] Invalid value: `",
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, error->value,
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, "`, for option: `--",
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, error->string,
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, "`", KMND_TERMINAL_FOREGROUND_RED);

        kmnd_terminal_text(terminal, "", KMND_TERMINAL_OPTIONS_NONE);
    }else if (error->type == KMND_ERROR_TYPE_INVALID_INPUT) {
        kmnd_terminal_text(terminal, "[!] Invalid value: `",
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, error->value,
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, "`, for input: `",
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, error->string,
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, "`", KMND_TERMINAL_FOREGROUND_RED);

        kmnd_terminal_text(terminal, "", KMND_TERMINAL_OPTIONS_NONE);
    }else if (error->type == KMND_ERROR_TYPE_MISSING_INPUT) {
        kmnd_terminal_text(terminal, "[!] Missing value for input: `",
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, error->string,
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, "`", KMND_TERMINAL_FOREGROUND_RED);

        kmnd_terminal_text(terminal, "", KMND_TERMINAL_OPTIONS_NONE);
    }else if (error->type == KMND_ERROR_TYPE_MISSING_OPTION) {
        kmnd_terminal_text(terminal, "[!] Missing value for option: `--",
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, error->string,
                           KMND_TERMINAL_FOREGROUND_RED |
                           KMND_TERMINAL_OPTIONS_NO_NEWLINE);

        kmnd_terminal_text(terminal, "`", KMND_TERMINAL_FOREGROUND_RED);

        kmnd_terminal_text(terminal, "", KMND_TERMINAL_OPTIONS_NONE);
    }
}
