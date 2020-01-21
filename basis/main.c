#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "servo.h"

//INIT
void init()
{
	uartInit();   // serielle Ausgabe an PC
	//ADCInit(0);   // Analoge Werte einlesen
	//PWMInit();    // Pulsweite auf D6 ausgeben 
	timerInit();  // "Systemzeit" initialisieren
	//servoInit();  // Servoansteuerung initialisieren


}

int main(void)
{
	init();
	//uartInit()
	
	uart_putc("H");
 	while (1)
	{
		char c = uart_getc();
		uart_putc(c);
		uart_putc(c);
		uart_putc(c);
	}	
}
