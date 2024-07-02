#include "test_utils.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The number of mallocs in user code until the next one fails, or -1 for
// successful mallocs
int mallocs_until_fail = -1;

static void reset_mallocs_until_fail(void)
{
    mallocs_until_fail = -1;
}

// create_channel() tests
START_TEST(test_create_channel_basic)
{
    Channel *c = createChannel();
    ck_assert_msg(c, "createChannel() should not return NULL");
    ck_assert_msg(c->numUsers == 0, "createChannel() should be initialized with zero users");
    ck_assert_msg(!c->users, "createChannel() should initialize `users` with NULL");
    ck_assert_msg(!c->posts.size, "createChannel() should initialize `posts` to have a size of zero");
    ck_assert_msg(!c->posts.head, "createChannel() should initialize `posts` with a NULL head");
    _free_channel(c);
}
END_TEST

START_TEST(test_create_channel_malloc_failure)
{
    mallocs_until_fail = 0;
    Channel *c = createChannel();
    ck_assert_msg(!c, "createChannel() imporperly handles malloc failure");
}
END_TEST

// ******************************************************************************
// createAccount tests
// ******************************************************************************

// Null channel test
START_TEST(test_create_account_null_channel)
{
    Channel *channel = NULL;
    const char *username = "Greg";
    int accountID = 1;

    ck_assert_msg(createAccount(channel, username, accountID) == FAILURE, "createAccount should return FAILURE when channel is NULL");
}
END_TEST

// Null username test
START_TEST(test_create_account_null_username)
{
    Channel *channel = _create_channel();
    const char *username = NULL;
    int accountID = 1;

    ck_assert_msg(createAccount(channel, username, accountID) == FAILURE, "createAccount should return FAILURE when username is NULL");
    _free_channel(channel);
}
END_TEST

// Users array is null
START_TEST(test_create_account_from_empty)
{
    Channel *channel = _create_channel();
    const char *username = "Greg";
    int accountID = 1;

    // Create excepted
    Channel *expected = _create_channel();
    _create_account(expected, username, accountID);

    // Ensure that it returns right thing
    ck_assert_msg(createAccount(channel, username, accountID) != FAILURE, "createAccount should not return FAIlURE with valid username and valid channel");
    // Check channels are the same
    _compare_channels(expected, channel, "");

    _free_channel(channel);
    _free_channel(expected);
}
END_TEST

START_TEST(test_create_account_malloc_failure)
{
    Channel *c = _create_channel();
    Channel *expected_c = _duplicate_channel(c);

    // createAccount requires 3 malloc calls to be successful

    mallocs_until_fail = 0;
    ck_assert_msg(createAccount(c, "test", 1) == FAILURE, "createAccount should return FAILURE whenever malloc fails");
    _compare_channels(expected_c, c, "createAccount should not alter the channel: ");

    mallocs_until_fail = 1;
    ck_assert_msg(createAccount(c, "test", 1) == FAILURE, "createAccount should return FAILURE whenever malloc fails");
    _compare_channels(expected_c, c, "createAccount should not alter the channel: ");

    mallocs_until_fail = 2;
    ck_assert_msg(createAccount(c, "test", 1) == FAILURE, "createAccount should return FAILURE whenever malloc fails");
    _compare_channels(expected_c, c, "createAccount should not alter the channel: ");

    mallocs_until_fail = 3;
    ck_assert_msg(createAccount(c, "test", 1) == SUCCESS, "createAccount should be successful if malloc does not fail");
    Account expected_accounts[] = {{"test", 1}};
    _add_accounts_to_channel(expected_c, expected_accounts, 1);
    _compare_channels(expected_c, c, "");

    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_create_account_realloc_failure)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"test", 0}, {"rso31", 1}};
    _add_accounts_to_channel(c, accounts, 2);
    Channel *expected_c = _duplicate_channel(c);

    // if accounts is not null, we wait for two malloc calls until they have to call realloc to expand the arraylist
    mallocs_until_fail = 2;
    ck_assert_msg(createAccount(c, "pgupta359", 3) == FAILURE, "createAccount should return FAILURE whenever malloc fails");
    _compare_channels(expected_c, c, "createAccount should not alter the channel: ");

    mallocs_until_fail = 3;
    ck_assert_msg(createAccount(c, "pgupta359", 3) == SUCCESS, "createAccount should be successful if malloc does not fail");
    _create_account(expected_c, "pgupta359", 3);
    _compare_channels(expected_c, c, "");

    // try again with another account to make sure they realloc again
    mallocs_until_fail = 2;
    ck_assert_msg(createAccount(c, "rbafna3", 4) == FAILURE, "createAccount should return FAILURE whenever malloc fails");
    mallocs_until_fail = 3;
    ck_assert_msg(createAccount(c, "rbafna3", 4) == SUCCESS, "createAccount should be successful if malloc does not fail");
    _create_account(expected_c, "rbafna3", 4);
    _compare_channels(expected_c, c, "");

    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

// Short Not Duplicate Check (4 - 5 users)
START_TEST(test_create_account_no_duplicates_short)
{
    Channel *channel = _create_channel();

    Channel *expected = _create_channel();
    // Add Accounts to channel
    // Create Accounts first
    char *usernames[4] = {"Richard", "Rohan", "Greg", "Jayden"};
    int ids[4] = {0, 1, 2, 3};

    for (int i = 0; i < 4; i++)
    {
        // Expected
        int expected_result = _create_account(expected, usernames[i], ids[i]);

        // Student
        int actual_result = createAccount(channel, usernames[i], ids[i]);

        ck_assert_msg(expected_result == actual_result, "Expected result did not equal actual result");

        // Compare
        _compare_channels(expected, channel, "");
    }

    _free_channel(channel);
    _free_channel(expected);
}
END_TEST

// Long Not Duplicate Check (10 - 20 users)
START_TEST(test_create_account_no_duplicates_long)
{
    Channel *channel = _create_channel();

    Channel *expected = _create_channel();
    // Add Accounts to channel
    // Create Accounts first
    char *usernames[17] = {"Justin", "Jeff", "Richard", "Marto", "Sophie", "Gal", "Sarah", "Saloni", "Alex", "Annelise", "Darren", "Gilbert", "Dan", "Aaron", "Lydia", "Prabhav", "Heather"};
    int ids[17] = {5, 23, 45, 3, 32, 12, 78, 90, 56, 2, 7, 1, 58, 54, 21, 66, 70};

    for (int i = 0; i < 17; i++)
    {
        // Expected
        int expected_result = _create_account(expected, usernames[i], ids[i]);

        // Student
        int actual_result = createAccount(channel, usernames[i], ids[i]);

        ck_assert_msg(expected_result == actual_result, "Expected result did not equal actual result");

        // Compare
        _compare_channels(expected, channel, "");
    }

    _free_channel(channel);
    _free_channel(expected);
}
END_TEST

