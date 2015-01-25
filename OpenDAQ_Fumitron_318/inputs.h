/// \file inputs.h
/// \brief Header for inpts.c 
/// \details 
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
#ifndef INPUTS_H
#define INPUTS_H
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <avr/pgmspace.h>


//files for types of inputs:
#include "data_legnths.h"
#include "sixteen_bit.h"
#include "CO.h"
#include "SHT25.h"
#include "CO2_serial.h"
#include "10_bit.h"
#include "i2c_rtc.h"

#include "utilities.h"
#include "suart.h"
#include "uart.h" 


//these are all the functions that each channel's block of code will do:
#define JUST_COUNT      0
#define SAMPLE          1
#define REPORT          9
#define TEST            10


//structures:

struct sample_data{
    char name[INPUTS_MAX][NAME_MAX];    //all the names
    char calval[INPUTS_MAX][NAME_MAX];  //all the calvals
    char offset[INPUTS_MAX][NAME_MAX]; //all the offsets
    //int16_t offset[INPUTS_MAX];         //all the offsets
    uint8_t number_inputs;
    uint8_t disabled[INPUTS_MAX];       //if channel is disabled set to 1
    RTC rtc;            //clock structure with time and date
    char serialout[LINESIZE];
    uint8_t header_id_val;//header ID
    /// This is the entire data string to send and store.
}; 


//functions:
void sample_all_channels(uint8_t input_function, struct sample_data *sample);

#endif
