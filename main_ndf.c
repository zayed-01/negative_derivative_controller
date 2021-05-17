#include "Piconfig_V1.1.h"
 

char elect_type = 0;
volatile signed int ADC_Data, Acc_Data ;
int out_data1 , output;

unsigned int count_send = 0, k = 0, count = 0;
char showme;
bool buffer_control = true;
bool print_control = false;
double float_out;

unsigned short out_data;

//****************FUNC PROTOTYPE************************//
void save_data();
void CIRCULAR_BUFFER(signed int Avg1,signed int Avg2) ;
//void CIRCULAR_BUFFER(signed int Avg1, float Avg2); //for sending the out the float values
//void CIRCULAR_BUFFER(signed int Avg1, float Avg2);

 resonant_controller ndf_controller = {0.0001, 0.08, Sample_time, Vmax, Vmin} ;
low_pass_f filter_1 = {0, cut_off_f};
moving_avg_def moving_average;

void __ISR(_TIMER_3_VECTOR, IPL2AUTO) Timer3Handler(void)
{
 mT3ClearIntFlag();
 output = (int)NDF_controller(&ndf_controller,ADC_Data);
 out_data = (unsigned short)(abs(output)) ;
 //float_out = (double)NDF_controller(&ndf_controller,ADC_Data);

 
 elect_type = 'U';
   
 if (ADC_Data> 0){ //going towards bottom electrode 
      //chn A is top electrode. Write the higher value to the channel B
       DAC_WRITE(2047-out_data,2047+out_data);
      // DAC_WRITE(0,0);
        //DAC_WRITE(2047+out_data,2047-out_data); //positive feedback
        elect_type = 'U';
    }
    else {
     elect_type = 'L';
     DAC_WRITE(2047+out_data,2047-out_data); // (2147,1947)
    // DAC_WRITE(0,0);
     //DAC_WRITE(2047-out_data,2047+out_data);
    }
     


         if (buffer_control){
      // CIRCULAR_BUFFER(ADC_Data,(signed int)output);
         CIRCULAR_BUFFER(ADC_Data,output);
       // CIRCULAR_BUFFER(output,ndf_controller.smooth_velocity);
     }
    
    TOGGLE_LED( ); 
}  


void __ISR(_EXTERNAL_1_VECTOR, IPL3AUTO) INT1_ISR(void)
{
  mINT1ClearIntFlag();
  
signed int  packets[5] ={0,0,0,0,0};
get_data(packets,cmd_dummy); 
  
IN_data.resp = packets[0] >> 8 ;
IN_data.chan1 = packets[1] >> 8 ; //
IN_data.DAC_chan = packets[2] >> 8  ;       //D2
IN_data.Vout_chan = packets[3] >> 8 ;  //D3
IN_data.bias_chan = packets[4] >> 8 ;

 ADC_Data3 = IN_data.Vout_chan;   //D3
 ADC_Data2 = IN_data.DAC_chan; //D2
          
//ADC_Data = moving_average_filter(&moving_average, ADC_Data2);
 ADC_Data =  ADC_Data2;
 Acc_Data = ADC_Data3;
out_data1 = ADC_Data >> 13 ; 

  // TOGGLE_LED( );
                                } 


void main(void)
{ 
  SYSTEMConfig(sys_clock, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
  ANSELA = 0; //make sure analog is cleared
  ANSELB = 0;    
 

  
  
  int i;
  for ( i = 0; i < max_count; i++){
    FIFO[i] = 0x000000;
    Test[i] = 0x000000;
}      



INIT_UART();
INIT_LED();
INIT_DACSPI( );

DAC_WRITE(0,0);
Init_Timer3();
ADC_startup( );   

init_low_pass(&filter_1);
init_moving_average(&moving_average);
        while (1)
{        
  
         do{
        ;
    }while(!print_control); 
         
            save_data(); 
       
}

}

/*********Function Definition***********************/


void save_data(){
    for(sent = 0; sent < NSAMPLES; ++sent){
        //sprintf(buffer,"%f\t%f\t%f\n",((double)(sent+1)*0.0002),((double)(FIFO[k]*4.884)/pow(2,24)), ((double)Test[k]*4.884)/pow(2,24));  //((double)Test[k]*10)/pow(2,12));
        sprintf(buffer,"%f\t%f\t%d\n",((double)(sent+1)*0.0002),((double)(FIFO[sent]*4.884)/pow(2,24)), Test[sent]); 
       // sprintf(buffer,"%f\t%f\t%f\n",((double)(sent+1)*0.0002), FIFO[sent], Test[sent]); 
        //sprintf(buffer,"%.3f\t%d\t%.3f\n",((double)(sent+1)*0.0002), FIFO[sent],Test[sent]);
        while(!UARTTransmitterIsReady(UART2));
              SendString(buffer);                                    
        FIFO[sent] = 0x00;
        Test[sent] = 0x00;      
    }
   count_send =0 ;  
   print_control = false;
   buffer_control = true;

}




////For printing Float
//void CIRCULAR_BUFFER(signed int Avg1, float Avg2){  //signed short Avg2
//
//        FIFO[count_send] = Avg1;   
//        Test[count_send] = Avg2;   
//
//        ++count_send;   
//                                       
//    if (count_send >= NSAMPLES){
//        buffer_control = false; // cannot collect more data
//        print_control = true;
//        
//    }
//}


//////for integers
void CIRCULAR_BUFFER(signed int Avg1, signed int Avg2){  //signed short Avg2

        FIFO[count_send] = Avg1;   
        Test[count_send] = Avg2;   

        ++count_send;   
                                       
    if (count_send >= NSAMPLES){
        buffer_control = false; // cannot collect more data
        print_control = true;
        
    }
}
