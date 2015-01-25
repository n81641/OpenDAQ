/// \file utilities.c
/// \brief OpenDAQ misc controls 
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

#include "utilities.h"
#include "uart.h"


volatile uint8_t suart_timedout_flag; // global for timeout
volatile uint8_t i2c_timeout_flag; // global for timeout
volatile uint8_t test_timer ;//overflow counter.  16=1 sec


ISR(TIMER0_OVF_vect) //timeout for periferal tests
{
    
    test_timer ++;
    if (test_timer >= 12) // a second +
    {
        i2c_timeout_flag = 1;
        test_timer = 0;
        suart_timedout_flag = 1;
        PORTC |= (1<<6); //set flag pin
        TCCR0 = 0;// stop counter
        
    }
    
}

uint8_t bitisset(uint32_t number, uint8_t bit)
{
	//bit = number & (1 << x);
	if ( number & (1UL <<bit) )
	{return 1;}
	else 
	{return 0;}

}

void spitout( char *bigline)//sends out and blanks string
{

    
    uart_puts(bigline);
    for (uint8_t i= 0; i<LINESIZE ;i++){bigline[i] = '\0';}//blank serialout
}

void toggle_led(void)
{
    if bit_is_set(PIND,5){PORTD &=~ (1<<5);}//led - low
    else{PORTD |= (1<<5);}

}

///starts timer for peripheral timeout
void start_timer0(void)
{ 
    cli();
    TIMSK |= (1<<TOIE0);        //set  Overflow Interrupt Enable
	TCNT0 = 0x00;				// set the timer to 0
	TCCR0 |= ((1<<CS00)|(1<<CS02)); //  prescale the timer to be clock source to /1024 prescale  @ 4.19mhz fills 16x second
	sei();
}
