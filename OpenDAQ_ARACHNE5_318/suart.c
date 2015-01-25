///
/// \file suart.c
/// \brief software uart 
/// \details Multiple software uarts can be addd here. Setup pns and timing in suart.h 
/// \author Karl Walter.  karl (at) waltech.com
/// \copyright Copyright (c) 2014 Karl Walter.  karl (at) waltech.com.
///This software is distributed under the MIT License. 
///Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
///associated documentation files (the "Software"), to deal in the Software without restriction, 
///including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
///and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
///subject to the following conditions:
///The above copyright notice and this permission notice shall be included in all copies or substantial 
///portions of the Software.
///The software is provided "as is", without warranty of any kind expressed or implied, 
///including but not limited to the warranties of merchantability, fitness for a particular 
///purpose and noninfringement.  In no event shall the authors or copyright holders be liable 
///for any claim, damages or other liability whether in an action of contract, tort or 
///otherwise, arising from, out of or in connection with the software or the use or other 
///dealings in the software.
///
#include "suart.h"
#include "uart.h"



void get_line (uint8_t uart_num, char *buff, uint8_t len)//get line from soft uart discards all after len, before return. blocking.
{
	uint8_t c, i;

	c = i = 0;
	for (;;) {

		switch(uart_num)
        {
            case 1:
                c = rcvr1();
                break; 
            case 2:
                c = rcvr2();
                break; 
            case 3:
                c = rcvr2();
                break; 
        }
		if (c == '\r') break;
		if (c == '\n') break;
        if (suart_timedout_flag == 1)break;
		if ((c == '\b') && i) i--;
		if ((c >= ' ') && (i < len - 1))
				buff[i++] = c;
	}
	buff[i] = '\0';
}

void suart_puts(uint8_t uart_num, const char* str)
{
    while(*str)
        {
            switch(uart_num)
            {
                case 1:
                xmit1(*str++);
                break; 
                case 2:
                xmit2(*str++);
                break; 
                case 3:
                xmit3(*str++);
                break; 
            }
            _delay_ms(CHR_DELAY);//character delay 
        }
}


 
void suart_init(uint8_t uart_num)
{
    switch(uart_num)
    {
        case 1:  
            DDRP1T |= (1<<PN1T);
            _delay_ms(20);
            xmit1('\0'); //throw away char
            break;
        case 2: 
            DDRP2T |= (1<<PN2T);
            xmit2('\0');
            _delay_ms(20);
            break;
        case 3: 
            DDRP3T |= (1<<PN3T);
            xmit2('\0');
            _delay_ms(20);
            break;

    }

}




void xmit1(uint8_t abyte)
{ 
    asm volatile(
    "in     __tmp_reg__,      __SREG__   \n\t"       //Save flags 
    "mov    r24, %0                     \n\t"       // get byte to transmit
	"com	r24		                    \n\t"       //one's complement 
	"ldi	r25, 10		                \n\t"       //Bit counter
	"cli			                    \n\t"       //Start critical section

"bitloop%=:                                   \n\t" 	    //----- Bit transferring loop 
    TBITDELAY                                      //load bit dealy
"bitwait%=:                                   \n\t" 	
    "dec	r23     	                \n\t"       //Wait for a bit time
	"brne	bitwait%=		                    \n\t"       //
	"brcs	lowout%=		                    \n\t"       //bit to be sent
	OUT1_1			                                // set pin to 1
"lowout%=:                                   \n\t" 	
    "brcc	donebit%=		                    \n\t"       //
	OUT1_0			                                //  set pin to 0
"donebit%=:                                   \n\t" 	
    "lsr	r24     	                \n\t"       //Get next bit into C
	"dec	r25     	                \n\t"       //All bits sent?
	"brne	bitloop%=	     	                \n\t"       //  no, coutinue

	"out __SREG__,   __tmp_reg__         \n\t"
    :                                       //outputs
    :"r" (abyte), "I" (OUT1PINPORT)           //inputs
    : "r23", "r24", "r25"                   //clobbers list (registers used inside)
    );
}

