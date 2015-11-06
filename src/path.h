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

#ifndef __path_h
#define __path_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "command.h"
#include "input.h"
#include "option.h"

kmnd_t *kmnd_path(kmnd_t *kmnd, const char *path);

kmnd_command_t *kmnd_command_path(kmnd_t *kmnd, const char *path);
kmnd_option_t *kmnd_option_path(kmnd_t *kmnd, const char *path);
kmnd_input_t *kmnd_input_path(kmnd_t *kmnd, const char *path);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __path_h */
