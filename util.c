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
    newBubble.x_vel = 0;
    newBubble.y_vel = 0;
    newBubble.colour = BLACK;
    newBubble.visited = false;

    int i;
    for(i = 0; i < 8; i++)
    {
        newBubble.next[i] = NULL;
    }

    return newBubble;
}

void initializeBubbles()
{
    int it;
    for(it = 0; it < ARRAY_SIZE; it++)
    {
        BUBBLES[it] = bubbleConstructor();

        BUBBLES[it].id = it;
        BUBBLES[it].x_loc = it % X_MAX;
        BUBBLES[it].y_loc = it % Y_MAX;

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
                BUBBLES[it].colour = YELLOW;
                break;
        }  
    }
}

void initializeUserBubble()
{
    int it;
    for(it = 0; it < 10; it++)
    {
        USER_BUBBLES[it] = bubbleConstructor();

        USER_BUBBLES[it].id = it;
        USER_BUBBLES[it].x_loc = X_MAX / 2;
        USER_BUBBLES[it].y_loc = Y_MAX - BUBBLE_WIDTH/2;

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
    int i, j, x_dir, y_dir;
    for(i = 0; i < ARRAY_SIZE; i++)
    {
        for(x_dir = BUBBLES[i].x_loc - BUBBLE_WIDTH; x_dir <= BUBBLES[i].x_loc + BUBBLE_WIDTH; x_dir += BUBBLE_WIDTH)
        {
            for(y_dir = BUBBLES[i].y_loc - BUBBLE_WIDTH; y_dir <= BUBBLES[i].y_loc + BUBBLE_WIDTH; y_dir += BUBBLE_WIDTH)
            {
                if(!(x_dir == BUBBLES[i].x_loc && y_dir == BUBBLES[i].y_loc) && (x_dir > 0 && x_dir < X_MAX) && (y_dir > 0 && y_dir < Y_MAX*0.5))
                {
                    int neighbourId = coordsToId(x_dir, y_dir);
                    if(neighbourId != -1 && BUBBLES[i].colour == BUBBLES[neighbourId].colour)
                    {
                        for(j = 0; j < 8; j++)
                        {
                            if(BUBBLES[i].next[j] == NULL)
                            {
                               BUBBLES[i].next[j] = &BUBBLES[neighbourId];
                               break; 
                            } 
                        }
                    } 
                }
            }    
        }
    }
}

void wipeoutMatchingColours(struct bubble* startingBubble)
{
    startingBubble->visited = true;

    int i;
    for(i = 0; i < 8; i++)
    {
        if(startingBubble->next[i] != NULL && !startingBubble->next[i]->visited) wipeoutMatchingColours(startingBubble->next[i]);
        startingBubble->colour = BLACK;
        return;
    }
}

void drawBubbles()
{
    int count, x, y;
    for(count = 0; count < ARRAY_SIZE; count++)
    {
        for(x = BUBBLES[count].x_loc-(BUBBLE_WIDTH/2); x <= BUBBLES[count].x_loc+(BUBBLE_WIDTH/2); x+=(BUBBLE_WIDTH/2))
        {
            for(y = BUBBLES[count].y_loc-(BUBBLE_WIDTH/2); y <= BUBBLES[count].y_loc+(BUBBLE_WIDTH/2); y+=(BUBBLE_WIDTH/2))
            {
                if(x>=0 && x<=X_MAX && y>=0 && y<=Y_MAX) plot_pixel(x, y, BUBBLES[count].colour);
            }
        }
    }
}

int coordsToId(int x, int y)
{
    int i;
    for(i = 0; i < ARRAY_SIZE; i++)
    {
        if(BUBBLES[i].x_loc == x && BUBBLES[i].y_loc == y) return BUBBLES[i].id;
    }

    return -1;
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
			plot_pixel(x, y, BLACK);
		}
	}
}

void swap(int *x, int *y)
{
	int temp = *x;
	*x = *y;
	*y = temp; 
}

void wait_for_vsync()
{
    int *pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
	*pixel_ctrl_ptr = 1;
	volatile int *reg_status = pixel_ctrl_ptr + 3;
	
	while(1)
	{
		if((*reg_status & 1) == 0) break;
	}
	
	return;
}