#ifndef __LOWPASSFILTER_H
#define __LOWPASSFILTER_H

#include <dsplib_dsp.h>

#define pi 3.1415
#define cut_off_f 800 //Cutoff ferqeucny of the 1st order filter 
#define window_size  3 //Its the average length or number of data point
#define B  2           //2 Biquad Sections

/*********************************************************
1st order Low Pass filter: Defining a exponential type  low pass filter
*********************************************************/
typedef struct {
  float prevoutput, Fc ;
} low_pass_f;


/*********************************************************
Moving average filter: Defining a movning average variable
*********************************************************/
typedef struct {
  int sum;
  int data_history[window_size];
  int window_count ;
} moving_avg_def ;



/*********************************************************
Function prototype for 1sr order low pass filter
*********************************************************/
float low_pass_filter(low_pass_f *p,float input );
void init_low_pass(low_pass_f *p) ;


/*********************************************************
Function prototype for movning averge filter
********************************************************/
extern void init_moving_average(moving_avg_def *avg_filter );
extern int32 moving_average_filter(moving_avg_def *avg_filter , int ADC_value);

/*********************************************************
Function prototype for 4th order biquad filter
********************************************************/
void iir_coefficients_setup(void);



#endif