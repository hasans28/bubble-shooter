#include "util.h"

int calculateBubblesArraySize()
{
    int tot_x = X_MAX / BUBBLE_WIDTH;
    int tot_y = (Y_MAX * 0.5) / BUBBLE_WIDTH;

    return tot_x * tot_y;
}

struct bubble bubbleConstructor()
{
    struct bubble newBubble;
    newBubble.id = NULL;
    newBubble.x_loc = NULL;
    newBubble.y_loc = NULL;
    newBubble.colour = BLACK;
    newBubble.next = NULL;

    return newBubble;
}

bool checkForMatch(struct bubble* bubble1, struct bubble* bubble2)
{
    return (abs(bubble1->x_loc - bubble2->x_loc) <= 2) && (abs(bubble1->y_loc - bubble2->y_loc) <= 2);
}

void initializeBubbles()
{
    BUBBLES = (struct bubble*)malloc(ARRAY_SIZE * sizeof(struct bubble));

    int it;
    for(it = 0; it < ARRAY_SIZE; it++)
    {
        BUBBLES[it] = bubbleConstructor();
        int randomColour = rand()%4;

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
                break;
        }  
    }
}

void setupColourMatchLinks()
{
    struct bubble uniqueColours[4];

    int i, j;
    for(i = 0; i < 4; i++)
    {
        uniqueColours[i] = bubbleConstructor();
    }

    for(i = 0; i < ARRAY_SIZE; i++)
    {
        for(j = 0; j < uniqueColours; j++)
        {
            if(BUBBLES[i].colour == uniqueColours[j].colour)
            {
                bool checkIfMatch = checkForMatch(&uniqueColours[j], &BUBBLES[i]);
                if (checkIfMatch){
                    BUBBLES[i].next = &BUBBLES[uniqueColours[j].id];
                    uniqueColours[j].id = BUBBLES[i].id;
                } 

            }else if(uniqueColours[j].colour == BLACK)
            {
                uniqueColours[j].id = BUBBLES[i].id;
                uniqueColours[j].x_loc = BUBBLES[i].x_loc;
                uniqueColours[j].y_loc = BUBBLES[i].y_loc;
                uniqueColours[j].colour = BUBBLES[i].colour;
            }
        }
    }
}

void wipeoutMatchingColours(struct bubble* startingBubble)
{
    if (startingBubble->next != NULL) wipeoutMatchingColours(startingBubble->next);
    startingBubble->colour = BLACK;
    return;
}

void drawBubbles()
{
    int count, x, y;
    for(count = 0; count < ARRAY_SIZE; count++)
    {
        for(x = BUBBLES[count].x_loc-1; x <= BUBBLES[count].x_loc+1; x++)
        {
            for(y = BUBBLES[count].y_loc-1; y <= BUBBLES[count].y_loc+1; y++)
            {
                if(x>=0 && x<=X_MAX && y>=0 && y<=Y_MAX) plot_pixel(x, y, BUBBLES[count].colour);
            }
        }
    }
}

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(PIXEL_BUFFER_START + (y << 10) + (x << 1)) = line_color;
}

void draw_line(int x0, int y0, int x1, int y1, short int line_color)
{
	bool is_steep = abs(y1 - y0) > abs(x1 - x0);
	
	if(is_steep){
		swap(&x0, &y0);
		swap(&x1, &y1);
	}
	
	if(x0 > x1){
		swap(&x0, &x1);
		swap(&y0, &y1);
	}
	
	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = -(deltax / 2);
	int y = y0;
	
	int y_step;
	if (y0 < y1) y_step = 1;
	else y_step = -1;
	
	int x;
	for(x = x0; x <= x1; x++)
	{
		if(is_steep) plot_pixel(y, x, line_color);
		else plot_pixel(x, y, line_color);
		error = error + deltay;
		
		if(error >= 0)
		{
			y = y + y_step;
			error = error - deltax;
		}
	}
}

void clear_screen()
{
	int x, y;
	for (x = 0; x < X_MAX; x++)
	{
		for (y = 0; y < Y_MAX; y++)
		{
			plot_pixel(x, y, 0x0000);
		}
	}
}

void swap(int *x, int *y)
{
	int temp = *x;
	*x = *y;
	*y = temp; 
}