/*
 * Copyright (C) 2015-11-05, Tim van Elsloo
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

#include "../../src/error.h"

TEST(ErrorFixture, UnknownCommand) {
    const char *args[] = { "kmnd", "does_not_exist" };

    EXPECT_DEATH({
        kmnd_t *kmnd = kmnd_new("foobar", "This is foobar", NULL, NULL);

        kmnd_fd(kmnd, STDERR_FILENO);

        EXPECT_NE(0, kmnd_run(kmnd, sizeof(args) / sizeof(*args), args));

        kmnd_free(kmnd);

        exit(1);
    }, "Unknown command: `does_not_exist`");
}

TEST(ErrorFixture, UnknownOption) {
    const char *args[] = { "kmnd", "--does_not_exist" };

    EXPECT_DEATH({
        kmnd_t *kmnd = kmnd_new("foobar", "This is foobar", NULL, NULL);

        kmnd_fd(kmnd, STDERR_FILENO);

        EXPECT_NE(0, kmnd_run(kmnd, sizeof(args) / sizeof(*args), args));

        kmnd_free(kmnd);

        exit(1);
    }, "Unknown option: `--does_not_exist`");
}

TEST(ErrorFixture, NotABoolean) {
    const char *args[] = { "kmnd", "--not_a_boolean" };

    EXPECT_DEATH({
        kmnd_t *string = kmnd_string_new('n', "not_a_boolean",
                                         "This is not a boolean",
                                         KMND_FLAGS_REQUIRED, NULL);
        kmnd_t *kmnd = kmnd_new("foobar", "This is foobar", NULL, string, NULL);

        kmnd_fd(kmnd, STDERR_FILENO);

        EXPECT_NE(0, kmnd_run(kmnd, sizeof(args) / sizeof(*args), args));

        kmnd_free(kmnd);

        exit(1);
    }, "Not a boolean option: `--not_a_boolean`");
}

TEST(ErrorFixture, InvalidValue) {
    const char *args[] = { "kmnd", "--boolean=abc" };

    EXPECT_DEATH({
        kmnd_t *boolean = kmnd_boolean_new('b', "boolean",
                                           "This is a boolean",
                                           KMND_FLAGS_REQUIRED, 0);
        kmnd_t *kmnd = kmnd_new("foobar", "This is foobar", NULL, boolean,
                                NULL);

        kmnd_fd(kmnd, STDERR_FILENO);

        EXPECT_NE(0, kmnd_run(kmnd, sizeof(args) / sizeof(*args), args));

        kmnd_free(kmnd);

        exit(1);
    }, "Invalid value: `abc`, for option: `--boolean`");
}

int invalid_input_validator(kmnd_t *kmnd, const char *string) {
    return -1;
}

TEST(ErrorFixture, InvalidInput) {
    const char *args[] = { "kmnd", "always invalid" };

    EXPECT_DEATH({
        kmnd_t *input = kmnd_input_new("input", "This is input.",
                                       KMND_FLAGS_REQUIRED,
                                       invalid_input_validator);
        kmnd_t *kmnd = kmnd_new("foobar", "This is foobar", NULL, input, NULL);

        kmnd_fd(kmnd, STDERR_FILENO);

        EXPECT_NE(0, kmnd_run(kmnd, sizeof(args) / sizeof(*args), args));

        kmnd_free(kmnd);

        exit(1);
    }, "Invalid value: `always invalid`, for input: `input`");
}

TEST(ErrorFixture, MissingInput) {
    const char *args[] = { "kmnd" };

    EXPECT_DEATH({
        kmnd_t *input = kmnd_input_new("input", "This is input.",
                                       KMND_FLAGS_REQUIRED,
                                       invalid_input_validator);
        kmnd_t *kmnd = kmnd_new("foobar", "This is foobar", NULL, input, NULL);

        kmnd_fd(kmnd, STDERR_FILENO);

        EXPECT_NE(0, kmnd_run(kmnd, sizeof(args) / sizeof(*args), args));

        kmnd_free(kmnd);

        exit(1);
    }, "Missing value for input: `input`");
}

TEST(ErrorFixture, MissingOption) {
    const char *args[] = { "kmnd" };

    EXPECT_DEATH({
        kmnd_t *string = kmnd_string_new('s', "string", "This is a string.",
                                         KMND_FLAGS_REQUIRED, NULL);
        kmnd_t *kmnd = kmnd_new("foobar", "This is foobar", NULL, string, NULL);

        kmnd_fd(kmnd, STDERR_FILENO);

        EXPECT_NE(0, kmnd_run(kmnd, sizeof(args) / sizeof(*args), args));

        kmnd_free(kmnd);

        exit(1);
    }, "Missing value for option: `--string`");
}
