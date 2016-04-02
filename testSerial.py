import serial, sys, time

#replace the string
arduino = serial.Serial('/dev/cu.usbmodem1411')
arduino.baudrate = 115200

#give some time for the serial communication to finish setting up 
time.sleep(1)

#send some data over
arduino.write("Start Communication".encode())

count = 1
while True:
	line = arduino.readline()
	#have to use sys becaus print appends a \n
	sys.stdout.write(line)
	print count
	count = count + 1