///
/// \file i2c_rtc.c
/// \brief communication with the real time clock via I2c  
/// \details works with DS1307 I2C clock IC
/// \author "theusch" via avrfreaks.net.
/// \copyright This software is published under the public domain.
///Avrfreaks.net regarding posts: Any remarks, suggestions, ideas, graphics, or other material, information or other communication 
///(collectively "Communications") you transmit or post on this website is deemed public and non-proprietary. 
///You agree that Atmel has no obligation with respect to the Communications and has the right to copy, 
///disseminate, incorporate and otherwise use the Communications and all data, images sounds, text, and other 
///things embodied in the Communications for any purpose, commercial or non-commercial. Any Communications 
///transmitted or posted by you that are threatening, libelous, defamatory, obscene, pornographic, or otherwise 
///unlawful, are strictly prohibited and may be actionable by Atmel.
///

#include "i2c_rtc.h"
#include "i2cmaster.h"
#include <string.h>
#include "uart.h"

#define   RTC_SEC_PER_HOUR   (60l * 60l) 
#define   RTC_SEC_PER_DAY      (RTC_SEC_PER_HOUR * 24l) 
#define   RTC_SEC_PER_YEAR   (RTC_SEC_PER_DAY * 365l) 

// Seconds from Jan.1, 1970 through Dec. 31, 2008. 
//   -- 39 years 
//   -- Leap years: 72, 76, 80, 84, 88, 92, 96, 00, 04, 08 
// 
//   According to Excel, there are 14245 days between those two dates. 
//      365 * 39 = 14235 so that would be 10 leap years. 
// 
#define   RTC_SEC_TILL_2009   (RTC_SEC_PER_DAY * 14245L) 

#define   RTC_YEAR_MIN   9   // 2009, as an 8-bit binary year 
#define   RTC_YEAR_MAX   25   // 2025, as an 8-bit binary year 

// Elapsed days at the end of each month shown (ignoring leap years) 
uint16_t rtc_days[13] = 
{ 
0, 
31,      // J 
59,      // F 
90,      // M 
120,   // A 
151,   // M 
181,   // J 
212,   // J 
243,   // A 
273,   // S 
304,   // O 
334,   // N 
365      // D 
};



//RTC functions 

void i2c_read_block(uint8_t dev, uint8_t adr, uint8_t cnt, void* buff)// Device addr, Read start addr, byte count, data buffer 
{
	uint8_t *rbuff = buff;
	i2c_start_wait(dev);
	i2c_write(adr); //send start of block address
	i2c_start(dev | 1);//select device in read mode 
	
	do {					// Receive data 
		cnt--;
		*rbuff++ = i2c_read(cnt ? 1 : 0);
		} while (cnt);
	i2c_stop();
}

void i2c_write_block(uint8_t dev, uint8_t adr, uint8_t cnt, void* buff)// Device addr, Read start addr, byte count, data buffer 
{
	uint8_t *wbuff = buff;
	i2c_start_wait(dev);
	i2c_write(adr); //send start of block address
	do {						// Send data 
			if (i2c_write(*wbuff++)) break;
		} while (--cnt);
	i2c_stop();
}

uint8_t rtc_gettime (RTC *rtc)
{
	uint8_t buf[8];

	i2c_read_block(0xD0, 0, 7, buf);

	rtc->sec = (buf[0] & 0x0F) + ((buf[0] >> 4) & 7) * 10;
	rtc->min = (buf[1] & 0x0F) + (buf[1] >> 4) * 10;
	rtc->hour = (buf[2] & 0x0F) + ((buf[2] >> 4) & 3) * 10;
	rtc->wday = (buf[2] & 0x07);
	rtc->mday = (buf[4] & 0x0F) + ((buf[4] >> 4) & 3) * 10;
	rtc->month = (buf[5] & 0x0F) + ((buf[5] >> 4) & 1) * 10;
	rtc->year = 2000 + (buf[6] & 0x0F) + (buf[6] >> 4) * 10;

	return 1;
}

uint8_t rtc_settime (const RTC *rtc)
{
	uint8_t buf[8];
	buf[0] = rtc->sec / 10 * 16 + rtc->sec % 10;
	buf[1] = rtc->min / 10 * 16 + rtc->min % 10;
	buf[2] = rtc->hour / 10 * 16 + rtc->hour % 10;
	buf[3] = rtc->wday & 7;
	buf[4] = rtc->mday / 10 * 16 + rtc->mday % 10;
	buf[5] = rtc->month / 10 * 16 + rtc->month % 10;
	buf[6] = (rtc->year - 2000) / 10 * 16 + (rtc->year - 2000) % 10;
	buf[0] &=~ (1<<7);//set clock to go
	i2c_write_block(0xD0, 0, 7, buf);
	return(1);
}


