import serial
import sys
import threading
import time

class Reader(threading.Thread):
    def __init__(self):
        threading.Thread.__init__( self )
        self.daemon = True

        self.last_read_lock = threading.Lock()
        self.last_read = (0, 0)

    def getLastRead(self):
        with self.last_read_lock:
            return self.last_read

    def run(self):
        s = serial.Serial( "/dev/ttyACM0" )
        s.write( 'a' )

        while True:
            line = s.read(13)
            if not line.endswith( "\r\n" ):
                s.read(1)
                continue

            try:
                voltage = int(line[0:3], 16)
                current = int(line[4:7], 16)
                checksum = int(line[8:11], 16)
            except:
                continue

            if voltage ^ current != checksum:
                print "Checksum fail"
                continue
            
            with self.last_read_lock:
                self.last_read = (voltage, current)

class Printer(threading.Thread):
    def __init__(self, source):
        threading.Thread.__init__( self )
        self.daemon = True
        self.source = source

    def run(self):
        while True:
            print self.source.getLastRead()
            time.sleep(1)

if __name__ == "__main__":
    r = Reader()
    p = Printer(r)
    r.start()
    p.start()
    time.sleep(10)
