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

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/ioctl.h>
#elif __APPLE__
#include <sys/ttycom.h>
#include <sys/ioctl.h>
#endif

#include "terminal.h"

kmnd_terminal_t *kmnd_terminal_new(const int fd) {
    /* Make sure we stop immediately when passed an invalid file descriptor. */
    if (fcntl(fd, F_GETFD) == -1)
        return NULL;

    kmnd_terminal_t *terminal = malloc(sizeof(kmnd_terminal_t));

    if (terminal == NULL)
        return NULL;

    memset(terminal, 0, sizeof(kmnd_terminal_t));

    terminal->fd = fd;

    return terminal;
}

kmnd_terminal_t *kmnd_terminal_new_default(void) {
    return kmnd_terminal_new(STDOUT_FILENO);
}

void kmnd_terminal_free(kmnd_terminal_t *terminal) {
    if (terminal->is_default)
        return;

    memset(terminal, 0, sizeof(kmnd_terminal_t));
    free(terminal);
}

kmnd_terminal_t *kmnd_terminal_default(void) {
    static kmnd_terminal_t *terminal = NULL;

    if (terminal == NULL) {
        terminal = kmnd_terminal_new(STDOUT_FILENO);
        terminal->is_default = 1;
    }

    return terminal;
}

unsigned char
kmnd_terminal_supports_formatting(const kmnd_terminal_t *terminal) {
    return (unsigned char) (isatty(terminal->fd) == 1);
}

static uint8_t kmnd_terminal_pop_count(kmnd_terminal_options_t options) {
    uint8_t count = 0;

    while (options > 0) {
        if ((options & 1) == 1)
            count += 1;

        options >>= 1;
    }

    return count;
}

static const struct {
    kmnd_terminal_options_t option;
    const char *code;
} kmnd_terminal_codes[] = {
    { KMND_TERMINAL_STYLE_BOLD,         "1;" },
    { KMND_TERMINAL_STYLE_ITALIC,       "3;" },
    { KMND_TERMINAL_STYLE_UNDERLINE,    "4;" },

    { KMND_TERMINAL_EFFECT_BLINK,       "5;" },
    { KMND_TERMINAL_EFFECT_REVERSED,    "7;" },
    { KMND_TERMINAL_EFFECT_CONCEALED,   "8;" },

    { KMND_TERMINAL_FOREGROUND_BLACK,   "30;" },
    { KMND_TERMINAL_FOREGROUND_RED,     "31;" },
    { KMND_TERMINAL_FOREGROUND_GREEN,   "32;" },
    { KMND_TERMINAL_FOREGROUND_YELLOW,  "33;" },
    { KMND_TERMINAL_FOREGROUND_BLUE,    "34;" },
    { KMND_TERMINAL_FOREGROUND_MAGENTA, "35;" },
    { KMND_TERMINAL_FOREGROUND_CYAN,    "36;" },
    { KMND_TERMINAL_FOREGROUND_WHITE,   "37;" },

    { KMND_TERMINAL_BACKGROUND_BLACK,   "40;" },
    { KMND_TERMINAL_BACKGROUND_RED,     "41;" },
    { KMND_TERMINAL_BACKGROUND_GREEN,   "42;" },
    { KMND_TERMINAL_BACKGROUND_YELLOW,  "43;" },
    { KMND_TERMINAL_BACKGROUND_BLUE,    "44;" },
    { KMND_TERMINAL_BACKGROUND_MAGENTA, "45;" },
    { KMND_TERMINAL_BACKGROUND_CYAN,    "46;" },
    { KMND_TERMINAL_BACKGROUND_WHITE,   "47;" },
};

static void kmnd_terminal_start_options(kmnd_terminal_t *terminal,
                                        const kmnd_terminal_options_t options) {
    if (kmnd_terminal_supports_formatting(terminal) == 0)
        return;

    const uint8_t count = kmnd_terminal_pop_count(options);

    if (count == 0)
        return;

    char string[2 + count * 2];
    strcpy(string, "\x1B[");

    const size_t num_codes = sizeof(kmnd_terminal_codes) /
                             sizeof(*kmnd_terminal_codes);

    size_t i;
    for (i = 0; i < num_codes; i ++) {
        if ((options & kmnd_terminal_codes[i].option) ==
            kmnd_terminal_codes[i].option) {
            strcat(string, kmnd_terminal_codes[i].code);
        }
    }

    /* Replace the last semicolon with the `m`. */
    string[strlen(string) - 1] = 'm';

    write(terminal->fd, string, strlen(string));
}

