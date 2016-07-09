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

TEST(OptionUint64Fixture, NameDescriptionFlags) {
    /* Create a new uint64 option. */
    kmnd_t *uint64 = kmnd_uint64_new('i', "uint64", "This is a uint64.",
                                     KMND_FLAGS_REQUIRED, 0);

    kmnd_option_t *option = (kmnd_option_t *) uint64;

    /* Make sure that the character, name and description are correctly set. */
    EXPECT_EQ('i', option->character);
    EXPECT_STREQ("uint64", option->core.name);
    EXPECT_STREQ("This is a uint64.", option->core.description);
    EXPECT_EQ(KMND_FLAGS_REQUIRED, option->flags);

    /* Free the option. */
    kmnd_free(uint64);
}

TEST(OptionUint64Fixture, DefaultValue) {
    /* Create a new uint64 option. */
    kmnd_t *uint64 = kmnd_uint64_new('i', "uint64", "This is a uint64.",
                                     KMND_FLAGS_REQUIRED, 4294967296);

    kmnd_option_t *option = (kmnd_option_t *) uint64;

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(4294967296, kmnd_uint64_get(uint64, NULL));

    /* Free the option. */
    kmnd_free(uint64);
}

TEST(OptionUint64Fixture, ParseMin) {
    /* Create a new uint64 option. */
    kmnd_t *uint64 = kmnd_uint64_new('i', "uint64", "This is a uint64.",
                                     KMND_FLAGS_REQUIRED, 4294967296);

    kmnd_option_t *option = (kmnd_option_t *) uint64;
    kmnd_option_activate((kmnd_t *) option, option, "0");

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(0, kmnd_uint64_get(uint64, NULL));

    /* Free the option. */
    kmnd_free(uint64);
}

TEST(OptionUint64Fixture, ParseMax) {
    /* Create a new uint64 option. */
    kmnd_t *uint64 = kmnd_uint64_new('i', "uint64", "This is a uint64.",
                                     KMND_FLAGS_REQUIRED, 4294967296);

    kmnd_option_t *option = (kmnd_option_t *) uint64;
    kmnd_option_activate((kmnd_t *) option, option, "18446744073709551615");

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(18446744073709551615UL, kmnd_uint64_get(uint64, NULL));

    /* Free the option. */
    kmnd_free(uint64);
}

TEST(OptionUint64Fixture, ParseUnderflow) {
    EXPECT_DEATH({
        /* Create a new uint64 option. */
        kmnd_t *kmnd = kmnd_new(NULL, NULL, NULL, NULL);
        kmnd_fd(kmnd, STDERR_FILENO);

        kmnd_t *uint64 = kmnd_uint64_new('i', "uint64", "This is a uint64.",
                                         KMND_FLAGS_REQUIRED, 4294967296);

        kmnd_option_t *option = (kmnd_option_t *) uint64;

        EXPECT_EQ(-1, kmnd_option_activate(kmnd, option, "-1"));

        /* Free the option. */
        kmnd_free(uint64);

        exit(1);
    }, "Invalid value");
}

TEST(OptionUint64Fixture, ParseOverflow) {
    EXPECT_DEATH({
        /* Create a new uint64 option. */
        kmnd_t *kmnd = kmnd_new(NULL, NULL, NULL, NULL);
        kmnd_fd(kmnd, STDERR_FILENO);

        kmnd_t *uint64 = kmnd_uint64_new('i', "uint64", "This is a uint64.",
                                         KMND_FLAGS_REQUIRED, 4294967296);

        kmnd_option_t *option = (kmnd_option_t *) uint64;

        EXPECT_EQ(-1, kmnd_option_activate(kmnd, option, "18446744073709551616"));

        /* Free the option. */
        kmnd_free(uint64);

        exit(1);
    }, "Invalid value");
}


