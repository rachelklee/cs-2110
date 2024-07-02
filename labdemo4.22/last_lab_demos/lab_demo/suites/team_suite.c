/**
 * @file stack_suite.c
 * @author Sameer
 * @brief Testing Suites to Final-C - Fall 2022
 * @date 2022-26-07
 */

// Check Docs
// Tutorial : https://libcheck.github.io/check/doc/check_html/check_3.html
// Check API: https://libcheck.github.io/check/doc/doxygen/html/check_8h.html

// System Headers
#include <string.h>
#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/wait.h> // For grabbing return value of system call

// TA Headers
#include "test_utils.h"

int mallocs_until_fail = -1;
struct mallocinfo *malloclist;
static int _player_equals(struct player *p1, struct player *p2);

// allow infinite mallocs by default
static void reset_mallocs_until_fail_and_malloclist(void) {
    malloclist = NULL;
    mallocs_until_fail = -1;
}

static void free_malloclist(void) {
    struct mallocinfo *curr = malloclist;
    while (curr != NULL) {
        struct mallocinfo *next = curr->next;
        free(curr);
        curr = next;
    }
}

/* createPlayer tests */

START_TEST(test_create_player_normal) {
    const char *name = "Megatron GOAT";
    int age = 37;
    int number = 81;
    struct player *player;

    int retVal = createPlayer(name, age, number, &player);

    ck_assert_msg(retVal == SUCCESS, "createPlayer() should return SUCCESS if it succeeds");
    ck_assert_msg(player->name != name, "createPlayer() should deep copy the name");
    ck_assert_msg(strncmp(player->name, name, strlen(name) + 1) == 0, "createPlayer() should copy the name correctly (expected \"Tom Brady\", got \"%s\")", player->name);
    ck_assert_msg(player->age == age, "createPlayer() should copy the age correctly (expected 45, got %d)", player->age);
    ck_assert_msg(player->number == number, "createPlayer() should copy the number correctly (expected 12, got %d)", player->number);

    free(player->name);
    free(player);
}
END_TEST

START_TEST(test_create_player_malloc_failure_0) {
    const char *name = "King Henry";
    int age = 28;
    int number = 22;
    struct player *player;

    // The actual tests
    mallocs_until_fail = 0;
    int retVal = createPlayer(name, age, number, &player);
    ck_assert_msg(retVal == FAILURE, "createPlayer() should return FAILURE on malloc failure");
}
END_TEST

START_TEST(test_create_player_failures) {
    struct player *player;
    int retVal = createPlayer(NULL, 0, 0, &player);
    ck_assert_msg(retVal == FAILURE, "createPlayer() should return FAILURE when name is NULL");
    ck_assert_msg(_num_malloc_calls() == 0, "createPlayer() shouldn't call malloc if name is NULL");

    retVal = createPlayer("A", -1, 0, &player);
    ck_assert_msg(retVal == FAILURE, "createPlayer() should return FAILURE when age is negative");
    ck_assert_msg(_num_malloc_calls() == 0, "createPlayer() shouldn't call malloc if age is negative");

    retVal = createPlayer("A", 0, -1, &player);
    ck_assert_msg(retVal == FAILURE, "createPlayer() should return FAILURE when number is negative");
    ck_assert_msg(_num_malloc_calls() == 0, "createPlayer() shouldn't call malloc if number is negative");

    retVal = createPlayer("A", 0, 0, NULL);
    ck_assert_msg(retVal == FAILURE, "createPlayer() should return FAILURE on when dataOut is NULL");
    ck_assert_msg(_num_malloc_calls() == 0, "createPlayer() shouldn't call malloc if dataOut is NULL");
}
END_TEST

START_TEST(test_create_player_malloc_failure_1) {
    const char *name = "Joe Brrrr";
    int age = 25;
    int number = 9;
    struct player *player;
    
    // actual tests
    mallocs_until_fail = 1;
    int retVal = createPlayer(name, age, number, &player);
    ck_assert_msg(retVal == FAILURE, "createPlayer() should return FAILURE on malloc failure");
}
END_TEST

/* addToRoster tests*/

START_TEST(test_add_to_roster_empty) {
    struct player *player = _create_player("Tom Brady", 92, 12);
    struct roster *roster = _create_roster(1, 0);

    int retVal = addToRoster(roster, player);
    ck_assert_msg(retVal == SUCCESS, "addToRoster() should return SUCCESS if it succeeds");
    ck_assert_msg(roster->size == 1, "addToRoster() should increase size by 1 (expected 1, got %d)", roster->size);
    ck_assert_msg(roster->capacity == 1, "addToRoster() shouldn't change capacity if it doesn't need to (expected 1, got %d)", roster->capacity);
    ck_assert_msg(_num_malloc_calls() == 0, "addToRoster() shouldn't call malloc or realloc when it's not needed");
    ck_assert_msg(_player_equals(roster->players[0], player) == 1, "addToRoster should add the passed in player to the end of the roster");

    free(player->name);
    free(player);
    free(roster->players);
    free(roster);
}
END_TEST

