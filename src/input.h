/*
 * Copyright (C) 2015-11-04, Tim van Elsloo
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

#ifndef __kmnd_input_h
#define __kmnd_input_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <kmnd.h>

typedef struct kmnd_input_s kmnd_input_t;

#include "core.h"

struct kmnd_input_s {
    struct kmnd_s core;

    kmnd_flags_t flags;
    kmnd_validator_cb *validator;

    char *value;
};

int kmnd_input_activate(kmnd_input_t *input, kmnd_t *kmnd, const char *string);

unsigned char kmnd_input_required(const kmnd_input_t *input);

unsigned char kmnd_input_activated(const kmnd_input_t *input);

void kmnd_input_free(kmnd_input_t *input);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __kmnd_input_h */
