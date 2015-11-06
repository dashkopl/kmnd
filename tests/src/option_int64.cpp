/*
 * Copyright (C) 2015-11-02, Tim van Elsloo
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

#include "../../src/option.h"

#include "malloc.h"

TEST(OptionInt64Fixture, NameDescriptionFlags) {
    KMND_MEM_LEAK_PRE();

    /* Create a new int64 option. */
    kmnd_t *int64 = kmnd_int64_new('i', "int64", "This is a int64.",
                                   KMND_FLAGS_REQUIRED, 0);

    kmnd_option_t *option = (kmnd_option_t *) int64;

    /* Make sure that the character, name and description are correctly set. */
    EXPECT_EQ('i', option->character);
    EXPECT_STREQ("int64", option->core.name);
    EXPECT_STREQ("This is a int64.", option->core.description);
    EXPECT_EQ(KMND_FLAGS_REQUIRED, option->flags);

    /* Free the option. */
    kmnd_free(int64);

    KMND_MEM_LEAK_POST();
}

TEST(OptionInt64Fixture, DefaultValue) {
    KMND_MEM_LEAK_PRE();

    /* Create a new int64 option. */
    kmnd_t *int64 = kmnd_int64_new('i', "int64", "This is a int64.",
                                   KMND_FLAGS_REQUIRED, -2147483649);

    kmnd_option_t *option = (kmnd_option_t *) int64;

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(-2147483649, kmnd_int64_get(int64, NULL));

    /* Free the option. */
    kmnd_free(int64);

    KMND_MEM_LEAK_POST();
}

TEST(OptionInt64Fixture, ParseMin) {
    KMND_MEM_LEAK_PRE();

    /* Create a new int64 option. */
    kmnd_t *int64 = kmnd_int64_new('i', "int64", "This is a int64.",
                                   KMND_FLAGS_REQUIRED, -2147483649);

    kmnd_option_t *option = (kmnd_option_t *) int64;
    kmnd_option_activate((kmnd_t *) option, option, "-9223372036854775808");

    /* Make sure that the value is correctly set. The ull is necessary to
     * silence a (false) warning in Clang. Note that because we cast to int64_t
     * immediately afterwards, it doesn't matter that much and GTest will still
     * print the integer correctly. */
    int64_t expected = -9223372036854775808ull;
    EXPECT_EQ(expected, kmnd_int64_get(int64, NULL));

    /* Free the option. */
    kmnd_free(int64);

    KMND_MEM_LEAK_POST();
}

TEST(OptionInt64Fixture, ParseMax) {
    KMND_MEM_LEAK_PRE();

    /* Create a new int64 option. */
    kmnd_t *int64 = kmnd_int64_new('i', "int64", "This is a int64.",
                                   KMND_FLAGS_REQUIRED, -2147483649);

    kmnd_option_t *option = (kmnd_option_t *) int64;
    kmnd_option_activate((kmnd_t *) option, option, "9223372036854775807");

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(9223372036854775807, kmnd_int64_get(int64, NULL));


    /* Free the option. */
    kmnd_free(int64);

    KMND_MEM_LEAK_POST();
}

TEST(OptionInt64Fixture, ParseUnderflow) {
    EXPECT_DEATH({
        /* Create a new int64 option. */
        kmnd_t *kmnd = kmnd_new(NULL, NULL, NULL, NULL);
        kmnd_fd(kmnd, STDERR_FILENO);

        kmnd_t *int64 = kmnd_int64_new('i', "int64", "This is a int64.",
                                       KMND_FLAGS_REQUIRED, -2147483649);

        kmnd_option_t *option = (kmnd_option_t *) int64;

        EXPECT_EQ(-1, kmnd_option_activate(kmnd, option, "-9223372036854775809"));

        /* Free the option. */
        kmnd_free(int64);

        exit(1);
    }, "Invalid value");
}

TEST(OptionInt64Fixture, ParseOverflow) {
    EXPECT_DEATH({
        /* Create a new int64 option. */
        kmnd_t *kmnd = kmnd_new(NULL, NULL, NULL, NULL);
        kmnd_fd(kmnd, STDERR_FILENO);

        kmnd_t *int64 = kmnd_int64_new('i', "int64", "This is a int64.",
                                       KMND_FLAGS_REQUIRED, -2147483649);

        kmnd_option_t *option = (kmnd_option_t *) int64;

        EXPECT_EQ(-1, kmnd_option_activate(kmnd, option, "9223372036854775808"));

        /* Free the option. */
        kmnd_free(int64);

        exit(1);
    }, "Invalid value");
}


