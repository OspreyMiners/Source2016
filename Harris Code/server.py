import serial
import time
import sys
import socket

# constants

DUE_PORT = '/dev/ttyACM0'
DUE_BAUD_RATE = 9600
DUE_TIMEOUT = 2

SERIAL_END = b'\x00'

SERVER_IP = '192.168.1.80'
SERVER_PORT = 12000

COMMAND_SIZE = 3

# TCP server
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# ensure that the address can be reused incase we run the program more than once
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind((SERVER_IP, SERVER_PORT))
server.listen(1)

print "Successfully started TCP server"

# serial communication
due = serial.Serial()

due.port = DUE_PORT
due.baudRate = DUE_BAUD_RATE
due.timeout = DUE_TIMEOUT

due.open()

time.sleep(2)

if due.is_open:
	print "Successfully started serial communication with arduino"

# server loop

client, address = server.accept()

print "Successfully received client for TCP server"

while True:
	data = client.recv(16)
	if not data: break	
	if len(data) == COMMAND_SIZE:
		due.write(data)
		due.write(SERIAL_END)

# cleanup


# stop both motor channels
due.write(b'cl\x80')
due.write(b'cr\x80')

client.close()
due.close()
server.close()
