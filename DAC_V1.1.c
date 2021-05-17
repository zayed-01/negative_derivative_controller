
#include "DAC_V1.1.h"

unsigned int pwm_period = 320-1;//640-1;//320-1; //PWm period calculation kelvin Lych
unsigned int pwm_on_time = 160-1;//320-1;//160-1 ;
unsigned int controller_period = 6400-1;//320-1;



unsigned char DAC_send(unsigned char des);


void Init_Timer3(void)
{
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_1, controller_period);
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_2);
    mT3ClearIntFlag(); // and clear the interrupt flag
}

/*
void INIT_PWM (void)
{
    // === Config timer and output compares to make pulses ========
  // set up timer2 to generate the wave period
  OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_1, pwm_period);
  //ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_3);
   //mT2ClearIntFlag(); // and clear the interrupt flag

  // set up compare3 for PWM mode
  OpenOC3(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE , pwm_on_time, pwm_on_time);
  // OC3 is PPS group 4, map to RPB9 (pin 18)
  PPSOutput(4, RPB9, OC3);
}
                */

void INIT_DACSPI (void)
 {
 mPORTBSetPinsDigitalOut(BIT_15 | BIT_11| BIT_7 ); // SCK||SDO||CS
    PPSOutput(2,RPB11,SDO2); // pin 22
    SPI2STATCLR = 0x40;
    SPI2CON = 0;
    char clr_Buf = SPI2BUF;
    SPI2BRG = 2; // Fpb/2/1 MHz  //determine the clock speed of SPI connection
    SPI2CON = 0x8320;
    DAC_CS = 1;                    // initial CS state pin 16
 } 

unsigned char DAC_send(unsigned char des)
{
    SPI2BUF = des; //write
    while (SPI2STATbits.SPIBUSY);
    return SPI2BUF;
    

  }

void DAC_WRITE(unsigned short chnB,unsigned short chnA)
{ 
    unsigned char byte0,byte1,byte2; 
    
    byte0 = (unsigned char) (chnB >> 4) ;
    byte1 = (unsigned char) ((chnB << 4) | (chnA >> 8));
    byte2 = (unsigned char) (chnA & 0xff);

    DAC_CS  = 0;

    DAC_send(byte0);
    DAC_send(byte1);
    DAC_send(byte2);
    
    DAC_CS  = 1;

}
