#ifndef _UTILH_
#define _UTILH_

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#include "Bubble.h"
#include "Globals.h"
#include "exceptions.h"
#include "keyResponses.h"
#include "address_map_arm.h"

bool collisionCheck();
bool setBoardReset();

int setTempScore();
int coordsToBubbleId(int x, int y);
int coordsToUserId(int x, int y);

void clear_screen();
void displayToHex();
void wait_for_vsync();
void plot_pixel(int x, int y, short int line_color);

#endif