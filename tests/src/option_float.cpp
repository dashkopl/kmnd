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
#include <math.h>

#include "../../src/option.h"

#include "malloc.h"

TEST(OptionFloatFixture, NameDescriptionFlags) {
    KMND_MEM_LEAK_PRE();

    /* Create a new float option. */
    kmnd_t *f = kmnd_float_new('f', "float", "This is a float.",
                               KMND_FLAGS_REQUIRED, 0);

    kmnd_option_t *option = (kmnd_option_t *) f;

    /* Make sure that the character, name and description are correctly set. */
    EXPECT_EQ('f', option->character);
    EXPECT_STREQ("float", option->core.name);
    EXPECT_STREQ("This is a float.", option->core.description);
    EXPECT_EQ(KMND_FLAGS_REQUIRED, option->flags);

    /* Free the option. */
    kmnd_free(f);

    KMND_MEM_LEAK_POST();
}

TEST(OptionFloatFixture, DefaultValue) {
    KMND_MEM_LEAK_PRE();

    /* Create a new float option. */
    kmnd_t *f = kmnd_float_new('f', "float", "This is a float.",
                               KMND_FLAGS_REQUIRED, -37.827281689071f);

    /* Make sure that the value is correctly set. */
    EXPECT_FLOAT_EQ(-37.827281689071f, kmnd_float_get(f, NULL));

    /* Free the option. */
    kmnd_free(f);

    KMND_MEM_LEAK_POST();
}

TEST(OptionFloatFixture, Simple) {
    KMND_MEM_LEAK_PRE();

    /* Create a new float option. */
    kmnd_t *f = kmnd_float_new('f', "float", "This is a float.",
                               KMND_FLAGS_REQUIRED, 0);

    kmnd_option_t *option = (kmnd_option_t *) f;
    kmnd_option_activate((kmnd_t *) option, option, "-37.827281689071");

    /* Make sure that the value is correctly set. */
    EXPECT_FLOAT_EQ(-37.827281689071f, kmnd_float_get(f, NULL));

    /* Free the option. */
    kmnd_free(f);

    KMND_MEM_LEAK_POST();
}

TEST(OptionFloatFixture, Hex) {
    KMND_MEM_LEAK_PRE();

    /* Create a new float option. */
    kmnd_t *f = kmnd_float_new('f', "float", "This is a float.",
                               KMND_FLAGS_REQUIRED, 0);

    kmnd_option_t *option = (kmnd_option_t *) f;
    kmnd_option_activate((kmnd_t *) option, option, "-0x37.827281689071p1");

    /* Make sure that the value is correctly set. */
    EXPECT_FLOAT_EQ(-0x37.827281689071p1, kmnd_float_get(f, NULL));

    /* Free the option. */
    kmnd_free(f);

    KMND_MEM_LEAK_POST();
}

TEST(OptionFloatFixture, Infinity) {
    KMND_MEM_LEAK_PRE();

    /* Create a new float option. */
    kmnd_t *f = kmnd_float_new('f', "float", "This is a float.",
                               KMND_FLAGS_REQUIRED, 0);

    kmnd_option_t *option = (kmnd_option_t *) f;
    kmnd_option_activate((kmnd_t *) option, option, "INF");

    /* Make sure that the value is correctly set. */
    EXPECT_FLOAT_EQ(INFINITY, kmnd_float_get(f, NULL));

    /* Free the option. */
    kmnd_free(f);

    KMND_MEM_LEAK_POST();
}

/* Precision is platform dependent(?) so unit tests are not yet included. */

TEST(OptionFloatFixture, Error) {
    EXPECT_DEATH({
        kmnd_t *kmnd = kmnd_new(NULL, NULL, NULL, NULL);
        kmnd_fd(kmnd, STDERR_FILENO);

        /* Create a new float option. */
        kmnd_t *f = kmnd_float_new('f', "float", "This is a float.",
                                   KMND_FLAGS_REQUIRED, 0);

        kmnd_option_t *option = (kmnd_option_t *) f;

        EXPECT_EQ(-1, kmnd_option_activate(kmnd, option, "abc"));

        /* Free the option. */
        kmnd_free(f);

        exit(1);
    }, "Invalid value");
}
