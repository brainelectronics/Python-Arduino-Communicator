#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# ----------------------------------------------------------------------------
# Port Scanner
# Copyright (c) 2015 brainelectronics.de
# Scharpf, Jonas
#
# All rights reserved.
#
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# ----------------------------------------------------------------------------

import serial
import platform
from serial.tools import list_ports
import threading
import time

class myThread (threading.Thread):
    myThreadFlag = 0
    def __init__(self, functionToCall, name="", *args):
        threading.Thread.__init__(self)
        self.name = name
        self.functionToCall = functionToCall
        self.functionArguments = args
    def run(self):
        if self.name is not "":
            print "Starting " + self.name
        time.sleep(1)
        #newCom.print_working()
        #newCom.find_port(9600, 1)
        self.functionToCall(*self.functionArguments)
        if self.name is not "":
            print self.name + " done"
        

class Communication(object):
    """docstring for Communication"""
    def __init__(self):
        self.truePorts = []

    def print_working(self):
        while not workingThread.myThreadFlag:
            print("."),
            time.sleep(1)
        if workingThread.myThreadFlag:
            newCom.printPorts()

    def find_port(self, baud, timeout):
        if platform.system() == 'Windows':
            ports = enumerate_serial_ports()
        elif platform.system() == 'Darwin':
            ports = [i[0] for i in list_ports.comports()]
        else:
            ports = glob.glob("/dev/ttyUSB*") + glob.glob("/dev/ttyACM*") + glob.glob('/dev/ttyS*')

        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                self.truePorts.append(port)
            except (OSError, serial.SerialException):
                pass
        
        #print(self.truePorts)
        workingThread.myThreadFlag = 1

    def printPorts(self):
        print(self.truePorts)

if __name__=='__main__':
    newCom = Communication()
    # Create new threads
    workingThread = myThread(newCom.print_working)
    scanningThread = myThread(newCom.find_port, "Scan", 9600, 1)

    # Start new Threads
    workingThread.start()
    scanningThread.start()

    #print(threading.enumerate())
