/*
 * Copyright (C) 2015-10-30, Tim van Elsloo
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

#ifdef __linux__
#define _GNU_SOURCE
#include <dlfcn.h>
#elif __APPLE__
#include <dlfcn.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "malloc.h"

typedef struct kmnd_mem_entry_s kmnd_mem_entry_t;

struct kmnd_mem_entry_s {
    void *pointer;
    size_t size;
    unsigned char freed;
    kmnd_mem_entry_t *next;
};

static ssize_t usage = 0;
static kmnd_mem_entry_t *entries = NULL;
static uintmax_t num_entries = 0;

ssize_t kmnd_mem_usage(void) {
    return usage;
}

void *malloc(const size_t size) {
    static void *(*_malloc)(const size_t) = NULL;

    if (_malloc == NULL)
        _malloc = dlsym(RTLD_NEXT, "malloc");

    void *pointer = _malloc(size);

    if (!pointer)
        return NULL;

    kmnd_mem_entry_t *entry = _malloc(sizeof(kmnd_mem_entry_t));
    entry->pointer = pointer;
    entry->size = size;
    entry->next = entries;
    entry->freed = 0;
    entries = entry;

    num_entries ++;
    usage += entry->size;

    return pointer;
}

char *strdup(const char *string) {
    char *dup = malloc(strlen(string) + 1);
    strcpy(dup, string);
    return dup;
}

void free(void *pointer) {
    static void (*_free)(void *) = NULL;

    if (_free == NULL)
        _free = dlsym(RTLD_NEXT, "free");

    kmnd_mem_entry_t *entry = entries;

    uintmax_t i;
    for (i = 0; i < num_entries; i ++) {
        if (entry->pointer == pointer) {
            entry->freed = 1;
            usage -= entry->size;
            break;
        }

        entry = entry->next;
    }

    _free(pointer);
}

unsigned char kmnd_mem_valid(const void *pointer) {
    if (pointer == NULL)
        return 0;

    const kmnd_mem_entry_t *entry = entries;

    uintmax_t i;
    for (i = 0; i < num_entries; i ++) {
        if (entry->pointer == pointer)
            return (unsigned char ) !entry->freed;

        entry = entry->next;
    }

    return 0;
}