void xmit2(uint8_t abyte)
{
	asm volatile(
    "in     __tmp_reg__,      __SREG__   \n\t"       //Save flags 
    "mov    r24, %0                     \n\t"       // get byte to transmit
	"com	r24		                    \n\t"       //one's complement 
	"ldi	r25, 10		                \n\t"       //Bit counter
	"cli			                    \n\t"       //Start critical section

"bitloop%=:                                   \n\t" 	    //----- Bit transferring loop 
    TBITDELAY                                      //load bit dealy
"bitwait%=:                                   \n\t" 	
    "dec	r23     	                \n\t"       //Wait for a bit time
	"brne	bitwait%=		                    \n\t"       //
	"brcs	lowout%=		                    \n\t"       //bit to be sent
	OUT2_1			                                // set pin to 1
"lowout%=:                                   \n\t" 	
    "brcc	donebit%=		                    \n\t"       //
	OUT2_0			                                //  set pin to 0
"donebit%=:                                   \n\t" 	
    "lsr	r24     	                \n\t"       //Get next bit into C
	"dec	r25     	                \n\t"       //All bits sent?
	"brne	bitloop%=	     	                \n\t"       //  no, coutinue

	"out __SREG__,   __tmp_reg__         \n\t"
    :                                       //outputs
    :"r" (abyte), "I" (OUT2PINPORT)           //inputs
    : "r23", "r24", "r25"                   //clobbers list (registers used inside)
    );
}
void xmit3(uint8_t abyte)
{
	asm volatile(
    "in     __tmp_reg__,      __SREG__   \n\t"       //Save flags 
    "mov    r24, %0                     \n\t"       // get byte to transmit
	"com	r24		                    \n\t"       //one's complement 
	"ldi	r25, 10		                \n\t"       //Bit counter
	"cli			                    \n\t"       //Start critical section

"bitloop%=:                                   \n\t" 	    //----- Bit transferring loop 
    TBITDELAY                                      //load bit dealy
"bitwait%=:                                   \n\t" 	
    "dec	r23     	                \n\t"       //Wait for a bit time
	"brne	bitwait%=		                    \n\t"       //
	"brcs	lowout%=		                    \n\t"       //bit to be sent
	OUT3_1			                                // set pin to 1
"lowout%=:                                   \n\t" 	
    "brcc	donebit%=		                    \n\t"       //
	OUT3_0			                                //  set pin to 0
"donebit%=:                                   \n\t" 	
    "lsr	r24     	                \n\t"       //Get next bit into C
	"dec	r25     	                \n\t"       //All bits sent?
	"brne	bitloop%=	     	                \n\t"       //  no, coutinue

	"out __SREG__,   __tmp_reg__         \n\t"
    :                                       //outputs
    :"r" (abyte), "I" (OUT3PINPORT)           //inputs
    : "r23", "r24", "r25"                   //clobbers list (registers used inside)
    );
}


