#include "Bubble.h"

#define X_MAX 340
#define Y_MAX 240

#define RED 0xF800
#define BLUE 0x001F
#define GREEN 0x07E0
#define YELLOW 0xFF00
#define BLACK 0x0000

#define BUBBLE_WIDTH 3
#define NULL -1

volatile int PIXEL_BUFFER_START;
int ARRAY_SIZE;
struct bubble* BUBBLES;
