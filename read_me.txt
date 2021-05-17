Test 1-7 is for NDF controller. THere is approximately 50% reduction in the amplitude. 
No






NDF is working a band pass filter after 100-300 hz output reamin as input. Doesnt change anything. 
decreases after  


Get the Phase lag with only dervative controller : due to systeem and other effect : Control force and input is almost same phase. But in resonance there will 90 degree phase shift

Apply a low pass filter on the derivate part and check the system output witht the phase lag : No phase shift



then apply DAC voltage and tune the gain : DAC voltage applied in two diferent configration Negative and positive feedback has been formed. Test 7 is for positive feedback 




Test 001  (done):  Channel A is the top electrode B is the bottom electrode==>
Based on the ADC  value i am applying the DAC value
DAC_WRITE(2047-out_data,2047+out_data); when ADC> 0


Test 002 (done) :  when ADC > 0 ; 
DAC_WRITE(2047+out_data,2047-out_data);
It cretes a positive feedback 
 
Test 003 : Form a negative feedback system and test at differenty frequency ( tested at 200  and 300 Hz ) for a band rage of 100-300 Hz

Test 004 : Test with velocity as a condition for applying DAc voltage

Test 005 : Frequency response of the of the the controller : Get the data out ==>>

Test 006 : Add Feedthrough 

Test 007 : test the controller outside of the band pass range

**Same step would go for other controller. Same thing just different gain

PID, PD, P, D + compensator and different gain tutning mechanism





**Open Loop data with velocity 


-- When to apply DAC value position or velocity?? 

Case 1 : Position value  position ( done and tested ) Test 1-7 has been performed based on the 
Case 2:  change direction based on velocity ( it can be performed on the resonance. There is a chance
or ask for it
In resonance u might need to design the a phase compensator. 
Thats the only way



****Velocity would have good chance to work in resonance. Generally position and velocity has a 90 phase change
velocity leads position data by 90 degree. Again before resonance positon and force is in phase
there is loss of energy ( thinking swing ) and again at resonance force and veloiry in phase wehreas position and
force is 90 degree phase. Velocity leads and force leads with that. Basically my signal would be delayed ( the positon signal would delaying) if i consider the changing the point to be the veloity which is already leading
. it can help me to predict and compensate for the large error.


