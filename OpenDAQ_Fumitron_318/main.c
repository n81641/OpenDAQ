///
/// \file main.c
/// \brief Main program for OpenEmissionsDAQ  
/// \details See inputs.c to configure and order the inputs.  \n 
///  See int main() section for details on the power up process and main loop.
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

//comment out to run with no RTC for troubleshooting purposes
# define CLOCKCONNECTED 


// freq 4194304 (in makefile)
//
//RH i2c addr: 	1000 000r/w = 0x80 +r/w
//CO i2c addr: 	1001 000r/w	= 0x90 +r/w  
//ADC i2c addr: 0010 100r/w	= 0x28 +r/w 
//
///////////////////////////////////////////  
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <compat/deprecated.h> 
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "avr/eeprom.h"
#include <avr/wdt.h> 
#include <avr/sfr_defs.h> // has bit is set/clear.
#include <stdio.h>

#include "uart.h"
#include "inputs.h"
#include "utilities.h"
#include "non_vol.h"
#include "suart.h"
#include "i2c_rtc.h"

//global vars//
///incremented every 16 sec for testing timeout.
volatile uint8_t sixteen; 

//////function prototypes vvvvv ////

uint8_t cal_val_input_delay(void);
void cal_menu(struct sample_data *data);//menu for entering things
void read_stored_vals(struct sample_data *data);
uint8_t showtime(void);
uint16_t timesample(struct sample_data *data);
void output_headers( struct sample_data *data); 


/////////////////////////////////////////////interrupt routines//////////////////////////////////////
ISR(TIMER1_OVF_vect)
{
    sixteen++;
}

//////////////////////////vvvvvvvvvvvvvvv MAIN  vvvvvvvvvvvvvvvvvvvvv///////

/// ### When the DAQ is powered up, the following occour:###
/// • All channels are counted by running the sample_all_channels() 
/// function in input.c wih case JUST_COUNT \n
/// • The user is given 5 second to enter "cal" and use the value entry menu in cal_menu()  \n
/// • The UART receive is disabled in order to avoid noise if disconnected \n
/// • All channels are tested. The function sample_all_channels() is run with case TEST.  
/// This also initilizes the channels that need. initilization   \n
/// • The Real Time Clock is tested.  The DAQ cannot continue if the clock does not respond. I.e. if it is disconnected\n
/// • The time it takes to run a sample loop is determined.  This number is used to determine when the next sampling should begin. \n
/// • The headers are read from the EEMEM (non volitale memory). These are then added to the output string: sample_data.serialout. \n
/// • The initial start time is calculated.  \n

