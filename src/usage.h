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

#ifndef __usage_h
#define __usage_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct kmnd_usage_s kmnd_usage_t;

#include <kmnd.h>

#include "command.h"
#include "core.h"

struct kmnd_usage_s {
    kmnd_t core;

    const char *command;
    const char *description;
};

void kmnd_usage_free(kmnd_usage_t *usage);

void kmnd_usage_print(kmnd_usage_t *usage, kmnd_command_t *command);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __usage_h */
