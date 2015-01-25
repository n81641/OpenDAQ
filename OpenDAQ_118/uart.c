///
/// \file uart.c
/// \brief hardware UART commands  
/// \details 
/// \author Roland Riegel <feedback@roland-riegel.de>
/// \copyright Copyright (c) 2006-2009 by Roland Riegel <feedback@roland-riegel.de>
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License version 2 as
/// published by the Free Software Foundation.
///

#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sfr_defs.h>
#include <avr/sleep.h>

#include "uart.h"
#include "utilities.h"


/* some mcus have multiple uarts */
#ifdef UDR0
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define UDR UDR0

#define UCSRA UCSR0A
#define UDRE UDRE0
#define RXC RXC0

#define UCSRB UCSR0B
#define RXEN RXEN0
#define TXEN TXEN0
#define RXCIE RXCIE0

#define UCSRC UCSR0C
#define URSEL 
#define UCSZ0 UCSZ00
#define UCSZ1 UCSZ01
#define UCSRC_SELECT 0
#else
#define UCSRC_SELECT (1 << URSEL)
#endif

#ifndef USART_RXC_vect
#if defined(UART0_RX_vect)
#define USART_RXC_vect UART0_RX_vect
#elif defined(UART_RX_vect)
#define USART_RXC_vect UART_RX_vect
#elif defined(USART0_RX_vect)
#define USART_RXC_vect USART0_RX_vect
#elif defined(USART_RX_vect)
#define USART_RXC_vect USART_RX_vect
#elif defined(USART0_RXC_vect)
#define USART_RXC_vect USART0_RXC_vect
#elif defined(USART_RXC_vect)
#define USART_RXC_vect USART_RXC_vect
#else
#error "Uart receive complete interrupt not defined!"
#endif
#endif

#define BAUD 9600UL
#define UBRRVAL (F_CPU/(BAUD*16)-1)

void uart_init()
{
    /* set baud rate */
    UBRRH = UBRRVAL >> 8;
    UBRRL = UBRRVAL & 0xff;
    /* set frame format: 8 bit, no parity, 1 bit */
    UCSRC = UCSRC_SELECT | (1 << UCSZ1) | (1 << UCSZ0);
    /* enable serial receiver and transmitter */
    UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);

}

void uart_putc(uint8_t c)
{
    
    if(c == '\n')
        uart_putc('\r'); //add return

    /* wait until transmit buffer is empty */
    while(!(UCSRA & (1 << UDRE)));

    /* send next byte */
    UDR = c;
}



void uart_putw_dec(uint16_t w)
{
    uint16_t num = 10000;
    uint8_t started = 0;

    while(num > 0)
    {
        uint8_t b = w / num;
        if(b > 0 || started || num == 1)
        {
            uart_putc('0' + b);
            started = 1;
        }
        w -= b * num;

        num /= 10;
    }
}

void uart_putdw_dec(uint32_t dw)
{
    uint32_t num = 1000000000;
    uint8_t started = 0;

    while(num > 0)
    {
        uint8_t b = dw / num;
        if(b > 0 || started || num == 1)
        {
            uart_putc('0' + b);
            started = 1;
        }
        dw -= b * num;

        num /= 10;
    }
}

void uart_puts(const char* str) 
{
    while(*str)
        uart_putc(*str++);
        _delay_ms(CHAR_DELAY);
}

void uart_puts_p(PGM_P str)
{
    while(1)
    {
        uint8_t b = pgm_read_byte_near(str++);
        if(!b)
            break;

        uart_putc(b);
    }
}

uint8_t uart_getc()
{

    /* wait until receive buffer is full */
    while(!(UCSRA & (1 << RXC)));
    uint8_t b = UDR;
    if(b == '\r')
        b = '\n';
    return b;
}

uint8_t uart_getc_timeout()
{
    uint8_t b = '\0';
    /* wait until receive buffer is full */
    suart_timedout_flag = 0;
    start_timer0();
    while(!(UCSRA & (1 << RXC)))
    {
       if (suart_timedout_flag == 1){break;}
    }
    b = UDR;
    if(b == '\r')
        b = '\n';

    return b;
}


//////read line from uart/////
uint8_t read_line(char* buffer, uint8_t buffer_length)
{
    memset(buffer, 0, buffer_length);

    uint8_t read_length = 0;
    while(read_length < buffer_length - 1)
	{	
		//reset_button_check();		
		uint8_t c = uart_getc();
        if(c == 0x08 || c == 0x7f)
        {
            if(read_length < 1)
                continue;
            --read_length;
            buffer[read_length] = '\0';

            uart_putc(0x08);
            uart_putc(' ');
            uart_putc(0x08);

            continue;
        }
        uart_putc(c);
        if(c == '\n')//echo
        {
            buffer[read_length] = '\0';
            break;
        }
        else
        {
            buffer[read_length] = c;
            ++read_length;
        }
    }
    return read_length;
}




EMPTY_INTERRUPT(USART_RXC_vect)

