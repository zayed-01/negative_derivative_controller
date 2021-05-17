

#ifndef __PICONFIG_V1_H    /* Guard against multiple inclusion */
#define __PICONFIG_V1_H





#define _SUPPRESS_PLIB_WARNING 
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING

#include <plib.h>
#include<stdbool.h>
#include <stdio.h>
#include <math.h>
#include <dsplib_dsp.h>
    
 
#include "Uart_Print_V1.1.h" // Print uart
#include "Pic32_delay_V1.1.h" // Delay Header
#include "ADC_V1.1.h"  // ADC header    
#include"DAC_V1.1.h"
#include "ndf.h"
#include "Lpfilter.h"   


    

             //   pin 4 and pin 5 for PGEC/D  ;   REFCLCK OUT  FOM PIN 9
//=============================================================
// 60 MHz
#pragma config FNOSC = FRCPLL, POSCMOD = OFF
//#pragma config OSCIOFNC = ON // output refrence clock on      
//#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20, FPLLODIV = DIV_2  //40 MHz
//#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_16, FPLLODIV = DIV_256  //250Khz
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_16, FPLLODIV = DIV_1  //64 MHz
// #pragma config FPBDIV = DIV_8 // PB 31.25Khz  // 8mhz
//#pragma config FPBDIV = DIV_1 // PB 40 MHz
#pragma config FPBDIV = DIV_2 // PB 32 MHz

#pragma config FWDTEN = OFF,  JTAGEN = OFF
#pragma config FSOSCEN = OFF  //PINS 11 and 12 to secondary oscillator!
#pragma config FUSBIDIO = OFF, FVBUSONIO = OFF
//#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config ICESEL = 3      // Program with PGEC1/PGED1 program ports
#pragma config DEBUG = OFF   // RB4 and RB5
  
    
//==============================================================//
// Protothreads configure
    
//#define use_vref_debug
// IF use_uart_serial IS defined, two pins are used by the uart `11`
// Go to pt_cornell_1_2_2.h and search for "SET UART i/o PINS"
//#define use_uart_serial
//==============================================================//
// set up system cpu clock parameters
#define sys_clock       64000000   
#define pb_clock       (sys_clock/2)      // divide by two
#define period          (((pb_clock/1000)- 1))
          
#define BUFF_SIZE       60
#define max_count       2450
#define Points_No        1
#define DECIMATE 0
    

/****GLOBAL Variables*******************/     
char buffer[BUFF_SIZE] = {0};

volatile signed int FIFO[max_count] ;
volatile signed int  Test[max_count];
//volatile float Test[max_count];
//volatile float FIFO[max_count] ;
char channel[max_count] ;

extern unsigned int count_send, k, count;
unsigned int cmd_dummy[5] = {0x250000, 0, 0, 0, 0};
unsigned int NSAMPLES = (unsigned int)max_count ;
unsigned int sent = 0;
volatile signed int print = 0,ADC_Data2 = 0,ADC_New2 = 0,ADC_Data3 = 0,ADC_New3 = 0,ADC_Data4 = 0,ADC_New4 = 0;

typedef signed int fix16 ;
#define multfix16(a,b) ((fix16)(((( signed long long)(a))*(( signed long long)(b)))>>16)) //multiply two fixed 16:16
#define float2fix16(a) ((fix16)((a)*65536.0)) // 2^16
#define fix2float16(a) ((float)(a)/65536.0)
#define fix2int16(a)    ((int)((a)>>16))
#define int2fix16(a)    ((fix16)((a)<<16))
#define divfix16(a,b) ((fix16)((((signed long long)(a)<<16)/(b)))) 
#define sqrtfix16(a) (float2fix16(sqrt(fix2float16(a)))) 
#define absfix16(a) abs(a)
#define onefix16 0x00010000 // int2fix16(1)


typedef struct
{	
    signed int resp;
    signed int chan1;
    signed int DAC_chan;
    signed int Vout_chan;
    signed int bias_chan;
}  adc_data ;


volatile adc_data IN_data = {0,0,0,0,0};



#endif /* _PICONFIG_H */

