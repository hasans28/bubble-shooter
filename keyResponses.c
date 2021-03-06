#include "keyResponses.h"

void keyZeroResponse()
{
    int i;
    for(i = 0; i < USER_ARRAY_SIZE; i++)
    {
        if(USER_BUBBLES[i].id != -1 && USER_BUBBLES[i].x_loc < X_MAX - BUBBLE_WIDTH)
        {
            USER_BUBBLES[i].x_loc += BUBBLE_WIDTH;
        } 
    }
}

void keyOneResponse()
{
    int i;
    for(i = 0; i < USER_ARRAY_SIZE; i++)
    {
        if(USER_BUBBLES[i].id != -1 && USER_BUBBLES[i].x_loc != 0)
        {
            USER_BUBBLES[i].x_loc -= BUBBLE_WIDTH;
        } 
    }
}

void keyTwoResponse()
{
    int i;
    for(i = USER_ARRAY_SIZE-1; i >= 0; i--)
    {
        if(USER_BUBBLES[i].id != -1)
        {
            USER_BUBBLES[i].velocity = -(BUBBLE_WIDTH);
            USER_BUBBLES[i].id = -1;
            return;
        } 
    }
}