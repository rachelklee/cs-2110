/**
 * CS 2110 - Fall 2022
 * Final Exam - Implement Team
 *
 * Do not modify this file!
 */

#ifndef TEAM_H
#define TEAM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FAILURE 1
#define SUCCESS 0

// This is just to remove compiler warnings associated with unused variables.
// Delete calls to this as you implement functions.
#define UNUSED(x) ((void)(x))

#define MAX_ROSTER_SIZE 16

struct player {
    char *name;     // pointer to player's name
    int age;        // the player's age
    int number;     // the player's jersey number
};

struct roster {
    int size; // current number of players on the roster
    int capacity; // the max number of players the roster can currently hold / how many available elements are in the array
    struct player **players; // pointer to element 0 of array of pointers to players
};

int createPlayer(const char *name, int age, int number, struct player **dataOut);
int addToRoster(struct roster *roster, struct player *newPlayer);

#endif