int main()
{

    DDRD |= (1<<5); //port D led
    uart_init();// setup uart 

    struct sample_data data;//main structure to haul around all the data and related
    for (uint8_t i= 0; i<LINESIZE ;i++){data.serialout[i] = '\0';}//init (blank) serialout
	
    sample_all_channels(JUST_COUNT, &data);// count channels
    
    //cal mode check 
    if (cal_val_input_delay())//check if user wants to enter cal mode
        {cal_menu(&data);} //go to cal menu
        
    UCSRB &=~ (1 << RXEN); UCSRB &=~ (1 << RXCIE);//uart input disabled anti-noise//
    
    //test channels
    uart_puts_p(PSTR("#test chans\n"));
    sample_all_channels(TEST, &data);// test each channel, record if not working
#ifdef   CLOCKCONNECTED   
    // check RTC
     uart_puts_p(PSTR("#test result rtc="));
     uart_putw_dec(i2c_rtc_init());
    uart_putc('\n');
#endif	
    //time sampling
    uart_puts_p(PSTR("#time sampling\n")); 
	uint32_t starttime; 
    uint32_t nowtime;
    uint16_t sample_secs = timesample(&data);//runs a sample loop
    
    
    //headers 
    uart_puts_p(PSTR("#read headers\n"));
    read_stored_vals(&data);
    uart_puts_p(PSTR("#headers: \n"));
    output_headers( &data);
    
#ifdef   CLOCKCONNECTED  
     rtc_gettime(&data.rtc);// Get time from rtc
#endif    
     
     nowtime = bintime2unixtime(&data.rtc);
    uint32_t power_on_sec;//second trackers
    uint32_t elapsed_sec;//second trackers
    power_on_sec = starttime = nowtime + sample_secs;//set first start time    
    _delay_ms(1);//must wait between time queries
    /// ### The main loop is started: ###
    ///
    /// • The RTC is polled every 1 ms until the start time is matched. \n
    /// • The date and time is added to the output string: sample_data.serialout, then the Header ID, and the elapsed seconds.\n
    /// • All the channels are sampled and the values are added to the output string: sample_data.serialout. \n
    /// This is done by running the function sample_all_channels()  with case SAMPLE
    
    
    while(1)//sample away 
    {
#ifdef   CLOCKCONNECTED 
         while (nowtime<starttime)//wait for time here, polling RTC
         {
             rtc_gettime(&data.rtc);// Get time from rtc
             nowtime = bintime2unixtime(&data.rtc);
            _delay_ms(1); 
         }
#endif
        char temps[12] = "\0\0\0\0\0\0\0\0\0\0\0\0"; //used for number to string conversion
        //date time:
        //year   
        ultoa (data.rtc.year,temps,10);    
        strcat(data.serialout,temps);
 
        //month
        ultoa (data.rtc.month,temps,10);    
        if (strlen(temps) == 1)
        {
			strcat(data.serialout,"0");
		}	
        strcat(data.serialout,temps);

        //day
        ultoa (data.rtc.mday,temps,10); 
        if (strlen(temps) == 1)
        {
			strcat(data.serialout,"0");
		}	   
        strcat(data.serialout,temps);
       
        strcat(data.serialout," ");
        
        //hour
        ultoa (data.rtc.hour,temps,10);  
        if (strlen(temps) == 1)
        {
			strcat(data.serialout,"0");
		}	     
        strcat(data.serialout,temps);
        strcat(data.serialout,":");
        //min
        ultoa (data.rtc.min,temps,10); 
        if (strlen(temps) == 1)
        {
			strcat(data.serialout,"0");
		}	      
        strcat(data.serialout,temps);
        strcat(data.serialout,":");
        //sec
        ultoa (data.rtc.sec,temps,10); 
        if (strlen(temps) == 1)
        {
			strcat(data.serialout,"0");
		}	      
        strcat(data.serialout,temps);
        strcat(data.serialout,",");
        
        starttime = nowtime + sample_secs;//set next start time
        elapsed_sec = nowtime - power_on_sec; 
        
        sprintf(temps, "%ld", elapsed_sec);
        strcat(data.serialout,temps);
        strcat(data.serialout,","); //add a comma
        //elapsed seconds done
        
        
        //Header ID:
        itoa( data.header_id_val,temps,10);//header ID
        strcat(data.serialout,temps);
        strcat(data.serialout,","); 

        
        sample_all_channels(SAMPLE, &data);//sample
        (strcat(data.serialout,"\n"));
        spitout(data.serialout);//output
        
    }
    
}

