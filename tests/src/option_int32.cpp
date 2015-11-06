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

TEST(OptionInt32Fixture, NameDescriptionFlags) {
    KMND_MEM_LEAK_PRE();

    /* Create a new int32 option. */
    kmnd_t *int32 = kmnd_int32_new('i', "int32", "This is a int32.",
                                   KMND_FLAGS_REQUIRED, 0);

    kmnd_option_t *option = (kmnd_option_t *) int32;

    /* Make sure that the character, name and description are correctly set. */
    EXPECT_EQ('i', option->character);
    EXPECT_STREQ("int32", option->core.name);
    EXPECT_STREQ("This is a int32.", option->core.description);
    EXPECT_EQ(KMND_FLAGS_REQUIRED, option->flags);

    /* Free the option. */
    kmnd_free(int32);

    KMND_MEM_LEAK_POST();
}

TEST(OptionInt32Fixture, DefaultValue) {
    KMND_MEM_LEAK_PRE();

    /* Create a new int32 option. */
    kmnd_t *int32 = kmnd_int32_new('i', "int32", "This is a int32.",
                                   KMND_FLAGS_REQUIRED, -32769);

    kmnd_option_t *option = (kmnd_option_t *) int32;

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(-32769, kmnd_int32_get(int32, NULL));

    /* Free the option. */
    kmnd_free(int32);

    KMND_MEM_LEAK_POST();
}

TEST(OptionInt32Fixture, ParseMin) {
    KMND_MEM_LEAK_PRE();

    /* Create a new int32 option. */
    kmnd_t *int32 = kmnd_int32_new('i', "int32", "This is a int32.",
                                   KMND_FLAGS_REQUIRED, -32769);

    kmnd_option_t *option = (kmnd_option_t *) int32;
    kmnd_option_activate((kmnd_t *) option, option, "-2147483648");

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(-2147483648, kmnd_int32_get(int32, NULL));

    /* Free the option. */
    kmnd_free(int32);

    KMND_MEM_LEAK_POST();
}

TEST(OptionInt32Fixture, ParseMax) {
    KMND_MEM_LEAK_PRE();

    /* Create a new int32 option. */
    kmnd_t *int32 = kmnd_int32_new('i', "int32", "This is a int32.",
                                   KMND_FLAGS_REQUIRED, -32769);

    kmnd_option_t *option = (kmnd_option_t *) int32;
    kmnd_option_activate((kmnd_t *) option, option, "2147483647");

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(2147483647, kmnd_int32_get(int32, NULL));

    /* Free the option. */
    kmnd_free(int32);

    KMND_MEM_LEAK_POST();
}

TEST(OptionInt32Fixture, ParseUnderflow) {
    EXPECT_DEATH({
        /* Create a new int32 option. */
        kmnd_t *kmnd = kmnd_new(NULL, NULL, NULL, NULL);
        kmnd_fd(kmnd, STDERR_FILENO);

        kmnd_t *int32 = kmnd_int32_new('i', "int32", "This is a int32.",
                                       KMND_FLAGS_REQUIRED, -32769);

        kmnd_option_t *option = (kmnd_option_t *) int32;

        EXPECT_EQ(-1, kmnd_option_activate(kmnd, option, "-2147483649"));

        /* Free the option. */
        kmnd_free(int32);

        exit(1);
    }, "Invalid value");
}

TEST(OptionInt32Fixture, ParseOverflow) {
    EXPECT_DEATH({
        /* Create a new int32 option. */
        kmnd_t *kmnd = kmnd_new(NULL, NULL, NULL, NULL);
        kmnd_fd(kmnd, STDERR_FILENO);

        kmnd_t *int32 = kmnd_int32_new('i', "int32", "This is a int32.",
                                       KMND_FLAGS_REQUIRED, -32769);

        kmnd_option_t *option = (kmnd_option_t *) int32;

        EXPECT_EQ(-1, kmnd_option_activate(kmnd, option, "2147483648"));

        /* Free the option. */
        kmnd_free(int32);

        exit(1);
    }, "Invalid value");
}
