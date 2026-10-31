#!/usr/bin/env python3
"""Create six isolated, equal-size red bars with known axis directions."""
import math
from pathlib import Path
import struct
import zlib

WIDTH, HEIGHT = 640, 360
BAR_CENTERS = (60, 160, 260, 360, 460, 570)
TILTS = (0, 25, -25, 60, -60, 90)


def main():
    pixels = bytearray(bytes((20, 20, 20)) * WIDTH * HEIGHT)
    for center_x, tilt in zip(BAR_CENTERS, TILTS):
        radians = math.radians(tilt)
        sine, cosine = math.sin(radians), math.cos(radians)
        for y in range(HEIGHT):
            for x in range(WIDTH):
                dx, dy = x + 0.5 - center_x, y + 0.5 - 180
                along = dx * sine - dy * cosine
                across = dx * cosine + dy * sine
                if abs(along) <= 50 and abs(across) <= 8:
                    offset = (y * WIDTH + x) * 3
                    pixels[offset:offset + 3] = bytes((240, 20, 20))  # PNG RGB.

    def chunk(kind, data):
        return (struct.pack('!I', len(data)) + kind + data +
                struct.pack('!I', zlib.crc32(kind + data) & 0xffffffff))

    raw = b''.join(b'\x00' + pixels[row * WIDTH * 3:(row + 1) * WIDTH * 3]
                   for row in range(HEIGHT))
    png = (b'\x89PNG\r\n\x1a\n' +
           chunk(b'IHDR', struct.pack('!2I5B', WIDTH, HEIGHT, 8, 2, 0, 0, 0)) +
           chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b''))
    destination = Path(__file__).resolve().parents[1] / 'assets/direction_scene.png'
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(png)
    print(f'Created synthetic direction image: {destination}')


if __name__ == '__main__':
    main()