///See if user wants to enter cal mode on startup
uint8_t cal_val_input_delay(void)
{
	// delays for 5 sec, allows input of cal values, names and offsets
	char command[5];// command string
    uint8_t calmode = 0; //flag for calmode or not 
	PORTD |= (1<<5); //LED on
	uart_puts_p(PSTR("# type cal (in 5 sec)\n"));
	uint32_t timeout= 0;
	uint32_t counts_to_timeout = 1300000;// 1.3m = ~5 sec
	UCSRB |= (1 << RXEN);
	UCSRB |= (1 << RXCIE);//uart input enabled//
	while( !(UCSRA & (1 << RXC)) )// check for nothing in the rx buff 
	{
		timeout++;
		if (timeout>=counts_to_timeout)//and not timed out
		{break;}
	}
	if (timeout<counts_to_timeout)
	{		
		if (uart_getc() == 'c')// have to do this to prevent noise from causing cal mode
		{
            uart_putc('c');
            read_line(command, 4);// read up to 3 chars
            if(strcmp_P(command, PSTR("al")) == 0)// checks if "al" is there
            {
                uart_puts_p(PSTR("Ver: "));uart_putw_dec(VERSION);
                uart_putc('\n');
                calmode = 1;
            }
        }
    }
    PORTD &=~ (1<<5); //LED off
    return calmode;
}
///input menu for cal values and names.             
void cal_menu(struct sample_data *data)
{
    char command[NAME_MAX+1] = "";
    read_stored_vals(data);
    RTC rtc;//struct for real time clock setting
    while(1)//do the menu for inputting stuff
    {
        uart_puts_p(PSTR("1: Names \n2: Cal val \n3: offset \n4: time \n5: CO2 \n" ));
        uart_puts_p(PSTR(" enter>"));
        if(read_line(command,3) < 1)    //input up to x - 1 characters
            continue;
        uint8_t choice = atoi(command);
        switch(choice)
        {
         case 1 :
            uart_puts_p(PSTR("\nName Entry (7 char max)\n"));
            uart_puts_p(PSTR("\nchan:  name\n"));
            //uart_puts_p(PSTR("\nchannels: ")); uart_putw_dec(data->number_inputs);uart_putc('\n');
            for (uint8_t i = 0; i<data->number_inputs; i ++ )//output chans names and 
            {
                uart_putw_dec(i);  uart_puts_p(PSTR(": "));  
                uart_puts(data->name[i]); uart_putc('\n');      
            }
            uart_putw_dec(data->number_inputs);  uart_puts_p(PSTR(": "));
            uart_puts_p(PSTR("header ID  ")); uart_putw_dec(data->header_id_val); uart_putc('\n');
            memset(command,'\0',NAME_MAX+1);//clear choice 
            uart_puts_p(PSTR("\n enter chan num>"));
            if((read_line(command, 3) > 0))	//read one line  
            {
                uint8_t name_number = atoi(command);
                uart_puts_p(PSTR(" entered: "));
                uart_putw_dec(name_number);
                uart_putc('\n');
                memset(command,'\0',NAME_MAX+1);//clear choice
                if (name_number < data->number_inputs) 
                {
                    uart_puts_p(PSTR("enter new name >"));
                    if((read_line(command, NAME_MAX) > 0))	//read one line 
                    {
                        uart_puts_p(PSTR(" entered: "));
                        uart_puts(command);
                        uart_putc('\n');
                        strcpy(data->name[name_number], command); //record
                        read_write_stored(WRITE_NAME ,data, name_number);//store
                    }
                }
                if (name_number == data->number_inputs)
                {
                    uart_puts_p(PSTR("enter ID >"));
                    if((read_line(command, 4) > 0))	//read one line
                    {
                        uint8_t id = atoi(command);
                        uart_puts_p(PSTR(" entered: "));
                        uart_putw_dec(id);
                        uart_putc('\n');
                        data->header_id_val = id;//record
                        read_write_stored(WRITE_HDR_ID ,data, 0);//store
                    }
                }
            }
            break;      
         case 2 :
            uart_puts_p(PSTR("\nCal val Entry (7 char max)\n"));
            uart_puts_p(PSTR("\nchan:  name:  value:\n"));
            for (uint8_t i = 0; i<data->number_inputs; i ++ )//output chans names and 
            {
                uart_putw_dec(i);                      uart_puts_p(PSTR(": "));  
                uart_puts(data->name[i]);              uart_puts_p(PSTR(": "));
                uart_puts(data->calval[i]);            uart_putc('\n');      
            }
            memset(command,'\0',NAME_MAX+1);//clear choice 
            uart_puts_p(PSTR("\n enter chan number>"));
            if((read_line(command, 3) > 0))	//read one line  up to 3 characters
            {
                uint8_t name_number = atoi(command);
                uart_puts_p(PSTR(" entered: "));
                uart_putw_dec(name_number);
                uart_putc('\n');
                memset(command,'\0',NAME_MAX+1);//clear choice
                uart_puts_p(PSTR("enter value >"));
                if((read_line(command, NAME_MAX) > 0))	//read one line  up to 10 characters
                {
                    uart_puts_p(PSTR("entered: "));
                    uart_puts(command);
                    uart_putc('\n');
                    strcpy(data->calval[name_number], command); //record
                    read_write_stored(WRITE_CALVAL ,data, name_number);//store
                }
            }
            break;
         case 3 :
            uart_puts_p(PSTR("\nOffset Entry (7 char max)\n"));
            uart_puts_p(PSTR("\nchan:  name:  offset:\n"));
            for (uint8_t i = 0; i<data->number_inputs; i ++ )//output chans names and 
            {
                uart_putw_dec(i);                   uart_puts_p(PSTR(": "));  
                uart_puts(data->name[i]);              uart_puts_p(PSTR(": "));
                uart_puts(data->offset[i]);        uart_putc('\n');      
            }
            memset(command,'\0',NAME_MAX+1);//clear choice 
            uart_puts_p(PSTR("\n enter channel number>"));
            if((read_line(command, 4) > 0))	//read one line  up to 3 characters
            {
                uint8_t name_number = atoi(command);
                uart_puts_p(PSTR("entered: "));
                uart_putw_dec(name_number);
                uart_putc('\n');
                memset(command,'\0',NAME_MAX+1);//clear choice
                uart_puts_p(PSTR("enter value >"));
                if((read_line(command, NAME_MAX) > 0))	//read one line  up to 10 characters
                {
                    uart_puts_p(PSTR("entered: "));
                    uart_puts(command);
                    uart_putc('\n');
                    strcpy(data->offset[name_number], command); //record
                    read_write_stored(WRITE_OFFSET ,data, name_number);//store
                }
            }
            break;
         case 4 : //Time set RTC
            showtime();
            memset(command,'\0',NAME_MAX+1);//clear choice
            uart_puts_p(PSTR("change? (y/n)"));uart_puts_p(PSTR(" >"));
            if((read_line(command, 3) > 0))	//read one line
            {
                if(strcmp_P(command, PSTR("y")) == 0)
                {     
                    uart_puts_p(PSTR("\n yr, 4 digit"));uart_puts_p(PSTR(" >"));
                    if((read_line(command, 7) > 0))	//read one line  up to 6 characters
                    { rtc.year = atoi(command);}
                    
                    uart_puts_p(PSTR("\n month, 2 dgt"));uart_puts_p(PSTR(" >"));
                    if((read_line(command, 7) > 0))	//read one line  up to 6 characters
                    { rtc.month = atoi(command);}
                    
                    uart_puts_p(PSTR("\n day, 2 dgt"));uart_puts_p(PSTR(" >"));
                    if((read_line(command, 7) > 0))	//read one line  up to 6 characters
                    { rtc.mday = atoi(command);}
                            
                    uart_puts_p(PSTR("\n hr, 24hr 2 dgt"));uart_puts_p(PSTR(" >"));
                    if((read_line(command, 7) > 0))	//read one line  up to 6 characters
                    { rtc.hour = atoi(command);}
                            
                    uart_puts_p(PSTR("\n mins, 2 dgt"));uart_puts_p(PSTR(" >"));
                    if((read_line(command, 7) > 0))	//read one line  up to 6 characters
                    {  rtc.min = atoi(command); }
                    
                    rtc.sec = 0;
                    uart_putw_dec(rtc_settime (&rtc));
                    uart_puts_p(PSTR("\n"));
                    showtime();
                }
                else showtime();
            }
            break;
         case 5 :
            memset(command,'\0',NAME_MAX+1);//clear choice 
            uart_puts_p(PSTR("\n enter S-Uart chan 1,2,3 >"));
            if((read_line(command, 3) < 1)){;}	//read one line  
            uint8_t choice = atoi(command);
            uart_puts_p(PSTR("direct connection to S-uart ")); uart_putw_dec(choice); uart_putc('\n');
            uart_puts_p(PSTR("use RET CR command ends\n"));
            direct_conn(choice);
            uart_puts_p(PSTR("not implemented\n"));
            break;
        }
    }
}

