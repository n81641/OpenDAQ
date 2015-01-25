/// \file 10_bit.c
/// \brief Control for the internal 10 bit ADC 
/// \details For ATMega32 microcontroller.
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
#include "10_bit.h"


static volatile uint16_t adcData;
static volatile uint8_t adcDataL;
static volatile uint8_t adcDataH;

//interrupt for built-in adc
ISR(ADC_vect) 
{
    adcDataL = ADCL;
    adcDataH = ADCH;
    
    adcData = 0;
    adcData = adcData | adcDataH;
    adcData = adcData << 8;
    adcData = adcData | adcDataL;
}



static void delay(uint16_t us) //.delay used in adc readings
{
	volatile uint16_t i; // variable for software dealy
    for (i=us; i>1; i--)		//software time dealy that may survive -0s optimization.
			{
			asm volatile ("nop"::);
			}
}

void adc_init(void)
{
    ADCSRA |= ( (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0) );//  sets adc clock prescaler to 128 see page 204
    //ADMUX =  ( (1<<REFS0)|(1<<REFS1) );// sets ref volts to internal 2.56 see page 214
	//set at sample time
    ADCSRA |= (1<<ADIE); // enable ADC conversion complete interrupt
    ADCSRA |= (1<<ADEN); // enable the ADC
}

int16_t read_adc_onboard(uint8_t channel, uint8_t oversamples)
{
	sei();//set enable interrupts
	uint16_t total = 0;
	for (uint8_t i = 0;i<oversamples;i++)
	{
		delay(5000UL);   
        //maybe check channel here to make sure is sane?  
		ADMUX = channel; 
		ADMUX |= ( (1<<REFS0)|(1<<REFS1));//2.56 internal V ref 
		delay(5000UL);
		ADCSRA |= (1<<ADSC);
		delay(10000UL);     
		total = total + adcData;
	}
	total = total/oversamples;
	return total;				
}


