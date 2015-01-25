/// \file non_vol.c
/// \brief Stores and retreives Cal vals, names and offsets in EEMEM
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
#include "non_vol.h"
#include "inputs.h"  //so we have the structure
#include "uart.h"

char EEMEM Names_eemem [INPUTS_MAX][NAME_MAX];// name spots in the EEMEM
char EEMEM CalVals_eemem [INPUTS_MAX][NAME_MAX];// cal value spots in the EEMEM
char EEMEM Offset_eemem [INPUTS_MAX][NAME_MAX];
uint8_t EEMEM header_id = 0;


void read_write_stored(uint8_t input_function, struct sample_data *data, uint8_t channel)
{

    switch(input_function)
    {
        case READ_NAME      ://reads from eemem
            eeprom_read_block ((void*)data->name[channel], (const void*)Names_eemem[channel],NAME_MAX);
        case WRITE_NAME     ://writes to eemem 
            eeprom_write_block ((const void*)data->name[channel], (void*)Names_eemem[channel],NAME_MAX);
        case READ_CALVAL    ://reads from eemem
            eeprom_read_block ((void*)data->calval[channel], (const void*)CalVals_eemem[channel],NAME_MAX);
        case WRITE_CALVAL   ://writes to eemem
            eeprom_write_block ((const void*)data->calval[channel], (void*)CalVals_eemem[channel],NAME_MAX);
        case READ_OFFSET    ://reads from eemem
            eeprom_read_block ((void*)data->offset[channel], (const void*)Offset_eemem[channel],NAME_MAX);
        case WRITE_OFFSET   ://writes to eemem
            eeprom_write_block ((const void*)data->offset[channel], (void*)Offset_eemem[channel],NAME_MAX);
        case READ_HDR_ID   :
            data->header_id_val = eeprom_read_byte(&header_id);
        case WRITE_HDR_ID  :
            eeprom_write_byte (&header_id, data->header_id_val);
    }
 
}
