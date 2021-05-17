
#ifndef _PIC32_DELAY_H    /* Guard against multiple inclusion */
#define _PIC32_DELAY_H



 #define LED             LATAbits.LATA0
#define TOGGLE_LED( )    (mPORTAToggleBits(BIT_0))

/* -----Delay Functions-- ------------------ -= = */
//void Delay_ms(int delay);
void Delay_ms(int delay);
void Delay_s(int delay);
void Delay_us(int delay);
void Delay_03us(int delay);
void INIT_LED(void);

    
       

#endif



