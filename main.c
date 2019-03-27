#include "util.h"

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;

    initializeBubbles();

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

    while (1)
    {
        /* Erase any boxes and lines that were drawn in the last iteration */
        clear_screen();
        // code for drawing the boxes and lines 
        drawBubbles();
        // code for updating the locations of boxes

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        PIXEL_BUFFER_START = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}