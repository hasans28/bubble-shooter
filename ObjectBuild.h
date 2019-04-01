#ifndef _OBJECTBUILDH_
#define _OBJECTBUILDH_

#include "util.h"

struct bubble bubbleConstructor();
void initializeBubbles();
void initializeUserBubble();
void drawBubbles();
void drawUserBubbles();
void setupColourMatchLinks();
void wipeoutMatchingColours(struct bubble*);

#endif