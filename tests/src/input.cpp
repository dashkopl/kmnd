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

#include "../../src/input.h"

static int validator(kmnd_t *kmnd, const char *string) {
    return 0;
}

TEST(InputFixture, NameDescriptionFlags) {
    kmnd_t *kmnd = kmnd_input_new("foobar", "This is foobar",
                                  KMND_FLAGS_REQUIRED, validator);

    kmnd_input_t *input = (kmnd_input_t *) kmnd;

    EXPECT_STREQ("foobar", input->core.name);
    EXPECT_STREQ("This is foobar", input->core.description);
    EXPECT_EQ(KMND_FLAGS_REQUIRED, input->flags);
    EXPECT_TRUE(validator == input->validator);
    EXPECT_EQ(0, kmnd_input_activated(input));
    EXPECT_EQ(1, kmnd_input_required(input));

    kmnd_free(kmnd);
}

TEST(InputFixture, Activate) {
    kmnd_t *kmnd = kmnd_input_new("foobar", "This is foobar",
                                  KMND_FLAGS_REQUIRED, validator);

    kmnd_input_t *input = (kmnd_input_t *) kmnd;

    EXPECT_EQ(0, kmnd_input_activate(input, kmnd, "seafood"));

    EXPECT_EQ(1, kmnd_input_activated(input));

    EXPECT_STREQ("seafood", kmnd_input_get(kmnd, NULL));

    kmnd_free(kmnd);
}

static int validator_fail(kmnd_t *kmnd, const char *string) {
    return -1;
}

TEST(InputFixture, ActivateFail) {
    kmnd_t *kmnd = kmnd_input_new("foobar", "This is foobar",
                                  KMND_FLAGS_REQUIRED, validator_fail);

    kmnd_input_t *input = (kmnd_input_t *) kmnd;

    EXPECT_NE(0, kmnd_input_activate(input, kmnd, "seafood"));

    EXPECT_EQ(0, kmnd_input_activated(input));

    EXPECT_TRUE(NULL == kmnd_input_get(kmnd, NULL));

    kmnd_free(kmnd);
}
