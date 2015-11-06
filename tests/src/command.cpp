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

#include "../../src/command.h"

#include "malloc.h"

static int kmnd_test_run_called = 0;

static int kmnd_test_run(kmnd_t *kmnd) {
    kmnd_test_run_called ++;

    return 0;
}

TEST(CommandFixture, NoChildren) {
    KMND_MEM_LEAK_PRE();

    kmnd_command_t *kmnd = (kmnd_command_t *) kmnd_new("foobar", "This is the "
                                                       "description of foobar.",
                                                       kmnd_test_run, NULL);

    EXPECT_STREQ("foobar", kmnd->core.name);
    EXPECT_STREQ("This is the description of foobar.", kmnd->core.description);
    EXPECT_TRUE(kmnd_test_run == kmnd->run);

    kmnd_free((kmnd_t *) kmnd);

    KMND_MEM_LEAK_POST();
}

TEST(CommandFixture, BooleanOptions) {
    KMND_MEM_LEAK_PRE();

    kmnd_test_run_called = 0;

    kmnd_t *foo = kmnd_boolean_new('f', "foo", "This is foo.",
                                   KMND_FLAGS_NONE, 0),
           *bar = kmnd_boolean_new('b', "bar", "This is bar.",
                                   KMND_FLAGS_NONE, 0);


    kmnd_t *kmnd = kmnd_new("foobar", "This is the description of foobar.",
                            kmnd_test_run, foo, bar, NULL);

    const char *args[3] = { "kmnd", "-f", "-b" };

    EXPECT_EQ(0, kmnd_run(kmnd, 3, args));
    EXPECT_EQ(1, kmnd_boolean_get(kmnd, "foo"));
    EXPECT_EQ(1, kmnd_boolean_get(kmnd, "bar"));
    EXPECT_EQ(1, kmnd_test_run_called);

    kmnd_free(kmnd);

    KMND_MEM_LEAK_POST();
}

TEST(CommandFixture, StringOptions) {
    KMND_MEM_LEAK_PRE();

    kmnd_test_run_called = 0;

    kmnd_t *foo = kmnd_boolean_new('f', "foo", "This is foo.",
                                   KMND_FLAGS_NONE, 0),
    *bar = kmnd_string_new('b', "bar", "This is bar.",
                            KMND_FLAGS_NONE, "abc");


    kmnd_t *kmnd = kmnd_new("foobar", "This is the description of foobar.",
                            kmnd_test_run, foo, bar, NULL);

    const char *args[3] = { "kmnd", "-f", "-b=barfood" };

    EXPECT_EQ(0, kmnd_run(kmnd, 3, args));
    EXPECT_STREQ("barfood", kmnd_string_get(kmnd, "bar"));
    EXPECT_EQ(1, kmnd_test_run_called);

    kmnd_free(kmnd);

    KMND_MEM_LEAK_POST();
}

static int kmnd_test_subcommand_run_called = 0;

static int kmnd_test_subcommand_run(kmnd_t *kmnd) {
    kmnd_test_subcommand_run_called ++;

    return 0;
}

TEST(CommandFixture, Subcommands) {
    KMND_MEM_LEAK_PRE();

    kmnd_test_run_called = 0;
    kmnd_test_subcommand_run_called = 0;

    kmnd_t *foo = kmnd_boolean_new('f', "foo", "This is foo.",
                                   KMND_FLAGS_NONE, 0),
           *bar = kmnd_string_new('b', "bar", "This is bar.",
                                  KMND_FLAGS_NONE, "abc"),
           *xyz = kmnd_new("xyz", "This is the description of xyz.",
                           kmnd_test_subcommand_run, bar, NULL);

    kmnd_t *kmnd = kmnd_new("foobar", "This is the description of foobar.",
                            kmnd_test_run, foo, xyz, NULL);

    const char *args[4] = { "kmnd", "xyz", "-f", "-b=barfood" };

    EXPECT_EQ(0, kmnd_run(kmnd, 4, args));
    EXPECT_EQ(1, kmnd_boolean_get(kmnd, "foo"));
    EXPECT_STREQ("barfood", kmnd_string_get(kmnd, "xyz.bar"));
    EXPECT_EQ(1, kmnd_test_subcommand_run_called);
    EXPECT_EQ(0, kmnd_test_run_called);

    kmnd_free(kmnd);

    KMND_MEM_LEAK_POST();
}
