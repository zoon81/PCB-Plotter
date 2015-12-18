import serial
import time

ser = serial.Serial()
ser.port = "/dev/ttyUSB0"
ser.xonxoff = False
ser.rtscts = False
ser.dsrdtr = False
ser.baudrate = 19200
ser.timeout = None
ser.open()
var = 1
#first send delay data
delayXY = 1
delayZ =  10
ser.flushInput()
ser.flushOutput()

print( 'Waiting for HELLO:' + str(ser.readline()) )
ser.write( bytes( (str(delayXY).zfill(3)) , "ASCII" ) )
ser.write( bytes( (str(delayZ).zfill(3))  ,  "ASCII"))

fi = open('nc', 'r')
for f in fi:
	line = f.strip()
	print('Sending:' +line)
	print( 'Received:' + str(ser.readline()) )
	ser.write( bytes(line +'\n' , "ASCII"))
	while (var == 1):
		serial_line = ser.readline()
		print( str(serial_line) ) # If using Python 2.x use: print serial_line
		if(b'ok' in serial_line):
			break

print( ser.readline() )
ser.close()
