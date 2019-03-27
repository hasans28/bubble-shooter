#ifndef _GLOBALSH_
#define _GLOBALSH_

#include "Bubble.h"

#define X_MAX 320
#define Y_MAX 240

#define RED 0xF800
#define BLUE 0x001F
#define GREEN 0x07E0
#define YELLOW 0xFF00
#define BLACK 0x0000

#define BUBBLE_WIDTH 20
//#define NULL -1

extern volatile int PIXEL_BUFFER_START;
extern struct bubble BUBBLES[96];
extern struct bubble USER_BUBBLES[10];

#endif