START_TEST(test_add_to_roster_normal_1) {
    struct player *player = _create_player("Harrison Butker", 27, 7);
    struct roster *roster = _create_roster(6, 5);
    struct player *head = roster->players[0];

    int retVal = addToRoster(roster, player);
    ck_assert_msg(retVal == SUCCESS, "addToRoster() should return SUCCESS if it succeeds");
    ck_assert_msg(roster->size == 6, "addToRoster() should increase size by 1 (expected 6, got %d)", roster->size);
    ck_assert_msg(roster->capacity == 6, "addToRoster() shouldn't change capacity if it doesn't need to (expected 6, got %d)", roster->capacity);
    ck_assert_msg(_num_malloc_calls() == 0, "addToRoster() shouldn't call malloc or realloc when it's not needed");
    for (int i = 0; i < 5; i++) {
        ck_assert_msg(_player_equals(roster->players[i], head) == 1, "addToRoster() shouldn't change existing players on the roster");
    }
    ck_assert_msg(_player_equals(roster->players[5], player) == 1, "addToRoster should add the passed in player to the end of the roster");

    free(head->name);
    free(head);
    free(player->name);
    free(player);
    free(roster->players);
    free(roster);
}
END_TEST

START_TEST(test_add_to_roster_normal_2) {
    struct player *player = _create_player("Harrison Butker", 27, 7);
    struct roster *roster = _create_roster(MAX_ROSTER_SIZE, MAX_ROSTER_SIZE - 1);
    struct player *head = roster->players[0];

    int retVal = addToRoster(roster, player);
    ck_assert_msg(retVal == SUCCESS, "addToRoster() should return SUCCESS if it succeeds");
    ck_assert_msg(roster->size == MAX_ROSTER_SIZE, "addToRoster() should increase size by 1 (expected 6, got %d)", roster->size);
    ck_assert_msg(roster->capacity == MAX_ROSTER_SIZE, "addToRoster() shouldn't change capacity if it doesn't need to (expected 6, got %d)", roster->capacity);
    ck_assert_msg(_num_malloc_calls() == 0, "addToRoster() shouldn't call malloc or realloc when it's not needed");
    for (int i = 0; i < MAX_ROSTER_SIZE - 1; i++) {
        ck_assert_msg(_player_equals(roster->players[i], head) == 1, "addToRoster() shouldn't change existing players on the roster");
    }
    ck_assert_msg(_player_equals(roster->players[MAX_ROSTER_SIZE - 1], player) == 1, "addToRoster should add the passed in player to the end of the roster");

    free(head->name);
    free(head);
    free(player->name);
    free(player);
    free(roster->players);
    free(roster);
}
END_TEST

START_TEST(test_add_to_roster_resize_1) {
    struct player *player = _create_player("Kyle Pitts", 22, 8);
    struct roster *roster = _create_roster(4, 4);
    struct player *head = roster->players[0];

    int retVal = addToRoster(roster, player);
    ck_assert_msg(retVal == SUCCESS, "addToRoster() should return SUCCESS if it succeeds");
    ck_assert_msg(roster->size == 5, "addToRoster() should increase size by 1 (expected 5, got %d)", roster->size);
    ck_assert_msg(roster->capacity == 8, "addToRoster() should double capacity when it's needed (expected 8, got %d)", roster->capacity);
    ck_assert_msg(_num_malloc_calls() == 1, "addToRoster() should resize the array when necessary");
    for (int i = 0; i < 4; i++) {
        ck_assert_msg(_player_equals(roster->players[i], head) == 1, "addToRoster() shouldn't change existing players on the roster");
    }
    ck_assert_msg(_player_equals(roster->players[4], player) == 1, "addToRoster should add the passed in player to the end of the roster");

    free(head->name);
    free(head);
    free(player->name);
    free(player);
    free(roster->players);
    free(roster);
}
END_TEST

START_TEST(test_add_to_roster_resize_2) {
    struct player *player = _create_player("Lazy", 12, 9);
    struct roster *roster = _create_roster(13, 13);
    struct player *head = roster->players[0];

    int retVal = addToRoster(roster, player);
    ck_assert_msg(retVal == SUCCESS, "addToRoster() should return SUCCESS if it succeeds");
    ck_assert_msg(roster->size == 14, "addToRoster() should increase size by 1 (expected 5, got %d)", roster->size);
    ck_assert_msg(roster->capacity == MAX_ROSTER_SIZE, "addToRoster() should increase capacity when it's needed (expected %d, got %d)", MAX_ROSTER_SIZE, roster->capacity);
    ck_assert_msg(_num_malloc_calls() == 1, "addToRoster() should resize the array when necessary");
    for (int i = 0; i < 13; i++) {
        ck_assert_msg(_player_equals(roster->players[i], head) == 1, "addToRoster() shouldn't change existing players on the roster");
    }
    ck_assert_msg(_player_equals(roster->players[13], player) == 1, "addToRoster should add the passed in player to the end of the roster");

    free(head->name);
    free(head);
    free(player->name);
    free(player);
    free(roster->players);
    free(roster);
}
END_TEST

