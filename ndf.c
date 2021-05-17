#include "ndf.h"
#include<math.h>
#include "Lpfilter.h"
extern low_pass_f filter_1;

int NDF_controller ( resonant_controller *ndf, int measured_value){
    int dac_out;
    double measured_voltage , velocity ;
    
measured_voltage =  (double)measured_value/pow(2,23)*2.442 ;
 velocity = (double)(measured_voltage - ndf->prev_meas)/.0001 ;
 ndf->smooth_velocity = low_pass_filter(&filter_1, velocity);
 
//printf("%f \t\n",velocity) ;
//ndf->voltage_out =   (double)(ndf->K1)*.0917*velocity - ndf->K1*.0917*ndf->velocity_history[2] + 
//         1.905*ndf->controller_history[1] - 0.9633*ndf->controller_history[2] ;


//ndf->voltage_out = 0.08544*velocity -0.08544*ndf->velocity_history[1]+ 1.797*ndf->controller_history[0]- 0.829*ndf->controller_history[1] ;
ndf->voltage_out = 0.08544*ndf->smooth_velocity -0.08544*ndf->velocity_history[1]+ 1.797*ndf->controller_history[0]- 0.829*ndf->controller_history[1] ;
 


ndf->velocity_history[1] = ndf->velocity_history[0] ;
ndf->velocity_history[0] =  ndf->smooth_velocity ;
ndf->controller_history[1] = ndf->controller_history[0] ;
ndf->controller_history[0]=  ndf->voltage_out ;
ndf->prev_meas = measured_voltage;

ndf->control_out = (ndf->Kp*measured_voltage) + (ndf->Kd*ndf->voltage_out)   ; //with the Kp turn on i would say it as a PR controller

//if (ndf->voltage_out > ndf->voltage_max){
//		ndf->voltage_out = ndf->voltage_max;
//	}else if (ndf->voltage_out < ndf->voltage_min){
//		ndf->voltage_out = ndf->voltage_min ;	
//	}
	
dac_out =(int)((ndf->control_out*2047)/5);

//printf("%d \t\n",dac_out) ;
 return dac_out ; 
}