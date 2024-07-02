/**
 * CS 2110 - Fall 2022
 * Final Exam - Implement Team
 *
 * Name:
 */

/**
 * The following structs are defined in team.h. You will need to use these to store your team's data:
 *
 * struct player {
 *   char *name;     // pointer to player's name
 *   int age;        // the player's age
 *   int number;     // the player's jersey number
 * };
 *
 * struct roster {
 *   int size; // current number of players on the roster
 *   int capacity; // the max number of players the roster can currently hold
 *   struct player **players; // pointer to element 0 of array of pointers to players
 * };
 * 
 * IMPORTANT: string.h has been included so you are encouraged to use any string functions (strlen, strncpy, etc.) that you may need.
 */

// DO NOT MODIFY THE INCLUDE(s) LIST
#include <stdio.h>
#include "team.h"

/** createPlayer
 *
 * Creates a player struct by allocating memory for it on the heap.
 * Take all of the passed in arguments and store them in the created player struct.
 * 
 * ------------------------------------------------------------
 * Here is the struct definition of a player for reference:
 * struct player {
 *   char *name;     // pointer to player's name
 *   int age;        // the player's age
 *   int number;     // the player's jersey number
 * };
 * ------------------------------------------------------------
 * 
 * If this function succeeds, it should return SUCCESS and modify dataOut to
 * hold the newly created player. If it fails, it should return FAILURE
 * and leave dataOut unchanged.
 * 
 * @param name The name of the player
 * @param age The age of the player
 * @param number The number of the player
 * @param dataOut A pointer to pointer to return the created player
 * @return FAILURE if any of the following are true:
 *         (1) name is NULL
 *         (2) age is negative
 *         (3) number is negative
 *         (4) dataOut is NULL
 *         (5) a malloc failure occurs
 *         Otherwise, return SUCCESS
 */
int createPlayer(const char *name, int age, int number, struct player **dataOut) {
  struct player *newPlayer = (struct player*)malloc(sizeof(struct player));

  int name_len = strlen(name);
  char *n_name = (char*) malloc(name_len + 1);
  if (!n_name) {
	return FAILURE;
  }
  strncpy(n_name, name, name_len + 1);
  newPlayer->name = n_name;

  newPlayer->age = age;
  newPlayer->number = number;

  *dataOut = newPlayer;

  return SUCCESS;

}

/** addToRoster
 * 
 * Add a player to the players array in the roster struct. You may assume that if 
 * the player struct passed in is not NULL, then it has already been allocated on the heap. 
 * 
 * ------------------------------------------------------------
 * Here is the struct definition of a roster for reference:
 * struct roster {
 *   int size; // current number of players on the roster
 *   int capacity; // the max number of players the roster can currently hold
 *   struct player **players; // pointer to element 0 of array of pointers to players
 * };
 * ------------------------------------------------------------
 * 
 * The maximum capacity/size for the array is MAX_ROSTER_SIZE, which can be found in team.h
 * 
 * If the array is not filled (i.e. when the size less than the capacity), then add the player to the end of the array.
 * If the array is filled, then do one of the following:
 *    (1) Resize the array and double its capacity
 *    (2) Resize the array to MAX_ROSTER_SIZE if doubling the capacity causes it to exceed MAX_ROSTER_SIZE
 *    (3) Don't resize the array if the capacity is already at MAX_ROSTER_SIZE
 * 
 * 
 * @param roster a pointer to the roster struct
 * @param newPlayer a pointer to a previously allocated player struct
 * @return FAILURE if any of the following are true:
 *         (1) roster or its player array are null
 *         (2) the roster's size is negative or is greater than the capacity
 *         (3) the roster's capacity is negative or is greater than MAX_ROSTER_SIZE
 *         (4) the roster is at capacity MAX_ROSTER_SIZE and is filled
 *         (5) newPlayer is NULL
 *         (6) a malloc or realloc failure occurs
 *         Otherwise, return SUCCESS
 */
int addToRoster(struct roster *roster, struct player *newPlayer) {
  if (!roster || !roster->players) {
	return FAILURE;
  }
  if (roster->size > roster->capacity || roster->size < 0) {
	return FAILURE;
  }
  if (roster->capacity > MAX_ROSTER_SIZE || roster->capacity < 0) {
	return FAILURE;
  }

  // maybe let them initialize roster->players if it's null if this seems too easy
  if (roster->size == roster->capacity) {
	if (roster->size == MAX_ROSTER_SIZE) {
	  return FAILURE;
	}
	struct player **temp;
	int capacity = 0;
	if (roster->capacity * 2 > MAX_ROSTER_SIZE) {
	  temp = (struct player **) realloc(roster->players, sizeof(struct player *) * MAX_ROSTER_SIZE);
	  capacity = MAX_ROSTER_SIZE;
	} else {
	  temp = (struct player **) realloc(roster->players, sizeof(struct player *) * 2 * roster->capacity);
	  capacity = roster->capacity * 2;
	}
	if (!temp) {
	  return FAILURE;
	}
	roster->players = temp;
	roster->capacity = capacity;
  }
  roster->players[roster->size] = newPlayer;
  roster->size++;
  return SUCCESS;
}
