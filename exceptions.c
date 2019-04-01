#include "exceptions.h"

// Define the exception handlers here 
void __attribute__ ((interrupt)) __cs3_reset (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_undef (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_swi (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_pabort (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_dabort (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_irq (void)
{
	// Read the ICCIAR from the processor interface 
	int address = MPCORE_GIC_CPUIF + ICCIAR; 
	int int_ID = *((int *) address); 
   
    if (int_ID == PS2_IRQ)				// check if interrupt is from the PS/2
		PS2_ISR ();
	else
		while (1);									// if unexpected, then halt

	// Write to the End of Interrupt Register (ICCEOIR)
	address = MPCORE_GIC_CPUIF + ICCEOIR;
	*((int *) address) = int_ID;

	return;
} 

void __attribute__ ((interrupt)) __cs3_isr_fiq (void)
{
    while(1);
}

void config_PS2() {
    volatile int *PS2_ptr = (int *) 0xFF200100;
    *(PS2_ptr) = 0xFF; // Reset
    *(PS2_ptr + 1) = 1;
}

// PS2 service routine.
void PS2_ISR() {
    volatile int * PS2_ptr = (int *) 0xFF200100; // PS/2 port address
    int PS2_data, RVALID;
    
    PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
    RVALID = PS2_data & 0x8000; // extract the RVALID field
    if (RVALID) {
        // Save the last three bytes of data.
        ps2_byte_1 = ps2_byte_2;
        ps2_byte_2 = ps2_byte_3;
        ps2_byte_3 = PS2_data & 0xFF;  

        // Process data.
        if(ps2_byte_1 == 0xE0 && ps2_byte_2 == 0xF0 && ps2_byte_3 == 0x74) {
            keyZeroResponse(); // RIGHT
        }
        else if(ps2_byte_1 == 0xE0 && ps2_byte_2 == 0xF0 && ps2_byte_3 == 0x6B) {
            keyOneResponse(); // LEFT
        }
        else if(ps2_byte_2 == 0xF0 && ps2_byte_3 == 0x29) {
            keyTwoResponse(); // SpaceBar
        }
        else {
            return;
        }
    }
}

/*--------------From the DE1-SoC Manual-----------------------*/

// Initialize the banked stack pointer register for IRQ mode
void set_A9_IRQ_stack(void) {
    int stack, mode;
    stack = 0xFFFFFFFF - 7; // top of A9 on-chip memory, aligned to 8 bytes
    /* change processor to IRQ mode with interrupts disabled */
    mode = 0b11010010;
    asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
    /* set banked stack pointer */
    asm("mov sp, %[ps]" : : [ps] "r" (stack));
    /* go back to SVC mode before executing subroutine return! */
    mode = 0b11010011;
    asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}

// Turn on interrupts in the ARM processor.
void enable_A9_interrupts(void)
{
	int status = SVC_MODE | INT_ENABLE;
	asm("msr cpsr,%[ps]" : : [ps]"r"(status));
}

// Configure the Generic Interrupt Controller (GIC).
void config_GIC(void)
{
	int address;	// used to calculate register addresses

	/* enable some examples of interrupts */
  	config_interrupt (PS2_IRQ, CPU0);

  	// Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts for lowest priority 
	address = MPCORE_GIC_CPUIF + ICCPMR;
  	*((int *) address) = 0xFFFF;       

  	// Set CPU Interface Control Register (ICCICR). Enable signaling of interrupts
	address = MPCORE_GIC_CPUIF + ICCICR;
	*((int *) address) = ENABLE;

	// Configure the Distributor Control Register (ICDDCR) to send pending interrupts to CPUs 
	address = MPCORE_GIC_DIST + ICDDCR;
	*((int *) address) = ENABLE;   
}

/* 
 * Configure registers in the GIC for individual interrupt IDs.
*/
void config_interrupt (int int_ID, int CPU_target)
{
	int n, addr_offset, value, address;
	/* Set Interrupt Processor Targets Register (ICDIPTRn) to cpu0. 
	 * n = integer_div(int_ID / 4) * 4
	 * addr_offet = #ICDIPTR + n
	 * value = CPU_target << ((int_ID & 0x3) * 8)
	 */
	n = (int_ID >> 2) << 2;
	addr_offset = ICDIPTR + n;
	value = CPU_target << ((int_ID & 0x3) << 3);
	
	/* Now that we know the register address and value, we need to set the correct bits in 
	 * the GIC register, without changing the other bits */
	address = MPCORE_GIC_DIST + addr_offset;
	hw_write_bits((int *) address, 0xff << ((int_ID & 0x3) << 3), value);  
    
	/* Set Interrupt Set-Enable Registers (ICDISERn). 
	 * n = (integer_div(in_ID / 32) * 4
	 * addr_offset = 0x100 + n
	 * value = enable << (int_ID & 0x1F) */
	n = (int_ID >> 5) << 2; 
	addr_offset = ICDISER + n;
	value = 0x1 << (int_ID & 0x1f);
	/* Now that we know the register address and value, we need to set the correct bits in 
	 * the GIC register, without changing the other bits */
	address = MPCORE_GIC_DIST + addr_offset;
	hw_write_bits((int *) address, 0x1 << (int_ID & 0x1f), value);    
}

void hw_write_bits(volatile int * addr, volatile int unmask, volatile int value)
{     
    *addr = ((~unmask) & *addr) | value;
}