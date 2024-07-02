#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

struct sprite {
    int row;
    int col;
    int length;
    int velRow;
    int velCol;
    int clicks;
    int maxClicks;
};

int wallCollision(struct sprite p);
void drawGrid(void);
void drawMap(void);

void clickCounter(void);

void initializePlayer(struct sprite *player);

#endif
