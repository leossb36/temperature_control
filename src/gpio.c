/*
  Control Intensity of LED using PWM on Raspberry pi
  http://www.electronicwings.com
 */

#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>
#include <softPwm.h>  /* include header file for software PWM */
#include <unistd.h>

#define DEVICE_FAN 5
#define DEVICE_RESISTOR 4

void handle_device(int PWM_pin, int intensity){

	wiringPiSetup();		/* initialize wiringPi setup */
	pinMode(PWM_pin,OUTPUT);	/* set GPIO as output */
	softPwmCreate(PWM_pin,1,100);	/* set PWM channel along with range*/

	softPwmWrite(PWM_pin, intensity);
	usleep(10000);
}


void using_fan(int intensity) {
	handle_device(DEVICE_FAN, intensity);
}

void using_resistor(int intensity) {
	handle_device(DEVICE_RESISTOR, intensity);
}