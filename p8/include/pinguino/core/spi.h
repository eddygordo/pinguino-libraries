/*	----------------------------------------------------------------------------
    FILE:			spi.h
    PROJECT:		Pinguino
    PURPOSE:		SPI Hardware library
    PROGRAMER:		R�gis Blanchot <rblanchot@gmail.com>
                    Francisco J. S�nchez Rivas (FJ_Sanchez) <fran@mipixel.com>
    FIRST RELEASE:	20 Jun. 2012
    LAST RELEASE:	30 Nov. 2015
    ----------------------------------------------------------------------------
    CHANGELOG:
    30 Nov 2015 - R�gis Blanchot - added PIC16F1459 support
    ----------------------------------------------------------------------------
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    --------------------------------------------------------------------------*/
 
#ifndef __SPI_H__
#define __SPI_H__

#include <compiler.h>

// SPI modules
#define SPISW   0
#define SPI1    1
#define SPI2    2

// pins
#if   defined(__16F1459)

    #define SD_CS           PORTCbits.RC6       // SPI1 Chip Select cf. io.c/IO_remap()
    #define SSPIN           TRISCbits.TRISC6    // SPI1 Chip Select TRIS
    #define SDIPIN          TRISBbits.TRISB4    // SPI1 SDI Master input/Slave output TRIS
    #define SCKPIN          TRISBbits.TRISB6    // SPI1 SCK Clock TRIS
    #define SDOPIN          TRISCbits.TRISC7    // SPI1 SDO Master output/Slave input TRIS

#elif defined(__18f2455)  || defined(__18f4455)  || \
      defined(__18f2550)  || defined(__18f4550)  || \
      defined(__18f25k50) || defined(__18f45k50)
    
    #define SD_CS           PORTAbits.RA5       // D13 - Chip Select
    #define SSPIN           TRISAbits.TRISA5    // D13 - Chip Select TRIS
    #define SDIPIN          TRISBbits.TRISB0    // D0  - SDI Master input/SDO Slave output TRIS
    #define SCKPIN          TRISBbits.TRISB1    // D1  - SCK Clock TRIS
    #define SDOPIN          TRISCbits.TRISC7    // D23 - SDO Master output/SDI Slave input TRIS

#elif defined(__18f26j50)|| defined(__18f46j50) || \
      defined(__18f27j53)|| defined(__18f47j53)

    // SPI1 (Note: SPI SDO1 and Serial RX1 are the same)
    #define SD_CS           LATBbits.LATB6      // SPI1 Chip Select
    #define SSPIN           TRISBbits.TRISB6    // SPI1 Chip Select TRIS
    #define SDIPIN          TRISBbits.TRISB5    // SPI1 SDO Master input/Slave output TRIS
    #define SCKPIN          TRISBbits.TRISB4    // SPI1 SCK Clock TRIS
    #define SDOPIN          TRISCbits.TRISC7    // SPI1 SDI Master output/Slave input TRIS

    // SPI2
    #define SD_CS2          LATBbits.LATB0      // SPI2 Chip Select
    #define SS2PIN          TRISBbits.TRISB0    // SPI2 Chip Select TRIS
    #define SDI2PIN         TRISBbits.TRISB3    // SPI2 SDO Master input/Slave output TRIS
    #define SCK2PIN         TRISBbits.TRISB2    // SPI2 SCK Clock TRIS
    #define SDO2PIN         TRISBbits.TRISB1    // SPI2 SDI Master output/Slave input TRIS

#else

    #error "Your processor is not yet supported"

#endif

/*  ----------------------------------------------------------------------------
    ---------- *** MODES ***
    ----------------------------------------------------------------------------
    SSPM<3:0>: Synchronous Serial Port Mode Select bits
    0000 = SPI Master mode, clock = FOSC/4
    0001 = SPI Master mode, clock = FOSC/16
    0010 = SPI Master mode, clock = FOSC/64
    0011 = SPI Master mode, clock = TMR2 output/2
    0100 = SPI Slave mode, clock = SCK pin, SS pin control enabled
    0101 = SPI Slave mode, clock = SCK pin, SS pin control disabled, SS can be used as I/O pin
    ---------------------------------------------------------------------------*/

// SPI Role (and Clock Divider)
#define SPI_MASTER              0b0000
#define SPI_MASTER8             0b0000
#define SPI_MASTER_FOSC_4       0b0000
#define SPI_MASTER_FOSC_8       0b1010  // xxj5x only
#define SPI_MASTER_FOSC_16      0b0001
#define SPI_MASTER_FOSC_64      0b0010
#define SPI_MASTER_FOSC_T2      0b0011
#define SPI_SLAVE_SS            0b0100
#define SPI_SLAVE               0b0101

// SPI Clock Divider (and Role)
#define SPI_CLOCK_DIV4          SPI_MASTER_FOSC_4
#define SPI_CLOCK_DIV8          SPI_MASTER_FOSC_8
#define SPI_CLOCK_DIV16         SPI_MASTER_FOSC_16
#define SPI_CLOCK_DIV64         SPI_MASTER_FOSC_64
#define SPI_CLOCK_TIMER2        SPI_MASTER_FOSC_T2

// SPI Data Mode
#define SPI_MODE0               0x00
#define SPI_MODE1               0x04
#define SPI_MODE2               0x08
#define SPI_MODE3               0x0C

//
#define SPI_FALLING_EDGE        0x00  // negated
#define SPI_RISING_EDGE         0x01  // negated

// SPI Sample Phase
#define SPI_SMPEND              1<<7
#define SPI_SLEW_RATE_ENABLE    0
#define SPI_SLEW_RATE_DISABLE   1
#define SPI_STANDARD_SPEED_MODE 1
#define SPI_HIGH_SPEED_MODE     0

// SPI BitOrder
#define SPI_LSBFIRST            0
#define SPI_MSBFIRST            1

/// Typedef

typedef struct
{
    u8  mode;
    u32 divider;
    u8  role;
    u8  bitorder;
    u8  phase;
    u8  sda;
    u8  sck;
    u8  cs;
} spi_t;

/// Prototypes

void SPI_select(u8 module);
void SPI_deselect(u8 module);
void SPI_begin(u8 module);
void SPI_close(u8 module);
void SPI_init(u8 module, u8 sync_mode, u8 bus_mode, u8 smp_phase);
void SPI_setMode(u8 module, u8 mode);
void SPI_setBitOrder(u8 module, u8 bitorder);
void SPI_setDataMode(u8 module, u8 mode);
void SPI_setClockDivider(u8 module, u8 clock);
u8 SPI_write(u8 module, u8 datax);
u8 SPI_read(u8 module);

#ifdef SPIINT
void SPI1_interrupt();
static void SPI1_onEvent (u8(*func)(u8));
static u8 (*SPI1_onEvent_func)( u8);

#if defined(__18f26j50)|| defined(__18f46j50) || \
    defined(__18f27j53)|| defined(__18f47j53)

void SPI2_interrupt();
static void SPI2_onEvent (u8(*func)(u8));
static u8 (*SPI2_onEvent_func)( u8);

#endif
#endif

/// Globals

#if defined(__18f26j50)|| defined(__18f46j50) || \
    defined(__18f27j53)|| defined(__18f47j53)
    
    #define NUMOFSPI 3                  // 2 SPI Hardware + 1 SPI Software

#else

    #define NUMOFSPI 2                  // 1 SPI Hardware + 1 SPI Software

#endif

spi_t _spi[NUMOFSPI];

#endif   /* __SPI_H__ */