// Short Duplicate Check (4 - 5 users)
START_TEST(test_create_account_duplicates_short)
{
    Channel *channel = _create_channel();

    Channel *expected = _create_channel();
    // Add Accounts to channel
    // Create Accounts first
    char *usernames[4] = {"Richard", "Richard", "Richard", "Greg"};
    int ids[4] = {0, 0, 0, 2110};

    for (int i = 0; i < 4; i++)
    {
        // Expected
        int expected_result = _create_account(expected, usernames[i], ids[i]);

        // Student
        int actual_result = createAccount(channel, usernames[i], ids[i]);

        ck_assert_msg(expected_result == actual_result, "Expected result did not equal actual result");

        // Compare
        _compare_channels(expected, channel, "");
    }

    _free_channel(channel);
    _free_channel(expected);
}
END_TEST

// Long Duplicate Check (10 - 20 users)
START_TEST(test_create_account_duplicates_long)
{
    Channel *channel = _create_channel();

    Channel *expected = _create_channel();
    // Add Accounts to channel
    // Create Accounts first
    char *usernames[17] = {"Justin", "Jeff", "Justin", "Marto", "Sophie", "Gal", "Sophie", "Saloni", "Alex", "Alex", "Darren", "Gilbert", "Dan", "Justin", "Lydia", "Prabhav", "Heather"};
    int ids[17] = {5, 23, 5, 3, 32, 12, 32, 90, 56, 56, 7, 1, 4, 54, 21, 66, 5};

    for (int i = 0; i < 17; i++)
    {
        // Expected
        int expected_result = _create_account(expected, usernames[i], ids[i]);

        // Student
        int actual_result = createAccount(channel, usernames[i], ids[i]);

        // Check return vals
        ck_assert_msg(expected_result == actual_result, "Expected result did not equal actual result");

        // Compare
        _compare_channels(expected, channel, "");
    }

    _free_channel(channel);
    _free_channel(expected);
}
END_TEST

// add_reaction() tests
START_TEST(test_add_reaction_from_empty)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}};
    int numAccounts = 2;
    _add_accounts_to_channel(c, accounts, numAccounts);
    Channel *expected_c = _duplicate_channel(c);

    Post posts[] = {
        {0, 0, "hello, world", {{0}}, 0}};
    int numPosts = 1;
    _add_posts_to_channel(c, posts, numPosts);

    Post posts_expected[] = {
        {0, 0, "hello, world", {{1, THUMBS_UP}}, 1}};
    _add_posts_to_channel(expected_c, posts_expected, numPosts);

    ck_assert_msg(!addReaction(c->posts.head->data, 1, THUMBS_UP), "addReaction should return SUCCESS if args are valid");
    _compare_channels(expected_c, c, "");
    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_add_reaction_basic)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}};
    int numAccounts = 3;
    _add_accounts_to_channel(c, accounts, numAccounts);
    Channel *expected_c = _duplicate_channel(c);

    Post posts[] = {
        {0, 0, "hello, world", {{1, THUMBS_UP}, {0, SKULL}}, 2}};
    int numPosts = 1;
    _add_posts_to_channel(c, posts, numPosts);

    Post posts_expected[] = {
        {0, 0, "hello, world", {{1, THUMBS_UP}, {0, SKULL}, {2, SMILEY}}, 3}};
    _add_posts_to_channel(expected_c, posts_expected, numPosts);

    ck_assert_msg(!addReaction(c->posts.head->data, 2, SMILEY), "addReaction should return SUCCESS if args are valid");
    _compare_channels(expected_c, c, "");
    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_add_reaction_multiple_posts)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}, {"pgupta359", 3}, {"rbafna8", 5}};
    int numAccounts = 5;
    _add_accounts_to_channel(c, accounts, numAccounts);
    Channel *expected_c = _duplicate_channel(c);

    Post posts[] = {
        {0, 0, "hello, world", {{0, SMILEY}, {1, SKULL}}, 2},
        {1, 0, "become a TA!", {{5, SKULL}, {1, THUMBS_UP}, {2, SMILEY}}, 3},
        {2, 0, "hw9 best hw", {{1, SKULL}}, 1}};
    int numPosts = 3;
    _add_posts_to_channel(c, posts, numPosts);

    Post posts_expected[] = {
        {0, 0, "hello, world", {{0, SMILEY}, {1, SKULL}, {3, THUMBS_UP}}, 3},
        {1, 0, "become a TA!", {{5, SKULL}, {1, THUMBS_UP}, {2, SMILEY}, {3, HEART}, {0, SKULL}}, 5},
        {2, 0, "hw9 best hw", {{1, SKULL}, {2, SMILEY}}, 2}};
    _add_posts_to_channel(expected_c, posts_expected, numPosts);

    Post *post1 = c->posts.head->data;
    Post *post2 = c->posts.head->next->data;
    Post *post3 = c->posts.head->next->next->data;

    ck_assert_msg(!addReaction(post2, 3, HEART), "addReaction should return SUCCESS if args are valid");
    ck_assert_msg(!addReaction(post3, 2, SMILEY), "addReaction should return SUCCESS if args are valid");
    ck_assert_msg(!addReaction(post1, 3, THUMBS_UP), "addReaction should return SUCCESS if args are valid");
    ck_assert_msg(!addReaction(post2, 0, SKULL), "addReaction should return SUCCESS if args are valid");

    _compare_channels(expected_c, c, "");
    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_add_reaction_invalid_arg_checks)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}};
    int numAccounts = 2;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {0, 0, "hello, world", {{0}}, 0}};
    int numPosts = 1;
    _add_posts_to_channel(c, posts, numPosts);

    Channel *expected_c = _duplicate_channel(c);

    ck_assert_msg(addReaction(NULL, 0, SKULL) == FAILURE, "addReaction() should return FAILURE when given NULL pointer as post");

    _compare_channels(expected_c, c, "Channel has been incorrectly modified: ");
    _free_channel(expected_c);
    _free_channel(c);
}
END_TEST

