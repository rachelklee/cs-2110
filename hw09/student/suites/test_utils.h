#include <check.h>
#include <stdlib.h>
#include "../slack.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define tcase_hack(suite, setup_fixture, teardown_fixture, func)        \
    {                                                                   \
        TCase *tc = tcase_create(STRINGIFY(func));                      \
        tcase_add_checked_fixture(tc, setup_fixture, teardown_fixture); \
        tcase_add_test(tc, func);                                       \
        suite_add_tcase(s, tc);                                         \
    }

#define ck_assert_eq_msg(expected, actual, var_name, msg_prefix) (ck_assert_msg(expected == actual, "%sExpected `%s`: %d, actual: %d", msg_prefix, var_name, expected, actual))

#define ck_assert_enum_eq_msg(expected, actual, var_name, enum_str_arr, msg_prefix) (ck_assert_msg(expected == actual, "%sExpected `%s`: %s, actual: %s", msg_prefix, var_name, enum_str_arr[expected], enum_str_arr[actual]))

#define ck_assert_str_eq_msg(expected, actual, var_name, msg_prefix) (ck_assert_msg(!strcmp(expected, actual), "%sExpected `%s`: \"%s\", actual: \"%s\"", msg_prefix, var_name, expected, actual))

extern char *_malloc_string(const char *str);

extern Channel *_create_channel(void);

extern void _compare_post(Post *expected, Post *actual, char *msg_prefix);

extern void _compare_account(Account *expected, Account *actual, char *msg_prefix);

extern void _compare_channels(Channel *expected, Channel *actual, char *msg_prefix);

extern void _free_channel(Channel *channel);

extern void _add_accounts_to_channel(Channel *channel, Account *accounts, int numAccounts);

extern int _create_account(Channel *channel, const char *username, int accountID);

extern void _add_posts_to_channel(Channel *channel, Post *posts, int numPosts);
extern void _add_post_to_end_of_channel(Channel *channel, Post *post);

extern Channel *_duplicate_channel(Channel *channel);
