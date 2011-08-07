#!/usr/bin/env python

import sys
import cairo
import rsvg
import gtk
import gobject
import math

import show

OFFSET = math.radians( 138 )
SCALE = math.radians( 360 - 96 )

POWER_LENGTH = 130
POWER_X = 455 + 57
POWER_Y = 313
POWER_TXT_X = 460
POWER_TXT_Y = 380
POWER_TXT_SIZE = 60

VOLTAGE_LENGTH = 80
VOLTAGE_X = 135 + 57
VOLTAGE_Y = 633
VOLTAGE_TXT_X = 155
VOLTAGE_TXT_Y = 688
VOLTAGE_TXT_SIZE = 40

CURRENT_LENGTH = 80
CURRENT_X = 774 + 57
CURRENT_Y = 633
CURRENT_TXT_X = 800
CURRENT_TXT_Y = 688
CURRENT_TXT_SIZE = 40

ENERGY_X = 310 + 57
ENERGY_Y = 648
ENERGY_HEIGHT = 90
ENERGY_WIDTH = 270
ENERGY_TXT_X = 320 + 57
ENERGY_TXT_Y = 735
ENERGY_TXT_SIZE = 60

BACKGROUND = "bg.svg"

class App:
    def __init__( self, reader ):
        self.win = gtk.Window ()

        svg = rsvg.Handle( file = BACKGROUND )
        self.width, self.height = svg.props.width, svg.props.height

        self.bg = cairo.ImageSurface( cairo.FORMAT_ARGB32, svg.props.width, svg.props.height )
        cr = cairo.Context( self.bg )
        cr.set_source_rgb( 0, 0, 0 )
        cr.rectangle( 0, 0, self.width, self.height )
        cr.fill()
        svg.render_cairo(cr)

        self.win.resize( self.width, self.height )
        self.win.connect( "expose-event", self.expose_cairo )
        self.win.connect( "delete-event", self.quit )
        self.win.connect( "destroy", self.quit)
        self.win.show_all()

        self.voltage = (0.0, 0.0, 0.0)
        self.current = (0.0, 0.0, 0.0)
        self.power = (0.0, 0.0, 0.0)
        self.energy = 0.0

        gobject.timeout_add( 50, self.timeout, reader )
        gtk.main()

    def quit(self, *args):
        gtk.main_quit()

    def drawLine( self, cr, x, y, length, value, max, color ):
        cr.set_source_rgb( *color )
        cr.move_to( x, y )

        if value > max:
            value = max

        angle = OFFSET + (value/max) * SCALE
        cr.line_to( x + math.cos( angle ) * length, y + math.sin( angle ) * length ) 
        cr.stroke()

    def drawPower( self, cr ):
        self.drawLine( cr, POWER_X, POWER_Y, POWER_LENGTH, self.power[0], 180, ( 255, 255, 255 ) )
        self.drawLine( cr, POWER_X, POWER_Y, POWER_LENGTH, self.power[1], 180, ( 255, 0, 0) )
        self.drawLine( cr, POWER_X, POWER_Y, POWER_LENGTH, self.power[2], 180, ( 0, 0, 255) )

        cr.set_source_rgb( 255, 0, 0 )
        cr.move_to( POWER_TXT_X, POWER_TXT_Y )
        cr.set_font_size(POWER_TXT_SIZE)
        cr.show_text( "%3.1f W" % self.power[1] )

    def drawCurrent( self, cr ):
        self.drawLine( cr, CURRENT_X, CURRENT_Y, CURRENT_LENGTH, self.current[0], 18, ( 255, 255, 255 ) )
        self.drawLine( cr, CURRENT_X, CURRENT_Y, CURRENT_LENGTH, self.current[1], 18, ( 255, 0, 0) )
        self.drawLine( cr, CURRENT_X, CURRENT_Y, CURRENT_LENGTH, self.current[2], 18, ( 0, 0, 255 ) )

        cr.set_source_rgb( 255, 0, 0 )
        cr.move_to( CURRENT_TXT_X, CURRENT_TXT_Y )
        cr.set_font_size(CURRENT_TXT_SIZE)
        cr.show_text( "%.1f A" % self.current[1] )

    def drawVoltage( self, cr ):
        self.drawLine( cr, VOLTAGE_X, VOLTAGE_Y, VOLTAGE_LENGTH, self.voltage[0], 18, ( 255, 255, 255 ) )
        self.drawLine( cr, VOLTAGE_X, VOLTAGE_Y, VOLTAGE_LENGTH, self.voltage[1], 18, ( 255, 0, 0 ) )
        self.drawLine( cr, VOLTAGE_X, VOLTAGE_Y, VOLTAGE_LENGTH, self.voltage[2], 18, ( 0, 0, 255 ) )

        cr.set_source_rgb( 255, 0, 0 )
        cr.move_to( VOLTAGE_TXT_X, VOLTAGE_TXT_Y )
        cr.set_font_size(VOLTAGE_TXT_SIZE)
        cr.show_text( "%.1f V" % self.voltage[1] )

    def drawEnergy( self, cr ):
        cr.set_source_rgb( 0, 255, 0 )
        proportion = self.energy / 50000
        cr.rectangle( ENERGY_X, ENERGY_Y, ENERGY_WIDTH * proportion, ENERGY_HEIGHT )
        cr.fill()

        cr.set_source_rgb( 255, 0, 0 )
        cr.move_to( ENERGY_TXT_X, ENERGY_TXT_Y )
        cr.set_font_size(ENERGY_TXT_SIZE)
        cr.show_text( "%5.0f J" % self.energy )

    def expose_cairo(self, win, event):
        cr = win.window.cairo_create()
        cr.set_source_surface( self.bg, 0, 0 )
        cr.paint()

        cr.select_font_face("Machine BT")
        cr.set_line_width( 5 )

        self.drawVoltage( cr )
        self.drawCurrent( cr )
        self.drawPower( cr )
        self.drawEnergy( cr )

        return True

    def timeout(self, reader):
        self.voltage, self.current, self.power, self.energy = reader.getLastRead()
        self.expose_cairo( self.win, None)

        return True

if __name__ == '__main__':
    reader = show.Reader()
    reader.start()

    class R:
        def __init__(self):
            self.i = float(0)

        def getLastRead(self):
            self.i += 0.01
            if self.i >= 1:
                self.i = 0
            return (self.i * 1024, 0)

    #reader = R()
    app = App( reader )