START_TEST(test_add_reaction_duplicate_accountid)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}};
    int numAccounts = 3;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {0, 0, "hello, world", {{1, THUMBS_UP}, {0, SMILEY}}, 2}};
    int numPosts = 1;
    _add_posts_to_channel(c, posts, numPosts);

    Channel *expected_c = _duplicate_channel(c);

    ck_assert_msg(addReaction(c->posts.head->data, 1, SKULL) == FAILURE, "addReaction() should return FAILURE when a reaction with the same accountID already exists");

    _compare_channels(expected_c, c, "Channel has been incorrectly modified: ");
    _free_channel(expected_c);
    _free_channel(c);
}
END_TEST

START_TEST(test_add_reaction_array_limit)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}, {"lol", 3}, {"lmfao", 4}, {"qwerty", 5}, {"pgupta359", 6}, {"ggould8", 7}, {"rbafna8", 8}, {"buzz", 9}, {"bruh", 10}};
    int numAccounts = 11;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {0, 0, "hello, world", {{2, THUMBS_UP}, {0, SKULL}, {1, SKULL}, {10, SKULL}, {5, SMILEY}, {6, SOBBING}, {7, SOBBING}, {3, THUMBS_UP}, {9, HEART}, {4, HEART}}, 10}};
    int numPosts = 1;
    _add_posts_to_channel(c, posts, numPosts);

    Channel *expected_c = _duplicate_channel(c);

    ck_assert_msg(addReaction(c->posts.head->data, 8, THUMBS_UP) == FAILURE, "addReaction() should return FAILURE when reactions array is already at max capacity");

    _compare_channels(expected_c, c, "Channel has been incorrectly modified: ");
    _free_channel(expected_c);
    _free_channel(c);
}
END_TEST

// ******************************************************************************
// findUser tests
// ******************************************************************************

// Null Channel Test
START_TEST(test_find_user_null_channel)
{
    Channel *channel = NULL;

    // Search for specific TA
    Account *student_result = findUser(channel, 1);

    // Check NULL
    ck_assert_msg(student_result == NULL, "findUser should return NULL when channel is NULL");
    // Check channel unchanged
    ck_assert_msg(channel == NULL, "Channel was changed during search");
}
END_TEST

// Don't find in short user list
START_TEST(test_find_user_not_found_short_list)
{
    Channel *channel = _create_channel();

    char *usernames[4] = {"Richard", "Rohan", "Greg", "Jayden"};
    int ids[4] = {0, 1, 2, 3};

    for (int i = 0; i < 4; i++)
    {
        // Create channel user works with
        _create_account(channel, usernames[i], ids[i]);
    }
    Channel *expected = _duplicate_channel(channel);

    // Search for specific TA
    Account *student_result = findUser(channel, 4);

    // Check NULL
    ck_assert_msg(student_result == NULL, "findUser should return NULL when user is not in channel");
    // Check channel unchanged
    _compare_channels(expected, channel, "Channel was changed during findUser(): ");

    _free_channel(channel);
    _free_channel(expected);
}
END_TEST

// Don't find in long user list
START_TEST(test_find_user_not_found_long_list)
{
    Channel *channel = _create_channel();

    char *usernames[17] = {"Justin", "Jeff", "Richard", "Marto", "Sophie", "Gal", "Sarah", "Saloni", "Alex", "Annelise", "Darren", "Gilbert", "Dan", "Aaron", "Lydia", "Prabhav", "Heather"};
    int ids[17] = {5, 23, 45, 3, 32, 12, 78, 90, 56, 2, 7, 1, 58, 54, 21, 66, 70};

    for (int i = 0; i < 17; i++)
    {
        // Create channel user works with
        _create_account(channel, usernames[i], ids[i]);
    }
    Channel *expected = _duplicate_channel(channel);

    // Search for specific TA
    Account *student_result = findUser(channel, 99);

    // Check NULL
    ck_assert_msg(student_result == NULL, "findUser should return NULL when user is not in channel");
    // Check channel unchanged
    _compare_channels(expected, channel, "Channel was changed during findUser(): ");

    _free_channel(channel);
    _free_channel(expected);
}
END_TEST

// Find in short users list
START_TEST(test_find_user_found_short_list)
{
    Channel *channel = _create_channel();

    char *usernames[4] = {"Richard", "Rohan", "Greg", "Jayden"};
    int ids[4] = {0, 1, 2, 3};

    for (int i = 0; i < 4; i++)
    {
        // Create channel user works with
        _create_account(channel, usernames[i], ids[i]);
    }
    Channel *expected = _duplicate_channel(channel);

    // Check if right acccount
    for (int i = 0; i < 4; i++)
        ck_assert_ptr_eq(channel->users[i], findUser(channel, ids[i]));

    // Check channel unchanged
    _compare_channels(expected, channel, "Channel was changed during findUser(): ");

    _free_channel(channel);
    _free_channel(expected);
}
END_TEST

// Find in long users list
START_TEST(test_find_user_found_long_list)
{
    Channel *channel = _create_channel();

    char *usernames[17] = {"Justin", "Jeff", "Richard", "Marto", "Sophie", "Gal", "Sarah", "Saloni", "Alex", "Annelise", "Darren", "Gilbert", "Dan", "Aaron", "Lydia", "Prabhav", "Heather"};
    int ids[17] = {5, 23, 45, 3, 32, 12, 78, 90, 56, 2, 7, 1, 58, 54, 21, 66, 70};

    for (int i = 0; i < 17; i++)
    {
        // Create channel user works with
        _create_account(channel, usernames[i], ids[i]);
    }
    Channel *expected = _duplicate_channel(channel);

    // Check if right acccount
    for (int i = 0; i < 17; i++)
        ck_assert_ptr_eq(channel->users[i], findUser(channel, ids[i]));

    // Check channel unchanged
    _compare_channels(expected, channel, "Channel was changed during findUser(): ");

    _free_channel(channel);
    _free_channel(expected);
}
END_TEST

