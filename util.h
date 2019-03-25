#include "stdbool.h"
#include "math.h"
#include "Bubble.h"
#include "Globals.h"

int calculateBubblesArraySize();
bool checkForMatch(struct bubble*, struct bubble*);
struct bubble bubbleConstructor();

void clear_screen();
void plot_pixel(int x, int y, short int line_color);
void draw_line(int x0, int y0, int x1, int y1, short int line_color);
void swap(int *x, int *y);

void initializeBubbles();
void drawBubbles();
void setupColourMatchLinks();
void wipeoutMatchingColours(struct bubble*);