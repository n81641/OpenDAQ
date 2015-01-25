///
/// \file inputs.c
/// \brief All the input functions, in order for sampling reside here  
/// \details Add, delete, order blocks for each input needed.
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
#include "inputs.h"
#include "i2cmaster.h"
#include "utilities.h"
#include "suart.h"
#include "uart.h" 





void calc_and_string(int32_t value, struct sample_data *sample,uint8_t decimal)
{
    float temp;
    float newval;
    char temps[12] = "\0\0\0\0\0\0\0\0\0\0\0\0";
    if (value < 65535L)//not overscale, apply scale and offset
    {
        //get the calval:
        temp = strtod(sample->calval[sample->number_inputs] ,0);
        newval = value * temp;
        temp = strtod(sample->offset[sample->number_inputs] ,0);
        newval = newval + temp;
    }
    else newval = 65536UL;
    dtostrf(newval,1,decimal,temps);//dtostrf(newval, min_width, digits after decimal, temps);
    strcat(sample->serialout,temps);
    strcat(sample->serialout,","); //add a comma
}


///this samples all the channels
void sample_all_channels(uint8_t input_function, struct sample_data *sample)
{
    
    int32_t value = 0;//value from input
    sample->number_inputs = 0;
    uint8_t report_chan =0;
    if (input_function == REPORT)
    {
        input_function = SAMPLE;
        report_chan =1;
    }




    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////0
// CO sample, sixteen bit channel, differential, init via I2c :
    switch(input_function)
    {
        case SAMPLE :
			toggle_led( ); 
            value = read_adc(0x28, 0xA0, 0x88);//ch 0,1 diff 
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            i2c_timeout_flag = 0;
            start_timer0();//starts the timeout timer
            Init_CO();
            TCCR0 = 0;// stop counter
            sample->disabled[sample->number_inputs] = i2c_timeout_flag; //enable or diasble channel
            i2c_timeout_flag = 0;
            uart_puts_p(PSTR("#test result ch "));uart_putw_dec(sample->number_inputs); uart_putc('=');
            uart_putw_dec(sample->disabled[sample->number_inputs]);uart_putc('\n');
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////1
    // CO bkg, sixteen bit channel, differential :
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( ); 
            value = read_adc(0x28, 0xA1, 0x88);//ch 2,3 diff 
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////2
    // C02 sample via serial communication: 

    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( );
            if(sample->disabled[sample->number_inputs] == 1)//don't sample if disabled
            {
                calc_and_string(0, sample,1);
                break;
            }
            value = get_CO2(1);//gets value form suart1
            calc_and_string(value, sample,0);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST           ://test channel
            sample->disabled[sample->number_inputs] = 0; //enable channel
            init_CO2(1);
            suart_timedout_flag = 0; DDRC |= (1<<6); PORTC &=~ (1<<6); //set flag pin low
            start_timer0();//starts the timeout timer
            get_CO2(1);
            TCCR0 = 0;// stop counter
            sample->disabled[sample->number_inputs] = suart_timedout_flag; //enable or diasble channel
            suart_timedout_flag = 0; PORTC &=~ (1<<6); //set flag pin low
            uart_puts_p(PSTR("#test result ch "));uart_putw_dec(sample->number_inputs); uart_putc('=');
            uart_putw_dec(sample->disabled[sample->number_inputs]);uart_putc('\n');
            break; 
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////
 
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////3
    // C02 bkg via serial communication: 

    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( );
            if(sample->disabled[sample->number_inputs] == 1)//don't sample if disabled
            {
                calc_and_string(0, sample,1);
                break;
            }
            value = get_CO2(2);//gets value form suart1
            calc_and_string(value, sample,0);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST           ://test channel
            sample->disabled[sample->number_inputs] = 0; //enable channel
            init_CO2(2);
            suart_timedout_flag = 0; DDRC |= (1<<6); PORTC &=~ (1<<6); //set flag pin low
            start_timer0();//starts the timeout timer
            get_CO2(2);
            TCCR0 = 0;// stop counter
            sample->disabled[sample->number_inputs] = suart_timedout_flag; //enable or diasble channel
            suart_timedout_flag = 0; PORTC &=~ (1<<6); //set flag pin low
            uart_puts_p(PSTR("#test result ch "));uart_putw_dec(sample->number_inputs); uart_putc('=');
            uart_putw_dec(sample->disabled[sample->number_inputs]);uart_putc('\n');
            break; 
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    ////////////////////// 
    
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////4
    // PMred, sixteen bit channel, differential :
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( ); 
            value = read_adc(0x28, 0xA2, 0x88);//ch 4,5 diff 
            calc_and_string(value, sample,0);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////
    
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////5
    // PMIR, sixteen bit channel, differential :
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( ); 
            value = read_adc(0x28, 0xA3, 0x88);//ch 6,7 diff 
            calc_and_string(value, sample,0);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////6
    // Teflon flow, sixteen bit channel, differential :
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( ); 
            value = read_adc(0x28, 0xA4, 0x88);//ch 8,9 diff 
            calc_and_string(value, sample,0);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////7
    // Quartz flow, sixteen bit channel, differential :
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( ); 
            value = read_adc(0x28, 0xA5, 0x88);//ch 10,11 diff 
            calc_and_string(value, sample,0);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////8
    //Thermocouple, sixteen bit channel:
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( );
            value = read_adc(0x28, 0xBF, 0x88);//channel 15 single ended
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST           ://test channel
			sample->disabled[sample->number_inputs] = 0; //enable channel
			i2c_init();
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////
    
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////9
    // Sample temp, 10 bit channel
    switch(input_function)
    {
        case SAMPLE  : 
			toggle_led( );
            value =read_adc_onboard(2, 5); //channel 2, 5 oversamples
            calc_and_string(value, sample,0);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            adc_init();
            break;

    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////10
    // Bkg temp, 10 bit channel
    switch(input_function)
    {
        case SAMPLE  : 
			toggle_led( );
            value =read_adc_onboard(3, 5); //channel 3, 5 oversamples
            calc_and_string(value, sample,0);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            adc_init();
            break;

    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////
    
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////11
    // RH from SHT25:
    struct hum_dat check_humid = {0,0};
    switch(input_function)
    {
        case SAMPLE  :
            toggle_led( );
            if(sample->disabled[sample->number_inputs] == 1)//don't sample if disabled
            {
                calc_and_string(0, sample,1);
                break;
            }
            check_humid = ReadSHT25();
            calc_and_string(check_humid.SHT_rh, sample,0);//no decimal
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            i2c_timeout_flag = 0;
            start_timer0();//starts the timeout timer
            ReadSHT25();//try communicating with channel
            TCCR0 = 0;// stop counter
            sample->disabled[sample->number_inputs] = i2c_timeout_flag; //enable or diasble channel
            i2c_timeout_flag = 0;
            uart_puts_p(PSTR("#test result ch "));uart_putw_dec(sample->number_inputs); uart_putc('=');
            uart_putw_dec(sample->disabled[sample->number_inputs]);uart_putc('\n');
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////12
    // Temprature from SHT25:
    //!>>>>>>>always put after RH, as sampling is done in that block<<<<<<<<<<<
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( );
            if(sample->disabled[sample->number_inputs] == 1)//don't sample if disabled
            {
                calc_and_string(0, sample,1);
                break;
            }
            calc_and_string(check_humid.SHT_temp, sample,0);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
			sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            i2c_timeout_flag = 0;
            start_timer0();//starts the timeout timer
            ReadSHT25();//try communicating with channel
            TCCR0 = 0;// stop counter
            sample->disabled[sample->number_inputs] = i2c_timeout_flag; //enable or diasble channel
            i2c_timeout_flag = 0;
            uart_puts_p(PSTR("#test result ch "));uart_putw_dec(sample->number_inputs); uart_putc('=');
            uart_putw_dec(sample->disabled[sample->number_inputs]);uart_putc('\n');
            break;

    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////13
    // battery voltage, 10 bit channel
    switch(input_function)
    {
        case SAMPLE  : 
			toggle_led( );
            value =read_adc_onboard(4, 5); //channel 4, 5 oversamples
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            adc_init();
            break;

    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////



    uint16_t len = strlen(sample->serialout);
    if  (sample->serialout[len-1] == ',')//remove last comma
    {sample->serialout[len-1] = '\0';}
  

}





  /*!




Information to setup each type of channel:

16 bit single ended and differential:
Edit the hex values for the desired channel. See the chart below.  Copy the three hex numbers from the chart  and replace the ones in the "read_adc" line: 
value = read_adc(0x28, 0xB0, 0x88);//channel 

16 bit with CO chip initialization:
This is the same as the 16 bit channels above, but initialization of the CO potenstat chip is done initially by I2C 

16 bit channel addresses:

0x28, 0xB0, 0x88	 ch 0 single ended
0x28, 0xB8, 0x88	ch 1 single ended
0x28, 0xA0, 0x88	ch 0,1 diff
0x28, 0xB1, 0x88	ch 2 single ended
0x28, 0xB9, 0x88	ch 3 single ended
0x28, 0xA1, 0x88	 ch 2,3 diff
0x28, 0xB2, 0x88	ch 4 single ended
0x28, 0xBA, 0x88	 ch 5 single ended
0x28, 0xA2, 0x88	ch 4,5 diff
0x28, 0xB3, 0x88	ch 6 single ended
0x28, 0xBB, 0x88	ch 7 single ended
0x28, 0xA3, 0x88	ch 6,7 diff
0x28, 0xB4, 0x88	ch 8 single ended
0x28, 0xBC, 0x88	ch 9 single ended
0x28, 0xA4, 0x88	ch 8,9 diff
0x28, 0xB5, 0x88	ch 10 single ended
0x28, 0xBD, 0x88	ch 11 single ended
0x28, 0xA5, 0x88	ch 10,11 diff
0x28, 0xB6, 0x88	ch 12 single ended
0x28, 0xBE, 0x88	ch 13 single ended
0x28, 0xA6, 0x88	ch 12,13 diff
0x28, 0xB7, 0x88	ch 14 single ended
0x28, 0xBF, 0x88	ch 15 single ended
0x28, 0xA7, 0x88	 ch 14,15 diff
10 bit ADC:
Edit the channel and the oversamples:
value =read_adc_onboard(1, 5); //channel 1, 5 oversamples
The voltage reference is 2.56v from the ADC's internal source.

Soft uart CO2 :
Edit the soft uart number: (1,2 or 3) 
get_CO2(1); Two instances suart_init(1);One instance 

Software UART Pins:  
First s-uart: On the 6 pin connector along with gnd, 5v, and reset.  Labeled Rx2 and Tx2. 
Second s-uart: On the 10 bit/GPIO connector. Tx and Rx are labeled ch0,  ch1
Third s-uart: On the 10 bit/GPIO connector. Tx and Rx are labeled ch6,  ch7

SHT25 Relative Humidity sensor: 
There can be only one on the I2C bus. No editing required.  Note: the SHT25 RH input block must come before the SHT25 temperature block, as the data is requested only once and has both numbers.

Decimal point location for all channels: On each channel there is the following line:
calc_and_string(value, sample,4);
Edit the number of digits after the decimal point you want. In this case it is four. 
*/

///Channel Templates

    /*!
     * 
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////
// CO sample, sixteen bit channel, differential, init via I2c :
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( ); 
            value = read_adc(0x28, 0xA0, 0x88);//ch 0,1 diff 
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            i2c_timeout_flag = 0;
            start_timer0();//starts the timeout timer
            Init_CO();
            TCCR0 = 0;// stop counter
            sample->disabled[sample->number_inputs] = i2c_timeout_flag; //enable or diasble channel
            i2c_timeout_flag = 0;
            uart_puts_p(PSTR("#test result ch "));uart_putw_dec(sample->number_inputs); uart_putc('=');
            uart_putw_dec(sample->disabled[sample->number_inputs]);uart_putc('\n');
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////2
    // C02 sample via serial communication: 

    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( );
            if(sample->disabled[sample->number_inputs] == 1)//don't sample if disabled
            {
                calc_and_string(0, sample,1);
                break;
            }
            value = get_CO2(1);//gets value form suart1
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST           ://test channel
            sample->disabled[sample->number_inputs] = 0; //enable channel
            init_CO2(1);
            suart_timedout_flag = 0; DDRC |= (1<<6); PORTC &=~ (1<<6); //set flag pin low
            start_timer0();//starts the timeout timer
            get_CO2(1);
            TCCR0 = 0;// stop counter
            sample->disabled[sample->number_inputs] = suart_timedout_flag; //enable or diasble channel
            suart_timedout_flag = 0; PORTC &=~ (1<<6); //set flag pin low
            uart_puts_p(PSTR("#test result ch "));uart_putw_dec(sample->number_inputs); uart_putc('=');
            uart_putw_dec(sample->disabled[sample->number_inputs]);uart_putc('\n');
            break; 
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////
 
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////
    // C02 bkg via serial communication: 

    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( );
            if(sample->disabled[sample->number_inputs] == 1)//don't sample if disabled
            {
                calc_and_string(0, sample,1);
                break;
            }
            value = get_CO2(2);//gets value form suart1
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST           ://test channel
            sample->disabled[sample->number_inputs] = 0; //enable channel
            suart_init(2);
            _delay_ms(1);
            suart_timedout_flag = 0; DDRC |= (1<<6); PORTC &=~ (1<<6); //set flag pin low
            start_timer0();//starts the timeout timer
            get_CO2(2);
            TCCR0 = 0;// stop counter
            sample->disabled[sample->number_inputs] = suart_timedout_flag; //enable or diasble channel
            suart_timedout_flag = 0; PORTC &=~ (1<<6); //set flag pin low
            uart_puts_p(PSTR("#test result ch "));uart_putw_dec(sample->number_inputs); uart_putc('=');
            uart_putw_dec(sample->disabled[sample->number_inputs]);uart_putc('\n');
            break; 
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    ////////////////////// 
    
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////
    // PM, sixteen bit channel, differential :
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( ); 
            value = read_adc(0x28, 0xA6, 0x88);//ch 12,13 diff 
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////
    
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////
    // Dilution flow, sixteen bit channel, differential :
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( ); 
            value = read_adc(0x28, 0xA5, 0x88);//ch 10,11 diff 
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////    

    
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////
    // Sample temp, 10 bit channel
    switch(input_function)
    {
        case SAMPLE  : 
			toggle_led( );
            value =read_adc_onboard(2, 5); //channel 2, 5 oversamples
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            adc_init();
            break;

    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

    
    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////
    // RH from SHT25:
    struct hum_dat check_humid = {0,0};
    switch(input_function)
    {
        case SAMPLE  :
            toggle_led( );
            if(sample->disabled[sample->number_inputs] == 1)//don't sample if disabled
            {
                calc_and_string(0, sample,1);
                break;
            }
            check_humid = ReadSHT25();
            calc_and_string(check_humid.SHT_rh, sample,1);//one digit after decimal
            
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            //uart_puts_p(PSTR("#test ch "));uart_putw_dec(sample->number_inputs); uart_putc('\n');
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            i2c_timeout_flag = 0;
            //uart_puts_p(PSTR("#start timer and read\n"));
            start_timer0();//starts the timeout timer
            ReadSHT25();//try communicating with channel
            TCCR0 = 0;// stop counter
            sample->disabled[sample->number_inputs] = i2c_timeout_flag; //enable or diasble channel
            i2c_timeout_flag = 0;
            uart_puts_p(PSTR("#test result ch "));uart_putw_dec(sample->number_inputs); uart_putc('=');
            uart_putw_dec(sample->disabled[sample->number_inputs]);uart_putc('\n');
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////

    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////
    // Temprature from SHT25:
    //!>>>>>>>always put after RH, as sampling is done in that block<<<<<<<<<<<
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( );
            if(sample->disabled[sample->number_inputs] == 1)//don't sample if disabled
            {
                calc_and_string(0, sample,1);
                break;
            }
            calc_and_string(check_humid.SHT_temp, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
			sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            i2c_timeout_flag = 0;
            start_timer0();//starts the timeout timer
            ReadSHT25();//try communicating with channel
            TCCR0 = 0;// stop counter
            sample->disabled[sample->number_inputs] = i2c_timeout_flag; //enable or diasble channel
            i2c_timeout_flag = 0;
            uart_puts_p(PSTR("#test result ch "));uart_putw_dec(sample->number_inputs); uart_putc('=');
            uart_putw_dec(sample->disabled[sample->number_inputs]);uart_putc('\n');
            break;
        case INIT_CHAN      ://initializes the channel, if needed
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////


    ////THIS IS A CHANNEL vvvvvvvvvvvvv    //////////////////////
    // ADC chip temp channel:
    switch(input_function)
    {
        case SAMPLE  :
			toggle_led( );
            value = read_adc(0x28, 0xA0, 0xC0);//chip temp mux for ADC
            calc_and_string(value, sample,1);
            if (report_chan ==1)//report channel run
            { 
                uart_putc('#');
                uart_putw_dec(sample->number_inputs);
                uart_putc(' ');
            }
            break;
        case TEST   :
            sample->disabled[sample->number_inputs] = 0; //enable channel
            i2c_init();
            break;
    }
    sample->number_inputs ++ ;
    ////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^    //////////////////////   


    */