// search_palindrome() tests
START_TEST(test_search_palindrome_basic)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}};
    int numAccounts = 2;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {0, 0, "hello, world", {{0}}, 0},
        {1, 1, "racecar", {{0}}, 0},
        {2, 0, "not a palindrome", {{0}}, 0}};
    int numPosts = 3;
    _add_posts_to_channel(c, posts, numPosts);
    Channel *expected_c = _duplicate_channel(c);

    int ans_ind = -1;
    Post *student_ans = searchForPalindrome(c, &ans_ind);

    ck_assert_ptr_eq(c->posts.head->next->data, student_ans);
    ck_assert_eq_msg(1, ans_ind, "index (out variable)", "");

    _compare_channels(expected_c, c, "Channel has been incorrectly modified: ");
    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_search_palindrome_spaces_specials)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}};
    int numAccounts = 2;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {0, 0, "hello, world", {{0}}, 0},
        {2, 0, "not a palindrome", {{0}}, 0},
        {1, 1, ".test asdf!, bob ,!fdsa tset.", {{0}}, 0}};
    int numPosts = 3;
    _add_posts_to_channel(c, posts, numPosts);
    Channel *expected_c = _duplicate_channel(c);

    int ans_ind = -1;
    Post *student_ans = searchForPalindrome(c, &ans_ind);

    ck_assert_ptr_eq(c->posts.head->next->next->data, student_ans);
    ck_assert_eq_msg(2, ans_ind, "index (out variable)", "");

    _compare_channels(expected_c, c, "Channel has been incorrectly modified: ");
    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_search_palindrome_multiple_matches)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}};
    int numAccounts = 2;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {1, 1, "racecar", {{0}}, 0},
        {0, 0, "hello, world", {{0}}, 0},
        {2, 0, "not a palindrome", {{0}}, 0},
        {3, 1, "asdffdsa", {{0}}, 0}};
    int numPosts = 4;
    _add_posts_to_channel(c, posts, numPosts);
    Channel *expected_c = _duplicate_channel(c);

    int ans_ind = -1;
    Post *student_ans = searchForPalindrome(c, &ans_ind);

    // should return the first post seen that's a palindrome
    ck_assert_ptr_eq(c->posts.head->data, student_ans);
    ck_assert_eq_msg(0, ans_ind, "index (out variable)", "");

    _compare_channels(expected_c, c, "Channel has been incorrectly modified: ");
    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_search_palindrome_invalid_args)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}};
    int numAccounts = 2;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {0, 0, "hello, world", {{0}}, 0},
        {1, 1, "racecar", {{0}}, 0},
        {2, 0, "not a palindrome", {{0}}, 0}};
    int numPosts = 3;
    _add_posts_to_channel(c, posts, numPosts);
    Channel *expected_c = _duplicate_channel(c);

    int ans_ind = -1;
    Post *student_ans = searchForPalindrome(NULL, &ans_ind);

    ck_assert_ptr_null(student_ans);
    ck_assert_eq_msg(-1, ans_ind, "index (out variable)", "Out variable was modified: ");

    _compare_channels(expected_c, c, "Channel has been incorrectly modified: ");
    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_search_palindrome_not_found)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}};
    int numAccounts = 2;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {0, 0, "hello, world", {{0}}, 0},
        {1, 1, "bruh moment", {{0}}, 0},
        {2, 0, "not a palindrome", {{0}}, 0}};
    int numPosts = 3;
    _add_posts_to_channel(c, posts, numPosts);
    Channel *expected_c = _duplicate_channel(c);

    int ans_ind = rand();
    int expected_ind = ans_ind;
    Post *student_ans = searchForPalindrome(c, &ans_ind);

    ck_assert_ptr_null(student_ans);
    ck_assert_eq_msg(expected_ind, ans_ind, "index (out variable)", "Out variable was modified: ");

    _compare_channels(expected_c, c, "Channel has been incorrectly modified: ");
    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

// delete_reaction() tests
START_TEST(test_delete_reaction_basic)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}};
    int numAccounts = 3;
    _add_accounts_to_channel(c, accounts, numAccounts);
    Channel *expected_c = _duplicate_channel(c);

    Post posts[] = {
        {0, 0, "hello, world", {{1, THUMBS_UP}, {0, SKULL}, {2, SMILEY}}, 3}};
    int numPosts = 1;
    _add_posts_to_channel(c, posts, numPosts);

    Post posts_expected[] = {
        {0, 0, "hello, world", {{1, THUMBS_UP}, {2, SMILEY}}, 2}};
    _add_posts_to_channel(expected_c, posts_expected, numPosts);

    ck_assert_msg(deleteReaction(c->posts.head->data, 0, SKULL) == SUCCESS, "deleteReaction should return SUCCESS if the reaction is found");
    _compare_channels(expected_c, c, "");
    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_delete_reaction_multiple_posts)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}, {"pgupta359", 3}, {"rbafna3", 5}};
    int numAccounts = 5;
    _add_accounts_to_channel(c, accounts, numAccounts);
    Channel *expected_c = _duplicate_channel(c);

    Post posts[] = {
        {0, 0, "hello, world", {{0, SMILEY}, {1, SKULL}, {3, THUMBS_UP}}, 3},
        {1, 0, "become a TA!", {{5, SKULL}, {1, THUMBS_UP}, {0, SKULL}, {2, SMILEY}, {3, HEART}}, 5},
        {2, 0, "hw9 best hw", {{1, SKULL}}, 1}};
    int numPosts = 3;
    _add_posts_to_channel(c, posts, numPosts);

    Post posts_expected[] = {
        {0, 0, "hello, world", {{1, SKULL}}, 1},
        {1, 0, "become a TA!", {{1, THUMBS_UP}, {2, SMILEY}}, 2},
        {2, 0, "hw9 best hw", {{0}}, 0}};
    _add_posts_to_channel(expected_c, posts_expected, numPosts);

    Post *post1 = c->posts.head->data;
    Post *post2 = c->posts.head->next->data;
    Post *post3 = c->posts.head->next->next->data;

    ck_assert_msg(!deleteReaction(post1, 3, THUMBS_UP), "deleteReaction should return SUCCESS if the reaction is found");
    ck_assert_msg(!deleteReaction(post2, 5, SKULL), "deleteReaction should return SUCCESS if the reaction is found");
    ck_assert_msg(!deleteReaction(post2, 0, SKULL), "deleteReaction should return SUCCESS if the reaction is found");
    ck_assert_msg(!deleteReaction(post1, 0, SMILEY), "deleteReaction should return SUCCESS if the reaction is found");
    ck_assert_msg(!deleteReaction(post3, 1, SKULL), "deleteReaction should return SUCCESS if the reaction is found");
    ck_assert_msg(!deleteReaction(post2, 3, HEART), "deleteReaction should return SUCCESS if the reaction is found");

    _compare_channels(expected_c, c, "");
    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_delete_reaction_invalid_arg_checks)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}};
    int numAccounts = 2;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {0, 0, "hello, world", {{0}}, 0}};
    int numPosts = 1;
    _add_posts_to_channel(c, posts, numPosts);

    Channel *expected_c = _duplicate_channel(c);

    ck_assert_msg(deleteReaction(NULL, 0, SKULL) == FAILURE, "deleteReaction() should return FAILURE when given NULL pointer as post");

    _compare_channels(expected_c, c, "Channel has been incorrectly modified: ");
    _free_channel(expected_c);
    _free_channel(c);
}
END_TEST

