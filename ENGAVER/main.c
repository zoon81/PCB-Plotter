#include "DevBoard_lib.h"
#include "StepperControlling.c"

#include <stdlib.h>
#include </usr/avr/include/string.h>

#define LINE_BUFFER_LENGTH 512
#define steepdelay 3
#define StepDelay 0
#define steepdelayZ 50
#define Xmin 0
#define Xmax 40
#define Ymin 0
#define Ymax 40
#define Zmin 0
#define Zmax 1

// Should be right for DVD steppers, but is not too important here
const int stepsPerRevolution = 20;

/* Structures, global variables    */
struct point
{
    float x;
    float y;
    float z;
};
// Current position of plothead
struct point actuatorPos;

//  Drawing settings, should be OK
float StepInc = 1;
int LineDelay = 50;
int penDelay = 50;

// Motor steps to go 1 millimeter.
// Use test sketch to go 100 steps. Measure the length of line.
// Calculate steps per mm. Enter here.
float StepsPerMillimeterX = 6.0;
float StepsPerMillimeterY = 6.0;

// Drawing robot limits, in mm
// OK to start with. Could go up to 50 mm if calibrated well.

float Xpos = 0;
float Ypos = 0;
float Zpos = 0;
extern void fowardB(int16_t steep, int8_t delay2);
extern void backwardB(int16_t steep, int8_t delay2);
extern void fowardA(int16_t steep, int8_t delay2);
extern void backwardA(int16_t steep, int8_t delay2);
extern void upZ(int8_t delay2);
extern void downZ(int8_t delay2);
extern void moveXY(int16_t yxseep,int8_t ydir,int8_t xdir,int8_t delay2);
extern void STEEPERCONTROLLING_INIT();

void drawLine(float x1, float y1);
void processIncomingLine( char* line, int charNB );
char* find_char(char *line , char c);

uint8_t main(){
    char line[ LINE_BUFFER_LENGTH ];
    char c;
    int lineIndex;
    uint8_t lineIsComment, lineSemiColon;

    lineIndex = 0;
    lineSemiColon = 0;
    lineIsComment = 0;

    UARTConfig();
    STEEPERCONTROLLING_INIT();
    UARTSendString( "\n\rWaiting for commands:");

    while (1){

        c = UARTReceive();

            if(( c == '\n') || (c == '\r') )               // End of line reached
            {
                if( lineIndex > 0 )                          // Line is complete. Then execute!
                {
                    line[ lineIndex ] = '\0';                   // Terminate string
#ifdef verbose
                        UARTSendString( "\n\rReceived : ");
                        UARTSendString( line );
#endif // verbose

                    processIncomingLine( line, lineIndex );
                    lineIndex = 0;
                }
                else
                {
                    // Empty or comment line. Skip block.
                }
                lineIsComment = 0;
                lineSemiColon = 0;
                UARTSendString("\n\rok");
                UARTSendString( "\n\rWaiting for commands:");
            }
            else
            {
                if ( (lineIsComment) || (lineSemiColon) )     // Throw away all comment characters
                {
                    if ( c == ')' )
                        lineIsComment = 0;     // End of comment. Resume line.
                }
                else
                {
                    if ( c <= ' ' )                   // Throw away whitepace and control characters
                    {
                    }
                    else if ( c == '/' )                      // Block delete not supported. Ignore character.
                    {
                    }
                    else if ( c == '(' )                      // Enable comments flag and ignore all characters until ')' or EOL.
                    {
                        lineIsComment = 1;
                    }
                    else if ( c == ';' )
                    {
                        lineSemiColon = 1;
                    }
                    else if ( lineIndex >= LINE_BUFFER_LENGTH-1 )
                    {
                        UARTSendString( "\n\rERROR - lineBuffer overflow" );
                        lineIsComment = 0;
                        lineSemiColon = 0;
                    }
                    else if ( c >= 'a' && c <= 'z' )          // Upcase lowercase
                    {
                        line[ lineIndex++ ] = c-'a'+'A';
                    }
                    else
                    {
                        line[ lineIndex++ ] = c;
                    }
                }
            }
    }
    return 0;
}

