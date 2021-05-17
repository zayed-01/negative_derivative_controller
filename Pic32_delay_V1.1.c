
#include "Pic32_delay_V1.1.h"

#define _SUPPRESS_PLIB_WARNING 
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>
#define CORE_TIMER_COUNTU        10
#define CORE_TIMER_COUNTN        32
#define CORE_TIMER_COUNT          32000
#define CORE_TIMER_COUNTS        32000000


void Delay_03us(int delay)
{
_CP0_SET_COUNT(0);
 while( _CP0_GET_COUNT() < delay * CORE_TIMER_COUNTU) {
     ;
 }
}


void Delay_ms(int delay)
{
_CP0_SET_COUNT(0);
 while( _CP0_GET_COUNT() < delay * CORE_TIMER_COUNT ) {
     ;
 }
}
void Delay_us(int delay)
{
_CP0_SET_COUNT(0);
 while( _CP0_GET_COUNT() < delay * CORE_TIMER_COUNTN ) {
     ;
 }
}


void Delay_s(int delay)
{
_CP0_SET_COUNT(0);
 while( _CP0_GET_COUNT() < delay * CORE_TIMER_COUNTS ) {
     ;
 }
}

void INIT_LED(void)
 {
    mPORTASetPinsDigitalOut(BIT_0); //LED OUT
    LED = 0;         // initial LED state
                        }