#ifndef UART_H
#define UART_H
///
/// \file uart.h
/// \brief Header for uart.c
/// \details 
/// \author Roland Riegel <feedback@roland-riegel.de>
/// \copyright Copyright (c) 2006-2009 by Roland Riegel <feedback@roland-riegel.de>
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License version 2 as
/// published by the Free Software Foundation.
///


#define CHAR_DELAY 1 //milisecond delay between characters in uart_puts()

#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


void uart_init();

void uart_putc(uint8_t c);

void uart_putw_dec(uint16_t w);
void uart_putdw_dec(uint32_t dw);

void uart_puts(const char* str);
void uart_puts_p(PGM_P str);

uint8_t uart_getc();
uint8_t uart_getc_timeout();

uint8_t read_line(char* buffer, uint8_t buffer_length);
 	


#endif

