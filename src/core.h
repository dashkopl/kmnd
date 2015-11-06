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

#ifndef __core_h
#define __core_h

#include <kmnd.h>

typedef enum kmnd_type_e {
    KMND_TYPE_COMMAND = 0,
    KMND_TYPE_OPTION  = 1,
    KMND_TYPE_USAGE   = 2,
    KMND_TYPE_INPUT   = 3
} kmnd_type_t;

struct kmnd_s {
    kmnd_type_t type;

    /* This is the name of the (sub)command or option. */
    const char *name;

    /* This is the description of the (sub)command or option, used in the usage
     * text. */
    const char *description;
};

#endif /* __core_h */
