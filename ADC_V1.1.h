#ifndef _ADC_V1_H
#define _ADC_V1_H

#define _SUPPRESS_PLIB_WARNING 
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING

#include <plib.h>
#include <stdbool.h>

/*============GLOBAL VARIABLES DEFINITIONS================*/
#define two24       16777216.0 
#define Ref           4.884
#define BI_TO_VOLT(x) (((double)(x) * ((double)Ref/(double)two24)))
#define VOLT_TO_BI(x) ((unsigned int)((double)two24/(double)Ref) * (double)(x))

/* -----ADC macros for MCU-- ------------------ -= = */
#define CS                   PORTBbits.RB4
#define DRDY                 PORTAbits.RA3

//****************************************************************************
//
// Channel data structure
//
//****************************************************************************


//****************************************************************************
//
// ADC Definitions*
//
//******************************************************************************
//Read/Write Registers  - Ack and Command
#define ADC_READ_REG_CMD                                                                                       0x2000
#define ADC_READ_MREG_CMD                                                                                    0x2000
#define ADC_WRITE_REG_CMD                                                                                      0x4000
#define ADC_WRITE_MREG_CMD                                                                                   0x6000

#define ADC_READ_REG_ACK                                                                                         0x2000
#define ADC_READ_MREG_ACK                                                                                      0x6000
#define ADC_WRITE_REG_ACK                                                                                        0x2000
#define ADC_WRITE_MREG_ACK                                                                                     0x4000


//System Commands and Ack
#define         NUL                                                                                                          ((unsigned short)0x0000)
#define         READY											      ((unsigned short)0xFF04)
#define         RESET                                                                                                       ((unsigned short)0x0011)
#define         STANDBY                                                                                                  ((unsigned short)0x0022)
#define         WAKEUP										               ((unsigned short)0x0033)
#define         LOCK                                                                                                        ((unsigned short)0x0555)
#define         UNLOCK                                                                                                   ((unsigned short)0x0655)


#define READ_REG (address, register_count) 					(ADC_READ_REG_CMD | (address<<8) | register_count)
//Please note that register_count tells A04 device register_count+1 registers to read

#define READ_MREG (address, register_count) 		                          (ADC_READ_MREG_CMD| (address<<8) | register_count)
#define WRITE_REG (address, data) 							(ADC_WRITE_REG_CMD | (address<<8) | data)
#define WRITE_MREG (address, register_count) 		                           (ADC_WRITE_MREG_CMD | (address<<8) | register_count)
#define WRITE_MREG_AWORD (data1, data2)			                  ((data1<<8) | data2)
//data1 is written to a register at address N.  data2 is written to next register at address N+1.

#define WRITE_MREG_ACK (address, register_count) 			         (ADC_WRITE_MREG_ACK  | (address<<8) | register_count)
#define WRITE_REG_ACK (address, data) 					        (ADC_WRITE_REG_ACK  | (address<<8) | data)
#define READ_REG_RESPONSE_PARSE (response, op_code, address, data) \
{ \
	op_code=(response & 0xE000); \
	address = ((response & 0x1F00)>>8); \
	data=(response & 0x00FF);\
}
#define READ_MREG_ACK (address, register_count) 			(ADC_READ_MREG_ACK | (address<<8) | register_count)
#define READ_MREG_RESPONSE_PARSE (response, data1, data2) \
{ \
	data1=((response & 0xFF00)>>8);\
	data2=(response & 0x00FF);\
}

/* ADS131A0x Register Settings Export */
/******************************************************************************/
/* This file contains the register map settings stub */
// General defines
#define ADS131A0x_REGISTER_COUNT 17
/* Register #define values (register address and value) */
/******************************************************************************/
/* This section contains the defines for register address and register settings */
//Register Addresses
#define ID_MSB															0x00 //ID Control Register MSB
#define ID_LSB															0x01 //ID Control Register LSB
#define STAT_1															0x02 //Status 1 Register
#define STAT_P															0x03 //Positive Input Fault Detect Status
#define STAT_N															0x04 //Negative Input Fault Detect Status
#define STAT_S															0x05 //SPI Status Register
#define ERROR_CNT														0x06 //Error Count Register
#define STAT_M2															0x07 //Hardware Mode Pin Status Register
//#define RESERVED														0x08
//#define RESERVED														0x09
//#define RESERVED														0x0A
#define A_SYS_CFG														0x0B //Analog System Configuration Register
#define D_SYS_CFG														0x0C //Digital System Configuration Register
#define CLK1															0x0D //Clock Configuration 1 Register
#define CLK2															0x0E //Clock Configuration 2 Register
#define ADC_ENA													         0x0F //ADC Channel Enable Register
//#define RESERVED														0x10
#define ADC1													         0x11 //ADC Channel 1 Digital Gain Configuration
#define ADC2													         0x12 //ADC Channel 2 Digital Gain Configuration
#define ADC3													         0x13 //ADC Channel 3 Digital Gain Configuration
#define ADC4													         0x14 //ADC Channel 4 Digital Gain Configuration
#define CHANNEL													4 //ADC Number of channels
#define STATUS                                                                                                             1 //ADC Number of status channels
#define  XTAL_CLKIN                                                                                                                          0x00 //ADC clock source. This bit selects the source for ICLK; 0 : XTAL1/CLKIN pin or XTAL1/CLKIN and XTAL2 pins; 1: SCLK pin
#define  SCLK                                                                                                                                     0x01

//CLKIN divider ratio. These bits set the CLKIN divider ratio to generate the internal fICLK frequency. ICLK is used as the fSCLK output when the ADC is operating in synchronous master mode.
#define CLK1_CONFIG(CLKSRC,CLK_DIV)                                     (CLKSRC<<4|((CLK_DIV<<1)&0b1110)) //CLK1: Clock Configuration 1 Register (address = 0Dh) [reset = 08h] //This register configures the ADC clocking and sets the internal clock divider

//ICLK divider ratio. These bits set the divider ratio to generate the ADC modulator clock, fMOD, from the fICLK signal.
#define CLK2_CONFIG(OSR,ICLK_DIV)                                       (((ICLK_DIV<<5)&0b1110)|OSR) //CLK2: Clock Configuration 2 Register (address = 0Eh) [reset = 86h] //This register configures the ADC modulator clock and oversampling ratio for the converter.


/*****************************************************
* Global Prototypes
*****************************************************/
signed int data_direction(signed int incoming);
void get_data(volatile signed int packets[5],unsigned int dummy[5] );
void ADC_startup(void);

#endif



