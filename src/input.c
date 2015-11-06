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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "path.h"

kmnd_t *kmnd_input_new(const char *name, const char *description,
                       const kmnd_flags_t flags, kmnd_validator_cb *validator) {
    kmnd_input_t *input = malloc(sizeof(kmnd_input_t));

    if (input == NULL)
        return NULL;

    memset(input, 0, sizeof(kmnd_input_t));

    input->core.type = KMND_TYPE_INPUT;
    input->core.name = name;
    input->core.description = description;
    input->flags = flags;
    input->validator = validator;

    return (kmnd_t *) input;
}

int kmnd_input_activate(kmnd_input_t *input, kmnd_t *kmnd, const char *string) {
    int res = input->validator ? input->validator(kmnd, string) : 0;

    if (res != 0)
        return res;

    assert(input->value == NULL);
    assert(string != NULL);
    input->value = strdup(string);

    return 0;
}

unsigned char kmnd_input_required(const kmnd_input_t *input) {
    return (unsigned char) ((input->flags & KMND_FLAGS_REQUIRED) != 0);
}

unsigned char kmnd_input_activated(const kmnd_input_t *input) {
    return (unsigned char) (input->value != NULL);
}

void kmnd_input_free(kmnd_input_t *input) {
    free(input->value); input->value = NULL;
    memset(input, 0, sizeof(kmnd_input_t));
    free(input);
}

const char *kmnd_input_get(kmnd_t *kmnd, const char *path) {
    kmnd_input_t *input = kmnd_input_path(kmnd, path);

    if (input == NULL)
        return NULL;

    return input->value;
}
