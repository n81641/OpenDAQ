/// \file non_vol.h
/// \brief Header for non_vol.c
/// \details Stores and retreives Cal vals, names and offsets in EEMEM
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
#ifndef NONVOL_H
#define NONVOL_H
#include <avr/pgmspace.h>
#include "avr/eeprom.h"

#include "data_legnths.h"

#include "inputs.h"

#define READ_NAME       2
#define WRITE_NAME      3
#define READ_CALVAL     4
#define WRITE_CALVAL    5
#define READ_OFFSET     6
#define WRITE_OFFSET    7
#define READ_HDR_ID    8
#define WRITE_HDR_ID    9



/// send function, data structure, channel to read/write

void read_write_stored(uint8_t input_function, struct sample_data *data, uint8_t channel);

#endif
