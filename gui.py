#!/usr/bin/env python

import sys
import cairo
import rsvg
import gtk
import gobject
import math

import show

BORDER_WIDTH = 10

OFFSET = math.radians( 138 )
SCALE = math.radians( 360 - 96 )
angle = 0
win = None
img = None
reader = None
WIDTH=0
HEIGHT=0

def delete_cb(win, event):
    gtk.main_quit()

def expose_cairo(win, event):
    cr = win.window.cairo_create()
    cr.set_source_surface( img, 0, 0 )
    cr.paint()

    cr.set_source_rgb( 255, 255, 255 )
    cr.set_line_width( 5 )
    centre_x, centre_y = WIDTH / 2, HEIGHT / 2
    cr.move_to( centre_x, centre_y )
    
    LENGTH = 80
    cr.line_to( centre_x + math.cos( angle ) * LENGTH, centre_y + math.sin( angle ) * LENGTH ) 
    cr.stroke()

    return True

def timeout():
    global angle, reader

    voltage = float(reader.getLastRead()[0]) / 1024
    print voltage
    angle = OFFSET + voltage * SCALE

    expose_cairo( win, None)
    return True

def main():
    global win, img, WIDTH, HEIGHT
    win = gtk.Window ()
    win.connect("delete-event", delete_cb)

    svg = None
    if (len (sys.argv) > 1):
        svg = rsvg.Handle(file=sys.argv[1])
    else:
        raise SystemExit("need svg file")

    WIDTH, HEIGHT = svg.props.width, svg.props.height
    win.resize( WIDTH, HEIGHT )
    img = cairo.ImageSurface( cairo.FORMAT_ARGB32, svg.props.width, svg.props.height )
    cr = cairo.Context( img )
    svg.render_cairo(cr)

    win.connect("expose-event", expose_cairo)
    
    win.show_all()
    win.connect("destroy", lambda w: gtk.main_quit())
    gobject.timeout_add( 50, timeout )
    gtk.main()

if __name__ == '__main__':
    reader = show.Reader()
    reader.start()
    main()
