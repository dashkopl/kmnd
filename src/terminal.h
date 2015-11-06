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

#ifndef __terminal_h
#define __terminal_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

typedef struct kmnd_terminal_s kmnd_terminal_t;

typedef enum kmnd_terminal_options_e {
    KMND_TERMINAL_OPTIONS_NONE = (1 << 0),

    KMND_TERMINAL_OPTIONS_NO_NEWLINE = (1 << 1),

    KMND_TERMINAL_STYLE_BOLD = (1 << 2),
    KMND_TERMINAL_STYLE_ITALIC = (1 << 3),
    KMND_TERMINAL_STYLE_UNDERLINE = (1 << 4),

    KMND_TERMINAL_EFFECT_BLINK = (1 << 5),
    KMND_TERMINAL_EFFECT_REVERSED = (1 << 6),
    KMND_TERMINAL_EFFECT_CONCEALED = (1 << 7),

    KMND_TERMINAL_FOREGROUND_BLACK = (1 << 8),
    KMND_TERMINAL_FOREGROUND_RED = (1 << 9),
    KMND_TERMINAL_FOREGROUND_GREEN = (1 << 10),
    KMND_TERMINAL_FOREGROUND_YELLOW = (1 << 11),
    KMND_TERMINAL_FOREGROUND_BLUE = (1 << 12),
    KMND_TERMINAL_FOREGROUND_MAGENTA = (1 << 13),
    KMND_TERMINAL_FOREGROUND_CYAN = (1 << 14),
    KMND_TERMINAL_FOREGROUND_WHITE = (1 << 15),

    KMND_TERMINAL_BACKGROUND_BLACK = (1 << 16),
    KMND_TERMINAL_BACKGROUND_RED = (1 << 17),
    KMND_TERMINAL_BACKGROUND_GREEN = (1 << 18),
    KMND_TERMINAL_BACKGROUND_YELLOW = (1 << 19),
    KMND_TERMINAL_BACKGROUND_BLUE = (1 << 20),
    KMND_TERMINAL_BACKGROUND_MAGENTA = (1 << 21),
    KMND_TERMINAL_BACKGROUND_CYAN = (1 << 22),
    KMND_TERMINAL_BACKGROUND_WHITE = (1 << 23),
} kmnd_terminal_options_t;

struct kmnd_terminal_s {
    int fd;
    unsigned char is_default;

    const char *indent;
    kmnd_terminal_options_t indent_options;

    uint16_t line_chars;
};

/**
 * This function returns a new terminal instance that outputs to the given FILE.
 * It will also automatically determine support for custom text styles,
 * background colors and foreground colors and the current number of columns.
 */
kmnd_terminal_t *kmnd_terminal_new(const int fd);

/**
 * This function returns a singleton of the default terminal for stdout. Note
 * that calling this function twice will return the same pointer. Also note that
 * the terminal returned by this function may not be freed.
 */
kmnd_terminal_t *kmnd_terminal_default(void);

/**
 * This function returns a new terminal for stdout.
 */
kmnd_terminal_t *kmnd_terminal_new_default(void);

/**
 * This function frees up memory and other resources used by this terminal.
 */
void kmnd_terminal_free(kmnd_terminal_t *terminal);

/**
 * This function returns a boolean that indicates whether this terminal
 * supports formatting (such as colors, font styles, etc.). Normally, this
 * function will return true. When the output is piped to a file etc. however
 * it returns false.
 */
unsigned char
kmnd_terminal_supports_formatting(const kmnd_terminal_t *terminal);

/**
 * This function writes the given string to the terminal with the given text
 * formatting options.
 */
void kmnd_terminal_text(kmnd_terminal_t *terminal, const char *text,
                        const kmnd_terminal_options_t options);

/**
 * This function is very similar to _text except that it does additional
 * formatting.
 * `string` -> string in blue
 * ``string`` -> `string` in blue
 * *string* or _string_ -> string in italic
 * **string** or __string__ -> string in bold
 */
void kmnd_terminal_format(kmnd_terminal_t *terminal, const char *text,
                          const kmnd_terminal_options_t options);

/**
 * This function sets the indent of the terminal. Use indent=NULL to turn off
 * all indentation (note that line-wrapping is still active though).
 */
void kmnd_terminal_indent(kmnd_terminal_t *terminal, const char *indent,
                          const kmnd_terminal_options_t options);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __terminal_h */
