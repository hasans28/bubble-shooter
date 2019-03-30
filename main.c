#include "util.h"

volatile int PIXEL_BUFFER_START;

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    volatile int * KEY_ptr = (int *)KEY_BASE;
    int KEY_Value;
    bool Reset_Board;

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    PIXEL_BUFFER_START = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    PIXEL_BUFFER_START = *(pixel_ctrl_ptr + 1); // we draw on the back buffer

    while(1)
    {
        Reset_Board = false;

        initializeBubbles();
        setupColourMatchLinks();
        initializeUserBubble();

        while (!Reset_Board)
        {
            KEY_Value = *(KEY_ptr);
            if(KEY_Value != 0)
            {
                if(KEY_Value == 1)keyZeroResponse();
                else if(KEY_Value == 2)keyOneResponse();
                else if(KEY_Value == 4)keyTwoResponse();
                while(*KEY_ptr);
            }
            /* Erase any boxes and lines that were drawn in the last iteration */
            clear_screen();
            displayToHex();
            // code for drawing the boxes and lines 
            drawBubbles();
            drawUserBubbles();
            // code for updating the locations of boxes
            Reset_Board = setBoardReset();

            wait_for_vsync(); // swap front and back buffers on VGA vertical sync
            PIXEL_BUFFER_START = *(pixel_ctrl_ptr + 1); // new back buffer
        }
    }
    
}