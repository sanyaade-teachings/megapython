/*
 * PyMite - A flyweight Python interpreter for 8-bit microcontrollers and more.
 * Copyright 2006 Dean Hall
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#undef __FILE_ID__
#define __FILE_ID__ 0x51

/**
 * PyMite platform-specific routines for AVR target
 *
 * Log
 * ---
 *
 * 2007/01/31   #86: Move platform-specific code to the platform impl file
 * 2007/01/10   #75: Added time tick service for desktop (POSIX) and AVR. (P.Adelt)
 * 2006/12/26   #65: Create plat module with put and get routines
 */


#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include "../pm.h"


/***************************************************************
 * Configuration
 **************************************************************/

/**
 * When defined, the AVR target configures Timer/Counter0 to generate an
 * overflow interrupt to call pm_vmPeriodic().
 * If you configure T/C0 yourself, disable this define and be sure to
 * periodically call pm_vmPeriodic(usec)!
 * Has no meaning on non-AVR.
 */
/*#define AVR_DEFAULT_TIMER_SOURCE */


/***************************************************************
 * Constants
 **************************************************************/
 
#ifdef AVR_DEFAULT_TIMER_SOURCE

/* Hint: 1,000,000 µs/s * 256 T/C0 clock cycles per tick * 8 CPU clocks per  
 * T/C0 clock cycle / x,000,000 CPU clock cycles per second -> µs per tick  
 */  
#define PLAT_TIME_PER_TICK_USEC (1000000ULL*256ULL*8ULL/F_CPU)

#endif /* AVR_DEFAULT_TIMER_SOURCE */

#define UBRR_VALUE ((F_CPU / (16UL * UART_BAUD)) - 1)

/* Select which serial port to use as default platform output if more than one exists */
#define SERIAL 0
/* See if a port already exists */
#if UBRRH && UBRRL && UCSRB && TXEN && RXEN && USBS && UCSZ0
#else
  #if SERIAL == 0
    #define UBRRH UBRR0H
    #define UBRRL UBRR0L
    #define UCSRB UCSR0B
    #define UCSRC UCSR0C
    #undef TXEN
    #define TXEN TXEN0
    #undef RXEN
    #define RXEN RXEN0
    #undef USBS
    #define USBS USBS0
    #undef UCSZ0
    #define UCSZ0 UCSZ00
  #endif /* SERIAL == 0 */

  #if SERIAL == 1
    #define UBRRH UBRR1H
    #define UBRRL UBRR1L
    #define UCSRB UCSR1B
    #define UCSRC UCSR1C
    #undef TXEN
    #define TXEN TXEN1
    #undef RXEN
    #define RXEN RXEN1
    #undef USBS
    #define USBS USBS1
    #undef UCSZ0
    #define UCSZ0 UCSZ10
  #endif /* SERIAL == 1 */
#endif /* serial port shit */

/***************************************************************
 * Functions
 **************************************************************/

void plat_USART_init(unsigned int ubrr)
{
    /* Set the baud rate register */
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char)ubrr;
    /* Enable the transmit and receive pins */
    UCSRB = _BV(TXEN) | _BV(RXEN);
    /* Set frame format: 8data, 2stop bit */
    UCSRC = _BV(USBS) | (3<<UCSZ0);
}

/*
 * AVR target shall use stdio for I/O routines.
 * The UART or USART must be configured for the interactive interface to work.
 */
PmReturn_t
plat_init(void)
{
    /* PORT BEGIN: Set these UART/USART SFRs properly for your AVR */
    plat_USART_init(UBRR_VALUE);
    /* PORT END */
    
#ifdef AVR_DEFAULT_TIMER_SOURCE
    /* PORT BEGIN: Configure a timer that fits your needs. */
    /* Use T/C0 in synchronous mode, aim for a tick rate of
     * several hundred Hz */
#if (TARGET_MCU == atmega103) || (TARGET_MCU == atmega128)
    /* set T/C0 to use synchronous clock */
    ASSR &= ~(1<<AS0);
    /* set prescaler to /8 */
    TCCR0 &= ~0x07;
    TCCR0 |= (1<<CS01);
#else
/*#error No timer configuration is implemented for this AVR.*/
#endif 
#endif /* AVR_DEFAULT_TIMER_SOURCE */
    /* PORT END */

    return PM_RET_OK;
}

#ifdef AVR_DEFAULT_TIMER_SOURCE
ISR(TIMER0_OVF_vect) 
{
    /* TODO Find a clever way to handle bad return code, maybe use 
     * PM_REPORT_IF_ERROR(retval) when that works on AVR inside an
     * interrupt.
     */ 
    pm_vmPeriodic(PLAT_TIME_PER_TICK_USEC);
}
#endif


/*
 * Gets a byte from the address in the designated memory space
 * Post-increments *paddr.
 */
uint8_t
plat_memGetByte(PmMemSpace_t memspace, uint8_t const **paddr)
{
    uint8_t b = 0;

    switch (memspace)
    {
        case MEMSPACE_RAM:
            b = **paddr;
            *paddr += 1;
            return b;

        case MEMSPACE_PROG:
            b = pgm_read_byte(*paddr);
            *paddr += 1;
            return b;

        case MEMSPACE_EEPROM:
            b = eeprom_read_byte(*paddr);
            *paddr += 1;
            return b;

        case MEMSPACE_SEEPROM:
        case MEMSPACE_OTHER0:
        case MEMSPACE_OTHER1:
        case MEMSPACE_OTHER2:
        case MEMSPACE_OTHER3:
        default:
            return 0;
    }
}


/*
 * UART receive char routine MUST return exactly and only the received char;
 * it should not translate \n to \r\n.
 * This is because the interactive interface uses binary transfers.
 */
PmReturn_t
plat_getByte(uint8_t *b)
{
    PmReturn_t retval = PM_RET_OK;

    /* PORT BEGIN: Set these UART/USART SFRs properly for your AVR */
    /* Loop until serial receive is complete */
    loop_until_bit_is_set(UCSR0A, RXC0);
    
    /* If a framing error or data overrun occur, raise an IOException */
    if (UCSR0A & (_BV(FE0) | _BV(DOR0)))
    {
        PM_RAISE(retval, PM_RET_EX_IO);
        return retval;
    }
    *b = UDR0;
    /* PORT END */

    return retval;
}


/*
 * UART send char routine MUST send exactly and only the given char;
 * it should not translate \n to \r\n.
 * This is because the interactive interface uses binary transfers.
 */
PmReturn_t
plat_putByte(uint8_t b)
{
    /* PORT BEGIN: Set these UART/USART SFRs properly for your AVR */
    /* Loop until serial data reg is empty (from previous transfer) */
    loop_until_bit_is_set(UCSR0A, UDRE0);
    
    /* Put the byte to send into the serial data register */
    UDR0 = b;
    /* PORT END */

    return PM_RET_OK;
}


/*
 * This operation is made atomic by temporarily disabling
 * the interrupts. The old state is restored afterwards.
 */
PmReturn_t
plat_getMsTicks(uint32_t *r_ticks)
{
    /* Critical section start */
    unsigned char _sreg = SREG;
    cli();
    *r_ticks = pm_timerMsTicks;
    SREG = _sreg;
    /* Critical section end */
    return PM_RET_OK;
}


void 
plat_reportError(PmReturn_t result)
{
    /* TODO #97: Implement plat_reportError for AVR */
}
