///
/// \file SHT25.h
/// \brief Header for SHT25.c 
/// \details 
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
#ifndef SHT25_H
#define SHT25_H
#include <stdint.h>
#include "i2cmaster.h"

//structure:
struct hum_dat{uint16_t SHT_temp;uint16_t SHT_rh;};

//functions:
struct hum_dat ReadSHT25(void);
#endif
