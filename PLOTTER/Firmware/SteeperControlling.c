/*
 * SteeperControlling.h
 *
 *  Created on: 2014.07.09.
 *      Author: zoon
 */
#ifndef STEEPERCONTROLLING_H_
#define STEEPERCONTROLLING_H_
#ifndef _AVR_IO_H_
#include <avr/io.h>
#endif

//MotorC PORTC 0-3 Z axis
//MorotB PORTD 4-7 Y axis
//MotorA PORTD 0-3 X axis

#define MotorX PORTB
#define MororY PORTB
#define MotorZ PORTC

extern void delay_ms(uint16_t delay);

int8_t stepsA[8] = { 2, 3, 1, 5, 4, 12, 8, 10 };
int8_t stepsB[8] = { 32, 48, 16, 80, 64, 192, 128, 160 };
int8_t stepsC[4] = { 3, 6, 12, 9 };
int8_t pozA = 0, pozB = 0, pozC = 0;

void fowardA(int16_t steep, int8_t delay2) {
	int16_t  i;
	for (i = 0; i < steep; i++) {
		if (pozA > 7){
			pozA = 0;
		}

		MotorX = stepsA[pozA];
		pozA++;
		delay_ms(delay2);
	}

}

void fowardB(int16_t steep, int8_t delay2) {
	int16_t i;
	for (i = 0; i < steep; i++) {
		if (pozB > 7)
			pozB = 0;
		MororY = stepsB[pozB];
		pozB++;
		delay_ms(delay2);
	}

}

void backwardA(int16_t steep, int8_t delay2) {
	int16_t i;
	for (i = 0; i < steep; i++) {
		if (pozA < 0)
			pozA = 7;
		MotorX = stepsA[pozA];
		pozA--;
		delay_ms(delay2);
	}

}

void backwardB(int16_t steep, int8_t delay2) {
	int16_t i;
	for (i = 0; i < steep; i++) {
		if (pozB < 0)
			pozB = 7;
		MororY = stepsB[pozB];
		pozB--;
		delay_ms(delay2);
	}
	}

void downZ(int8_t delay2) {
	int8_t i;
	for (i = 0; i < 70; i++) {
		if (pozC > 3)
			pozC = 0;
		MotorZ = stepsC[pozC];
		pozC++;
		delay_ms(delay2);
	}
}

void upZ(int8_t delay2) {
	int8_t i;
	for (i = 0; i < 70; i++) {
		if (pozC < 0)
			pozC = 3;
		MotorZ = stepsC[pozC];
		pozC--;
		delay_ms(delay2);
	}
}
void moveXY(int16_t yxseep,int8_t ydir,int8_t xdir,int8_t delay2)
{
	int16_t i;
	for(i=0;i<yxseep;i++)
	{
		MotorX = stepsA[pozA] + stepsB[pozB];
		if(ydir == 1)
		{
			if(pozA > 6)
				pozA = 0;
			else
				pozA++;
		}
		if(ydir == 0)
		{
			if(pozA < 1)
				pozA = 7;
			else
				pozA--;
		}
		if(xdir == 1)
				{
					if(pozB > 6)
						pozB = 0;
					else
						pozB++;
				}
		if(xdir == 0)
				{
					if(pozB < 1)
						pozB = 7;
					else
						pozB--;
				}
	delay_ms(delay2);
	}

}
#endif /* STEEPERCONTROLLING_H_ */
