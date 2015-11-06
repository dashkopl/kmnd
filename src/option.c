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

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "error.h"
#include "option.h"
#include "path.h"
#include "usage.h"

static kmnd_option_t *kmnd_option_new(const char character, const char *name,
                                      const char *description,
                                      const kmnd_flags_t flags) {
    kmnd_option_t *option = malloc(sizeof(kmnd_option_t));

    if (option == NULL)
        return NULL;

    memset(option, 0, sizeof(kmnd_option_t));

    option->core.type = KMND_TYPE_OPTION;
    option->character = character;
    option->core.name = name;
    option->core.description = description;
    option->flags = flags;

    return option;
}

void kmnd_option_free(kmnd_option_t *option) {
    free(option->value);

    memset(option, 0, sizeof(kmnd_option_t));
    free(option);
}

void kmnd_option_flag(kmnd_t *kmnd, kmnd_option_t *option) {
    if (option->flag == NULL) {
        kmnd_error_t error;
        kmnd_error_init_not_a_boolean(&error, option->core.name);
        kmnd_error_print(&error, kmnd);
        exit(1);
    }else
        option->flag(option);
}

/** -- boolean -- */

static void kmnd_boolean_flag(kmnd_option_t *option) {
    ((unsigned char *) option->value)[0] = 1;
}

static int kmnd_boolean_parse(kmnd_option_t *option, const char *string) {
    if (strcmp(string, "1")    == 0 || strcmp(string, "on")  == 0 ||
        strcmp(string, "true") == 0 || strcmp(string, "yes") == 0) {
        ((unsigned char *) option->value)[0] = 1;
        return 0;
    }else if (strcmp(string, "0")     == 0 || strcmp(string, "off") == 0 ||
              strcmp(string, "false") == 0 || strcmp(string, "no")  == 0) {
        ((unsigned char *) option->value)[0] = 0;
        return 0;
    }

    return -1;
}

kmnd_t *kmnd_boolean_new(const char character, const char *name,
                         const char *description, const kmnd_flags_t flags,
                         const unsigned char value) {
    kmnd_option_t *option;
    option = kmnd_option_new(character, name, description, flags);

    if (option == NULL)
        return NULL;

    option->value = malloc(1);

    if (option->value == NULL)
        return NULL;

    ((unsigned char *) option->value)[0] = value;

    option->flag = kmnd_boolean_flag;
    option->parse = kmnd_boolean_parse;

    return (kmnd_t *) option;
}

unsigned char kmnd_boolean_get(kmnd_t *kmnd, const char *path) {
    kmnd_option_t *option = kmnd_option_path(kmnd, path);
    assert(option != NULL);

    return ((unsigned char *) option->value)[0];
}

/** -- String Options -- */

static int kmnd_string_parse(kmnd_option_t *option, const char *string) {
    free(option->value);
    option->value = strdup(string);

    return 0;
}

kmnd_t *kmnd_string_new(const char character, const char *name,
                        const char *description, const kmnd_flags_t flags,
                        const char *value) {
    kmnd_option_t *option;
    option = kmnd_option_new(character, name, description, flags);

    if (option == NULL)
        return NULL;

    option->value = value ? strdup(value) : NULL;

    option->parse = kmnd_string_parse;

    return (kmnd_t *) option;
}

const char *kmnd_string_get(kmnd_t *kmnd, const char *path) {
    kmnd_option_t *option = kmnd_option_path(kmnd, path);
    assert(option != NULL);

    return ((const char *) option->value);
}

/** -- Float Options -- */

static int kmnd_float_parse(kmnd_option_t *option, const char *string) {
    errno = 0;
    char *end = NULL;

    char *locale = setlocale(LC_NUMERIC,"C");
    float value = strtof(string, &end);
    setlocale(LC_NUMERIC, locale);

    if ((end == NULL || end[0] == 0) && errno == 0) {
        *((float *) option->value) = value;
        return 0;
    }

    return -1;
}

kmnd_t *kmnd_float_new(const char character, const char *name,
                       const char *description, const kmnd_flags_t flags,
                       const float value) {
    kmnd_option_t *option;
    option = kmnd_option_new(character, name, description, flags);

    if (option == NULL)
        return NULL;

    option->value = malloc(sizeof(float));

    if (option->value == NULL) {
        kmnd_free((kmnd_t *) option);
        return NULL;
    }

    *((float *) option->value) = value;

    option->parse = kmnd_float_parse;

    return (kmnd_t *) option;
}

float kmnd_float_get(kmnd_t *kmnd, const char *path) {
    kmnd_option_t *option = kmnd_option_path(kmnd, path);
    assert(option != NULL);

    return *((float *) option->value);
}

/** -- Double Options -- */

static int kmnd_double_parse(kmnd_option_t *option, const char *string) {
    errno = 0;
    char *end = NULL;

    char *locale = setlocale(LC_NUMERIC,"C");
    double value = strtod(string, &end);
    setlocale(LC_NUMERIC, locale);

    if ((end == NULL || end[0] == 0) && errno == 0) {
        *((double *) option->value) = value;
        return 0;
    }

    return -1;
}

