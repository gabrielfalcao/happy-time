"""
Copyright (C) 2007 Lincoln de Sousa <lincoln@archlinux-br.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.
"""
import barcode

class BarBackend(object):
    def __init__(self, bc):
        self.bc = bc

    def render(self):
        raise NotImplementedError

    def __str__(self):
        raise NotImplementedError


class SvgBackend(BarBackend):
    def start(self):
        self.bc.height = self.bc.height or 100
        self.bc.width = self.bc.width or 104

        l = ['<?xml version="1.0" standalone="no"?>']
        l.append('<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.0//EN" '
                 '"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd">')
        l.append('<svg width="%i" height="%i">' % (self.bc.width,
            self.bc.height))
        l.append('<title>%s</title>' % self.bc.ascii)
        l.append('<desc>')
        l.append('\t%s:\t%s' % (self.bc.encoding, self.bc.ascii))
        l.append('\tGNU-barcode:\t%s' % barcode.BARCODE_VERSION);
        l.append('\tbarcode:\thttp://www.gnu.org')
        l.append('</desc>\n');
        return '\n'.join(l)

    def bars(self):
        height = 70
        is_bar = 0
        xpos = 0

        p = self.bc.partial
        total = len(p)
        index = 0
        
        l = ['<g fill="black">']
        while index < total:
            current = ord(p[index]) - 48
            if current > 9: # Guide bar
                height = 75
                index += 1
                current = ord(p[index]) - 48
                index += 1 # Skip the following 'a'
            else:
                height = 70

            if is_bar:
                l.append('\t<rect x="%i" y="0" height="%i" '
                         'width="%i" stroke-width="0"/>' %
                         (xpos, height, current))
                is_bar = 0
            else:
                is_bar = 1

            index += 1
            xpos += current

        l.append('</g>')

        # text...
        prev_x = 0
        correction = 0

        l.append('<g font-family="Helvetica" font-size="12">')
        for i in self.bc.textinfo.split():
            xpos, ypos, char = i.split(':')
            if int(xpos) - prev_x >= 10:
                correction += 2
            prev_x = int(xpos);
            l.append('\t<text x="%s" y="%s">%s</text>' %
                    (int(xpos) - correction, 80, char))
        l.append('</g>\n')

        return '\n'.join(l)

    def end(self):
        return '</svg>'

    def render(self):
        return self.start() + self.bars() + self.end()

    def __str__(self):
        return self.render()


if __name__ == '__main__':
    b = barcode.Barcode('988935689111')
    print SvgBackend(b)