///read all the names and cal vals in the EEMEM
void read_stored_vals(struct sample_data *data)
{
    uart_puts_p(PSTR("# num inputs: "));
    sample_all_channels(JUST_COUNT, data);//count channels
    uart_putw_dec(data->number_inputs);  uart_puts_p(PSTR("\n"));
    
    uart_puts_p(PSTR("# reading EEMEM "));
    
    uint8_t tempnum = data->number_inputs; //because num_inputs is getting overwritten
    
    for (uint8_t i = 0; i<tempnum ; i ++ )//read all the eemems into the samples structure
    {
        read_write_stored(READ_NAME,data, i);
        read_write_stored(READ_CALVAL,data, i);
        read_write_stored(READ_OFFSET,data,i); 
        uart_puts_p(PSTR("."));
    }
    read_write_stored(READ_HDR_ID,data, 0);//header ID
    uart_puts_p(PSTR("# done\n"));
    
    //validate calvals and offsets:
    float temp;
    for (uint8_t i = 0; i<data->number_inputs ; i ++ )
    {
        temp = strtod(data->calval[i] ,0);
        //uart_putdw_dec(temp);uart_puts_p(PSTR("<-- calval \n"));
        if ( (temp >65535 )|| (temp< -65535))
        {
            uart_puts_p(PSTR("# warning: corrupt calval\n")); 
        }
        temp = strtod(data->offset[i] ,0);
        //uart_putdw_dec(temp);uart_puts_p(PSTR("<-- offset \n"));
        if ( (temp >65535 )|| (temp< -65535))
        {
            uart_puts_p(PSTR("# warning: corrupt offset\n")); 
        }
    }

}