kmnd_t *kmnd_double_new(const char character, const char *name,
                        const char *description, const kmnd_flags_t flags,
                        const double value) {
    kmnd_option_t *option;
    option = kmnd_option_new(character, name, description, flags);

    if (option == NULL)
        return NULL;

    option->value = malloc(sizeof(double));

    if (option->value == NULL) {
        kmnd_free((kmnd_t *) option);
        return NULL;
    }

    *((double *) option->value) = value;

    option->parse = kmnd_double_parse;

    return (kmnd_t *) option;
}

double kmnd_double_get(kmnd_t *kmnd, const char *path) {
    kmnd_option_t *option = kmnd_option_path(kmnd, path);
    assert(option != NULL);

    return *((double *) option->value);
}

/** -- Scalar Options -- */

#define kmnd_scalar_new(N, T) \
    kmnd_t *kmnd_##N##_new(const char character, const char *name, \
                           const char *description, const kmnd_flags_t flags, \
                           const T value) { \
        kmnd_option_t *option; \
        option = kmnd_option_new(character, name, description, flags); \
        \
        if (option == NULL) \
            return NULL; \
        \
        option->value = malloc(sizeof(T)); \
        \
        if (option->value == NULL) { \
            kmnd_free((kmnd_t *) option); \
            return NULL; \
        } \
        \
        *((T *) option->value) = value; \
        \
        option->parse = kmnd_##N##_parse; \
        \
        return (kmnd_t *) option; \
    }

#define kmnd_scalar_get(N, T) \
    T kmnd_##N##_get(kmnd_t *kmnd, const char *path) { \
        kmnd_option_t *option = kmnd_option_path(kmnd, path); \
        assert(option != NULL); \
        \
        return *((T *) option->value); \
    }

static unsigned char kmnd_scalar_is_negative(const char *string) {
    size_t i, l = strlen(string);
    for (i = 0; i < l; i ++) {
        if (string[i] == '-')
            return 1;

        if (!isspace(string[i]))
            break;
    }

    return 0;
}

#define kmnd_scalar_parse(N, T, S, F, I, A) \
    static int kmnd_##N##_parse(kmnd_option_t *option, const char *string) { \
        if (I == 0 && kmnd_scalar_is_negative(string)) \
            return -1; \
        \
        char *end = NULL; \
        errno = 0; \
        F value = strto##S##max(string, &end, 10); \
        if ((end == NULL || end[0] == 0) && errno == 0) { \
            if (value < I || value > A) \
                return -1; \
            \
            *((T *) option->value) = value; \
            \
            return 0; \
        } \
        \
        return -1; \
    }




kmnd_scalar_parse(int8,   int8_t,   i, intmax_t,  INT8_MIN,  INT8_MAX)
kmnd_scalar_parse(int16,  int16_t,  i, intmax_t,  INT16_MIN, INT16_MAX)
kmnd_scalar_parse(int32,  int32_t,  i, intmax_t,  INT32_MIN, INT32_MAX)
kmnd_scalar_parse(int64,  int64_t,  i, intmax_t,  INT64_MIN, INT64_MAX)
kmnd_scalar_parse(uint8,  uint8_t,  u, uintmax_t, 0,         UINT8_MAX)
kmnd_scalar_parse(uint16, uint16_t, u, uintmax_t, 0,         UINT16_MAX)
kmnd_scalar_parse(uint32, uint32_t, u, uintmax_t, 0,         UINT32_MAX)
kmnd_scalar_parse(uint64, uint64_t, u, uintmax_t, 0,         UINT64_MAX)

kmnd_scalar_new(int8,   int8_t)
kmnd_scalar_new(int16,  int16_t)
kmnd_scalar_new(int32,  int32_t)
kmnd_scalar_new(int64,  int64_t)
kmnd_scalar_new(uint8,  uint8_t)
kmnd_scalar_new(uint16, uint16_t)
kmnd_scalar_new(uint32, uint32_t)
kmnd_scalar_new(uint64, uint64_t)

kmnd_scalar_get(int8,   int8_t)
kmnd_scalar_get(int16,  int16_t)
kmnd_scalar_get(int32,  int32_t)
kmnd_scalar_get(int64,  int64_t)
kmnd_scalar_get(uint8,  uint8_t)
kmnd_scalar_get(uint16, uint16_t)
kmnd_scalar_get(uint32, uint32_t)
kmnd_scalar_get(uint64, uint64_t)


unsigned char kmnd_option_required(const kmnd_option_t *option) {
    const kmnd_flags_t flag = (option->flags & KMND_FLAGS_REQUIRED);
    return (unsigned char) (flag == KMND_FLAGS_REQUIRED);
}

unsigned char kmnd_option_activated(const kmnd_option_t *option) {
    return option->activated;
}

int kmnd_option_activate(kmnd_t *kmnd, kmnd_option_t *option,
                         const char *string) {
    int res;

    assert(option->parse != NULL);

    if ((res = option->parse(option, string)) != 0) {
        kmnd_error_t error;
        kmnd_error_init_invalid_value(&error, option->core.name, string);
        kmnd_error_print(&error, kmnd);

        kmnd_command_t *command = (kmnd_command_t *) kmnd;

        if (command->usage)
            kmnd_usage_print(command->usage, command);
    }else
        option->activated = 1;

    return res;
}
