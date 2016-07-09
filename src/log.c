/*
 * Copyright (c) 2016 Tim van Elsloo
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
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <kmnd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "terminal.h"

void kmnd_logv(kmnd_t *kmnd, const kmnd_log_level_t level, const char *format,
               va_list args) {
    char buffer[256];
    vsprintf(buffer, format, args);

    int fd = (level <= KMND_LOG_LEVEL_INFO ? 1 : 2);

    const char *prefix              = NULL;
    kmnd_terminal_options_t options = KMND_TERMINAL_OPTIONS_NO_NEWLINE;

    switch (level) {
        case KMND_LOG_LEVEL_VERBOSE:
            prefix   = "[verb.] ";
            options |= KMND_TERMINAL_FOREGROUND_CYAN;
            break;
        case KMND_LOG_LEVEL_INFO:
            prefix   = "[info.] ";
            options |= KMND_TERMINAL_FOREGROUND_BLUE;
            break;
        case KMND_LOG_LEVEL_WARNING:
            prefix   = "[warn.] ";
            options |= KMND_TERMINAL_FOREGROUND_YELLOW;
            break;
        case KMND_LOG_LEVEL_ERROR:
            prefix   = "[error] ";
            options |= KMND_TERMINAL_FOREGROUND_MAGENTA;
            break;
        case KMND_LOG_LEVEL_FATAL:
            prefix   = "[fatal] ";
            options |= KMND_TERMINAL_FOREGROUND_RED;
            break;
    }

    kmnd_terminal_t *terminal = kmnd_terminal_new(fd);
    kmnd_terminal_text(terminal, prefix, options);
    kmnd_terminal_text(terminal, buffer, 0);
    kmnd_terminal_free(terminal);
}

void kmnd_log(kmnd_t *kmnd, const kmnd_log_level_t level,
              const char *format, ...) {
    va_list args;
    va_start(args, format);
    kmnd_logv(kmnd, level, format, args);
    va_end(args);
}

void __attribute__((noreturn)) kmnd_abort(kmnd_t *kmnd,
                                          const char *format, ...) {
    va_list args;
    va_start(args, format);
    kmnd_logv(kmnd, KMND_LOG_LEVEL_FATAL, format, args);
    va_end(args);

    abort();
}
