/* ************************************************************************** */
 // Header containing print functions
/* ************************************************************************** */

#ifndef _UART_PRINT_H    /* Guard against multiple inclusion */
#define _UART_PRINT_H




#define clrscr()  printf("\033[2J")    
#define sys_clk      64000000

/* -----UART parameters-- ------------------ -= = */
void SendChar(char c);
void SendString(char *string);
char ReadChar(void);
void ReadString(char *string, int length);
void ReadUART(char * message, int maxLength);


int buffer_full() ;
int buffer_empty();
void SendDataBuffer(const char *buffer, unsigned int size);
void DMA_send(char *data_byte);

/* -----UART INIT parameters-- ------------------ -= = */
void INIT_UART(void);
void INIT_DMA(unsigned int BUFF_SIZE, char buffer[BUFF_SIZE] );
void  End_transfer(void);




#endif /* _EXAMPLE_FILE_NAME_H */

