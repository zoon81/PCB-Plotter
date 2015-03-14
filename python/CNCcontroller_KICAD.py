import serial
import time

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
fi = open('nc', 'r')
#first send delay data
delayXY = 2
delayZ =  3
counter = 0
ser.write( str(delayXY).zfill(3) )
ser.write( str(delayZ).zfill(3) )
print ser.readline();

for f in fi:
	if f[0] in['X']:		
		xcoor = int( round(float(f[1:f.index('Y')]) / 28.93700787 ))
		#print f.index('Y')
		#print f[1:f.index('Y')]		
		#print xcoor
		ser.write( (str(xcoor).zfill(3)) )		
		#print f.index('D')
		#print f[f.index('Y')+1:f.index('D')]
		if f[f.index('Y')+1] in['-']:
			ycoor = int( round(float(f[(f.index('Y')+2):f.index('D')]) / 28.93700787 ))
		else:
			ycoor = 0
		
		#print ycoor
		ser.write( (str(ycoor).zfill(3)) )
		zcoor = f[f.index('D')+2]
		#print zcoor
		ser.write( str(zcoor) )
		if f[f.index('Y')+1] in['-']:
			print ((str(float(f[1:f.index('Y')]) / 28.93700787 ))+' '+ (str(float(f[(f.index('Y')+2):f.index('D')]) / 28.93700787 )))
		else:
			print ((str(float(f[1:f.index('Y')]) / 28.93700787 ))+' '+ (str(float(f[(f.index('Y')+1):f.index('D')]) / 28.93700787 )))
		print ((str(xcoor).zfill(3)) +' '+ (str(ycoor).zfill(3)) +' '+ str(zcoor))
		print("[Plotter] I go to:")
		print ser.readline();
		print("[Plotter] Sent:")
		print ser.readline();


ser.write( (str(0).zfill(3)) )
ser.write( (str(0).zfill(3)) )
ser.write( str(2) )
print("MCU Sent:")
print ser.readline();
ser.close()
