/*Maintains all interrupt handling and settings*/
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "globals.h"
#include "address_map_arm.h"

#define		CPU0	    0x01	// bit-mask; bit 0 represents cpu0
#define		ENABLE		0x1
#define		SVC_MODE	0b10011
#define	    PS2_IRQ		79
#define		INT_ENABLE	0b01000000

// Declarations.
extern int ps2_byte_1, ps2_byte_2, ps2_byte_3;

// Interrupt mode setup.
void set_A9_IRQ_stack(void);
void config_GIC(void);
void enable_A9_interrupts(void);
void config_PS2();
void config_interrupt (int int_ID, int CPU_target);
void hw_write_bits (volatile int * addr, volatile int unmask, volatile int value);

// Service Routines.
void PS2_ISR();

#endif