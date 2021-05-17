#include "ADC_V1.1.h"

//****************************************************************************
//
// Channel data structure
//
//****************************************************************************

/*****************************************************
* Local Functions
*****************************************************/
void SYS_Unlock(void);
void SYS_lock(void);
bool ADC_Ready(unsigned int timeout_ms);
signed int readADC(unsigned int dummy);
unsigned int  send(unsigned int  des);
void INT_ADC(void);
void ADC(void);
void INIT_ADC_SPI (void);
void INIT_EXT_INT(void);
void  INIT_CLCK(void);




void SYS_Unlock(void)
{
    SYSKEY = 0x12345678;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
}

void SYS_lock(void)
{
    SYSKEY = 0x00000000; 
}

bool ADC_Ready(unsigned int timeout_ms){

  unsigned int timeout = timeout_ms * 6000; // convert to # of loop iterations
  // wait for nDRDY interrupt or timeout - each iteration is about 20 ticks
  do{
    timeout--;
  } while (DRDY&& (timeout > 0));

  return (timeout > 0); // Did a nDRDY interrupt occur?
}

unsigned int send(unsigned int des){
  ADC_Ready(100);

  CS = 0; // start transaction

  SPI1BUF = des; //write

  while (!SPI1STATbits.SPIRBF)
    ;
  CS = 1; // end transaction
                     

  return SPI1BUF;
}

signed int readADC(unsigned int dummy){

  SPI1BUF = dummy; //write
  while (!SPI1STATbits.SPIRBF)
    ;
  return SPI1BUF;
}

void get_data(volatile signed int packets[5],unsigned int dummy[5] ){
    
  int i;
  for (i = 0; i < 5; i++)
    dummy[i] = 0x00; //  clear previous values 
 
  for (i = 0; i < 5; i++)
    packets[i] = 0x00; //  clear previous values 
    
  CS = 0; // start transaction
  for (i = 0; i < 5; i++)
    packets[i] = readADC(dummy[i]);
  
  CS = 1; // end transaction
}


signed int data_direction(signed int incoming ) 
{
    signed int out_data;
          if ( incoming  > 8388607)
            {
               out_data = incoming - 16777216 ;
                        }
          else 
          {
               out_data = incoming ; 
          }
    return out_data;
}

void INIT_ADC_SPI (void)
 {
 
  PPSInput(2, SDI1, RPB8);  // pin 17 //-> pin 5
  PPSOutput(3, RPB2, SDO1); // pin 6

  SPI1STATCLR = 0x40;
  SPI1CON = 0;
  int clearB = SPI1BUF;
  SPI1BRG = 0; // Fpb/2 
  SPI1CON = 0x8E20;

  mPORTBSetPinsDigitalOut(BIT_4 | BIT_14); // cs||SCK
  mPORTASetPinsDigitalIn(BIT_3);           // DRDY - pin12(PCB))
  CS = 1;                    // initial CS state pin 11
 
 } 

void INIT_EXT_INT(void)
{
  __builtin_disable_interrupts( );
  
 PPSInput(4, INT1, RPA3);
 ConfigINT1(EXT_INT_PRI_3 | FALLING_EDGE_INT | EXT_INT_ENABLE);
 mINT1ClearIntFlag(); 
 INTEnableSystemMultiVectoredInt( );
  
  __builtin_enable_interrupts( );
  
}

