#include "util.h"

struct bubble BUBBLES[96];
struct bubble USER_BUBBLES[10];
int HEX_SCORE = 0;

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
    for(i = 0; i < 8; i++)
    {
        newBubble.next[i] = NULL;
    }

    return newBubble;
}

void initializeBubbles()
{
    int it;
    for(it = 0; it < 96; it++)
    {
        BUBBLES[it] = bubbleConstructor();

        BUBBLES[it].id = it;
        BUBBLES[it].x_loc = (it*BUBBLE_WIDTH) % X_MAX;
        BUBBLES[it].y_loc = (it/16) * BUBBLE_WIDTH;

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
        USER_BUBBLES[it].x_loc = BUBBLE_WIDTH * 8;
        USER_BUBBLES[it].y_loc = Y_MAX - BUBBLE_WIDTH - 1;

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
    for(i = 0; i < 96; i++)
    {
        index = coordsToId(BUBBLES[i].x_loc-BUBBLE_WIDTH, BUBBLES[i].y_loc);
        if(index != -1)
        {
            if(BUBBLES[i].colour == BUBBLES[index].colour) BUBBLES[i].next[0] = &BUBBLES[index];
        }

        index = coordsToId(BUBBLES[i].x_loc+BUBBLE_WIDTH, BUBBLES[i].y_loc);
        if(index != -1)
        {
            if(BUBBLES[i].colour == BUBBLES[index].colour) BUBBLES[i].next[1] = &BUBBLES[index];
        }

        index = coordsToId(BUBBLES[i].x_loc, BUBBLES[i].y_loc-BUBBLE_WIDTH);
        if(index != -1)
        {
            if(BUBBLES[i].colour == BUBBLES[index].colour) BUBBLES[i].next[2] = &BUBBLES[index];
        }

        index = coordsToId(BUBBLES[i].x_loc, BUBBLES[i].y_loc+BUBBLE_WIDTH);
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

bool collisionCheck()
{
    int i;
    for(i = 0; i < 10; i++)
    {
        if(USER_BUBBLES[i].id == -1 && USER_BUBBLES[i].velocity != 0)
        {
            int coordsCheck = coordsToId(USER_BUBBLES[i].x_loc, USER_BUBBLES[i].y_loc-BUBBLE_WIDTH-1);
            if(coordsCheck != -1)
            {
                USER_BUBBLES[i].velocity = 0;
                if(USER_BUBBLES[i].colour == BUBBLES[coordsCheck].colour)
                {
                    struct bubble* bub = &USER_BUBBLES[i];
                    bub->colour = BLACK;
                    bub->next[0] = &BUBBLES[coordsCheck];
                    wipeoutMatchingColours(bub);
                    return true;
                }
            }
        }
    }
    return false;
}

void drawBubbles()
{
    int count, x, y;
    for(count = 0; count < 96; count++)
    {
        for(x = BUBBLES[count].x_loc; x <= BUBBLES[count].x_loc+BUBBLE_WIDTH-1; x++)
        {
            for(y = BUBBLES[count].y_loc; y <= BUBBLES[count].y_loc+BUBBLE_WIDTH-1; y++)
            {
                if(x>=0 && x<=X_MAX && y>=0 && y<=Y_MAX) plot_pixel(x, y, BUBBLES[count].colour);
            }
        }
    }
}

void drawUserBubbles()
{
    int count, x, y;
    for(count = 0; count < 10; count++)
    {
        USER_BUBBLES[count].y_loc += USER_BUBBLES[count].velocity;

        for(x = USER_BUBBLES[count].x_loc; x <= USER_BUBBLES[count].x_loc+BUBBLE_WIDTH-1; x++)
        {
            for(y = USER_BUBBLES[count].y_loc; y <= USER_BUBBLES[count].y_loc+BUBBLE_WIDTH-1; y++)
            {
                if(x>=0 && x<=X_MAX && y>=0 && y<=Y_MAX) plot_pixel(x, y, USER_BUBBLES[count].colour);
            }
        }
    }
}

int coordsToId(int x, int y)
{
    int i;
    for(i = 0; i < 96; i++)
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

int setTempScore()
{
    int i, total = 0;
    for(i = 0; i < 96; i++)
    {
        if(BUBBLES[i].colour == BLACK) total++;
        if(i < 10 && USER_BUBBLES[i].colour == BLACK) total++;
    }

    return total;
}

void displayToHex()
{
    if(collisionCheck())HEX_SCORE = setTempScore();

    volatile int * HEX3_HEX0_PTR = (int *)HEX3_HEX0_BASE;

    unsigned char seven_seg_decode_table[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    unsigned char hex_segs[] = {0, 0, 0, 0};
    unsigned int shift_buffer, nibble;
    unsigned char code;
    int i, j;
    int score = HEX_SCORE;

    for(i = 0; i < 4; i++)
    {
        j = score % 10;
        score /= 10;
        shift_buffer = j << (4 * i) | shift_buffer;
    }

    for (i = 0; i < 4; ++i) 
    {
        nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble
        code = seven_seg_decode_table[nibble];
        hex_segs[i] = code;
        shift_buffer = shift_buffer >> 4;
    }

    /* drive the hex displays */
    *(HEX3_HEX0_PTR) = *(int *)(hex_segs);

    return;
}