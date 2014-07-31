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
ser.write( str(delayXY).zfill(3) )
ser.write( str(delayZ).zfill(3) )
print ser.readline();

for f in fi:
	if f[0] in['X']:
		xcoor = int( round(float(f[1:7]) / 28.93700787 ))
		#print xcoor
		ser.write( (str(xcoor).zfill(3)) )
		ycoor = int( round(float(f[8:14]) / 28.93700787 ))
		#print ycoor
		ser.write( (str(ycoor).zfill(3)) )
		zcoor = f[16]
		#print zcoor
		ser.write( str(zcoor) )
		print ((str(float(f[1:7]) / 28.93700787 ))+' '+ (str(float(f[8:14]) / 28.93700787 )))
		print ((str(xcoor).zfill(3)) +' '+ (str(ycoor).zfill(3)) +' '+ str(zcoor))
		print("MCU Sent:")
		print ser.readline();
		print("MCU Sent:")
		print ser.readline();


ser.write( (str(0).zfill(3)) )
ser.write( (str(0).zfill(3)) )
ser.write( str(2) )
print("MCU Sent:")
print ser.readline();
ser.close()
