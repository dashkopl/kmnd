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

#include <kmnd.h>
#include <stdio.h>
#include <stdlib.h>

static int run_command(kmnd_t *kmnd) {
    printf("Verbose: %d\n", kmnd_boolean_get(kmnd, "verbose"));

    return 0;
}

static int try_command(kmnd_t *kmnd) {
    printf("Verbose: %d\n", kmnd_boolean_get(kmnd, "verbose"));
    printf("Url: %s\n", kmnd_string_get(kmnd, "try.url"));

    return 0;
}

int main(int argc, const char **argv) {
    kmnd_t *kmnd = kmnd_new("sample", NULL, run_command,
        kmnd_usage_new("sample `PATH`",
                       "Sample, the ``sample`` sampler\nsample."),

        kmnd_new("try", "*Try* `something`", try_command,
            kmnd_string_new('u', "url", "Try something from this URL",
                            KMND_FLAGS_NONE, 0),
             NULL
        ),

        kmnd_boolean_new('V', "verbose", "Show more debugging information",
                         KMND_FLAGS_NONE, 0),

        kmnd_input_new("path", "Path to the file you want to do nothing with.",
                       KMND_FLAGS_REQUIRED, NULL),

        NULL
    );

    return kmnd_run(kmnd, argc, argv);
}

