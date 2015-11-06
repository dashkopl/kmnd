/*
 * Copyright (C) 2015-10-31, Tim van Elsloo
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

#include <gtest/gtest.h>

#include "../../src/usage.h"

TEST(UsageFixture, Empty) {
    ASSERT_DEATH({
        kmnd_t *usage = kmnd_usage_new("sample `COMMAND|FILE`",
                                       "Sample, the ``sample`` sampler\n"
                                       "sample.");

        kmnd_command_t *kmnd = (kmnd_command_t *) kmnd_new("sample", NULL, NULL,
                                                       usage, NULL);
        kmnd_fd((kmnd_t *) kmnd, STDERR_FILENO);

        kmnd_usage_print((kmnd_usage_t *) usage, kmnd);

        kmnd_free((kmnd_t *) kmnd);

        exit(1);
    }, "^Usage:\n\n"
       "    $ sample COMMAND|FILE\n\n"
       "      Sample, the `sample` sampler\n"
       "      sample.\n\n"
       "Options:\n\n"
       "    -h/--help  Show help banner of specified command\n"
       "$");
}

TEST(UsageFixture, Subcommands) {
    ASSERT_DEATH({
        kmnd_t *usage = kmnd_usage_new("sample `COMMAND|FILE`",
                                       "Sample, the ``sample`` sampler\n"
                                       "sample.");

        kmnd_t *sub = kmnd_new("sub", "This is a subcommand", NULL, NULL);

        kmnd_command_t *kmnd = (kmnd_command_t *) kmnd_new("sample", NULL, NULL,
                                                       usage, sub, NULL);
        kmnd_fd((kmnd_t *) kmnd, STDERR_FILENO);

        kmnd_usage_print((kmnd_usage_t *) usage, kmnd);

        kmnd_free((kmnd_t *) kmnd);

        exit(1);
    }, "^Usage:\n\n"
       "    $ sample COMMAND|FILE\n\n"
       "      Sample, the `sample` sampler\n"
       "      sample.\n\n"
       "Commands:\n\n"
       "    \\+ sub      This is a subcommand\n\n"
       "Options:\n\n"
       "    -h/--help  Show help banner of specified command\n"
       "$");
}

TEST(UsageFixture, Options) {
    ASSERT_DEATH({
        kmnd_t *usage = kmnd_usage_new("sample `COMMAND|FILE`",
                                       "Sample, the ``sample`` sampler\n"
                                       "sample.");

        kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean",
                                           KMND_FLAGS_NONE, 0);

        kmnd_command_t *kmnd = (kmnd_command_t *) kmnd_new("sample", NULL, NULL,
                                                       usage, boolean, NULL);
        kmnd_fd((kmnd_t *) kmnd, STDERR_FILENO);

        kmnd_usage_print((kmnd_usage_t *) usage, kmnd);

        kmnd_free((kmnd_t *) kmnd);

        exit(1);
    }, "^Usage:\n\n"
       "    $ sample COMMAND|FILE\n\n"
       "      Sample, the `sample` sampler\n"
       "      sample.\n\n"
       "Options:\n\n"
       "    -b/--boolean  This is a boolean\n"
       "    -h/--help     Show help banner of specified command\n"
       "$");
}

TEST(UsageFixture, Inputs) {
    ASSERT_DEATH({
        kmnd_t *usage = kmnd_usage_new("sample `COMMAND|FILE`",
                                       "Sample, the ``sample`` sampler\n"
                                       "sample.");

        kmnd_t *input = kmnd_input_new("path", "This is a string",
                                       KMND_FLAGS_REQUIRED, NULL);

        kmnd_command_t *kmnd = (kmnd_command_t *) kmnd_new("sample", NULL, NULL,
                                                       usage, input, NULL);
        kmnd_fd((kmnd_t *) kmnd, STDERR_FILENO);

        kmnd_usage_print((kmnd_usage_t *) usage, kmnd);

        kmnd_free((kmnd_t *) kmnd);

        exit(1);
    }, "^Usage:\n\n"
       "    $ sample COMMAND|FILE\n\n"
       "      Sample, the `sample` sampler\n"
       "      sample.\n\n"
       "Input:\n\n"
       "      path     This is a string\n\n"
       "Options:\n\n"
       "    -h/--help  Show help banner of specified command\n"
       "$");
}

TEST(UsageFixture, All) {
    ASSERT_DEATH({
        kmnd_t *usage = kmnd_usage_new("sample `COMMAND|FILE`",
                                       "Sample, the ``sample`` sampler\n"
                                       "sample.");

        kmnd_t *sub = kmnd_new("sub", "This is a subcommand", NULL, NULL);
        kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean",
                                           KMND_FLAGS_NONE, 0);
        kmnd_t *input = kmnd_input_new("path", "This is a string",
                                       KMND_FLAGS_REQUIRED, NULL);

        kmnd_command_t *kmnd = (kmnd_command_t *) kmnd_new("sample", NULL, NULL,
                                                           usage, sub, boolean,
                                                           input, NULL);
        kmnd_fd((kmnd_t *) kmnd, STDERR_FILENO);

        kmnd_usage_print((kmnd_usage_t *) usage, kmnd);

        kmnd_free((kmnd_t *) kmnd);

        exit(1);
    }, "^Usage:\n\n"
       "    $ sample COMMAND|FILE\n\n"
       "      Sample, the `sample` sampler\n"
       "      sample.\n\n"
       "Commands:\n\n"
       "    \\+ sub         This is a subcommand\n\n"
       "Input:\n\n"
       "      path        This is a string\n\n"
       "Options:\n\n"
       "    -b/--boolean  This is a boolean\n"
       "    -h/--help     Show help banner of specified command\n"
       "$");
}
