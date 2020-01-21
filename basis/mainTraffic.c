#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "servo.h"

#define delay 10000

//2LEDs ON PC5,4, 3LEDs on PC3,2,1 and switch on PC0

//INIT
void init()
{
	//uartInit();   // serielle Ausgabe an PC
	//ADCInit(0);   // Analoge Werte einlesen
	//PWMInit();    // Pulsweite auf D6 ausgeben 
	timerInit();  // "Systemzeit" initialisieren
	//servoInit();  // Servoansteuerung initialisieren


}

int main(void)
{
	init();

	//2LED Light
	enum PedL {GREEN, RED} PedLight;


	//3LED Light
	enum TraL {TRGREEN,TRGREENYELLOW, TRRED, TRREDYELLOW} TrafLight;

	//Switch
	enum SwitchState {ON, OFF} Switch;

	PedLight = RED;
	TrafLight = GREEN;
	Switch = OFF;

	//setting register 5 to output (For LED)
	DDRC |= (1<<5); //RED
	//setting register 4 to output (For LED2)
	DDRC |= (1<<4); //YELLOW
	//setting register 3 to output (For LED3)
	DDRC |= (1<<3); //GREEN

	//setting register 1 to output (For PED LED)
	DDRC |= (1<<1); //GREEN PED
	//setting register 2 to output (For PED LED2)
	DDRC |= (1<<2); //RED PED

	//setting register 0 to input (for Switch)
	DDRC &= ~(1<<0);
	//Activate Pullup resistor (for switch)
	PORTC |= (1<<0);
	uint8_t oldc = 0;
	uint32_t next_ms_timer = getMsTimer()+delay;

 	while (1)
	{
		uint8_t sw = ! (PINC & (1<<0));
		if (sw > oldc)
		{
			Switch = ON;
			oldc = sw;
		}
		else
		{
			Switch = OFF;
			oldc = sw;
		}
		
		switch(Switch)
		{

			case ON:
				if (TrafLight != TRGREEN)
				{
					Switch = OFF;
				}	
				break;
			case OFF:
				break;
		}

		if (getMsTimer()>next_ms_timer || (Switch == ON))
		{
			//stateMachine
			switch(TrafLight)
			{
				case TRRED:
					PedLight = RED;
					TrafLight = TRREDYELLOW;
					next_ms_timer += delay/4;
					break;
				case TRGREEN:
					TrafLight = TRGREENYELLOW;
					next_ms_timer += delay/4;
					break;
				case TRGREENYELLOW:
					PedLight = GREEN;
					TrafLight = TRRED;
					next_ms_timer += delay;
					break;
				case TRREDYELLOW:
					TrafLight = TRGREEN;
					next_ms_timer += delay;
					break;
			}
		}
		
		//switch pins ON&OFF
		switch(TrafLight)
		{
			case TRRED:
				PORTC |= (1<<5); //REDON
				PORTC &= ~(1<<4); //YELLOWOFF
				PORTC &= ~(1<<3); //GREENOFF
				break;
			case TRGREEN:
				PORTC &= ~(1<<5); //REDOFF
				PORTC &= ~(1<<4); //YELLOWOFF
				PORTC |= (1<<3); //GREENON
				break;
			case TRGREENYELLOW:
				PORTC &= ~(1<<5); //REDOFF
				PORTC |= (1<<4); //YELLOWON
				PORTC &= ~(1<<3); //GREENOFF
				break;
			case TRREDYELLOW:
				PORTC &= ~(1<<5); //REDOFF
				PORTC |= (1<<4); //YELLOWON
				PORTC &= ~(1<<3); //GREENOFF
				break;
		}
		switch(PedLight)
		{
			case RED:
				PORTC |= (1<<2); //REDON
				PORTC &= ~(1<<1); //GREENOFF
				break;
			case GREEN:
				PORTC &= ~(1<<2); //REDOFF
				PORTC |= (1<<1); //GREENON
				break;
		}


	//NOT BLOCKED
	}	
}
