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

#include "../../src/option.h"

TEST(OptionStringFixture, NameDescriptionFlags) {
    /* Create a new boolean option. */
    kmnd_t *string = kmnd_boolean_new('s', "string", "This is a string.",
                                      KMND_FLAGS_REQUIRED, 0);

    kmnd_option_t *option = (kmnd_option_t *) string;

    /* Make sure that the character, name and description are correctly set. */
    EXPECT_EQ('s', option->character);
    EXPECT_STREQ("string", option->core.name);
    EXPECT_STREQ("This is a string.", option->core.description);
    EXPECT_EQ(KMND_FLAGS_REQUIRED, option->flags);

    /* Free the option. */
    kmnd_free(string);
}

TEST(OptionStringFixture, DefaultValue) {
    /* Create a new string option. */
    kmnd_t *string = kmnd_string_new('s', "string", "This is a string.",
                                     KMND_FLAGS_NONE, "foobar");

    /* Make sure that the value is correctly set. */
    EXPECT_STREQ("foobar", kmnd_string_get(string, NULL));

    /* Free the option. */
    kmnd_free(string);
}

TEST(OptionStringFixture, Parse) {
    /* Create a new string option. */
    kmnd_t *string = kmnd_string_new('s', "string", "This is a string.",
                                     KMND_FLAGS_NONE, "foobar");

    /* Parse a new value of different length. */
    int res = kmnd_option_activate(string, (kmnd_option_t *) string,
                                "Hello World!");
    EXPECT_EQ(0, res);

    /* Make sure that the value is correctly set. */
    EXPECT_STREQ("Hello World!", kmnd_string_get(string, NULL));

    /* Free the option. */
    kmnd_free(string);
}

/*
 * Every string option should be memsetted to 0 when freed.
 */
TEST(OptionStringFixture, Free) {
    /* Create a new string option. */
    kmnd_t *string = kmnd_string_new('s', "string", "This is a string.",
                                     KMND_FLAGS_NONE, "foobar");

    /* Parse a new value of different length. */
    int res = kmnd_option_activate(string, (kmnd_option_t *) string,
                                "Hello World!");
    EXPECT_EQ(0, res);

    /* Free the option. */
    kmnd_free(string);
}
