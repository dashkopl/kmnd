/*
 * Copyright (C) 2015-10-30, Tim van Elsloo
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

#include "../../src/terminal.h"

#include "malloc.h"

/*
 * The _default function should return a terminal with fd=stdout.
 */
TEST(TerminalFixture, DefaultFD) {
    KMND_MEM_LEAK_PRE();

    /* Get the default terminal. */
    kmnd_terminal_t *terminal = kmnd_terminal_new_default();

    /* Make sure that the FD is set to stdout. */
    EXPECT_EQ(STDOUT_FILENO, terminal->fd);

    /* Free the terminal. */
    kmnd_terminal_free(terminal);

    KMND_MEM_LEAK_POST();
}

/*
 * It should be possible to create a new terminal with a custom (non-stdout)
 * file descriptor.
 */
TEST(TerminalFixture, CustomFD) {
    KMND_MEM_LEAK_PRE();

    /* Create a new terminal. */
    kmnd_terminal_t *terminal = kmnd_terminal_new(STDERR_FILENO);

    /* Make sure that the FD is set to stderr. */
    EXPECT_EQ(STDERR_FILENO, terminal->fd);

    /* Free the terminal. */
    kmnd_terminal_free(terminal);

    KMND_MEM_LEAK_POST();
}

/*
 * Creating a new terminal should fail when passing an invalid file descriptor.
 */
TEST(TerminalFixture, InvalidFD) {
    KMND_MEM_LEAK_PRE();

    /* Create a new terminal. */
    kmnd_terminal_t *terminal = kmnd_terminal_new(-1);

    /* Make sure that no terminal is returned. */
    EXPECT_EQ(NULL, terminal);

    KMND_MEM_LEAK_POST();
}

/*
 * Every terminal should be memsetted to 0 when freed.
 */
TEST(TerminalFixture, Free) {
    KMND_MEM_LEAK_PRE();

    /* Create a new terminal. */
    kmnd_terminal_t *terminal = kmnd_terminal_new(STDERR_FILENO);

    /* Try to free the terminal. */
    kmnd_terminal_free(terminal);

    /* Check if it is freed. */
    EXPECT_FALSE(kmnd_mem_valid(terminal));

    KMND_MEM_LEAK_POST();
}
