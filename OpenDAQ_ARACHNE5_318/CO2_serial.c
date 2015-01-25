/// \file CO2_serial.c
/// \brief Control for the COZIR serial CO2 sensors   
/// \details UART comunication via cammand and receive of data.
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
#include "CO2_serial.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "uart.h"

void init_CO2(uint8_t CO2_chan)
{
    suart_init(CO2_chan);
    _delay_ms(1);
    suart_puts(CO2_chan,"K 1\r\n"); //set C02 sensor to streaming mode
    _delay_ms(100);
    suart_puts(CO2_chan,"K 1\r\n"); //set C02 sensor to streaming mode again to be sure
}

uint16_t get_CO2(uint8_t CO2_chan)//>>CO2<<////////
{
    char bufsurt[17] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    char data[5] = "\0\0\0\0\0";
    uint16_t c_o_ppm = 0;
    for(uint8_t j = 0; j<3; j++) //3 trys
    {
        get_line (CO2_chan,bufsurt, 17);//data
        //uart_puts("buffer: ");  uart_puts(bufsurt); uart_putc('\n');
        if (strlen(bufsurt)==16)  //exact legnth of data from CO2 sensor
        {break;} 
    }
    uint8_t Z_pos = 0;
    for(uint8_t i = 0; i<8; i++)//find the Z in the first 8 characters
    {
        if (bufsurt[i] == 'Z')
        {Z_pos = i;}
    }
	for(uint8_t i = 0; i<5; i++)//parse just the first number, Big Z
    {
        data[i] = bufsurt[i+Z_pos+2];
    }
    //uart_puts("data: ");  uart_puts(data); uart_putc('\n');
    c_o_ppm=strtoul(data, NULL,10);  
        
	return c_o_ppm;
}



