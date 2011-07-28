import serial
import sys
import threading
import time
import struct

SAMPLE_PERIOD = 1.0 / (8000000.0 / 64 / 63)
UNPACKER = struct.Struct(">HHH")

MA_LENGTH = 1000

class MovAvg:
    def __init__( self ):
        self.values = [0] * MA_LENGTH

    def update( self, N ):
        self.values.pop(0)
        self.values.append( N )

    def avg( self ):
        return float(sum(self.values)) / MA_LENGTH

    def max( self ):
        return max(self.values)

class Reader(threading.Thread):
    def __init__(self):
        threading.Thread.__init__( self )
        self.daemon = True

        self.voltages = MovAvg()
        self.currents = MovAvg()
        self.powers = MovAvg()
        self.last_read_lock = threading.Lock()
        self.last_read = ((0.0, 0.0, 0.0), (0.0, 0.0, 0.0), (0.0, 0.0, 0.0), 0.0)

    def getLastRead(self):
        with self.last_read_lock:
            return self.last_read

    def run(self):
        while True:
            s = serial.Serial( "/dev/ttyACM0" )
            try:
                s.write( 'a' )

                i = 0

                while True:
                    line = s.read(6)
                    voltage, current, checksum = UNPACKER.unpack(line)

                    if voltage ^ current != checksum:
                        print "Checksum fail"
                        s.read(1)
                        continue

                    if voltage > 10:
                        voltage = float(voltage) / 1024
                    else:
                        voltage = 0.0
                    self.voltages.update( voltage )

                    if current > 50:
                        current = float(current) / 1024
                    else:
                        current = 0.0
                    self.currents.update( current )

                    power = voltage * current
                    self.powers.update( power )
                    last_energy = self.last_read[3]
                    energy = last_energy + power * SAMPLE_PERIOD
                    self.last_read = (self.last_read[0], self.last_read[1], self.last_read[2], energy)

                    i += 1
                    if i % 50 == 0:
                        with self.last_read_lock:
                            self.last_read = ((voltage, self.voltages.avg(), self.voltages.max()),                                           (current, self.currents.avg(), self.currents.max()),
                                            (power, self.powers.avg(), self.powers.max()),
                                            energy)
            finally:
                s.close()


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
    while True:
        time.sleep(1)
