/*
 * LedControll.h
 *
 *  Created on: 2014.03.29.
 *      Author: zoon
 */
//						This is a special file for DIY AVR Development board
#ifndef DEVBOARDLIBS_H_
#define DEVBOARDLIBS_H_
#define USART_BAUDRATE 9600
#define LED_PORT PORTD
#define LED_DDR DDRD
#define LED_PIN PD6
#define UBRR_ERTEK ((F_CPU / (USART_BAUDRATE * 16UL)) - 1) // UBRR
#ifndef __HAS_DELAY_CYCLES
#include <util/delay.h>
#endif

#ifndef _AVR_IO_H_
#include <avr/io.h>
#endif

void delay_ms(uint16_t delay)
{
	while (delay)
	{
		_delay_ms(1);
		delay--;

	}
}
void LedBlink(uint8_t NumberOfBlinks, uint16_t delay)
{
	LED_DDR |= (1 << LED_PIN);
	while (NumberOfBlinks)
	{
		LED_PORT |= (1 << LED_PIN);
		delay_ms(delay);
		LED_PORT &= ~(1<< LED_PIN);
		delay_ms(delay);
	}

}

void UARTConfig()  // UART beallitasa
{

	UBRR0L = UBRR_ERTEK; // UBRR_ERTEK also 8 bitjenek betoltese az UBRRL regiszterbe
	UBRR0H = (UBRR_ERTEK >> 8); // UBRR_ERTEK felso 8 bitjenek betoltese az UBRRH regiszterbe
	// UCSR0A |= (1<<U2X0);

	//Ado es Vevo aramkorok bekapcsolasa

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}
char UARTReceive()
{
	while (!(UCSR0A & (1 << RXC0))){} // Varakozas amig nincs uj bejovo adat
	return UDR0;
}

void UARTSendChar(char data) // Ez a fuggveny a kuldendo adatot beirja az UDR regiszter kimeno pufferjebe
{
	while (!(UCSR0A & (1 << UDRE0))){} // Varakozas amig az Ado kesz nem lesz az adatkuldesre
	UDR0 = data;
}

void UARTSendString(char * str)
{
	while (*str)
	{
		UARTSendChar(*str);
		str++;
	}
}
char UARTSendInt(int number)
{
		//hány jegyű a szám
	int oszto=1;
	char jegy_hossz=1,i;
	while((number+1) / oszto > 9 )

	    {
	    oszto=oszto*10;
	    jegy_hossz++;
	    }

 for(i=0;i<jegy_hossz;i++)
 {
	 UARTSendChar( ((number/ oszto)+48) );
	 number-= number / oszto * oszto;
	 oszto=oszto/10;

 }
return 0;
}
#endif /* LEDCONTROLL_H_ */
