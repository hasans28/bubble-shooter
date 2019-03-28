#ifndef _UTILH_
#define _UTILH_

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "Bubble.h"
#include "Globals.h"
#include "keyResponses.h"
#include "address_map_arm.h"

struct bubble bubbleConstructor();

void clear_screen();
void plot_pixel(int x, int y, short int line_color);
void draw_line(int x0, int y0, int x1, int y1, short int line_color);
void swap(int *x, int *y);
int coordsToId(int x, int y);

void initializeBubbles();
void initializeUserBubble();
void drawBubbles();
void drawUserBubbles();
void setupColourMatchLinks();
void wipeoutMatchingColours(struct bubble*);

void collisionCheck();

void wait_for_vsync();

#endif