START_TEST(test_delete_reaction_not_found)
{
    Channel *c = _create_channel();
    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}};
    int numAccounts = 3;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {0, 0, "hello, world", {{1, THUMBS_UP}, {0, SMILEY}}, 2}};
    int numPosts = 1;
    _add_posts_to_channel(c, posts, numPosts);

    Channel *expected_c = _duplicate_channel(c);

    ck_assert_msg(deleteReaction(c->posts.head->data, 2, HEART) == FAILURE, "deleteReaction() should return FAILURE when the reaction is not found");
    ck_assert_msg(deleteReaction(c->posts.head->data, 1, SKULL) == FAILURE, "deleteReaction() should return FAILURE when the reaction is not found (check against BOTH reaction and accountID)");

    _compare_channels(expected_c, c, "Channel has been incorrectly modified: ");
    _free_channel(expected_c);
    _free_channel(c);
}
END_TEST

// ******************************************************************************
// deleteAccount tests
// ******************************************************************************

// Null Channel Test Case
START_TEST(test_delete_account_null)
{
    Channel *channel = NULL;

    ck_assert_msg(deleteAccount(channel, 32) == FAILURE, "deleteAccount should return FAILURE when channel is NULL");
}
END_TEST

// Not Found Case
START_TEST(test_delete_account_not_found)
{
    Channel *channel = _create_channel();

    // Create accoumts
    char *usernames[4] = {"Richard", "Rohan", "Greg", "Jayden"};
    int ids[4] = {0, 1, 2, 3};

    for (int i = 0; i < 4; i++)
    {
        // Create channel user works with
        _create_account(channel, usernames[i], ids[i]);
    }
    Channel *expected = _duplicate_channel(channel);

    // Check returns FAILURE
    ck_assert_msg(deleteAccount(channel, 32) == FAILURE, "deleteAccount should return FAILURE when Account is not found");

    // Check Channel has not changed
    _compare_channels(expected, channel, "Channel was changed when deleteAccount() couldn't find the target account: ");

    _free_channel(channel);
    _free_channel(expected);
}
END_TEST

// First Element in User Array Test Case
START_TEST(test_delete_account_first)
{
    Channel *c = _create_channel();
    Channel *expected_c = _duplicate_channel(c);

    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}, {"pgupta359", 3}, {"ggould8", 4}};

    Account accounts_expected[] = {{"rso31", 1}, {"asdf", 2}, {"pgupta359", 3}, {"ggould8", 4}};
    int numAccounts = 5;
    _add_accounts_to_channel(c, accounts, numAccounts);
    _add_accounts_to_channel(expected_c, accounts_expected, numAccounts - 1);

    ck_assert_msg(deleteAccount(c, 0) == SUCCESS, "deleteAccount() should return SUCCESS");

    _compare_channels(expected_c, c, "");

    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

// Last Element in User Array Test Case
START_TEST(test_delete_account_last)
{
    Channel *c = _create_channel();
    Channel *expected_c = _duplicate_channel(c);

    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}};

    Account accounts_expected[] = {{"testing", 0}, {"rso31", 1}};
    int numAccounts = 3;
    _add_accounts_to_channel(c, accounts, numAccounts);
    _add_accounts_to_channel(expected_c, accounts_expected, numAccounts - 1);

    ck_assert_msg(deleteAccount(c, 2) == SUCCESS, "deleteAccount() should return SUCCESS");

    _compare_channels(expected_c, c, "");

    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

// Remove posts as well
START_TEST(test_delete_account_with_posts)
{
    Channel *c = _create_channel();
    Channel *expected_c = _duplicate_channel(c);

    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}};

    Account accounts_expected[] = {{"testing", 0}, {"asdf", 2}};
    int numAccounts = 3;
    _add_accounts_to_channel(c, accounts, numAccounts);
    _add_accounts_to_channel(expected_c, accounts_expected, numAccounts - 1);

    Post posts[] = {
        {0, 0, "hello, world", {{0, SKULL}, {2, SMILEY}}, 2},
        {1, 1, "hooray", {{0}}, 0},
        {1, 2, "hi", {{2, HEART}}, 1},
    };
    int numPosts = 3;
    _add_posts_to_channel(c, posts, numPosts);

    Post posts_expected[] = {
        {0, 0, "hello, world", {{0, SKULL}, {2, SMILEY}}, 2},
        {1, 2, "hi", {{2, HEART}}, 1},
    };
    _add_posts_to_channel(expected_c, posts_expected, numPosts - 1);

    ck_assert_msg(deleteAccount(c, 1) == SUCCESS, "deleteAccount() should return SUCCESS");

    _compare_channels(expected_c, c, "");

    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

// Remove posts and reactions
START_TEST(test_delete_account_with_posts_and_reactions)
{
    Channel *c = _create_channel();
    Channel *expected_c = _duplicate_channel(c);

    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"asdf", 2}};

    Account accounts_expected[] = {{"testing", 0}, {"asdf", 2}};
    int numAccounts = 3;
    _add_accounts_to_channel(c, accounts, numAccounts);
    _add_accounts_to_channel(expected_c, accounts_expected, numAccounts - 1);

    Post posts[] = {
        {0, 0, "hello, world", {{0, SKULL}, {1, HEART}, {2, SMILEY}}, 3},
        {1, 1, "hooray", {{1, THUMBS_UP}}, 1},
        {1, 2, "hi", {{2, HEART}, {1, SOBBING}}, 2},
    };
    int numPosts = 3;
    _add_posts_to_channel(c, posts, numPosts);

    Post posts_expected[] = {
        {0, 0, "hello, world", {{0, SKULL}, {2, SMILEY}}, 2},
        {1, 2, "hi", {{2, HEART}}, 1},
    };
    _add_posts_to_channel(expected_c, posts_expected, numPosts - 1);

    ck_assert_msg(deleteAccount(c, 1) == SUCCESS, "deleteAccount() should return SUCCESS");

    _compare_channels(expected_c, c, "");

    _free_channel(c);
    _free_channel(expected_c);
}
END_TEST

