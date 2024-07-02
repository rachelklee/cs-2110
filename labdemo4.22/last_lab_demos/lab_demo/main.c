/**
 * CS 2110 - Fall 2022
 * Final Exam - Implement Stack
 *
 * You may use this file to test your functions from team.c
 */

#include "team.h"

/**
 * @brief A main function that you can use for testing.
 * 
 * Implementing this function is NOT necessary for the final.
 * You can use this if you want to create custom tests.
 * We have included a basic test as an example.
 * 
 */
int main(void) {
    // creating a player and allocating its data on the heap
    char *name = "THE BEST PLAYER IN THE WHOLE ENTIRE WORLD";
    int age = 5;
    int number = 1;
    struct player *player;
    createPlayer(name, age, number, &player);
    printf("Player name: %s\n", player->name);
    printf("Player age: %d\n", player->age);
    printf("Player number: %d\n\n", player->number);
    
    // create a roster on the stack
    struct roster roster = {0, 0, 0};
    roster.players = (struct player **) malloc(sizeof(struct player *));
    roster.capacity = 1;
    printf("Roster capacity: %d\n", roster.capacity);
    printf("Roster size: %d\n\n", roster.size);

    // add one player to the roster
    addToRoster(&roster, player);
    printf("Inside our roster after adding %d players:\n", roster.size);
    printf("Roster capacity: %d\n", roster.capacity);
    printf("Roster size: %d\n\n", roster.size);
    for (int i = 0; i < roster.size; i++) {
        printf("Player %d:\n", i);
        printf("name: %s\n", player->name);
        printf("age: %d\n", player->age);
        printf("number: %d\n", player->number);
        printf("\n");
    }

    return 0;
}
