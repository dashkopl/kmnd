/*
 * Copyright (C) 2015-10-08, Tim van Elsloo
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

#ifndef __kmnd_h
#define __kmnd_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

typedef struct kmnd_s kmnd_t;

typedef int (kmnd_run_cb)(kmnd_t *kmnd);

typedef enum kmnd_flags_e {
    KMND_FLAGS_NONE     = 0,
    KMND_FLAGS_REQUIRED = (1 << 0),
} kmnd_flags_t;

/**
 * This function creates a new kmnd. This can be either the root command or a
 * subcommand. The first argument is the name of the kmnd. If it is the root
 * command, you should use the name of the binary or supply NULL to indicate
 * that you want libkmnd to determine the name of the binary.
 * The next arguments are subsections, subcommands, options and inputs.
 * */
kmnd_t *kmnd_new(const char *name, const char *description, kmnd_run_cb *run,
                 kmnd_t *child, ...);

/**
 * This function can be used on kmnds and options to free the memory that is
 * allocated for them.
 */
void kmnd_free(kmnd_t *kmnd);

/**
 * This function can be used to override the default file descriptor (stdout).
 */
int kmnd_fd(kmnd_t *kmnd, const int fd);

/**
 * This function can be used to run the actual kmnd. The arguments you pass to
 * this function may simple be the argc and argv that is passed to your main
 * function.
 * This function also returns an exit code you can either directly return from
 * your main function or use for further processing.
 */
int kmnd_run(kmnd_t *kmnd, const int argc, const char **argv);

/**
 * This function returns a new usage section based on the command and
 * description that you provide. Note that both will automatically be
 * line-wrapped to the size of the output terminal.
 * Note that this will automatically add the function option "-h/--help"
 */
kmnd_t *kmnd_usage_new(const char *command, const char *description);

/** INPUT */

typedef int (kmnd_validator_cb)(kmnd_t *kmnd, const char *string);

/**
 * This function returns a new input that is included in the usage section and
 * will be resolved if present in the provided CLI line. If you don't want to
 * validate input and are willing to accept anything the user enters, pass
 * validator=NULL.
 */
kmnd_t *kmnd_input_new(const char *name, const char *description,
                       const kmnd_flags_t flags, kmnd_validator_cb *validator);

/**
 * This function returns the string value of the input at the given path. Note
 * that inputs are scoped to each command. In other words: you can only provide
 * and access inputs of the selected (sub)command.
 */
const char *kmnd_input_get(kmnd_t *kmnd, const char *path);

/** OPTIONS */

/**
 * This function returns a new boolean option. Valid input values are:
 * - on, 1, true, yes
 * - off, 0, false, no
 * */
kmnd_t *kmnd_boolean_new(const char character, const char *name,
                         const char *description, const kmnd_flags_t flags,
                         const unsigned char value);

/**
 * This function returns the boolean value at the given option path.
 */
unsigned char kmnd_boolean_get(kmnd_t *kmnd, const char *path);

/**
 * This function returns a new string option. Valid input values are:
 * - any string with 1 or more characters (including UTF8 of course)
 */
kmnd_t *kmnd_string_new(const char character, const char *name,
                        const char *description, const kmnd_flags_t flags,
                        const char *value);

/**
 * This function returns the string value at the given option path.
 */
const char *kmnd_string_get(kmnd_t *kmnd, const char *path);

/*
 * These functions create signed integer options. Each _intN creates a N-bit
 * signed integer (i.e. -2^(N-1) <= i < 2^(N-1)).
 */

kmnd_t *kmnd_int8_new(const char character, const char *name,
                      const char *description, const kmnd_flags_t flags,
                      const int8_t value);
kmnd_t *kmnd_int16_new(const char character, const char *name,
                       const char *description, const kmnd_flags_t flags,
                       const int16_t value);
kmnd_t *kmnd_int32_new(const char character, const char *name,
                       const char *description, const kmnd_flags_t flags,
                       const int32_t value);
kmnd_t *kmnd_int64_new(const char character, const char *name,
                       const char *description, const kmnd_flags_t flags,
                       const int64_t value);

/*
 * These functions create unsigned integer options. Each _uintN creates a N-bit
 * unsigned integer (i.e. 0 <= i < 2^N).
 */

kmnd_t *kmnd_uint8_new(const char character, const char *name,
                       const char *description, const kmnd_flags_t flags,
                       const uint8_t value);
kmnd_t *kmnd_uint16_new(const char character, const char *name,
                        const char *description, const kmnd_flags_t flags,
                        const uint16_t value);
kmnd_t *kmnd_uint32_new(const char character, const char *name,
                        const char *description, const kmnd_flags_t flags,
                        const uint32_t value);
kmnd_t *kmnd_uint64_new(const char character, const char *name,
                        const char *description, const kmnd_flags_t flags,
                        const uint64_t value);

/**
 * These functions return the signed integers for the given options.
 */
int8_t kmnd_int8_get(kmnd_t *kmnd, const char *path);
int16_t kmnd_int16_get(kmnd_t *kmnd, const char *path);
int32_t kmnd_int32_get(kmnd_t *kmnd, const char *path);
int64_t kmnd_int64_get(kmnd_t *kmnd, const char *path);

/**
 * These functions return the unsigned integers for the given options.
 */
uint8_t kmnd_uint8_get(kmnd_t *kmnd, const char *path);
uint16_t kmnd_uint16_get(kmnd_t *kmnd, const char *path);
uint32_t kmnd_uint32_get(kmnd_t *kmnd, const char *path);
uint64_t kmnd_uint64_get(kmnd_t *kmnd, const char *path);

/*
 * These functions create IEEE floating point options. Floats have single
 * precision (32 bits) whereas doubles have double precision (64 bits).
 */
kmnd_t *kmnd_float_new(const char character, const char *name,
                       const char *description, const kmnd_flags_t flags,
                       const float value);
kmnd_t *kmnd_double_new(const char character, const char *name,
                        const char *description, const kmnd_flags_t flags,
                        const double value);

/*
 * These functions return the IEEE floating point values for the given options.
 */
float kmnd_float_get(kmnd_t *kmnd, const char *path);
double kmnd_double_get(kmnd_t *kmnd, const char *path);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __kmnd_h */
