#ifndef I2CMASTER_H
#define I2CMASTER_H

///
/// \file i2cmaster.h
/// \brief Header for twimaster.c  
/// \details Basic routines for communicating with I2C slave devices. 
///This single masterimplementation is limited to one bus master on the I2C bus. 
///compiled with AVR-GCC 3.4.3 and  avr-libc 1.2.3
///For any AVR device with hardware TWI 
///This I2c library is implemented as a compact assembler software implementation of the I2C protocol 
///which runs on any AVR (i2cmaster.S) and as a TWI hardware interface for all AVR with built-in TWI hardware (twimaster.c).
///Since the API for these two implementations is exactly the same, an application can be linked either against the
///software I2C implementation or the hardware I2C implementation.
///Use 4.7k pull-up resistor on the SDA and SCL pin.
///Adapt the SCL and SDA port and pin definitions and eventually the delay routine in the module 
///i2cmaster.S to your target when using the software I2C implementation ! 
///Adjust the  CPU clock frequence F_CPU in twimaster.c or in the Makfile when using the TWI 
///hardware implementaion.
///
/// \author Peter Fleury pfleury (at) gmx.ch  http://jump.to/fleury
/// \copyright Open source under the GNU GPL license.
///



#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

#include <avr/io.h>

/// defines the data direction (reading from I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_READ    1

/// defines the data direction (writing to I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_WRITE   0


extern volatile uint8_t i2c_timeout_flag;


///initialize the I2C master interace. Need to be called only once 
        void i2c_init(void);


///Terminates the data transfer and releases the I2C bus 
        void i2c_stop(void);


///Issues a start condition and sends address and transfer direction 
        unsigned char i2c_start(unsigned char address);


///Issues a repeated start condition and sends address and transfer direction 
        unsigned char i2c_rep_start(unsigned char address);


///Issues a start condition and sends address and transfer direction \n
///   
/// If device is busy, use ack polling to wait until device ready \n
/// addr address and transfer direction of I2C device
///
        unsigned char i2c_start_wait(unsigned char address);

 
///Send one byte to I2C device\n
/// returns data  byte to be transfered
        unsigned char i2c_write( unsigned char data );


///read one byte from the I2C device, request more data from device \n
/// returns   byte read from I2C device
        unsigned char i2c_readAck(void);

///read one byte from the I2C device, read is followed by a stop condition \n
/// returns   byte read from I2C device

        unsigned char i2c_readNak(void);

extern unsigned char i2c_read(unsigned char ack);
#define i2c_read(ack)  (ack) ? i2c_readAck() : i2c_readNak();

#endif
