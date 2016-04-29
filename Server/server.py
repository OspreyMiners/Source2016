import datetime
import glob
import serial
import socket
import sys
import time
from time import sleep
from Queue import Queue, Empty
from threading import Thread
# Functions
''' - parseCommand
	Takes in the Serial Port and the command
Forwards the Command as long as it is not something
the Server has to handle
'''
def parseCommand(serialPort,command):
	if (command and (not command.isspace())):
		if command[0] == 'T':
			print "Test"
		else:
			serialPort.write(command + '\n')
	return
''' - appendToFile
	Takes in file name and the input text.
Using the name it opens file then appends
the input text with a timestamp of function call.
'''
def appendToFile(filename, text):
	with open(filename, "a") as updateFile:
		now = datetime.datetime.now().strftime("%F,%X,%f")
		updateFile.write(now + ", '" + text + "'\n")
	return
def getSerialPortname():
	if sys.platform.startswith('win'):
		ports = ['COM%s' % (i + 1) for i in range(256)]
	elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
		# this excludes your current terminal "/dev/tty"
		ports = glob.glob('/dev/tty[A-Za-z]*')
	elif sys.platform.startswith('darwin'):
		ports = glob.glob('/dev/tty.*')
	else:
		raise EnvironmentError('Unsupported platform')
	result = []
	for port in ports:
		try:
			s = serial.Serial(port)
			s.close()
			result.append(port)
		except (OSError, serial.SerialException):
			pass
	return result[0]

# Global Variables
#Server Port
serverIP = 'localhost' # port address
serverPort = 7012 # port number
serverBuffer = 1024 # bytes
#Serial Port Configured Microcontroller(Arduino) to Host
serialPort = getSerialPortname()
BaudRate = 9600
microController = serial.Serial(serialPort,BaudRate)
# Test Grabage
parseCommand(microController, 'Testing')
parseCommand(microController, 'C+')
sleep(1)
parseCommand(microController, 'WR')
parseCommand(microController, 'C-')
sleep(1)
parseCommand(microController, 'WF')
parseCommand(microController, 'C+')
input("hit enter to quit")
##Taken Input from commandPrompt
#epoll = select.epoll()
#epoll.register(p.stdout.fileno(), select.EPOLLIN)
