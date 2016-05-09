###	Dependencies	###
import datetime
import glob
import re
import select
import serial
import socket
import sys
import threading
import time
from time import sleep
from Queue import Queue, Empty
from threading import Thread
###	Custom Depend	###
#import ThreadedServer
###########################
###	FUNCTIONS START	###
###	Thread Lock	###
logAllFileName = datetime.datetime.now().strftime("%d-%m-%Y-%Ilog.csv");
''' - parseCommand
	Takes in the Serial Port and the command
Forwards the Command as long as it is not something
the Server has to handle
'''
def parseCommand(serialPort,command):
	if (command and (not command.isspace())):
		if command[0] == 'O':
			print "Shutting Off"
			return False
		else:
			appendToFile(logAllFileName , command)
			serialPort.write(command + '\n')
	return True
''' - parseFeedback
	Takes In Text to log the data.
It writes to the files by spawning threads.
Reccomended to spawn thread to complete this task.
'''
def parseFeedback(textVal):
	commands = textVal.replace('\n','')	
	if(len(commands) > 0):
		# Commands looping list
		while(len(commands) > 0):
			# Goes through all the commands
			cmd = commands
			if(re.match("Output:(.*)",cmd)):
				val = re.match("Output:(.*)",cmd)
				appendToFile("output.csv", val.group(1))
				return
			elif (re.match("Failure:(.*)",cmd)):
				val = re.match("Failure:(.*)",cmd)
				appendToFile("failure.csv", val.group(1))
				return
			elif (re.match("S([+-])(\d{0,2}.\d{0,2})",cmd)):
				val = re.match("S([+-])(\d{0,2}.\d{0,2})",cmd)
				if(val.group(1)=='-'):
					appendToFile("conveyor.csv", val.group(2))
				elif(val.group(1)=='+'):
					appendToFile("digger.csv", val.group(2))
				return
			elif (re.match("E([123]{1})([-+]\d{1,4})",cmd)):
				val = re.match("E([123]{1})([-+]\d{1,4})",cmd)
				if(val.group(1) == '1'):
					appendToFile("encLeft.csv", val.group(2))
				elif(val.group(1) == '2'):
					appendToFile("encRight.csv", val.group(2))
				elif(val.group(1) == '3'):
					appendToFile("encRack.csv", val.group(2))
				return
			else:
				return
	return
''' - appendToFile
	Takes in file name and the input text.
Using the name it opens file then appends
the input text with a timestamp of function call.
'''
def appendToFile(filename, text):
	f = open(filename, 'a')
	now = datetime.datetime.now().strftime("%F,%X,%f")
	f.write(now + ", '" + text + "'\n")
	f.close()
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
	if(len(result) == 0):
		print "Error no Serial Port Detected"
                sys.exit()
	return result[0]
###	FUNCTIONS END	###
###	MAIN START	###
# Global Variables
programOn = True
#Server Port
serverIP = 'localhost' # port address
serverPort = 7012 # port number
serverBuffer = 24 # bytes
#Create Listening Port
serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serverAddress = (serverIP, serverPort)
serverSocket.bind(serverAddress)
serverSocket.setblocking(1)
serverSocket.listen(2)
#Serial Port Configured Microcontroller(Arduino) to Host
serialPort = getSerialPortname()
serialBuffer = 64 #bytes
BaudRate = 9600
#Non-Blocking Serial Port Configured
microController = serial.Serial(serialPort,BaudRate,timeout=0.05)
###	I/O Streams	###
inputStreams = [serverSocket,microController,sys.stdin]
###	Control Loop	###
while programOn:
	(inStream,outStream,errorStream) = select.select(inputStreams, [], [])
	threads = []
	for dataS in inStream:
		if dataS == serverSocket:
			commands = serverSocket.recv(serverBuffer).split()
                        for cmd in commands:
				if not parseCommand(microController,cmd):
					programOn = False
		elif dataS == sys.stdin:
			commands = sys.stdin.readline().split()
			for cmd in commands:
				if not parseCommand(microController,cmd):
					programOn = False
		elif dataS == microController:
			data = microController.read(serialBuffer).replace('\n','').replace('\r','')
	#		print data
			parseFeedback(data)
			microController.flushInput()
###	MAIN END	###