///Add headers to serialout:
void output_headers( struct sample_data *data)
{
   
    //output cal vals (slope)  
    (strcat(data->serialout,"#calVals:,1,")) ;//add string and move pointer  //value placeholder over timestamp col
    char id_str[4] = "\0\0\0\0";
    itoa( data->header_id_val,id_str,10);
    strcat(data->serialout,id_str);
    strcat(data->serialout,",");
    
    for (uint8_t i = 0; i<data->number_inputs; i ++ )//output cal vals
    {
        strcat(data->serialout,data->calval[i]);
        if (i < (data->number_inputs-1))
        {(strcat(data->serialout,",")); }//add a comma
    }
    strcat(data->serialout,"\n");//add newline
    spitout(data->serialout);//out put to serial
    
    //output offsets
    (strcat(data->serialout,"#offsets:,0,")) ;//add string and move pointer  //value placeholder over timestamp col
    strcat(data->serialout,id_str);
    strcat(data->serialout,",");
    
    for (uint8_t i = 0; i<data->number_inputs; i ++ )//output cal vals
    {
        strcat(data->serialout,data->offset[i]);
        if (i < (data->number_inputs-1))
        {(strcat(data->serialout,",")); }//add a comma
    }
    strcat(data->serialout,"\n");//add newline
    spitout(data->serialout);//out put to serial
    
    strcat(data->serialout,"time,seconds,headID,");//add string 
    //char id_str[4] = "\0\0\0\0";
    //itoa( data->header_id_val,id_str,10);
    //strcat(data->serialout,id_str);
    for (uint8_t i = 0; i<data->number_inputs; i ++ )//output names
    {
        strcat(data->serialout,data->name[i]);
        if (i < (data->number_inputs-1))
        {strcat(data->serialout,","); }//add a comma
    }
    strcat(data->serialout,"\n");//add newline
    spitout(data->serialout);//out put to serial
    
    
}
///Check and calculate time for sampling period.
uint16_t timesample(struct sample_data *data)
{
    //check how long to run a sample set
    uint16_t sample_secs;
    
    //start 16 bit timer
    sixteen = 0;//16 second counter
    TIMSK |= (1<<TOIE1);                //set  Overflow Interrupt Enable
    TCNT1 = 0x00;				        // set the timer to 0
    TCCR1B |= ((1<<CS10)|(1<<CS12));    //  prescale the timer to be clock source to /1024 prescale  
    sei();
    sample_all_channels(REPORT, data);//run one sample loop
    TCCR1B |= 0x00;//Stop timer, check value
    //time in ms = sixteen*1000 +TCNT/4.09
    sample_secs = TCNT1/4;
    sample_secs = sample_secs + (sixteen*1000);//time in ms
    sample_secs = sample_secs + 600;//add 600 ms
    sample_secs = sample_secs/1000;//time in sec, remainder left off
    sample_secs = sample_secs + 1;//the next second
    for (uint8_t i= 0; i<LINESIZE ;i++){data->serialout[i] = '\0';}//init (blank) serialout
    uart_putc('\n');
	return sample_secs;
}

