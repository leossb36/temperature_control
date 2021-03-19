/*
  Control Intensity of LED using PWM on Raspberry pi
  http://www.electronicwings.com
 */

#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>
#include <softPwm.h>  /* include header file for software PWM */

void handle_device(int PWM_pin, int intensity){

	wiringPiSetup();		/* initialize wiringPi setup */
	pinMode(PWM_pin,OUTPUT);	/* set GPIO as output */
	softPwmCreate(PWM_pin,1,100);	/* set PWM channel along with range*/

	softPwmWrite (PWM_pin, intensity);
	usleep(10000);
}


void using_fan(int intensity) {
	handle_device(5, intensity);
}

void using_resistor(int intensity) {
	handle_device(4, intensity);
}