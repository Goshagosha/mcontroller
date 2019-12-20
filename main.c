/* 
 *	Basis
 *	2009 Benjamin Reh und Joachim Schleicher
 */
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "servo.h"

const int RED = 4;
const int GREEN = 3;
const int BLUE = 2;
const int DETECTOR = 5;
const int COMMS = 0;

const int detector_delay = 100;

void init();

void tell(int sign) {
	while (1) {
		for (int i = 0; i < sign/10; i++) {
			lights(COMMS,1);
			_delay_ms(333);
			lights(COMMS,0);
			_delay_ms(333);	
		}
		_delay_ms(1000);
		for (int i = 0; i < sign%10; i++) {
			lights(COMMS,1);
			_delay_ms(333);
			lights(COMMS,0);
			_delay_ms(333);	
		}
		_delay_ms(3000);	
	}
}

void lights (int pin, char val)
{
	if (val) {
		PORTC |= (1<<pin);
	} else {
		PORTC &= ~(1<<pin);
	};
}

uint8_t detect_light (int pin) {
	//darkness seems to give 110 as a result
	//232 seems to be close to max
	lights(RED, 0);
	lights(BLUE, 0);
	lights(GREEN,0);
	lights(pin, 1);
	_delay_ms(detector_delay);
	PORTC |= (1<<DETECTOR);
	uint8_t result = getADCValue(DETECTOR);
	lights(pin, 0);
	return result;
}

int main(void)
{
	//Init stuff
	init();
	ADCInit(0);
	DDRC |= (1<<RED);
	DDRC |= (1<<GREEN);
	DDRC |= (1<<BLUE);
	DDRC |= (1<<COMMS);
	DDRC &= ~(1<<DETECTOR);
	
	uint8_t red_res = detect_light(RED);
	uint8_t blue_res = detect_light(BLUE);
	uint8_t green_res = detect_light(GREEN);
	
	if (red_res > green_res && red_res > blue_res) lights(RED, 1);
	else if (green_res > red_res && green_res > blue_res) lights(GREEN, 1);
	else if(blue_res > red_res && blue_res > green_res) lights(BLUE, 1);
	
		
	while (1)
	{
		
		
	}	
}


//INIT
void init()
{
	uartInit();   // serielle Ausgabe an PC
	ADCInit(0);   // Analoge Werte einlesen
	PWMInit();    // Pulsweite auf D6 ausgeben 
	timerInit();  // "Systemzeit" initialisieren
	//servoInit();  // Servoansteuerung initialisieren
}