START_TEST(test_add_to_roster_malloc_failure) {
    struct player *player = _create_player("Sean Wahee", 80, 234);
    struct roster *roster = _create_roster(1, 1);
    struct player *head = roster->players[0];

    mallocs_until_fail = 0;
    int retVal = addToRoster(roster, player);
    ck_assert_msg(retVal == FAILURE, "addToRoster() should return FAILURE on malloc failure");
    ck_assert_msg(roster->size == 1, "addToRoster() shouldn't change size on failure (expected 1, got %d)", roster->size);
    ck_assert_msg(roster->capacity == 1, "addToRoster() shouldn't change capacity on failure (expected 1, got %d)", roster->capacity);
    ck_assert_msg(_player_equals(roster->players[0], head) == 1, "addToRoster() shouldn't change existing players on the roster on failure");

    free(head->name);   
    free(head);
    free(player->name);
    free(player);
    free(roster->players);
    free(roster);
}
END_TEST

START_TEST(test_add_to_roster_failures_size_capacity) {
    struct player *player = _create_player("Forrest GUMP", 22, 44);
    struct roster *roster1 = _create_roster(1, 0);
    struct roster *roster2 = _create_roster(MAX_ROSTER_SIZE, MAX_ROSTER_SIZE);
    struct player *head = roster2->players[0];

    int retVal = addToRoster(roster2, player);
    ck_assert_msg(retVal == FAILURE, "addToRoster() should return FAILURE when the array's capacity and size are both at MAX_ROSTER_SIZE");
    for (int i = 0; i < MAX_ROSTER_SIZE; i++) {
        ck_assert_msg(_player_equals(roster2->players[i], head) == 1, "addToRoster() shouldn't change existing players on the roster when it fails");
    }

    roster1->capacity = -1;
    roster2->capacity++;
    roster2->size--;
    retVal = addToRoster(roster1, player) && addToRoster(roster2, player);
    ck_assert_msg(retVal == FAILURE, "addToRoster() should return FAILURE when capacity exceeds MAX_ROSTER_SIZE or is negative");
    for (int i = 0; i < MAX_ROSTER_SIZE; i++) {
        ck_assert_msg(_player_equals(roster2->players[i], head) == 1, "addToRoster() shouldn't change existing players on the roster when it fails");
    }

    roster2->capacity--;
    roster2->size += 2;
    roster1->capacity = 1;
    roster1->size = -1;
    retVal = addToRoster(roster1, player) & addToRoster(roster2, player);
    ck_assert_msg(retVal == FAILURE, "addToRoster() should return FAILURE when size exceeds capacity or is negative");
    for (int i = 0; i < MAX_ROSTER_SIZE; i++) {
        ck_assert_msg(_player_equals(roster2->players[i], head) == 1, "addToRoster() shouldn't change existing players on the roster when it fails");
    }

    free(head->name);
    free(head);
    free(player->name);
    free(player);
    free(roster1->players);
    free(roster2->players);
    free(roster1);
    free(roster2);
}
END_TEST

START_TEST(test_add_to_roster_failures_NULL) {
    struct player *player = _create_player("Forrest GUMP", 22, 44);
    struct roster *roster = _create_roster(1, 0);

    int retVal = addToRoster(NULL, player);
    ck_assert_msg(retVal == FAILURE, "addToRoster() should return FAILURE when roster is null");
    
    retVal = addToRoster(roster, NULL);
    ck_assert_msg(retVal == FAILURE, "addToRoster() should return FAILURE when player is null");

    free(roster->players);
    roster->players = NULL;
    retVal = addToRoster(roster, player);
    ck_assert_msg(retVal == FAILURE, "addToRoster() should return FAILURE when the players array is null");

    free(player->name);
    free(player);
    free(roster);
}
END_TEST

// helper function. returns 1 if equal, return 0 for not equal
static int _player_equals(struct player *p1, struct player *p2) {
    if (strncmp(p1->name, p2->name, strlen(p1->name + 1)) == 0) {
        if (p1->age == p2->age && p1->number == p2->number) {
            return 1;
        }
    }
    return 0;
}

Suite *team_suite(void)
{
    Suite *s = suite_create("team_suite");

    // push tests
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_create_player_normal);
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_create_player_failures);
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_create_player_malloc_failure_0);
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_create_player_malloc_failure_1);

    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_add_to_roster_empty);
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_add_to_roster_normal_1);
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_add_to_roster_normal_2);
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_add_to_roster_resize_1);
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_add_to_roster_resize_2);
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_add_to_roster_malloc_failure);
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_add_to_roster_failures_NULL);
    tcase_singleton(s, reset_mallocs_until_fail_and_malloclist, free_malloclist, test_add_to_roster_failures_size_capacity);


    return s;
}
