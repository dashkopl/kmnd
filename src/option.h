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

#ifndef __option_h
#define __option_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct kmnd_option_s kmnd_option_t;

typedef void (kmnd_option_flag_cb)(kmnd_option_t *option);

typedef int (kmnd_option_parse_cb)(kmnd_option_t *option, const char *string);

#include "core.h"

struct kmnd_option_s {
    kmnd_t core;

    char character;

    kmnd_flags_t flags;

    kmnd_option_flag_cb *flag;
    kmnd_option_parse_cb *parse;

    void *value;

    unsigned char activated;
};

void kmnd_option_free(kmnd_option_t *option);

void kmnd_option_flag(kmnd_t *kmnd, kmnd_option_t *option);

int kmnd_option_activate(kmnd_t *kmnd, kmnd_option_t *option,
                         const char *string);

unsigned char kmnd_option_required(const kmnd_option_t *option);
unsigned char kmnd_option_activated(const kmnd_option_t *option);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __option_h */
