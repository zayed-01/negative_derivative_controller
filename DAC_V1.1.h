/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H




#define _SUPPRESS_PLIB_WARNING 
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#include <plib.h>
#include <stdbool.h>
    
    
#define DAC_CS  PORTBbits.RB7
    
    
void INIT_DACSPI (void);
void Init_Timer3(void);
void DAC_WRITE(unsigned short chnA,unsigned short chnB);

    

#endif /* _EXAMPLE_FILE_NAME_H */
