
#include "Uart_Print_V1.1.h"

#define _SUPPRESS_PLIB_WARNING 
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>
#include <stdbool.h> 

#define PB_DIVISOR  (1 << OSCCONbits.PBDIV) // read the peripheral bus divider, FPBDIV
#define PB_FREQ     (sys_clk / PB_DIVISOR)     //PB_DIVISOR // periperhal bus frequency
#define BAUDRATE     250000 // must match PC terminal emulator setting //57600
    

//unsigned int count_send = 0, k = 0, count = 0;
//extern unsigned int NSAMPLES;



void SendChar(char c)
{
  U2STAbits.UTXEN = 1; // Make sure transmitter is enabled
  // while(CTS)                       // Optional CTS use
  while (U2STAbits.UTXBF)
    ;          // Wait while buffer is full
  U2TXREG = c; // Transmit character
}

void SendString(char *string)
{
  U2STAbits.UTXEN = 1; // Make sure transmitter is enabled

  while (*string != '\0')
  {
    while (U2STAbits.UTXBF)
      ;                // Wait while buffer is full
    U2TXREG = *string; // Transmit one character
    string++;
    // Go to next character in string
  }
}

char ReadChar(void)
{
  //PORTDbits.RD15 = 0;                // Optional RTS use
  while (!U2STAbits.URXDA)
    ; // Wait for information to be received
  //PORTDbits.RD15 = 1;
  return U2RXREG; // Return received character
}

void ReadUART(char * message, int maxLength) {
    char data = 0;
    int complete = 0, num_bytes = 0;
    // loop until you get a ?\r? or ?\n?
    while (!complete) {
        if (U2STAbits.URXDA) { // if data is available
            data = U2RXREG; // read the data
            if ((data == '\n') || (data == '\r')) {
                complete = 1;
            } else {
                message[num_bytes] = data;
                ++num_bytes;
               // roll over if the array is too small
               if (num_bytes >= maxLength) {
                num_bytes = 0;
               }
             }
            }
           }
// end the string
message[num_bytes] = '\0';
}


void ReadString(char *string, int length)
{
  int count = length;

  do
  {
    *string = ReadChar(); // Read in character
    SendChar(*string);    // Echo character

    if (*string == 0x7F && count > length) // Backspace conditional
    {
      length++;
      string--;
      continue;
    }

    if (*string == '\r') // End reading if enter is pressed
      break;

    string++;
    length--;

  } while (length > 1);

  *string = '\0'; // Add null terminator
}

void SendDataBuffer(const char *buffer, unsigned int size)
{
    while(size)
    {
        while(!UARTTransmitterIsReady(UART2))
            ;
        UARTSendDataByte(UART2, *buffer);
        buffer++;
        size--;
    }

    while(!UARTTransmissionHasCompleted(UART2))
        ;
}
 
void DMA_send(char *data_byte)
{
 while(!UARTTransmitterIsReady(UART2))
             ;

 UARTSendDataByte(UART2, *data_byte);
        
 DmaChnEnable(DMA_CHANNEL1);
 while(!UARTTransmissionHasCompleted(UART2))
                    ;
                }

void INIT_UART(void)
{

  // #ifdef use_uart_serial
  // The RX pin must be one of the Group 2 input pins
  // RPA1, RPB1, RPB5, RPB8, RPB11
    
 PPSInput (2,U2RX,RPB5); //Assign U2RX to pin RPB5 --
  
  // The TX pin must be one of the Group 4 output pins
  // RPA3, RPB0, RPB9, RPB10, RPB14
  PPSOutput(4,RPB10,U2TX); //Assign U2TX to pin RPB10 --

  UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
 // UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
  UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
  UARTSetDataRate(UART2, PB_FREQ, BAUDRATE);
  UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
  // Feel free to comment this out
  //clrscr();

}

void INIT_DMA(unsigned int BUFF_SIZE, char buffer[BUFF_SIZE] )
  {
      // === set up DMA for UART output =========
  // configure the channel and enable end-on-match
   DmaChnOpen(DMA_CHANNEL1, DMA_CHN_PRI2, DMA_OPEN_MATCH);
  // trigger a byte everytime the UART is empty
    DmaChnSetEventControl(DMA_CHANNEL1, DMA_EV_START_IRQ_EN | DMA_EV_MATCH_EN | DMA_EV_START_IRQ(_UART2_TX_IRQ));
  // source and destination
     DmaChnSetTxfer(DMA_CHANNEL1, buffer, (void *)&U2TXREG, BUFF_SIZE, 1, 1);
  //  DmaChnSetTxfer(DMA_CHANNEL1, FIFO[max_count], (void *)&U2TXREG, max_count, 1, 1);
   
  // signal when done
   DmaChnSetEvEnableFlags(DMA_CHANNEL1, DMA_EV_BLOCK_DONE);
  // set null as ending character (of a string)
    DmaChnSetMatchPattern(DMA_CHANNEL1, '\n');
                   
                    }

void  End_transfer(void)
{
CloseTimer3();
CloseINT1();


}

/***********CIRCULAR BUFFER FUNCTIONS**************/
//int buffer_empty( ) 
//{ // return true if the buffer is empty (read = write)
//   
//    return (count_send == k);
//}
//
//int buffer_full( )
//{ // return true if the buffer is full.
//    
//return (((count_send + 1) % NSAMPLES) == k);
//}





/****************************************************/
/*void CIRCULAR_BUFFER(signed int Avg)   
  {    
    if( (!buffer_full()) && (print))
    { // if the buffer is full, a number is printed - 170
      // if a sweep is done, a number is printed - 85
       
        FIFO[count_send] = Avg;   
        ++count_send;   
//        if (count_send == 400)
//         {     
//            print = 1;
//           // count_send = 0;
//              }  
         if (count_send >= max_count)
         {     
            count_send = 0;
                    }                   
            }
    else
    {       
        FIFO[count_send] = 0x55;         
            }
                }           */

/********************************************************/
