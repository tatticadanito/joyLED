#include <wiringPi.h>
#include "myJoystick.h"

#define NPIN 30

int main(void)
{
	int ledPin[NPIN];
	int i, fd, result;
	struct js_event jse;

	wiringPiSetup();

	for(i=0; i<30; i++) {
		ledPin[i] = i;			//Array for LEDs
		pinMode(ledPin[i], OUTPUT);	//Set all pins to OUTPUT
	}

        fd = open_joystick("/dev/input/js0");	//Check Joystick
        if(fd == -1) return -1;

	for(i=0; i<18; i++)	read_event(fd, &jse);	//Ignore garbage inputs

	while(1) {
		result = read_event(fd, &jse);
		usleep(1000);
		printf("NUMBER: %d\tVALUE: %d\n", jse.number, jse.value);
		if(result == 1 && is_good_button(&jse)) {	//Turn ON/OFF LEDs buttons
			switch(jse.value) {
				case 1:	//ON
					if(jse.number < 7)	digitalWrite(ledPin[jse.number], HIGH);
					else	digitalWrite(ledPin[jse.number+1], HIGH);
					break;
				case 0:	//OFF
					if(jse.number < 7)	digitalWrite(ledPin[jse.number], LOW);
					else	digitalWrite(ledPin[jse.number+1], LOW);
					break;
			}
		}
		else if(result == 1 && is_good_analog(&jse)) {	//Turn ON/OFF LEDs D-PAD
			switch(jse.value) {
				case 32767:	//ON
					if(jse.number == 4)	digitalWrite(ledPin[9], HIGH);
					else if(jse.number == 5)	digitalWrite(ledPin[10], HIGH);
					break;
				case -32767:	//ON
					if(jse.number == 4)	digitalWrite(ledPin[11], HIGH);
					else if(jse.number == 5)	digitalWrite(ledPin[12], HIGH);
					break;
				case 0:	//OFF
					if(jse.number == 4) {
						digitalWrite(ledPin[9], LOW);
						digitalWrite(ledPin[11], LOW);
					}
					else if(jse.number == 5) {
						digitalWrite(ledPin[10], LOW);
						digitalWrite(ledPin[12], LOW);
					}
					break;
			}
		}
	}

	return 0;
}
