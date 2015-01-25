#ifndef ITOCRTC_H
#define ITOCRTC_H
/// \file i2c_rtc.h
/// \brief Header for i2c_rtc.c 
/// \details 
/// \author University of Illinois Department and Environmental and Civil Engineering
/// \copyright Copyright (c) 2014 University of Illinois Department and Environmental and Civil Engineering.
///This software is distributed under the MIT License. 
///Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
///associated documentation files (the "Software"), to deal in the Software without restriction, 
///including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
///and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
///subject to the following conditions:
///The above copyright notice and this permission notice shall be included in all copies or substantial 
///portions of the Software.
///The software is provided "as is", without warranty of any kind expressed or implied, 
///including but not limited to the warranties of merchantability, fitness for a particular 
///purpose and noninfringement.  In no event shall the authors or copyright holders be liable 
///for any claim, damages or other liability whether in an action of contract, tort or 
///otherwise, arising from, out of or in connection with the software or the use or other 
///dealings in the software.
///

#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "utilities.h"

typedef struct {	
	uint16_t	year;	// 2000..2099 
	uint8_t		month;	// 1..12 
	uint8_t		mday;	// 1.. 31 
	uint8_t		wday;	// 1..7 
	uint8_t		hour;	// 0..23 
	uint8_t		min;	// 0..59 
	uint8_t		sec;	// 0..59 
} RTC;

uint8_t i2c_rtc_init (void);//battery clock init
uint8_t rtc_settime (const RTC *rtc);//set time on battery clock
uint8_t rtc_gettime (RTC *rtc);//get time from battery clock
void i2c_write_block(uint8_t dev, uint8_t adr, uint8_t cnt, void* buff);// Device addr, Read start addr, byte count, data buffer 
void i2c_read_block(uint8_t dev, uint8_t adr, uint8_t cnt, void* buff);// Device addr, Read start addr, byte count, data buffer 

uint32_t bintime2unixtime(RTC *rtc);
uint8_t showtime(void);

#endif