static void kmnd_terminal_end_options(kmnd_terminal_t *terminal) {
    if (kmnd_terminal_supports_formatting(terminal) == 0)
        return;

    write(terminal->fd, "\x1B[0m", 4);
}

static void kmnd_terminal_size(kmnd_terminal_t *terminal, uint16_t *num_columns,
                               uint16_t *num_rows) {
    if (num_columns == NULL && num_rows == NULL)
        return;

    struct winsize size = {0};
    ioctl(terminal-> fd, TIOCGWINSZ, &size);

    if (num_columns != NULL)
        *num_columns = size.ws_col;

    if (num_rows != NULL)
        *num_rows = size.ws_row;
}

void kmnd_terminal_text(kmnd_terminal_t *terminal, const char *text,
                        const kmnd_terminal_options_t options) {
    kmnd_terminal_start_options(terminal, options);

    uint16_t max_chars;
    kmnd_terminal_size(terminal, &max_chars, NULL);

    if (max_chars == 0)
        max_chars = 80;

    size_t i;
    for (i = 0; i < strlen(text); i ++) {
        if (text[i] == '\n' ||
            terminal->line_chars == max_chars) {
            terminal->line_chars = 0;

            write(terminal->fd, "\n", 1);

            if (terminal->indent != NULL) {
                kmnd_terminal_end_options(terminal);
                kmnd_terminal_start_options(terminal, terminal->indent_options);

                write(terminal->fd, terminal->indent, strlen(terminal->indent));

                terminal->line_chars += strlen(terminal->indent);

                kmnd_terminal_end_options(terminal);
                kmnd_terminal_start_options(terminal, options);
            }

            if (text[i] == '\n')
                continue;
        }

        write(terminal->fd, text + i, 1);

        terminal->line_chars ++;
    }

    kmnd_terminal_end_options(terminal);

    if ((options & KMND_TERMINAL_OPTIONS_NO_NEWLINE) == 0) {
        write(terminal->fd, "\n", 1);
        terminal->line_chars = 0;
    }
}

#define CODE_OPTIONS KMND_TERMINAL_FOREGROUND_MAGENTA

void kmnd_terminal_format(kmnd_terminal_t *terminal, const char *text,
                          const kmnd_terminal_options_t options) {
    char buffer[strlen(text) + 1];
    buffer[0] = '\0';

    kmnd_terminal_options_t extra = KMND_TERMINAL_OPTIONS_NO_NEWLINE;

    size_t i;
    for (i = 0; i < strlen(text); i ++) {
        if (text[i] == '`') {
            /* Check if double `. */
            const char twice = (i + 1 < strlen(text) &&
                                text[i + 1] == text[i]);

            if (twice)
                i ++;

            if (twice && (extra & CODE_OPTIONS))
                strncat(buffer, text + i, 1);

            kmnd_terminal_text(terminal, buffer, options | extra);
            buffer[0] = '\0';

            if (extra & CODE_OPTIONS)
                extra &= ~CODE_OPTIONS;
            else
                extra |= CODE_OPTIONS;

            if (twice && (extra & CODE_OPTIONS))
                strncat(buffer, text + i, 1);

            continue;
        }else if (text[i] == '*' || text[i] == '_') {
            /* Check if twice. */
            const char twice = (i + 1 < strlen(text) &&
                                text[i + 1] == text[i]);

            kmnd_terminal_text(terminal, buffer, options | extra);
            buffer[0] = '\0';

            if (twice) {
                if (extra & KMND_TERMINAL_STYLE_BOLD)
                    extra &= ~KMND_TERMINAL_STYLE_BOLD;
                else
                    extra |= KMND_TERMINAL_STYLE_BOLD;
            }else {
                if (extra & KMND_TERMINAL_STYLE_UNDERLINE)
                    extra &= ~KMND_TERMINAL_STYLE_UNDERLINE;
                else
                    extra |= KMND_TERMINAL_STYLE_UNDERLINE;
            }

            continue;
        }

        strncat(buffer, text + i, 1);
    }

    kmnd_terminal_text(terminal, buffer, options | extra);

    if (!(options & KMND_TERMINAL_OPTIONS_NO_NEWLINE))
        kmnd_terminal_text(terminal, "", options);
}

void kmnd_terminal_indent(kmnd_terminal_t *terminal, const char *indent,
                          const kmnd_terminal_options_t options) {
    terminal->indent = indent;
    terminal->indent_options = options;
}
