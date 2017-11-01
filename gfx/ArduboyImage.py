#!/usr/bin/env python3

"""
    Copyright 2017 Alex Margarit <http://www.alxm.org/>

    ArduboyImage.py is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ArduboyImage.py is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ArduboyImage.py.  If not, see <http://www.gnu.org/licenses/>.
"""

import sys
from PIL import Image

def main(ImageName, UniqueName):
    image = Image.open(ImageName)
    width, height = image.size
    pixels = image.load()
    encoded_bytes = [width, height]

    for y_start in range(0, height, 8):
        for x in range(0, width):
            byte = 0

            for y in range(0, min(height - y_start, 8)):
                r, g, b = pixels[x, y_start + y]

                if r == 255 and g == 255 and b == 255:
                    byte |= 1 << y

            encoded_bytes.append(byte)

    formatted_bytes = ''

    for index, byte in enumerate(encoded_bytes):
        if index % 16 == 0:
            formatted_bytes += '\n    '

        formatted_bytes += '0x{:0>2x},'.format(byte)

    contents = """\
// Generated by gfx/ArduboyImage.py
PROGMEM static const uint8_t z_data_gfx_{0}_buffer[] = {{{3}
}};\
""".format(UniqueName, width, height, formatted_bytes)

    print(contents)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('Usage: {} Image.png UniqueName'.format(sys.argv[0]))
    else:
        main(sys.argv[1], sys.argv[2])