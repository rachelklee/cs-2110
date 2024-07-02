
#include <stdio.h>
#include <check.h>
#include <stddef.h>
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define RETURN_ERROR_VALUE -100
#include "mallocinfo.h"

// Suites
extern Suite *team_suite(void);

// Define a check containing only one test case of the same name
#define tcase_singleton(suite, setup_fixture, teardown_fixture, func)   \
    {                                                                   \
        TCase *tc = tcase_create(STRINGIFY(func));                      \
        tcase_add_checked_fixture(tc, setup_fixture, teardown_fixture); \
        tcase_add_test(tc, func);                                       \
        suite_add_tcase(s, tc);                                         \
    }

#include "../team.h"

// TODO: function declarations below

int _num_malloc_calls(void);
struct mallocinfo *_get_malloc_info(void*);
struct player * _create_player(char *name, int age, int number);
struct roster * _create_roster(int capacity, int size);
