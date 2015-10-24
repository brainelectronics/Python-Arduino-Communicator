"""
's1D12' to set digital Pin D12 to 1 (HIGH)
's0D9' to set digital Pin D9 to 0 (LOW)
's217D3' to set digital Pin D3 to 217 (PWM)
'gD1' to get the value of digital Pin D1
'gA0' to get the value of analog Pin A0
'#d0' to switch serial debug output off
'#f1' to switch serial feedback output off
"""
import serial
import time

port = serial.Serial("/dev/tty.usbmodem1411", 19200, timeout=3.0)
port.write("s127d13\n")

#port.write("s0D13\n")
#lineRead = str(port.readline())#[2:-6]
#print ("complete line: " + lineRead)	# read everything important to the monitor