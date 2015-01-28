Documentation for the Model 32 DAQ OpenDAQ firmware. {#mainpage}
============
\tableofcontents
\section Overview Overview
This firmware is designed to be easily edited to add, remove, and
change the order of inputs.\n
All the standard input blocks are in the file inputs.c \n
With minimal programming experience, other sensors with different
protocols may be added as well, by creating a new input block and
adding it to inputs.c \n
\section two Adding and ordering channels
\htmlonly Steps:
<ul>
<li>Install the compiler, libraries and editor.</li>
<li>Edit inputs.c, placing input code blocks in order.</li>
<li>Compile to a .hex file.</li>
<li>Upload this hex file to the DAQ </li>
</ul>
To compile on Ubuntu:<br>
Install the compiler, libraries, and the editor called Geany:<br>
<br>
<span style="color: rgb(51, 102, 255);">sudo
apt-get install gcc-avr
</span><br style="color: rgb(51, 102, 255);">
<span style="color: rgb(51, 102, 255);">sudo
apt-get install avr-libc</span><br
style="font-family: serif; color: rgb(51, 102, 255);">
<span style="color: rgb(51, 102, 255);">sudo
apt-get install geany</span><br>
<br>
Open the file input.c with Geany.<br>
There are blocks of code:<br>
<font color="green"> ////THIS IS A CHANNEL vvvvvvvvvvvvv
//////////////////////<br>
..<br>
--code in here--<br>
..<br>
////END OF THIS CHANNEL ^^^^^^^^^^^^^^^^^^ //////////////////////</font><br>
These are all the channels. These can be cut and pasted, added and
removed, and re-ordered. <br>
There are 8 types:<br>
<ul>
<li>16bit single
ended&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
(LTC2495 ADC IC from Linear Technologies)<br>
</li>
<li>16 bit differential</li>
<li>16 bit chip temperature</li>
<li>10
bit&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
(Onboard ADC on ATmega32 microcontroller)<br>
</li>
<li>Soft UART
CO2&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
(COZIR sensor )<span
style="color: rgb(0, 0, 0); font-family: Roboto,sans-serif; font-size: 14px; font-style: normal; font-variant: normal; font-weight: normal; letter-spacing: normal; line-height: 22px; text-align: start; text-indent: 0px; text-transform: none; white-space: normal; word-spacing: 0px; display: inline ! important; float: none; background-color: rgb(255, 255, 255);"></span>
</li>
<li>Sht25
R.H.&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
(Sensurion)<br>
</li>
<li>Sht25 temperature</li>
<li>16 bit with CO chip initialization&nbsp; (LMP91000 potenstat IC)<br>
</li>
</ul>
<strong>16 bit single ended and differential</strong>:<br>
Edit the hex values for the desired channel. See the chart below.&nbsp;
Copy the three hex numbers from the chart&nbsp; and replace the ones in
the "read_adc" line: <br>
<font color="green">value = read_adc(<span
style="color: rgb(204, 0, 0);">0x28, 0xB0, 0x88</span>);//channel </font>
<br>
<br>
<strong> 16 bit with CO chip initialization:</strong><br>
This is the same as the 16 bit channels above, but initialization of
the CO potenstat chip is done initially by I2C <br>
<br>
<span style="font-weight: bold;">16 bit channel addresses:</span><br>
<table style="text-align: left; width: 390px; height: 333px;" border="1"
cellpadding="2" cellspacing="2">
<tbody>
<tr>
<td style="vertical-align: top;">0x28, 0xB0, 0x88</td>
<td style="vertical-align: top;"> ch 0 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xB8, 0x88</td>
<td style="vertical-align: top;">ch 1 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xA0, 0x88</td>
<td style="vertical-align: top;">ch 0,1 diff</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xB1, 0x88</td>
<td style="vertical-align: top;">ch 2 single ended </td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xB9, 0x88</td>
<td style="vertical-align: top;">ch 3 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xA1, 0x88</td>
<td style="vertical-align: top;"> ch 2,3 diff </td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xB2, 0x88</td>
<td style="vertical-align: top;">ch 4 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xBA, 0x88</td>
<td style="vertical-align: top;"> ch 5 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xA2, 0x88</td>
<td style="vertical-align: top;">ch 4,5 diff</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xB3, 0x88</td>
<td style="vertical-align: top;">ch 6 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xBB, 0x88</td>
<td style="vertical-align: top;">ch 7 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xA3, 0x88</td>
<td style="vertical-align: top;">ch 6,7 diff</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xB4, 0x88</td>
<td style="vertical-align: top;">ch 8 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xBC, 0x88</td>
<td style="vertical-align: top;">ch 9 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xA4, 0x88</td>
<td style="vertical-align: top;">ch 8,9 diff</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xB5, 0x88</td>
<td style="vertical-align: top;">ch 10 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xBD, 0x88</td>
<td style="vertical-align: top;">ch 11 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xA5, 0x88</td>
<td style="vertical-align: top;">ch 10,11 diff</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xB6, 0x88</td>
<td style="vertical-align: top;">ch 12 single ended </td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xBE, 0x88</td>
<td style="vertical-align: top;">ch 13 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xA6, 0x88</td>
<td style="vertical-align: top;">ch 12,13 diff</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xB7, 0x88</td>
<td style="vertical-align: top;">ch 14 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xBF, 0x88</td>
<td style="vertical-align: top;">ch 15 single ended</td>
</tr>
<tr>
<td style="vertical-align: top;">0x28, 0xA7, 0x88</td>
<td style="vertical-align: top;"> ch 14,15 diff</td>
</tr>
</tbody>
</table>
<br>
<strong>10 bit</strong><span style="font-weight: bold;"> ADC:</span><br>
Edit the channel and the oversamples:<br>
<font color="green">value =read_adc_onboard(1, 5); //channel 1, 5
oversamples</font><br>
The voltage reference is 2.56v from the ADC's internal source.<br>
<br>
<strong>Soft uart CO2</strong><span style="font-weight: bold;">
:</span><br>
Edit the soft uart number: (1,2 or 3)
<br>
<font color="green">get_CO2(1); </font>Two instances
<br>
<font color="green">init_CO2(1);</font>One instance
<br>
<br>
<span style="font-weight: bold;">Software UART Pins:&nbsp;</span> <br>
First s-uart: On the 6 pin connector along with gnd, 5v, and
reset.&nbsp; Labeled Rx2 and Tx2. <br>
Second s-uart: On the 10 bit/GPIO connector. Tx and Rx are labeled
ch0,&nbsp; ch1<br>
Third s-uart: On the 10 bit/GPIO connector. Tx and Rx are labeled
ch6,&nbsp; ch7<br>
<br>
<span style="font-weight: bold;">SHT25 Relative Humidity sensor: </span>
<br>
There can be only one on the I2C bus. No editing required.&nbsp; Note:
the SHT25 RH input block must come before the SHT25 temperature block,
as the data is requested only once and has both numbers.<br>
<br>
<strong>Decimal point location for all channels:</strong>
On each channel there is the following line:<br>
<font color="green">calc_and_string(value, sample,4);</font><br>
Edit the number of digits after the decimal point you want. In this
case it is four.
<br>
\endhtmlonly
\section three Compiling
\htmlonly Save the file, then click the down arrow by the brick icon,
choose
"Make Custom Target" and type clean in the box. Return.<br>
Open the file Makefile and edit the version number and the device name:
<br>
<font color="green">PROJECTNAME=OpenDAQ_THISMACHINE&dollar;(VER)</font><br>
Channge THISMACHINE to the name of the machine the firmware has been customized for.<br>
<font color="green">VER := 118</font><br>
This must be a number. The tens and ones digits refer to the main firmware version.  
The hundreds spot is the verson number for the specific device. 
For example, if the firmware has been modified for THISMACHINE four times, the number would then be 418.<br>
Then choose "Make
All" After some other messages, the following message should appear in
the area below the code
editor window:
<br>
<br style="color: rgb(51, 102, 255);">
<font color="green"><span style="color: rgb(51, 102, 255);"> -O ihex
opendaq_THISMACHINE107.out opendaq_THISMACHINE107.hex</span><br style="color: rgb(51, 102, 255);">
<span style="color: rgb(51, 102, 255);">Compilation finished
successfully.</span><br style="color: rgb(51, 102, 255);">
<span style="color: rgb(51, 102, 255);">avr-objcopy -j .eeprom \</span><br
style="color: rgb(51, 102, 255);">
<span style="color: rgb(51, 102, 255);">--change-section-lma .eeprom=0 \</span><br
style="color: rgb(51, 102, 255);">
<span style="color: rgb(51, 102, 255);">-O ihex opendaq_THISMACHINE107.out
opendaq_THISMACHINE107.ee.hex</span><br>
</font><br>
Alternately, use these terminal commands: (in the firmware folder) <br>
<span style="color: rgb(51, 102, 255);">
&nbsp;&nbsp;&nbsp;&nbsp;make clean<br>
&nbsp;&nbsp;&nbsp;&nbsp;make
</span><br>
Now upload the hex file (opendaq_THISMACHINE107.hex in this example) to the DAQ.<br>
\endhtmlonly
\section four Uploading the .hex file to the DAQ
Prerequisites:
<br>
<ul>
<li>Avrdude:
<span style="color: rgb(51, 102, 255);">sudo apt-get install
avrdude <br>
</span></li>
<li><span style="color: rgb(51, 102, 255);"></span>&nbsp;DAQ
connected to serial port to which you have access (i.e. /dev/ttyUSB0 or
/dev/ttyS0, or /dev/ttyACM0) <br>
</li>
</ul>
Uploading:<br>
<ul>
<li>Power off the DAQ. Install the jumper on the angled 2-pin
connector Power the DAQ on. The DAQ is now in bootloader mode.</li>
<li>In a terminal, cd to the folder .hex file and issue the
command:&nbsp; <span style="color: rgb(51, 102, 255);">
avrdude -c avr109 -p atmega32 -P /dev/ttyS5 -e -U
flash:w:OpenDAQ_113.hex</span>&nbsp; (replace /dev/ttyS5 with the
proper port, and OpenDAQ_113 with the correct version name.)
</li>
</ul>
\section fourptfive Troubleshooting Notes

Soft UART pins are easy to get reversed.  Swap the pins or swap the assignments in suart.h 
Use the direct connection in the cal menu to test.

All external-to-the-DAQ channels are tested at startup.  If the channel replies, a 0 is shown, 
if there is a failure, that channel is disabled and a 1 will be shown.  

When the channels are sampled to determine timing, each enabled channel is reported with a # sign.

Check the clock time. If the clock fails, or reverts to default time after a battery change, the DAQ will not continue.
<br>
\section five Additional Information
<strong>Soft UART pin assignments:</strong><br>
S-UART 1: Rx(in): PIN B5, Tx(out): PORT B6. (On 6 pin reset connector)<br>
S-UART 2: Rx(in): PIN A0, Tx(out): PORT A1. (On GPIO connector)<br>
S-UART 3: Rx(in): PIN A6, Tx(out): PORT A7 (On GPIO connector)<br>
All are set to 9600,8n1<br>
These can be changed in suart.h \htmlonly 
<strong><br>DAQ microcontroller pins:</strong>
<table style="text-align: left; width: 100%;" border="1" cellpadding="2"
cellspacing="2">
<tbody>
<tr>
<td style="vertical-align: top;">I2C terminal<br>
<table style="text-align: left; width: 100%;" border="1"
cellpadding="2" cellspacing="2">
<tbody>
<tr>
<td style="vertical-align: top;">sda</td>
<td style="vertical-align: top;">PORT C1 <br>
</td>
</tr>
<tr>
<td style="vertical-align: top;">scl </td>
<td style="vertical-align: top;">PORT C0<br>
</td>
</tr>
</tbody>
</table>
<br>
</td>
<td style="vertical-align: top;">Serial
<table style="text-align: left; width: 100%;" border="1"
cellpadding="2" cellspacing="2">
<tbody>
<tr>
<td style="vertical-align: top;">Rx</td>
<td style="vertical-align: top;">PORT D0</td>
</tr>
<tr>
<td style="vertical-align: top;">Tx</td>
<td style="vertical-align: top;">PORT D1</td>
</tr>
</tbody>
</table>
<br>
</td>
<td style="vertical-align: top;">10 bit ADC/GPIO
<table style="text-align: left; width: 100%;" border="1"
cellpadding="2" cellspacing="2">
<tbody>
<tr>
<td style="vertical-align: top;">ch0</td>
<td style="vertical-align: top;">PORT A0</td>
</tr>
<tr>
<td style="vertical-align: top;">ch1<br>
</td>
<td style="vertical-align: top;">PORT A1<br>
</td>
</tr>
<tr>
<td style="vertical-align: top;">
<meta http-equiv="content-type"
content="text/html; charset=utf-8">
<span
style="color: rgb(0, 0, 0); font-family: Arial,'Liberation Sans','DejaVu Sans',sans-serif; font-size: 14px; font-style: normal; font-variant: normal; font-weight: normal; letter-spacing: normal; line-height: 17.8048px; text-align: left; text-indent: 0px; text-transform: none; white-space: normal; word-spacing: 0px; display: inline ! important; float: none; background-color: rgb(255, 255, 255);">↓</span></td>
<td style="vertical-align: top;">
<meta http-equiv="content-type"
content="text/html; charset=utf-8">
<span
style="color: rgb(0, 0, 0); font-family: Arial,'Liberation Sans','DejaVu Sans',sans-serif; font-size: 14px; font-style: normal; font-variant: normal; font-weight: normal; letter-spacing: normal; line-height: 17.8048px; text-align: left; text-indent: 0px; text-transform: none; white-space: normal; word-spacing: 0px; display: inline ! important; float: none; background-color: rgb(255, 255, 255);">↓</span></td>
</tr>
<tr>
<td style="vertical-align: top;">ch7</td>
<td style="vertical-align: top;">PORT A7</td>
</tr>
</tbody>
</table>
<br>
</td>
<td style="vertical-align: top;">6 pin terminal with reset
<table style="text-align: left; width: 100%;" border="1"
cellpadding="2" cellspacing="2">
<tbody>
<tr>
<td style="vertical-align: top;">H-rst</td>
<td style="vertical-align: top;">hard reset pin</td>
</tr>
<tr>
<td style="vertical-align: top;">Tx2 </td>
<td style="vertical-align: top;">PORT B5<br>
</td>
</tr>
<tr>
<td style="vertical-align: top;">Rx2</td>
<td style="vertical-align: top;">PORT B6<br>
</td>
</tr>
<tr>
<td style="vertical-align: top;">reset</td>
<td style="vertical-align: top;">PORT B7<br>
</td>
</tr>
</tbody>
</table>
<br>
</td>
</tr>
</tbody>
</table>
<br>
<strong>Communication with the CO2 serial port directly: </strong><br>
Start picocom with<br>
<font color="green">--omap crcrlf</font><br>
like this:<font color="green"> picocom --omap crcrlf /dev/ttyS0 </font><br>
That way it will send the required \r\n at the end of the command.<br>
Picocom version 1.7 or greater required.
<br>
\endhtmlonly
\section six Further Development
<strong>Important inline documentation on functions: <br>
</strong>
main()<br>
suart.h<br>
<strong>Format of the channel entries: <br>
</strong>Each block has two sections, SAMPLE and TEST.&nbsp; TEST is
done right after power-up, and will test and/or initializes each
channel.&nbsp; If a channel fails, it will be disabled, and will not be
sampled when the main sample loop runs, just&nbsp; A zero placeholder
will be the output.&nbsp; This way the DAQ can operate with a sensor
disconnected. The test is only done one time at startup, so a sensor
that is removed or disconnected later will stop the DAQ.&nbsp; <br>
<br>
<strong>Inputs and their corresponding files:</strong>
@image html OPENDAQ_diag.png
\htmlonly<br>
Each input has a file which contains the commands for sampling it. That
file usually
has a corresponding file containing the low-level communication code,
such as i2c or UART. These low-level files are shared and can be used
by any input or stand-alone.&nbsp; Channels that sample using built-in
devices, such as the 10-bit ADC have no low-level file. <br>
An example using a low-level file stand-alone:&nbsp; using soft-UART 3
in the main loop to output text for a UART enabled LCD display:
<br>
<span style="color: rgb(0, 153, 0);">suart_puts(3, const char*
"CL");&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp; &nbsp; //clears screen
</span><br style="color: rgb(0, 153, 0);">
<span style="color: rgb(0, 153, 0);">suart_puts(3, const char*
"TTsample completed");&nbsp;&nbsp; //sends text "sample completed"</span><br>
Where CL and TT are the display specific commands<br>
<br>
\endhtmlonly
