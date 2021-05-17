/* ************************************************************************** */
/* Date : 4/30/2021
 Negative derivative controller
 ndf.h
@Summary:
Describe all the necessary variable and structure definition  for the 
negative derivative controller
 */
//this is for 300 Hz
/* ************************************************************************** */

#ifndef _NDF_CONTROLLER_H
#define _NDF_CONTROLLER_H

#define Vmax 5
#define Vmin -5
#define Sample_time .0001

#define multplyint16(a,b) ((int16)((((long)(a))*((long)(b)))>>14)) //multiply two fixed 2.14
#define float2int16(a) ((int16)((a)*16384.0)) // 2^14
#define int16_2float16(a) ((float)(a)/16384.0)

typedef struct _resonant_controller {
 double Kd; // NDF controller gain
 double Kp ; 
 double T ;
 int voltage_max;
 int voltage_min;
 double velocity_history[2];
 double controller_history[2] ;
 double voltage_out ; 
 double prev_meas ;
 double control_out; 
 double smooth_velocity;
} resonant_controller ;


void Init_NDF(void);
int NDF_controller (resonant_controller *ndf, int measured_value);


#endif