void  INIT_CLCK(void)
{
SYS_Unlock(); 
OSCTUN = 0x00;
//REFOTRIM = 0x80000000;   
REFOTRIM = 0;   
REFOCON = 0x0019901;
SYS_lock(); 
PPSOutput(3, RPA2, REFCLKO); // pin 9 ;
}
/*******************************************************/
//void INT_ADC(void)
//{
//    unsigned int trash[5] = {0, 0,0,0,0};
//    signed int response[5] = {0, 0,0,0,0};
//    unsigned short Indata = 0;
//
//    
// do
//  {
//    sendcmd(UNLOCK);
//    Indata = sendcmd(NUL);
//    
//  } while (Indata != UNLOCK); // Unlock
//   //printf("UNLOCK DONE\n ");     
//   
//    do
//  {
//    Indata = sendcmd(RESET);
//    Indata = sendcmd(NUL);
//    
//     } while (Indata != READY); //(2) RESET Ready
//    // printf("RESET DONE\n ");  
//   
//     do
//  {
//    sendcmd(UNLOCK);
//    Indata = sendcmd(NUL);
//    
//  } while (Indata != UNLOCK); // Unlock
//  // printf("UNLOCK DONE\n ");     
//                                                
//  do
//  {
//     
//    sendcmd(0x4B68);
//    Indata = sendcmd(NUL);
//    
//  } while (Indata != 0x2B68); //(A_SYS_CFG) //6-E
//  //                     printf("A_SYS_CFG DONE\n ");                             
//
//  do
//  {
//    sendcmd(0x4C00);
//   Indata = sendcmd(NUL);
//    
//  } while (Indata != 0x2C00); //(D_SYS_CFG)
//      //                                printf("D_SYS_CFG DONE\n ");              
//  do
//  {
//    sendcmd(0x4D02);
//   Indata =  sendcmd(NUL);
//    
//  } while (Indata != 0x2D02); //(CLCKIN(1)
//        //                              printf("CLCKIN(1) DONE\n ");              
//  do
//  {
//    sendcmd(0x4E26); //4E25 change
//   Indata =  sendcmd(NUL);
//   
//  } while (Indata != 0x2E26); //CLCKIN(2)
////   printf("CLCKIN(2) DONE\n ");              
//  
//  do
//  {
//    sendcmd(0x4F0F);
//    Indata = sendcmd(NUL);
//   
//  } while (Indata != 0x2F0F); //(2) ADC ENABLED
//  //printf("ENABLED\n ");              
//                                   
//
//  do
//  {
//    sendcmd(WAKEUP);
//    get_data(response,trash);
//     
//  } while ((response[0]>>8) != WAKEUP); // WAKE UP
//   // printf("WOKEN UP\n ");                                
//        
//        
//    do
//       {   
//         trash[0] = LOCK<<8;
//         get_data(response,trash);
//         
//         trash[0] = NUL<<8;
//         get_data(response,trash);
//         
//        } while((response[0]>>8) != LOCK); //LOCK
//     //     printf("LOCKED\n ");              
//             
// trash[0] = 0x2500<<8;
// get_data(response,trash);     
// get_data(response,trash);
// get_data(response,trash);
// 
// trash[0] = NUL<<8;
// get_data(response,trash);
// 
//}

void ADC(void){
    unsigned int trash[5] = {0, 0,0,0,0};
    signed int reponse[5] = {0, 0,0,0,0};
    
      do
  {
    send(0x06550000);
    send(0x00000000);
    
  } while (SPI1BUF != 0x06550000); // Unlock
                                   
    do
  {
    send(0x00110000);
    send(0x00000000);
    
  } while (SPI1BUF != 0xff040000); // Reset
                                   
  
    do
  {
    send(0x06550000);
    send(0x00000000);
    
  } while (SPI1BUF != 0x06550000); // Unlock
                                   
 
  
  do
  {
    //EVM //send(0x4BE00000); 
    send(0x4B680000);
    send(0x00000000);
    
  } while (SPI1BUF != 0x2B680000); //(A_SYS_CFG) //6-E
                                   

  do
  {
    send(0x4C000000);
    send(0x00000000);
    
  } while (SPI1BUF != 0x2C000000); //(D_SYS_CFG)
                                   
  do
  {
    send(0x4D020000);
    send(0x00000000);
    
  } while (SPI1BUF != 0x2D020000); //(CLCKIN(1)
                                   
  do
  {
    send(0x4E260000); //4E25 change
    send(0x00000000);
   
  } while (SPI1BUF != 0x2E260000); //CLCKIN(2)

  
  do
  {
    send(0x4F0F0000);
    send(0x00000000);
   
  } while (SPI1BUF != 0x2F0F0000); //(2) ADC ENABLED
                                   
                                   

  do
  {
    send(0x00330000);
   get_data(reponse,trash);
     
  } while (reponse[0] != 0x00330000); // WAKE UP
                               
        
  do
  {   
    send (0x05550000);
    send (0x00000000);

  } while(SPI1BUF!=0x05550000); //LOCK

  int i;
  for( i= 0; i < 5 ; i++)
    reponse[0]=readADC(trash[i]);
                                  
}
void ADC_startup(void)
 {
 //INIT_CLCK( );
 INIT_ADC_SPI( );
 //INT_ADC( ); 
 ADC( );
 INIT_EXT_INT( );
}