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
    for(i = 0; i < 4; i++)
    {
        newBubble.next[i] = NULL;
    }

    return newBubble;
}

void initializeBubbles()
{
    int it;
    for(it = 0; it < BUBBLES_ARRAY_SIZE; it++)
    {
        BUBBLES[it] = bubbleConstructor();

        BUBBLES[it].id = it;
        BUBBLES[it].x_loc = (it*BUBBLE_WIDTH) % X_MAX;
        BUBBLES[it].y_loc = (it/16) * BUBBLE_WIDTH;

		//srand(time(0));
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
        USER_BUBBLES[it].y_loc = Y_MAX - BUBBLE_WIDTH;

		//srand(time(0));
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

bool collisionCheck()
{
    int i, j, bubbleId, userBubbleId, colour;
    struct bubble* next;
	bool wipeOut = false;
    for(i = 0; i < USER_ARRAY_SIZE; i++)
    {
        if(USER_BUBBLES[i].id == -1 && USER_BUBBLES[i].velocity != 0)
        {
			struct bubble* bub = &USER_BUBBLES[i];
			
            // Checking upwards
            bubbleId = coordsToBubbleId(USER_BUBBLES[i].x_loc, USER_BUBBLES[i].y_loc-BUBBLE_WIDTH);
            userBubbleId = coordsToUserId(USER_BUBBLES[i].x_loc, USER_BUBBLES[i].y_loc-BUBBLE_WIDTH);
            if(bubbleId != -1 || userBubbleId != -1)
            {
                 colour = (bubbleId == -1) ? USER_BUBBLES[userBubbleId].colour : BUBBLES[bubbleId].colour;
                 next = (bubbleId == -1) ? &USER_BUBBLES[userBubbleId] : &BUBBLES[bubbleId];
                 if(colour != BLACK)
                 {
                     bub->velocity = 0;
                     if(bub->colour == colour)
                     {
                         bub->colour = BLACK;
                         bub->next[0] = next; 
                         wipeOut = true;
                     }
                 }
            }

            //Checking to the left
            bubbleId = coordsToBubbleId(USER_BUBBLES[i].x_loc-BUBBLE_WIDTH, USER_BUBBLES[i].y_loc);
            userBubbleId = coordsToUserId(USER_BUBBLES[i].x_loc-BUBBLE_WIDTH, USER_BUBBLES[i].y_loc);
            if(bubbleId != -1 || userBubbleId != -1)
            {
                 colour = (bubbleId == -1) ? USER_BUBBLES[userBubbleId].colour : BUBBLES[bubbleId].colour;
                 next = (bubbleId == -1) ? &USER_BUBBLES[userBubbleId] : &BUBBLES[bubbleId];
                 if(colour != BLACK)
                 {
                     if(bub->colour == colour && bub->next[0] != NULL)
                     {
                         bub->velocity = 0;
                         bub->colour = BLACK;
                         bub->next[1] = next;
                         wipeOut = true;
                     }
                 }
            }

            //Checking to the right
            bubbleId = coordsToBubbleId(USER_BUBBLES[i].x_loc+BUBBLE_WIDTH, USER_BUBBLES[i].y_loc);
            userBubbleId = coordsToUserId(USER_BUBBLES[i].x_loc+BUBBLE_WIDTH, USER_BUBBLES[i].y_loc);
            if(bubbleId != -1 || userBubbleId != -1)
            {
                 colour = (bubbleId == -1) ? USER_BUBBLES[userBubbleId].colour : BUBBLES[bubbleId].colour;
                 next = (bubbleId == -1) ? &USER_BUBBLES[userBubbleId] : &BUBBLES[bubbleId];
                 if(colour != BLACK)
                 {
                     if(bub->colour == colour && bub->next[0] != NULL)
                     {
                         bub->velocity = 0;
                         bub->colour = BLACK;
                         bub->next[2] = next;
                         wipeOut = true;
                     }
                 }
            }
        
			if(wipeOut) wipeoutMatchingColours(bub);
		}
    }
	
    return wipeOut;
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

int coordsToBubbleId(int x, int y)
{
    int i;
    for(i = 0; i < BUBBLES_ARRAY_SIZE; i++)
    {
        if(BUBBLES[i].x_loc == x && BUBBLES[i].y_loc == y && BUBBLES[i].colour != BLACK) return BUBBLES[i].id;
    }

    return -1;
}

int coordsToUserId(int x, int y)
{
    int i;
    for(i = 0; i < USER_ARRAY_SIZE; i++)
    {
        if(USER_BUBBLES[i].x_loc == x && USER_BUBBLES[i].y_loc == y && USER_BUBBLES[i].colour != BLACK) return i;
    }

    return -1;
}

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(PIXEL_BUFFER_START + (y << 10) + (x << 1)) = line_color;
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

bool setBoardReset()
{
    int i, count = 0;
    for(i = 0; i < USER_ARRAY_SIZE; i++)
    {
        if(USER_BUBBLES[i].id == -1 && USER_BUBBLES[i].velocity == 0) count++;
    }
    if(count == USER_ARRAY_SIZE) return true;
    return false;
}

int setTempScore()
{
    int i, total = 0;
    for(i = 0; i < BUBBLES_ARRAY_SIZE; i++)
    {
        if(BUBBLES[i].colour == BLACK) total++;
        if(i < USER_ARRAY_SIZE && USER_BUBBLES[i].colour == BLACK) total++;
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