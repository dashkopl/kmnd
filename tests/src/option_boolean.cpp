/*
 * Copyright (C) 2015-10-29, Tim van Elsloo
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

TEST(OptionBooleanFixture, NameDescriptionFlags) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_REQUIRED, 0);

    kmnd_option_t *option = (kmnd_option_t *) boolean;

    /* Make sure that the character, name and description are correctly set. */
    EXPECT_EQ('b', option->character);
    EXPECT_STREQ("boolean", option->core.name);
    EXPECT_STREQ("This is a boolean.", option->core.description);
    EXPECT_EQ(KMND_FLAGS_REQUIRED, option->flags);

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, DefaultValueFalse) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 0);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(0, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, DefaultValueTrue) {
    KMND_MEM_LEAK_PRE();

    /* Again, create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 1);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(1, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, ParseFalseOff) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 1);

    int res = kmnd_option_activate(boolean, (kmnd_option_t *) boolean, "off");
    EXPECT_EQ(0, res);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(0, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, ParseFalseFalse) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 1);

    int res = kmnd_option_activate(boolean, (kmnd_option_t *) boolean, "false");
    EXPECT_EQ(0, res);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(0, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, ParseFalseNo) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 1);

    int res = kmnd_option_activate(boolean, (kmnd_option_t *) boolean, "no");
    EXPECT_EQ(0, res);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(0, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, ParseFalse0) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 1);

    int res = kmnd_option_activate(boolean, (kmnd_option_t *) boolean, "0");
    EXPECT_EQ(0, res);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(0, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, ParseTrueOn) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 0);

    int res = kmnd_option_activate(boolean, (kmnd_option_t *) boolean, "on");
    EXPECT_EQ(0, res);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(1, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, ParseTrueTrue) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 0);

    int res = kmnd_option_activate(boolean, (kmnd_option_t *) boolean, "true");
    EXPECT_EQ(0, res);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(1, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, ParseFalseYes) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 1);

    int res = kmnd_option_activate(boolean, (kmnd_option_t *) boolean, "yes");
    EXPECT_EQ(0, res);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(1, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, ParseTrue1) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 0);

    int res = kmnd_option_activate(boolean, (kmnd_option_t *) boolean, "1");
    EXPECT_EQ(0, res);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(1, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

TEST(OptionBooleanFixture, Flag) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 0);

    kmnd_option_flag(boolean, (kmnd_option_t *) boolean);

    /* Make sure that the value is correctly set. */
    EXPECT_EQ(1, kmnd_boolean_get(boolean, NULL));

    /* Free the option. */
    kmnd_free(boolean);

    KMND_MEM_LEAK_POST();
}

/*
 * Every boolean option should be memsetted to 0 when freed.
 */
TEST(OptionBooleanFixture, Free) {
    KMND_MEM_LEAK_PRE();

    /* Create a new boolean option. */
    kmnd_t *boolean = kmnd_boolean_new('b', "boolean", "This is a boolean.",
                                       KMND_FLAGS_NONE, 0);

    /* Update the value. */
    kmnd_option_flag(boolean, (kmnd_option_t *) boolean);

    /* Free the option. */
    kmnd_free(boolean);

    /* Check if it is freed. */
    EXPECT_FALSE(kmnd_mem_valid(boolean));

    KMND_MEM_LEAK_POST();
}
