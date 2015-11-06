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

#ifndef __kmnd_error_h
#define __kmnd_error_h

#include <kmnd.h>

typedef struct kmnd_error_s kmnd_error_t;

typedef enum kmnd_error_type_e {
    KMND_ERROR_TYPE_NONE            =  0,
    KMND_ERROR_TYPE_UNKNOWN         = -1,

    /**
     * This exception is thrown when the provided command is unknown.
     */
    KMND_ERROR_TYPE_UNKNOWN_COMMAND = -2,

    /**
     * This exception is thrown when the provided option is unknown.
     */
    KMND_ERROR_TYPE_UNKNOWN_OPTION  = -3,

    /**
     * This exception is thrown when the provided option is treated as if it is
     * a boolean even though it is not.
     */
    KMND_ERROR_TYPE_NOT_A_BOOLEAN   = -4,

    /**
     * This exception is thrown when the provided option value is invalid, e.g.
     * a number is expected and a string is provided.
     */
    KMND_ERROR_TYPE_INVALID_VALUE   = -5,

    /**
     * This exception is thrown when input is provided but the configured
     * validator returned an integer other than 0.
     */
    KMND_ERROR_TYPE_INVALID_INPUT   = -6,

    /**
     * This exception is thrown when no input is provided but input is
     * required (see KMND_FLAGS_REQUIRED).
     */
    KMND_ERROR_TYPE_MISSING_INPUT   = -7,

    /**
     * This exception is thrown when no value is provided for an option that is
     * required (see KMND_FLAGS_REQUIRED).
     */
    KMND_ERROR_TYPE_MISSING_OPTION  = -8
} kmnd_error_type_t;

struct kmnd_error_s {
    kmnd_error_type_t type;
    const char *string;
    const char *value;
};

void kmnd_error_init_unknown_command(kmnd_error_t *error, const char *string);
void kmnd_error_init_unknown_option(kmnd_error_t *error, const char *string);
void kmnd_error_init_not_a_boolean(kmnd_error_t *error, const char *string);
void kmnd_error_init_invalid_value(kmnd_error_t *error, const char *string,
                                   const char *value);
void kmnd_error_init_invalid_input(kmnd_error_t *error, const char *string,
                                   const char *value);
void kmnd_error_init_missing_input(kmnd_error_t *error, const char *string);
void kmnd_error_init_missing_option(kmnd_error_t *error, const char *string);

void kmnd_error_print(kmnd_error_t *error, kmnd_t *kmnd);

#endif /* __kmnd_error_h */