uint8_t i2c_rtc_init (void)
{
	i2c_init();
    uint8_t buf[8];	//RTC R/W buffer 
	uint8_t adr;

    i2c_timeout_flag = 0;
    start_timer0();

	// Read RTC registers 
	i2c_read_block(0xD0, 0, 8, buf);

	if (buf[7] & 0x20) {	// When data has been volatiled, set default time 
		// Clear nv-ram. Reg[8..63] 
		memset(buf, 0, 8);
		for (adr = 8; adr < 64; adr += 8)
			i2c_write_block(0x0D, adr, 8, buf);
		// Reset time to Jan 1, '08. Reg[0..7] 
		buf[4] = 1; buf[5] = 1; buf[6] = 8;
		i2c_write_block(0x0D, 0, 8, buf);
	}
    TCCR0 = 0;// stop counter
	return(i2c_timeout_flag);
}

uint8_t showtime(void)
{
    RTC rtc;//struct for real time clock setting
    uart_puts_p(PSTR("\n\r"));
    uint8_t ok = rtc_gettime(&rtc);// Get time from rtc
    uart_putc('#');
    uart_puts_p(PSTR("year: "));
    uart_putw_dec(rtc.year);
    uart_putc(' ');
    uart_puts_p(PSTR("month: "));
    uart_putw_dec(rtc.month);
    uart_putc(' ');
    uart_puts_p(PSTR("day: "));
    uart_putw_dec(rtc.mday);
    uart_putc(' ');
    uart_puts_p(PSTR("hour: "));
    uart_putw_dec(rtc.hour);
    uart_putc(' ');
    uart_puts_p(PSTR("min: "));
    uart_putw_dec(rtc.min);
    uart_putc(' ');
    uart_puts_p(PSTR("sec: "));
    uart_putw_dec(rtc.sec);
    uart_puts_p(PSTR("\n"));
    return ok;
}


uint32_t bintime2unixtime(RTC *rtc) 
{ 
// Convert the BCD time From DS1305/6 to an unsigned long UNIX timestamp 
//   of seconds since the epoch-- Jan. 1, 1970. 
// 
// As this app never needs to go back in time or specify arbitrary dates, 
//   simplifications can be done by simply calculating the whole-year 
//   seconds through an arbitrary year such as 2000, 2007, or 2008. 
// 

uint32_t         work;   // accumulator of seconds 
signed int      mult;   // working 

// Base seconds, 1970 through 2008 
   work = RTC_SEC_TILL_2009; 

// How many years since? 
   mult = rtc->year - RTC_YEAR_MIN; 
   if ((mult < 0) || (mult > (RTC_YEAR_MAX - RTC_YEAR_MIN + 1))) 
      { 
      mult = 0; 
      } 
// Add the whole years since start of 2009 
   work += (unsigned long)mult * RTC_SEC_PER_YEAR; 

// Add a day for each leap year in the prior years 
   work += ((unsigned long)mult/4) * RTC_SEC_PER_DAY; 

// Add a day for this year, if leap year and past Feb. 
   if ((mult+1) % 4 == 0) 
      { 
      // Leap year 
      if (rtc->month > 2) 
         { 
         // Past February 
         work += RTC_SEC_PER_DAY; 
         } 
      } 

// Add the days up to this month.  Use a table 
   mult = rtc->month; 
   if ((mult < 1) || (mult > 12)) 
      { 
      mult = 1; 
      } 

   work += ((unsigned long)rtc_days[mult-1]) * RTC_SEC_PER_DAY; 

// Add the days this month, up to today 
   mult = rtc->mday; 
   if ((mult < 1) || (mult > 31)) 
      { 
      mult = 1; 
      } 

   work += ((unsigned long)(mult-1)) * RTC_SEC_PER_DAY; 

// Add the seconds up to this hour 
   mult = rtc->hour; 
   if ((mult < 0) || (mult > 23)) 
      { 
      mult = 1; 
      } 

   work += ((unsigned long)(mult)) * RTC_SEC_PER_HOUR; 

// Add the seconds this hour 
   mult = (unsigned int)60 * (unsigned int)rtc->min + (unsigned int)rtc->sec; 

   work += mult; 

   return work; 
}