uint8_t rcvr1(void)
{
    uint8_t abyte= 0;
    asm volatile(
    
        "in     __tmp_reg__,      __SREG__              \n\t"       //Save flags
        "ldi	r24,     0x80	                        \n\t"      //Receiving shift reg load with 0x80
        //"cli	                                        \n\t"      //Start critical section //not used w/timeout
    
        "wait_idle%=:                                   \n\t"       // wait for idle
            "sbic %2, 6                                 \n\t"	         //Skip if flag pin is clear
            "rjmp error%=                               \n\t"
            SKIP1_IN_1                                               //macro looking for hi on rx pin
        "rjmp	   wait_idle%=   	                    \n\t"       //
    
        "wait_start%=:                                  \n\t"       // wait for start bit
            "sbic %2, 6                                 \n\t"	 // Skip if flag pin is 
            "rjmp error%=                               \n\t"
            SKIP1_IN_0                                                       //macro looking for hi on rx pin
        "rjmp	wait_start%=     	                    \n\t"       //
    
        H_BITDELAY      // load half bit's time
    
        "half_bit_delay%=:                              \n\t"       //Wait for half bit time
        "dec	r25                               \n\t"      //count down
        "brne   half_bit_delay%=                        \n\t"      //

        "bit_loop%=:                                    \n\t"   //----- Bit receiving loop
            BITDELAY      // load one bit's time
    
            "one_bit_delay%=:                           \n\t"       //Wait for one bit time
                "dec	r25                               \n\t"   //count down
            "brne   one_bit_delay%=                     \n\t"      //
            
            "lsr	r24          	                \n\t" //Next bit
            SKIP1_IN_0                                       //Get a data bit into r24
            "ori	r24,     0x80	                \n\t"  // record bit
        "brcc   bit_loop%=                              \n\t"      //continue until all bits received.
        "error%=:"
        "mov        %0,  r24                             \n\t" // byte received copied to abyte
        
        
        "out __SREG__,          __tmp_reg__                      \n\t"
        
        :"=r" (abyte)                       //outputs
        :"I" (IN1PINPORT) , "I" _SFR_IO_ADDR(PINC)          //inputs PINC is for timeoutflag
        : "r24", "r25", "r26"                   //clobbers list (registers used inside)
    );
 

    return abyte;

}
 
uint8_t rcvr2(void)
{
	uint8_t abyte= 0;
	 asm volatile(
    
        "in     __tmp_reg__,      __SREG__              \n\t"       //Save flags
        "ldi	r24,     0x80	                        \n\t"      //Receiving shift reg load with 0x80
        //"cli	                                        \n\t"      //Start critical section //not used w/timeout
    
        "wait_idle%=:                                   \n\t"       // wait for idle
            "sbic %2, 6                                 \n\t"	         //Skip if flag pin is clear
            "rjmp error%=                               \n\t"
            SKIP2_IN_1                                               //macro looking for hi on rx pin
        "rjmp	   wait_idle%=   	                    \n\t"       //
    
        "wait_start%=:                                  \n\t"       // wait for start bit
            "sbic %2, 6                                 \n\t"	 // Skip if flag pin is 
            "rjmp error%=                               \n\t"
            SKIP2_IN_0                                                       //macro looking for hi on rx pin
        "rjmp	wait_start%=     	                    \n\t"       //
    
        H_BITDELAY      // load half bit's time
    
        "half_bit_delay%=:                              \n\t"       //Wait for half bit time
        "dec	r25                               \n\t"      //count down
        "brne   half_bit_delay%=                        \n\t"      //

        "bit_loop%=:                                    \n\t"   //----- Bit receiving loop
            BITDELAY      // load one bit's time
    
            "one_bit_delay%=:                           \n\t"       //Wait for one bit time
                "dec	r25                               \n\t"   //count down
            "brne   one_bit_delay%=                     \n\t"      //
            
            "lsr	r24          	                \n\t" //Next bit
            SKIP2_IN_0                                       //Get a data bit into r24
            "ori	r24,     0x80	                \n\t"  // record bit
        "brcc   bit_loop%=                              \n\t"      //continue until all bits received.
        "error%=:"
        "mov        %0,  r24                             \n\t" // byte received copied to abyte
        
        
        "out __SREG__,          __tmp_reg__                      \n\t"
        
        :"=r" (abyte)                       //outputs
        :"I" (IN2PINPORT) , "I" _SFR_IO_ADDR(PINC)          //inputs PINC is for timeoutflag
        : "r24", "r25", "r26"                   //clobbers list (registers used inside)
    );
	
	
	
	return abyte;
}

