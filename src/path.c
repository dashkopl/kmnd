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

#include <stddef.h>
#include <string.h>

#include "command.h"
#include "path.h"

static kmnd_t *kmnd_find(kmnd_command_t *command, const char *name,
                         const size_t length) {
    size_t i;

    for (i = 0; i < command->num_commands; i ++) {
        if (strncmp(command->commands[i]->core.name, name, length) == 0)
            return (kmnd_t *) command->commands[i];
    }

    for (i = 0; i < command->num_options; i ++) {
        if (strncmp(command->options[i]->core.name, name, length) == 0)
            return (kmnd_t *) command->options[i];
    }

    for (i = 0; i < command->num_inputs; i ++) {
        if (strncmp(command->inputs[i]->core.name, name, length) == 0)
            return (kmnd_t *) command->inputs[i];
    }

    return NULL;
}

kmnd_t *kmnd_path(kmnd_t *kmnd, const char *path) {
    if (path == NULL || strlen(path) == 0)
        return kmnd;

    kmnd_command_t *command = (kmnd_command_t *) kmnd;

    if (path[0] == '.')
        path += 1;
    else {
        while (command->super != NULL)
            command = (kmnd_command_t *) command->super;
    }

    size_t i, j = 0;
    for (i = 0; i <= strlen(path); i ++) {
        if (path[i] == '.') {
            command = (kmnd_command_t *) kmnd_find(command, path + j, i - j);

            if (command == NULL || command->core.type != KMND_TYPE_COMMAND)
                return NULL;

            j = i + 1;
        }else if (path[i] == '\0') {
            return kmnd_find(command, path + j, i - j);
        }
    }

    return NULL;
}

kmnd_command_t *kmnd_command_path(kmnd_t *kmnd, const char *path) {
    kmnd_t *result = kmnd_path(kmnd, path);

    if (result == NULL || result->type != KMND_TYPE_COMMAND)
        return NULL;

    return (kmnd_command_t *) result;
}

kmnd_option_t *kmnd_option_path(kmnd_t *kmnd, const char *path) {
    kmnd_t *result = kmnd_path(kmnd, path);

    if (result == NULL || result->type != KMND_TYPE_OPTION)
        return NULL;

    return (kmnd_option_t *) result;
}

kmnd_input_t *kmnd_input_path(kmnd_t *kmnd, const char *path) {
    kmnd_t *result = kmnd_path(kmnd, path);

    if (result == NULL || result->type != KMND_TYPE_INPUT)
        return NULL;

    return (kmnd_input_t *) result;
}
