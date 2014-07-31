#define F_CPU 8000000L
#include "DevBoard_lib.h"
#include <stdlib.h>
#define steepdelay 3
#define zdelay 50

int16_t xcoordinate, ycoordinate, xnow = 0, ynow = 0;
int8_t zdir,xdir,ydir, znow = 2, delayXY, delayZ;

extern int fowardB(int16_t steep, int8_t delay2);
extern int backwardB(int16_t steep, int8_t delay2);
extern int fowardA(int16_t steep, int8_t delay2);
extern int backwardA(int16_t steep, int8_t delay2);
extern int upZ(int8_t delay2);
extern int downZ(int8_t delay2);
extern int moveXY(int16_t yxseep,int8_t ydir,int8_t xdir,int8_t delay2);
void getcoordinates() {

	xcoordinate = (UARTReceive() - 48) * 100;
	xcoordinate += (UARTReceive() - 48) * 10;
	xcoordinate += (UARTReceive() - 48);

	ycoordinate = (UARTReceive() - 48) * 100;
	ycoordinate += (UARTReceive() - 48) * 10;
	ycoordinate += (UARTReceive() - 48);

	zdir = UARTReceive() - 48;

	UARTSendInt(xcoordinate);
	UARTSendString(" ");
	UARTSendInt(ycoordinate);
	UARTSendString(" ");
	UARTSendChar(zdir + 48);
	UARTSendString("\n\r");
}
void getdelays() {
	delayXY = (UARTReceive() - 48) * 100;
	delayXY += (UARTReceive() - 48) * 10;
	delayXY += (UARTReceive() - 48);

	delayZ = (UARTReceive() - 48) * 100;
	delayZ += (UARTReceive() - 48) * 10;
	delayZ += (UARTReceive() - 48);
}
int main() {

	DDRB = 255;
	DDRC = 15;
	PORTB = 0;
	PORTC = 0;
	UARTConfig();

	UARTSendString("Hallo\n\r");
	getdelays();
	UARTSendString("DELAY: ");
	UARTSendInt(delayXY);
	UARTSendString(" ");
	UARTSendInt(delayZ);
	UARTSendString("\n\r");
	while (1) {
		PORTB = 0;
		PORTC = 0;
		getcoordinates();

		if (zdir != znow) {
			if (zdir == 2) {
				upZ(delayZ);
				//UARTSendString("Z going up\n\r");

			}
			if (zdir == 1) {
				downZ(delayZ);
				//UARTSendString("Z going down\n\r");
			}
		}
		//TEST MODE
		if (zdir == 3) {
			while (1) {
				downZ(delayZ);
				_delay_ms(500);
				upZ(delayZ);
				_delay_ms(500);
			}
		} else {
			//UARTSendString("Z not move\n\r");

		}
		if ( abs( abs(xnow-xcoordinate) - abs(ynow-ycoordinate) ) < 2 ) { //some correction because the when we rounded we lost some details

			if(xnow > xcoordinate)
				xdir = 0;
			else
				xdir = 1;

			if(ynow > ycoordinate)
				ydir = 0;
			else
				ydir = 1;

		/* DEBUG MESSANGE
		 * 	UARTSendString("I going to :");
			UARTSendInt(abs(ynow-ycoordinate));
			UARTSendString(" ");
			UARTSendInt(ydir);
			UARTSendString(" ");
			UARTSendInt(xdir);
			UARTSendString("\n\r");
		 */
			moveXY(abs(ynow-ycoordinate),xdir,ydir,delayXY);

		}
		else {
			if (xnow < xcoordinate) {
				fowardA(xcoordinate - xnow, delayXY);
				//UARTSendString("X going foward\n\r");
			}
			else {
				backwardA(xnow - xcoordinate, delayXY);
				//UARTSendString("X going backward\n\r");
			}

			if (ynow < ycoordinate) {
				fowardB(ycoordinate - ynow, delayXY);
				//UARTSendString("Y going foward\n\r");
			}
			else {
				backwardB(ynow - ycoordinate, delayXY);
				//UARTSendString("Y going backward\n\r");

			}

			//DEBUG MESSANGE
			//UARTSendString("\n\r");
		}

		ynow = ycoordinate;
		xnow = xcoordinate;
		znow = zdir;

		UARTSendString("OK\n\r");
	}

	PORTC = 0;
	PORTB = 0;
	endloop(100);

}