// delete_channel() tests
START_TEST(test_delete_channel_null_pointer)
{
    deleteChannel(NULL);
    // nothing to assert -- ensure no segfaults
}
END_TEST

START_TEST(test_delete_channel_empty)
{
    Channel *c = _create_channel();
    deleteChannel(c);
    // nothing to assert
}
END_TEST

START_TEST(test_delete_channel_with_accounts)
{
    Channel *c = _create_channel();

    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"pgupta359", 2}};
    int numAccounts = 3;
    _add_accounts_to_channel(c, accounts, numAccounts);

    deleteChannel(c);
    // nothing to assert
}
END_TEST

START_TEST(test_delete_channel_full)
{
    Channel *c = _create_channel();

    Account accounts[] = {{"testing", 0}, {"rso31", 1}, {"pgupta359", 3}, {"ggould8", 4}, {"rbafna3", 5}};
    int numAccounts = 5;
    _add_accounts_to_channel(c, accounts, numAccounts);

    Post posts[] = {
        {0, 0, "hello, world", {{1, SKULL}, {2, SMILEY}}, 2},
        {1, 0, "testing", {{0, THUMBS_UP}}, 1},
        {2, 1, "cowabunga", {{0, SOBBING}, {2, SMILEY}, {1, HEART}}, 3},
        {4, 3, "rso31 is a horrible TA", {{1, SKULL}, {3, THUMBS_UP}, {4, THUMBS_UP}, {5, THUMBS_UP}}, 4}};
    int numPosts = 4;
    _add_posts_to_channel(c, posts, numPosts);

    deleteChannel(c);
    // nothing to assert
}
END_TEST

// *********************************************************************
// createPost tests
// *********************************************************************

START_TEST(test_create_post_null_channel)
{
    ck_assert_msg(createPost(NULL, "you will never see this", 10, 20) == FAILURE,
                  "createPost should return FAILURE when channel is NULL");
}
END_TEST

START_TEST(test_create_post_null_text)
{
    Channel *c = _create_channel();
    Channel *gt = _create_channel();
    ck_assert_msg(createPost(c, NULL, 10, 20) == FAILURE,
                  "createPost should return FAILURE when text is NULL");
    _compare_channels(gt, c,
                      "createPost should not modify channel on failure: ");
    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_create_post_account_not_found)
{
    Channel *c = _create_channel();
    _create_account(c, "real person #1", 1);
    _create_account(c, "real person #2", 2);
    // real person #3 went missing, and was never heard from again ...
    _create_account(c, "real person #4", 4);
    _create_account(c, "real person #5", 5);

    Channel *gt = _create_channel();
    _create_account(gt, "real person #1", 1);
    _create_account(gt, "real person #2", 2);
    _create_account(gt, "real person #4", 4);
    _create_account(gt, "real person #5", 5);

    ck_assert_msg(createPost(c, "ooo i'm a ghost!", 3, 1) == FAILURE,
                  "createPost should return FAILURE when senderID is not "
                  "an account in the channel");
    _compare_channels(gt, c,
                      "createPost should not modify channel on failure: ");
    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_create_post_postid_not_unique)
{
    Channel *c = _create_channel();
    _create_account(c, "real person #1", 1);
    _create_account(c, "real person #2", 2);
    Post posts[] = {{10, 1, "Marco!", {{0}}, 0},
                    {20, 2, "Polo!", {{0}}, 0},
                    {30, 1, "Marco!", {{0}}, 0},
                    {40, 1, "Marco!", {{0}}, 0},
                    {50, 2, "Polo!", {{0}}, 0}};
    _add_posts_to_channel(c, posts, 5);

    Channel *gt = _create_channel();
    _create_account(gt, "real person #1", 1);
    _create_account(gt, "real person #2", 2);
    _add_posts_to_channel(gt, posts, 5);

    ck_assert_msg(createPost(c, "Polo!", 2, 40) == FAILURE,
                  "createPost should return FAILURE when postID is not "
                  "unique");
    _compare_channels(gt, c,
                      "createPost should not modify channel on failure: ");
    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_create_post_malloc_failure)
{
    Channel *c = _create_channel();
    _create_account(c, "the only real person", 1);

    Channel *gt = _create_channel();
    _create_account(gt, "the only real person", 1);
    char *msg = "I'm all alone ...";

    // 3 things need to be malloced: the post, the text, and the node.
    for (int i = 0; i < 3; ++i)
    {
        mallocs_until_fail = i;
        ck_assert_msg(createPost(c, msg, 1, 1) == FAILURE,
                      "createPost should return FAILURE when malloc fails");
        _compare_channels(gt, c,
                          "createPost should not modify channel on failure: ");
    }

    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_create_first_post)
{
    Channel *c = _create_channel();
    _create_account(c, "the only real person", 1);

    Channel *gt = _create_channel();
    _create_account(gt, "the only real person", 1);
    char *msg = "I'm all alone ...";
    Post post = {1, 1, msg, {{0}}, 0};
    _add_posts_to_channel(gt, &post, 1);

    ck_assert_msg(createPost(c, msg, 1, 1) == SUCCESS,
                  "createPost should return SUCCESS on success");
    _compare_channels(gt, c,
                      "createPost returned SUCCESS, but: ");
    // Make sure the string was actually malloced.
    ck_assert_msg(c->posts.head->data->text != msg,
                  "createPost did not copy the post text");

    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_create_many_posts)
{
    Channel *c = _create_channel();
    _create_account(c, "real person #1", 1);
    _create_account(c, "real person #2", 2);

    Channel *gt = _create_channel();
    _create_account(gt, "real person #1", 1);
    _create_account(gt, "real person #2", 2);
    Post posts[] = {{1, 1, "Marco!", {{0}}, 0},
                    {2, 2, "Polo!", {{0}}, 0},
                    {8, 1, "Marco!", {{0}}, 0},
                    {3, 2, "Polo!", {{0}}, 0},
                    {10, 1, "Marco!", {{0}}, 0},
                    {4, 2, "Polo!", {{0}}, 0}};

    for (int i = 0; i < 6; ++i)
    {
        int ret = createPost(c, posts[i].text, posts[i].senderID, posts[i].postID);
        ck_assert_msg(ret == SUCCESS,
                      "createPost should return SUCCESS on success");
        _add_post_to_end_of_channel(gt, posts + i);
        _compare_channels(gt, c,
                          "createPost returned SUCCESS, but: ");
        ck_assert_msg(c->posts.head->data->text != posts[i].text,
                      "createPost did not copy the post text");
    }

    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_find_post_null_channel)
{
    ck_assert_msg(!findPost(NULL, 1),
                  "findPost should return NULL if channel is NULL");
}
END_TEST

