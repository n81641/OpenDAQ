/// \file utilities.h
/// \brief Header for utilities.c
/// \details Timeout for testing, etc
/// \author University of Illinois Department and Environmental and Civil Engineering
/// \copyright Copyright (c) 2014 University of Illinois Department and Environmental and Civil Engineering.
///This software is distributed under the MIT License. 
///Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
///associated documentation files (the "Software"), to deal in the Software without restriction, 
///including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
///and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
///subject to the following conditions:
///
///\copyright The above copyright notice and this permission notice shall be included in all copies or substantial 
///portions of the Software.
///The software is provided "as is", without warranty of any kind expressed or implied, 
///including but not limited to the warranties of merchantability, fitness for a particular 
///purpose and noninfringement.  In no event shall the authors or copyright holders be liable 
///for any claim, damages or other liability whether in an action of contract, tort or 
///otherwise, arising from, out of or in connection with the software or the use or other 
///dealings in the software.
///
#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "stdint.h"
#include "data_legnths.h"

extern volatile uint8_t suart_timedout_flag; // global for timeout
extern volatile uint8_t i2c_timeout_flag; // global for timeout
extern volatile uint8_t test_timer;//overflow counter.  16=1 sec

uint8_t bitisset(uint32_t number, uint8_t bit);
void spitout( char *bigline);
void toggle_led(void);
void start_timer0(void);
#endif