void processIncomingLine( char* line, int charNB ){
    int currentIndex = 0;
    char buffer[ 64 ];                                 // Hope that 64 is enough for 1 parameter
    struct point newPos;
    char *indexY , *indexX;

    newPos.x = 0.0;
    newPos.y = 0.0;

    //  Needs to interpret
    //  G1 for moving
    //  G4 P300 (wait 150ms)
    //  G1 X60 Y30
    //  G1 X30 Y50
    //  M300 S30 (pen down)
    //  M300 S50 (pen up)
    //  Discard anything with a (
    //  Discard any other command!

    while( currentIndex < charNB )
    {
        switch ( line[ currentIndex++ ] )                // Select command, if any
        {
        case 'U':
            upZ(steepdelayZ);
            break;
        case 'D':
            downZ(steepdelayZ);
            break;
        case 'G':
            buffer[0] = line[ currentIndex++ ];          // /!\ Dirty - Only works with 2 digit commands @OPTIMALIZING
            //      buffer[1] = line[ currentIndex++ ];
            //      buffer[2] = '\0';
            buffer[1] = '\0';

            switch ( atoi( buffer ) )                    // Select G command || line[currentIndex]+48
            {
            case 0:                                // G00 & G01 - Movement or fast movement. Same here
            case 1:
                // /!\ Dirty - Suppose that X is before Y
                indexX = find_char( line, 'X' );  // Get X/Y position in the string (if any)
                indexY = find_char( line, 'Y' );
                if ( indexY <= 0 )
                {
                    newPos.x = atof( indexX + 1);
                    newPos.y = actuatorPos.y;
                    UARTSendString("\n\rGET_---X:");
                    UARTSendString(indexX);
                }
                else if ( indexX <= 0 )
                {
                    newPos.y = atof( indexY + 1);
                    newPos.x = actuatorPos.x;
                    UARTSendString("\n\rGET_Y");
                }
                else
                {
#ifdef verbose
                    UARTSendString("\n\rPOS_in_BUFFER_XY--X:");
                    UARTSendString(indexX);
                    UARTSendString("--Y:");
                    UARTSendString(indexY);
#endif // verbose
                    newPos.y = atof( indexY + 1);
                    indexY = '\0';
                    newPos.x = atof( indexX + 1);


                }
                drawLine(newPos.x, newPos.y );
                //        Serial.println("ok");
                actuatorPos.x = newPos.x;
                actuatorPos.y = newPos.y;
                break;
            }
            break;
        case 'M':
            buffer[0] = line[ currentIndex++ ];        // /!\ Dirty - Only works with 3 digit commands
            buffer[1] = line[ currentIndex++ ];
            buffer[2] = line[ currentIndex++ ];
            buffer[3] = '\0';
            switch ( atoi( buffer ) )
            {
            case 300:
            {
                char* indexS = strchr( line+currentIndex, 'S' );
                float Spos = atof( indexS + 1);
                //          Serial.println("ok");
                if (Spos == 30)
                {
                    downZ(steepdelayZ);
                }
                if (Spos == 50)
                {
                    upZ(steepdelayZ);
                }
                break;
            }
            case 114:                                // M114 - Repport position
                UARTSendString( "\n\rAbsolute position : X = " );
                UARTSendFloat( actuatorPos.x, 2);
                UARTSendString( "  -  Y = " );
                UARTSendFloat( actuatorPos.y, 2);
                break;
            default:
                UARTSendString( "\n\rCommand not recognized : M");
                UARTSendString( buffer );
            }
        }
    }



}


/*********************************
 * Draw a line from (x0;y0) to (x1;y1).
 * Bresenham algo from https://www.marginallyclever.com/blog/2013/08/how-to-build-an-2-axis-arduino-cnc-gcode-interpreter/
 * int (x1;y1) : Starting coordinates
 * int (x2;y2) : Ending coordinates
 **********************************/
void drawLine(float x1, float y1){

#ifdef verbose
        UARTSendString("\n\rDrawline:fx1, fy1: ");
        UARTSendFloat(x1,2);
        UARTSendString(",");
        UARTSendFloat(y1,2);
#endif // verbose

    //  Bring instructions within limits
    if (x1 >= Xmax)
    {
        x1 = Xmax;
    }
    if (x1 <= Xmin)
    {
        x1 = Xmin;
    }
    if (y1 >= Ymax)
    {
        y1 = Ymax;
    }
    if (y1 <= Ymin)
    {
        y1 = Ymin;
    }

 #ifdef verbose
        UARTSendString("\n\rXpos, Ypos: ");
        UARTSendFloat(Xpos,2);
        UARTSendString(",");
        UARTSendFloat(Ypos,2);
        UARTSendString("\n\rx1, y1: ");
        UARTSendFloat(x1,2);
        UARTSendString(",");
        UARTSendFloat(y1,2);
#endif // verbose

    //  Convert coordinates to steps
    x1 = (int)(x1*StepsPerMillimeterX);
    y1 = (int)(y1*StepsPerMillimeterY);
    float x0 = Xpos;
    float y0 = Ypos;

    //  Let's find out the change for the coordinates
    long dx = abs(x1-x0);
    long dy = abs(y1-y0);
    //int sx = x0<x1 ? StepInc : -StepInc;
    //int sy = y0<y1 ? StepInc : -StepInc;

    long i;
    long over = 0;

    if (dx > dy)
    {
        for (i=0; i<dx; ++i)
        {
            x0<x1 ? fowardA(1,steepdelay) : backwardA(1,steepdelay);
            over+=dy;
            if (over>=dx)
            {
                over-=dx;
                y0<y1 ? fowardB(1,steepdelay) : backwardB(1,steepdelay);
            }
            delay_ms(StepDelay);
        }
    }
    else
    {
        for (i=0; i<dy; ++i)
        {
            y0<y1 ? fowardB(1,steepdelay) : backwardB(1,steepdelay);
            over+=dx;
            if (over>=dy)
            {
                over-=dy;
                x0<x1 ? fowardA(1,steepdelay) : backwardA(1,steepdelay);
            }
            delay_ms(StepDelay);
        }
    }
#ifdef verbose

        UARTSendString("\n\rdx, dy:");
        UARTSendFloat(dx,2);
        UARTSendString(",");
        UARTSendFloat(dy,2);
        UARTSendString("\n\rGoing to (");
        UARTSendFloat(x0,2);
        UARTSendString(",");
        UARTSendFloat(y0,2);
#endif // verbose

    //Relasing the motor coils set outputs to low
    MotorX = 0;
    MororY = 0;
    MotorZ = 0;
    //  Delay before any next lines are submitted
    delay_ms(LineDelay);
    //  Update the positions
    Xpos = x1;
    Ypos = y1;
}
char* find_char(char *line , char c){
    while(line++){
        if( *line == c){
            return line;
        }
    }
    return 0;
}