uint8_t rcvr3(void)
{
	uint8_t abyte= 0;
	 asm volatile(
    
        "in     __tmp_reg__,      __SREG__              \n\t"       //Save flags
        "ldi	r24,     0x80	                        \n\t"      //Receiving shift reg load with 0x80
        //"cli	                                        \n\t"      //Start critical section //not used w/timeout
    
        "wait_idle%=:                                   \n\t"       // wait for idle
            "sbic %2, 6                                 \n\t"	         //Skip if flag pin is clear
            "rjmp error%=                               \n\t"
            SKIP3_IN_1                                               //macro looking for hi on rx pin
        "rjmp	   wait_idle%=   	                    \n\t"       //
    
        "wait_start%=:                                  \n\t"       // wait for start bit
            "sbic %2, 6                                 \n\t"	 // Skip if flag pin is 
            "rjmp error%=                               \n\t"
            SKIP3_IN_0                                                       //macro looking for hi on rx pin
        "rjmp	wait_start%=     	                    \n\t"       //
    
        H_BITDELAY      // load half bit's time
    
        "half_bit_delay%=:                              \n\t"       //Wait for half bit time
        "dec	r25                               \n\t"      //count down
        "brne   half_bit_delay%=                        \n\t"      //

        "bit_loop%=:                                    \n\t"   //----- Bit receiving loop
            BITDELAY      // load one bit's time
    
            "one_bit_delay%=:                           \n\t"       //Wait for one bit time
                "dec	r25                               \n\t"   //count down
            "brne   one_bit_delay%=                     \n\t"      //
            
            "lsr	r24          	                \n\t" //Next bit
            SKIP3_IN_0                                       //Get a data bit into r24
            "ori	r24,     0x80	                \n\t"  // record bit
        "brcc   bit_loop%=                              \n\t"      //continue until all bits received.
        "error%=:"
        "mov        %0,  r24                             \n\t" // byte received copied to abyte
        
        
        "out __SREG__,          __tmp_reg__                      \n\t"
        
        :"=r" (abyte)                       //outputs
        :"I" (IN3PINPORT) , "I" _SFR_IO_ADDR(PINC)          //inputs PINC is for timeoutflag
        : "r24", "r25", "r26"                   //clobbers list (registers used inside)
    );

	return abyte;
}

void direct_conn(uint8_t suart_num)
{
    UCSRB &=~ (1 << RXEN); UCSRB &=~ (1 << RXCIE);//uart input disabled//
    UCSRB &=~ (1 << TXEN); UCSRB &=~ (1 << TXCIE);//uart output disabled//
    DDRD |= (1<<1); //port D1 out: tx
    DDRD &=~ (1<<0); //port D0 in: rx
    
    if (suart_num == 1)
    {
        
        DDRP1T |=(1<<PN1T); 
        DDRP1R &=~(1<<PN1R);

        while(1)
        {
            //data from sensor ---->
            if bit_is_set(PINPT1, PN1R) PORTD |= (1<<1);//mirror pin B6 TX
            else PORTD &=~ (1<<1);
            //data to sensor    <----
            if bit_is_set(PIND, 0) OUT1PORT |= (1<<PN1T);
            else OUT1PORT &=~ (1<<PN1T);
        }
    }
        if (suart_num == 2)
    {
        DDRP2T |=(1<<PN2T); 
        DDRP2R &=~(1<<PN2R);
    
        while(1)
        {
            //data from sensor ---->
            if bit_is_set(PINPT2, PN2R) PORTD |= (1<<1);//mirror pin B6 TX
            else PORTD &=~ (1<<1);
        
            //data to sensor    <----
            if bit_is_set(PIND, 0) OUT2PORT |= (1<<PN2T);//mirror pin D0
            else OUT2PORT &=~ (1<<PN2T);
        }
    }
        if (suart_num == 3)
    {
        DDRP3T |=(1<<PN3T); 
        DDRP3R &=~(1<<PN3R);
    
        while(1)
        {
            //data from sensor ---->
            if bit_is_set(PINPT3, PN3R) PORTD |= (1<<1);//mirror pin B6 TX
            else PORTD &=~ (1<<1);
        
            //data to sensor    <----
            if bit_is_set(PIND, 0) OUT3PORT |= (1<<PN3T);//mirror pin D0
            else OUT3PORT &=~ (1<<PN3T);
        }
    }
}
