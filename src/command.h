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

#ifndef __command_h
#define __command_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdlib.h>

typedef struct kmnd_command_s kmnd_command_t;

#include "core.h"
#include "input.h"
#include "option.h"
#include "terminal.h"
#include "usage.h"

struct kmnd_command_s {
    kmnd_t core;

    /**
     * The callback is called when this command was the last (sub)command in
     * in line. Only one callback is called, even though multiple subcommands
     * may be supplied.
     */
    kmnd_run_cb *run;

    kmnd_command_t **commands;
    size_t num_commands;

    kmnd_option_t **options;
    size_t num_options;

    kmnd_input_t **inputs;
    size_t num_inputs;

    kmnd_usage_t *usage;

    kmnd_t *super;

    kmnd_terminal_t *terminal;
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __command_h */
