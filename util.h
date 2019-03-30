#ifndef _UTILH_
#define _UTILH_

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
//#include <time.h>

#include "Bubble.h"
#include "Globals.h"
#include "keyResponses.h"
#include "address_map_arm.h"

struct bubble bubbleConstructor();

void clear_screen();
void plot_pixel(int x, int y, short int line_color);
int coordsToBubbleId(int x, int y);
int coordsToUserId(int x, int y);

void initializeBubbles();
void initializeUserBubble();
void drawBubbles();
void drawUserBubbles();
void setupColourMatchLinks();
void wipeoutMatchingColours(struct bubble*);

int setTempScore();
bool collisionCheck();
void displayToHex();

void wait_for_vsync();

#endif