START_TEST(test_find_post_not_found)
{
    Post posts[] = {{1, 1, "Marco!", {{0}}, 0},
                    {2, 2, "Polo!", {{0}}, 0},
                    {8, 1, "Marco!", {{0}}, 0},
                    {3, 2, "Polo!", {{0}}, 0},
                    {10, 1, "Marco!", {{0}}, 0},
                    {4, 2, "Polo!", {{0}}, 0}};

    Channel *c = _create_channel();
    _create_account(c, "real person #1", 1);
    _create_account(c, "real person #2", 2);
    _add_posts_to_channel(c, posts, 6);

    Channel *gt = _create_channel();
    _create_account(gt, "real person #1", 1);
    _create_account(gt, "real person #2", 2);
    _add_posts_to_channel(gt, posts, 6);

    ck_assert_msg(!findPost(c, 5),
                  "findPost should return NULL if no post with id "
                  "postID exists");
    _compare_channels(gt, c, "findPost should not modify channel: ");

    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_find_post)
{
    Post posts[] = {{1, 1, "Marco!", {{0}}, 0},
                    {2, 2, "Polo!", {{0}}, 0},
                    {8, 1, "Marco!", {{0}}, 0},
                    {3, 2, "Polo!", {{0}}, 0},
                    {10, 1, "Marco!", {{0}}, 0},
                    {4, 2, "Polo!", {{0}}, 0}};

    Channel *c = _create_channel();
    _create_account(c, "real person #1", 1);
    _create_account(c, "real person #2", 2);
    _add_posts_to_channel(c, posts, 6);

    Channel *gt = _create_channel();
    _create_account(gt, "real person #1", 1);
    _create_account(gt, "real person #2", 2);
    _add_posts_to_channel(gt, posts, 6);

    Post *actual_post = c->posts.head->next->next->next->next->data;
    Post *found_post = findPost(c, 10);
    _compare_channels(gt, c, "findPost should not modify channel: ");

    ck_assert_msg(found_post, "findPost returned NULL, expected to find a post");
    char m[400];
    snprintf(m, 400, "findPost found post %p (id %d), expected post %p (id %d)",
             (void *)found_post, found_post->postID,
             (void *)actual_post, actual_post->postID);
    ck_assert_msg(found_post == actual_post, "%s", m);

    _free_channel(c);
    _free_channel(gt);
}

START_TEST(test_delete_post_null_channel)
{
    ck_assert_msg(deletePost(NULL, 1) == FAILURE,
                  "deletePost should return failure when channel is NULL");
}
END_TEST

START_TEST(test_delete_post_no_posts)
{
    Channel *c = _create_channel();
    _create_account(c, "real person #1", 1);
    Channel *gt = _create_channel();
    _create_account(gt, "real person #1", 1);

    ck_assert_msg(deletePost(c, 5) == FAILURE,
                  "deletePost should return FAILURE if no post with id "
                  "postID exists");
    _compare_channels(gt, c,
                      "deletePost should not modify channel on failure: ");

    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_delete_post_not_found)
{
    Post posts[] = {{1, 1, "Marco!", {{0}}, 0},
                    {2, 2, "Polo!", {{0}}, 0},
                    {8, 1, "Marco!", {{0}}, 0},
                    {3, 2, "Polo!", {{0}}, 0},
                    {10, 1, "Marco!", {{0}}, 0},
                    {4, 2, "Polo!", {{0}}, 0}};

    Channel *c = _create_channel();
    _create_account(c, "real person #1", 1);
    _create_account(c, "real person #2", 2);
    _add_posts_to_channel(c, posts, 6);

    Channel *gt = _create_channel();
    _create_account(gt, "real person #1", 1);
    _create_account(gt, "real person #2", 2);
    _add_posts_to_channel(gt, posts, 6);

    ck_assert_msg(deletePost(c, 5) == FAILURE,
                  "deletePost should return FAILURE if no post with id "
                  "postID exists");
    _compare_channels(gt, c,
                      "deletePost should not modify channel on failure: ");

    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_delete_first_post)
{
    Post posts[] = {{1, 1, "Marco!", {{0}}, 0},
                    {2, 2, "Polo!", {{0}}, 0},
                    {8, 1, "Marco!", {{0}}, 0},
                    {3, 2, "Polo!", {{0}}, 0},
                    {10, 1, "Marco!", {{0}}, 0},
                    {4, 2, "Polo!", {{0}}, 0}};

    Channel *c = _create_channel();
    _create_account(c, "real person #1", 1);
    _create_account(c, "real person #2", 2);
    _add_posts_to_channel(c, posts, 6);

    Channel *gt = _create_channel();
    _create_account(gt, "real person #1", 1);
    _create_account(gt, "real person #2", 2);
    // add all but the first post
    _add_posts_to_channel(gt, posts + 1, 5);

    ck_assert_msg(deletePost(c, 1) == SUCCESS,
                  "deletePost should return SUCCESS on success");
    _compare_channels(gt, c,
                      "deletePost should have deleted post with id 1, but: ");

    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_delete_only_post)
{
    Channel *c = _create_channel();
    _create_account(c, "the only real person", 1);
    char *msg = "I'm all alone ...";
    Post post = {1, 1, msg, {{0}}, 0};
    _add_posts_to_channel(c, &post, 1);

    Channel *gt = _create_channel();
    _create_account(gt, "the only real person", 1);

    ck_assert_msg(deletePost(c, 1) == SUCCESS,
                  "deletePost should return SUCCESS on success");
    _compare_channels(gt, c,
                      "deletePost should have deleted post with id 1, but: ");

    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_delete_middle_post)
{
    Post posts[] = {{1, 1, "Marco!", {{0}}, 0},
                    {2, 2, "Polo!", {{0}}, 0},
                    {8, 1, "Marco!", {{0}}, 0},
                    {3, 2, "Polo!", {{0}}, 0},
                    {10, 1, "Marco!", {{0}}, 0},
                    {4, 2, "Polo!", {{0}}, 0}};

    Channel *c = _create_channel();
    _create_account(c, "real person #1", 1);
    _create_account(c, "real person #2", 2);
    _add_posts_to_channel(c, posts, 6);

    Channel *gt = _create_channel();
    _create_account(gt, "real person #1", 1);
    _create_account(gt, "real person #2", 2);
    // add all but post 10
    _add_posts_to_channel(gt, posts, 4);
    _add_post_to_end_of_channel(gt, posts + 5);

    ck_assert_msg(deletePost(c, 10) == SUCCESS,
                  "deletePost should return SUCCESS on success");
    _compare_channels(gt, c,
                      "deletePost should have deleted post with id 10, but: ");

    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_delete_last_post)
{
    Post posts[] = {{1, 1, "Marco!", {{0}}, 0},
                    {2, 2, "Polo!", {{0}}, 0},
                    {8, 1, "Marco!", {{0}}, 0},
                    {3, 2, "Polo!", {{0}}, 0},
                    {10, 1, "Marco!", {{0}}, 0},
                    {4, 2, "Polo!", {{0}}, 0}};

    Channel *c = _create_channel();
    _create_account(c, "real person #1", 1);
    _create_account(c, "real person #2", 2);
    _add_posts_to_channel(c, posts, 6);

    Channel *gt = _create_channel();
    _create_account(gt, "real person #1", 1);
    _create_account(gt, "real person #2", 2);
    // add all but post 4
    _add_posts_to_channel(gt, posts, 5);

    ck_assert_msg(deletePost(c, 4) == SUCCESS,
                  "deletePost should return SUCCESS on success");
    _compare_channels(gt, c,
                      "deletePost should have deleted post with id 4, but: ");

    _free_channel(c);
    _free_channel(gt);
}
END_TEST

