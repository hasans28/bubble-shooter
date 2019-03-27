#ifndef _BUBBLEH_
#define _BUBBLEH_

#include <stdbool.h>

struct bubble 
{
    int id;                             // array index
    int x_loc; 
    int y_loc;                          // location of top_left
    int x_vel;
    int y_vel;
    short int colour;                   // colour of bubble
    bool visited;
    struct bubble* next[8];            // next connection to same coloured bubble
};

#endif