
///
/// \file suart.h
/// \brief header for suart.c Port and Pin setup here. 
/// \details Set up the pins for the soft UARTs here.\n
///Each software UART has an Rx and Tx pin. These can be assigned to any GPIO pins on the microcontroller.  \n
/// \n
/// \note To re-assign pins:\n
///
///In each section below, for each pin there are 
///3 places where the port letter needs to be changed, 
///and 3 places for the pin number to be changed.\n
///So for each pair of pins there will be 12 instances to be changed.  
///Only the letter (ex: B) and the number (ex: 7) need to be changed.
///The reason for this is that there is both the soft uart and the corresponding 
///direct connection (avalible to users in the cal menu) port and pin assignments done here.
///
/// \author Karl Walter.  karl (at) waltech.com
/// \copyright Copyright (c) 2014 Karl Walter.  karl (at) waltech.com.
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

#ifndef SUART
#define SUART

#include <avr/io.h>
#include <util/delay.h>
#include "utilities.h"
///Flag for stopping loop:




///Setup the soft uart pins and timings here
////////////////////////////////////////////////////////////////////////////////////////////////////////

//set the bit timing here: (will be same for all) 
#define	BITDELAY    "ldi r25, 145   \n\t"   //  Bit delay. (receive)
#define	H_BITDELAY	"ldi r25, 72    \n\t"	//  Half Bit delay. (receive)
#define	TBITDELAY   "ldi r23, 144   \n\t"   //  Bit delay -1 (transmit.)
#define CHR_DELAY 10 //character transmit delay in ms

// Bit rate settings:
//
//            1MHz  2MHz  4MHz  6MHz  8MHz  10MHz  12MHz  16MHz  20MHz
//   2.4kbps   138     -     -     -     -      -      -      -      -
//   4.8kbps    68   138     -     -     -      -      -      -      -
//   9.6kbps    33    68   138   208     -      -      -      -      -
//  19.2kbps     -    33    68   102   138    173    208      -      -
//  38.4kbps     -     -    33    50    68     85    102    138    172
//  57.6kbps     -     -    21    33    44     56     68     91    114
// 115.2kbps     -     -     -     -    21     27     33     44     56
//
// 9.6kbps 419439hz: 145 (by experiment)
//



///S-UART 1 
///Tx:
//port letter in 3 places, pin number in 3 places
#define OUT1PORT                      PORTB
#define OUT1PINPORT      _SFR_IO_ADDR(PORTB)     // the out port Tx
#define DDRP1T DDRB  //Port Letter, same as above
#define PN1T 6  //Pin number
#define	OUT1_1		"sbi %1, 6	\n\t"   // set the number after the comma 
#define	OUT1_0		"cbi %1, 6	\n\t"   // set the number after the comma
///Rx:
//port letter in 3 places, pin number in 3 places
#define PINPT1                      PINB
#define IN1PINPORT      _SFR_IO_ADDR(PINB)     // the in port Rx
#define DDRP1R DDRB  //Port Letter, same as above
#define PN1R 5  //Pin number
#define	SKIP1_IN_0	"sbic %1, 5 \n\t"	//
#define	SKIP1_IN_1	"sbis %1, 5 \n\t"	//

///S-UART 2 
///Tx:
//port letter in 3 places, pin number in 3 places
#define OUT2PORT                      PORTA
#define OUT2PINPORT      _SFR_IO_ADDR(PORTA)     // the out port Tx
#define DDRP2T DDRA  //Port Letter, same as above
#define PN2T 1  //Pin number
#define	OUT2_1		"sbi %1, 1	\n\t"   // set the number after the comma 
#define	OUT2_0		"cbi %1, 1	\n\t"   // set the number after the comma

///Rx:
//port letter in 3 place, pin nmber in 3 places 
#define PINPT2                      PINA
#define IN2PINPORT      _SFR_IO_ADDR(PINA)     // the in port Rx
#define DDRP2R DDRA  //Port Letter, same as above
#define PN2R 0  //Pin number
#define	SKIP2_IN_0	"sbic %1, 0 \n\t"	//
#define	SKIP2_IN_1	"sbis %1, 0 \n\t"	//


///S-UART 3 
///Tx:
//port letter in 3 places, pin number in 3 places
#define OUT3PORT                      PORTA
#define OUT3PINPORT      _SFR_IO_ADDR(PORTA)     // the out port Tx
#define DDRP3T DDRA  //Port Letter, same as above
#define PN3T 7  //Pin number
#define	OUT3_1		"sbi %1, 7	\n\t"   // set the number after the comma 
#define	OUT3_0		"cbi %1, 7	\n\t"   // set the number after the comma

///Rx:
//port letter in 3 place, pin nmber in 3 places 
#define PINPT3                      PINA
#define IN3PINPORT      _SFR_IO_ADDR(PINA)     // the in port Rx
#define DDRP3R DDRA  //Port Letter, same as above
#define PN3R 6  //Pin numbe
#define	SKIP3_IN_0	"sbic %1, 6 \n\t"	//
#define	SKIP3_IN_1	"sbis %1, 6 \n\t"	//






///////////////////////////////////////////////////////////////////////////////////////////////////////


void direct_conn(uint8_t suart_num);
void suart_puts(uint8_t uart_num, const char* str);
void get_line (uint8_t uart_num, char *buff, uint8_t len);//get line from soft uart
void suart_init(uint8_t uart_num);

/// can access up to three soft uarts

uint8_t rcvr1();//get character form soft uart
void xmit1(uint8_t);

uint8_t rcvr2();//get character form soft uart
void xmit2(uint8_t);

uint8_t rcvr3();//get character form soft uart
void xmit3(uint8_t);


#endif
