import serial
import sys

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

    print voltage, current