START_TEST(test_slack_coverage_1)
{
    Channel *c = createChannel();
    Channel *expected_c = _create_channel();
    ck_assert(createAccount(c, "testing", 0) == SUCCESS);
    ck_assert(createAccount(c, "rso31", 1) == SUCCESS);
    ck_assert(createAccount(c, "pgupta359", 1) == FAILURE);
    ck_assert(createAccount(c, "pgupta359", 2) == SUCCESS);

    ck_assert(createPost(c, "hello, world", 0, 0) == SUCCESS);
    ck_assert(createPost(c, "test", 1, 1) == SUCCESS);

    ck_assert(addReaction(c->posts.head->next->data, 0, SMILEY) == SUCCESS);

    ck_assert(deletePost(c, 0) == SUCCESS);

    ck_assert(createPost(c, "bruh", 1, 2) == SUCCESS);
    ck_assert(createPost(c, "this should fail", 2, 2) == FAILURE);
    ck_assert(createPost(c, "but this should pass", 2, 3) == SUCCESS);

    ck_assert(addReaction(c->posts.head->next->next->data, 1, SMILEY) == SUCCESS);
    ck_assert(addReaction(c->posts.head->next->next->data, 2, SMILEY) == SUCCESS);

    ck_assert(createPost(c, "another one", 1, 6) == SUCCESS);

    ck_assert(createAccount(c, "rbafna3", 3) == SUCCESS);
    ck_assert(deleteAccount(c, 1) == SUCCESS);

    Account accounts[] = {{"testing", 0}, {"pgupta359", 2}, {"rbafna3", 3}};

    Post posts[] = {
        {3, 2, "but this should pass", {{2, SMILEY}}, 1},
    };
    _add_accounts_to_channel(expected_c, accounts, 3);
    _add_posts_to_channel(expected_c, posts, 1);
    _compare_channels(expected_c, c, "");

    deleteChannel(c);
    _free_channel(expected_c);
}
END_TEST

START_TEST(test_slack_coverage_2)
{
    Channel *c = createChannel();
    Channel *expected_c = _create_channel();

    ck_assert(createAccount(c, "testing", 0) == SUCCESS);
    ck_assert(deleteAccount(c, 0) == SUCCESS);

    ck_assert(createAccount(c, "rso31", 0) == SUCCESS);
    ck_assert(createAccount(c, "pgupta359", 1) == SUCCESS);

    ck_assert(createPost(c, "testing123", 1, 0) == SUCCESS);
    ck_assert(createPost(c, "testing321", 0, 1) == SUCCESS);

    ck_assert(addReaction(c->posts.head->data, 0, SOBBING) == SUCCESS);
    ck_assert(addReaction(c->posts.head->next->data, 1, SKULL) == SUCCESS);

    ck_assert(deleteAccount(c, 1) == SUCCESS);
    ck_assert(deleteAccount(c, 0) == SUCCESS);

    _compare_channels(expected_c, c, "");

    deleteChannel(c);
    _free_channel(expected_c);
}
END_TEST

Suite *slack_suite(void)
{
    Suite *s = suite_create("slack_suite");

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_channel_basic);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_channel_malloc_failure);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_account_null_channel);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_account_null_username);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_account_from_empty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_account_malloc_failure);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_account_realloc_failure);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_account_no_duplicates_short);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_account_no_duplicates_long);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_account_duplicates_short);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_account_duplicates_long);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_post_null_channel);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_post_null_text);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_post_account_not_found);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_post_postid_not_unique);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_post_malloc_failure);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_first_post);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_create_many_posts);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_add_reaction_from_empty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_add_reaction_basic);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_add_reaction_multiple_posts);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_add_reaction_invalid_arg_checks);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_add_reaction_duplicate_accountid);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_add_reaction_array_limit);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_find_user_null_channel);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_find_user_not_found_short_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_find_user_not_found_long_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_find_user_found_short_list);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_find_user_found_long_list);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_find_post_null_channel);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_find_post_not_found);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_find_post);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_search_palindrome_basic);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_search_palindrome_spaces_specials);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_search_palindrome_multiple_matches);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_search_palindrome_invalid_args);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_search_palindrome_not_found);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_reaction_basic);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_reaction_multiple_posts);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_reaction_invalid_arg_checks);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_reaction_not_found);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_account_null);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_account_not_found);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_account_first);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_account_last);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_account_with_posts);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_account_with_posts_and_reactions);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_post_null_channel);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_post_not_found);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_first_post);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_only_post);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_middle_post);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_last_post);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_post_no_posts);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_channel_null_pointer);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_channel_empty);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_channel_with_accounts);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_delete_channel_full);

    tcase_hack(s, reset_mallocs_until_fail, NULL, test_slack_coverage_1);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_slack_coverage_2);

    return s;
}
