import serial
import time
import sys

print "Arg0:",sys.argv[1]
fi = open(sys.argv[1], 'r')
ser = serial.Serial()
ser.port = "/dev/ttyUSB0"
ser.xonxoff = False
ser.rtscts = False
ser.dsrdtr = False
ser.baudrate = 9600
ser.timeout = None
ser.open()
#get "hallo"
print ser.readline(); 
#first send delay data
delayXY = 2
delayZ =  5
counter = 1
ser.write( str(delayXY).zfill(3) )
ser.write( str(delayZ).zfill(3) )
print ser.readline();

for f in fi:
	if f[0] in['X']:
#parsing gerber file		
		xcoor = int( round(float(f[1:f.index('Y')]) / 28.93700787 ))
		if f[f.index('Y')+1] in['-']:
			ycoor = int( round(float(f[(f.index('Y')+2):f.index('D')]) / 28.93700787 ))
		else:
			ycoor = 0
		zcoor = f[f.index('D')+2]
		print counter,". ",f
		if f[f.index('Y')+1] in['-']:
			print ((str(float(f[1:f.index('Y')]) / 28.93700787 ))+' '+ (str(float(f[(f.index('Y')+2):f.index('D')]) / 28.93700787 )))
		else:
			print ((str(float(f[1:f.index('Y')]) / 28.93700787 ))+' '+ (str(float(f[(f.index('Y')+1):f.index('D')]) / 28.93700787 )))
		print ((str(xcoor).zfill(3)) +' '+ (str(ycoor).zfill(3)) +' '+ str(zcoor))
		
#sending data 
		ser.write( (str(xcoor).zfill(3)) )
		ser.write( (str(ycoor).zfill(3)) )
		ser.write( str(zcoor) )
#get feedback
		print("[Plotter] I go to:",ser.readline() )
		print("[Plotter] Sent:",ser.readline() )
		counter = counter + 1
		if(zcoor in['0']):
			var = raw_input("Press enter to continue!")


ser.write( (str(0).zfill(3)) )
ser.write( (str(0).zfill(3)) )
ser.write( str(2) )
print("MCU Sent:")
print ser.readline();
ser.close()
