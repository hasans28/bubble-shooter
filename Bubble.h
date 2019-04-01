#ifndef _BUBBLEH_
#define _BUBBLEH_

#include "util.h"

struct bubble 
{
    int id;                             // array index
    int x_loc; 
    int y_loc;                          // location of top_left
    int velocity;
    short int colour;                   // colour of bubble
    bool visited;
    struct bubble* next[4];            // next connection to same coloured bubble
};

#endif