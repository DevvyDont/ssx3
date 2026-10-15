#ifndef TESTS_CHECK_H
#define TESTS_CHECK_H

#include <stdio.h>

static int check_failures;

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (!(cond)) {                                                       \
            fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__, __LINE__, \
                    #cond);                                                  \
            check_failures++;                                                \
        }                                                                    \
    } while (0)

#define CHECK_DONE() (check_failures == 0 ? 0 : 1)

#endif
