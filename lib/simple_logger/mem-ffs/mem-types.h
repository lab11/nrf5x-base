/*
IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		FAT FILING SYSTEM FAT16 & FAT 32 DRIVER
//NXP LPC2365 SAMPLE PROJECT GENERIC GLOBAL HEADER FILE



//***************************
//***************************
//********** NOTES **********
//***************************
//***************************
//- To overcome a silicon issue the battery must be inserted for the first time (or when being replaced) with the board powered, or just before the board is powered.  If
//the battery is inserted and the board not powered for some time the current drain will be high.
//- UART0 is also used as the RS232 bootloader.  If GPIO pin P2.10 (P0.14 on some devices) is low while RESET is low it forces on-chip bootloader to take over control
//of the part after a reset.  If code protecting the chip you need the PCB capability to program via UART0 as once code protected the JTAG interface is disabled.
//- Using 'USB CrossConnect for ARM-RTCK' JTAG debugger:
//Debug interface type: ARM7TDI


//******************************************
//******************************************
//********** PROJECT GLOBAL FLAGS **********
//******************************************
//******************************************





//*************************************
//*************************************
//********** GENERAL DEFINES **********
//*************************************
//*************************************

//INCLUDE THE DEVICE HEADER FILE
#include  "nrf.h"


#define	nop		__asm("nop")


//----------------------
//----- OSCILLATOR -----
//----------------------
//Errata: Rev A devices: Code execution from internal flash is restricted to a maximum of 60MHz.
//12MHz Osc
//Fosc = 12Mhz
//Fcco = 480Mhz
//CCLK (CPU Clock) = 60Mhz
//PCLK (Peripheral Clock) selectable per peripheral.  Default = cclk / 4 = 15Mhz
//#define	INSTRUCTION_CLOCK_FREQUENCY				60000000


//----------------------
//----- INTERRUPTS -----
//----------------------
//#define	ENABLE_INT	libarm_enable_irq()				//Enable all unmasket interupts
//#define	DISABLE_INT	libarm_disable_irq()			//Disable all unmasket interupts


//----------------------
//----- WATCHDOG -----
//----------------------
//#ifdef DEBUG
//	#define	ClrWdt   __asm("nop")
//#else
//	#define	ClrWdt   DISABLE_INT; WDFEED = 0xaa; WDFEED = 0x55; ENABLE_INT					//Enable / reset the Watchdog timer by writing 0xAA followed by 0x55 to the WDFEED register
//#endif







//****************************************
//****************************************
//***** GLOBAL DATA TYPE DEFINITIONS *****
//****************************************
//****************************************
#ifndef GLOBAL_DATA_TYPE_INIT				//(Include this section only once for each source file)
#define	GLOBAL_DATA_TYPE_INIT

#define	CONSTANT	const 				//Define used for this as some compilers require an additional qualifier such as 'rom' to signify that a constant should be stored in program memory

#undef BOOL
#undef TRUE
#undef FALSE
#undef BYTE
#undef SIGNED_BYTE
#undef WORD
#undef SIGNED_WORD
#undef DWORD
#undef SIGNED_DWORD

//BOOLEAN - 1 bit:
typedef enum _BOOL { FALSE = 0, TRUE } BOOL;
//BYTE - 8 bit unsigned:
typedef unsigned char BYTE;
//SIGNED_BYTE - 8 bit signed:
typedef signed char SIGNED_BYTE;
//WORD - 16 bit unsigned:
typedef unsigned short WORD;
//SIGNED_WORD - 16 bit signed:
typedef signed short SIGNED_WORD;
//DWORD - 32 bit unsigned:
typedef unsigned long DWORD;
//SIGNED_DWORD - 32 bit signed:
typedef signed long SIGNED_DWORD;
//QWORD - 64 bit unsigned:
typedef unsigned long long QWORD;
//SIGNED_QWORD - 64 bit signed:
typedef signed long long SIGNED_QWORD;

//BYTE BIT ACCESS:
typedef union _BYTE_VAL
{
    struct
    {
        unsigned char b0:1;
        unsigned char b1:1;
        unsigned char b2:1;
        unsigned char b3:1;
        unsigned char b4:1;
        unsigned char b5:1;
        unsigned char b6:1;
        unsigned char b7:1;
    } bits;
    BYTE Val;
} BYTE_VAL;

//WORD ACCESS
typedef union _WORD_VAL
{
    WORD val;
    struct
    {
        BYTE LSB;
        BYTE MSB;
    } byte;
    BYTE v[2];
} WORD_VAL;
#define LSB(a)          ((a).v[0])
#define MSB(a)          ((a).v[1])

//DWORD ACCESS:
typedef union _DWORD_VAL
{
    DWORD val;
    struct
    {
        BYTE LOLSB;
        BYTE LOMSB;
        BYTE HILSB;
        BYTE HIMSB;
    } byte;
    struct
    {
        WORD LSW;
        WORD MSW;
    } word;
    BYTE v[4];
} DWORD_VAL;
#define LOWER_LSB(a)    ((a).v[0])
#define LOWER_MSB(a)    ((a).v[1])
#define UPPER_LSB(a)    ((a).v[2])
#define UPPER_MSB(a)    ((a).v[3])

//EXAMPLE OF HOW TO USE THE DATA TYPES:-
//	WORD_VAL variable_name;				//Define the variable
//	variable_name = 0xffffffff;			//Writing 32 bit value
//	variable_name.LSW = 0xffff;			//Writing 16 bit value to the lower word 
//	variable_name.LOLSB = 0xff;			//Writing 8 bit value to the low word least significant byte
//	variable_name.v[0] = 0xff;			//Writing 8 bit value to byte 0 (least significant byte)


#endif		//GLOBAL_DATA_TYPE_INIT
