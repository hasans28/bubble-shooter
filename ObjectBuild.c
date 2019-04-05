#include "ObjectBuild.h"

struct bubble bubbleConstructor()
{
    struct bubble newBubble;
    newBubble.id = NULL;
    newBubble.x_loc = NULL;
    newBubble.y_loc = NULL;
    newBubble.velocity = 0;
    newBubble.colour = BLACK;
    newBubble.visited = false;

    int i;
    for(i = 0; i < 4; i++)
    {
        newBubble.next[i] = NULL;
    }

    return newBubble;
}

//short NUM_TO_COLOUR[4] = {RED, BLUE, GREEN, YELLOW};

void initializeBubbles()
{
    int it;
    for(it = 0; it < BUBBLES_ARRAY_SIZE; it++)
    {
        BUBBLES[it] = bubbleConstructor();

        BUBBLES[it].id = it;
        BUBBLES[it].x_loc = (it*BUBBLE_WIDTH) % X_MAX;
        BUBBLES[it].y_loc = (it/16) * BUBBLE_WIDTH;

		//srand(7);
        int randomColour = rand()%4;
		
		//BUBBLES[it].colour = NUM_TO_COLOUR[randomColour];

        switch (randomColour)
        {
            case 0:
                BUBBLES[it].colour = RED;
                break;
            case 1:
                BUBBLES[it].colour = BLUE;
                break;
            case 2:
                BUBBLES[it].colour = GREEN;
                break;
            case 3:
                BUBBLES[it].colour = YELLOW;
                break;
            default:
                BUBBLES[it].colour = YELLOW;
                break;
        }  
    }
}

void initializeUserBubble()
{
    int it;
    for(it = 0; it < USER_ARRAY_SIZE; it++)
    {
        USER_BUBBLES[it] = bubbleConstructor();

        USER_BUBBLES[it].id = it;
        USER_BUBBLES[it].x_loc = BUBBLE_WIDTH * 8;
        USER_BUBBLES[it].y_loc = Y_MAX - BUBBLE_WIDTH;

		//srand(7);
        int randomColour = rand()%4;

        switch (randomColour)
        {
            case 0:
                USER_BUBBLES[it].colour = RED;
                break;
            case 1:
                USER_BUBBLES[it].colour = BLUE;
                break;
            case 2:
                USER_BUBBLES[it].colour = GREEN;
                break;
            case 3:
                USER_BUBBLES[it].colour = YELLOW;
                break;
            default:
                USER_BUBBLES[it].colour = YELLOW;
                break;
        }  
    }
}

void setupColourMatchLinks()
{
    int i, index;
    for(i = 0; i < BUBBLES_ARRAY_SIZE; i++)
    {
        index = coordsToBubbleId(BUBBLES[i].x_loc-BUBBLE_WIDTH, BUBBLES[i].y_loc);
        if(index != -1)
        {
            if(BUBBLES[i].colour == BUBBLES[index].colour) BUBBLES[i].next[0] = &BUBBLES[index];
        }

        index = coordsToBubbleId(BUBBLES[i].x_loc+BUBBLE_WIDTH, BUBBLES[i].y_loc);
        if(index != -1)
        {
            if(BUBBLES[i].colour == BUBBLES[index].colour) BUBBLES[i].next[1] = &BUBBLES[index];
        }

        index = coordsToBubbleId(BUBBLES[i].x_loc, BUBBLES[i].y_loc-BUBBLE_WIDTH);
        if(index != -1)
        {
            if(BUBBLES[i].colour == BUBBLES[index].colour) BUBBLES[i].next[2] = &BUBBLES[index];
        }

        index = coordsToBubbleId(BUBBLES[i].x_loc, BUBBLES[i].y_loc+BUBBLE_WIDTH);
        if(index != -1)
        {
            if(BUBBLES[i].colour == BUBBLES[index].colour) BUBBLES[i].next[3] = &BUBBLES[index];
        }
    }
}

void wipeoutMatchingColours(struct bubble* startingBubble)
{
    startingBubble->visited = true;

    int i;
    for(i = 0; i < 4; i++)
    {
        if(startingBubble->next[i] != NULL && !startingBubble->next[i]->visited) wipeoutMatchingColours(startingBubble->next[i]);
        startingBubble->colour = BLACK;
    }
    return;
}

void drawBubbles()
{
    int count, x, y;
    for(count = 0; count < BUBBLES_ARRAY_SIZE; count++)
    {
        for(x = BUBBLES[count].x_loc; x <= BUBBLES[count].x_loc+BUBBLE_WIDTH-1; x++)
        {
            for(y = BUBBLES[count].y_loc; y <= BUBBLES[count].y_loc+BUBBLE_WIDTH-1; y++)
            {
                if(x>=0 && x<=X_MAX && y>=0 && y<=Y_MAX && BUBBLES[count].colour != BLACK) plot_pixel(x, y, BUBBLES[count].colour);
            }
        }
    }
}

void drawUserBubbles()
{
    int count, x, y;
    for(count = 0; count < USER_ARRAY_SIZE; count++)
    {
		USER_BUBBLES[count].y_loc += USER_BUBBLES[count].velocity;

        for(x = USER_BUBBLES[count].x_loc; x <= USER_BUBBLES[count].x_loc+BUBBLE_WIDTH-1; x++)
        {
            for(y = USER_BUBBLES[count].y_loc; y <= USER_BUBBLES[count].y_loc+BUBBLE_WIDTH-1; y++)
            {
                if(x>=0 && x<=X_MAX && y>=0 && y<=Y_MAX && USER_BUBBLES[count].colour != BLACK) plot_pixel(x, y, USER_BUBBLES[count].colour);
            }
        }
    }
}