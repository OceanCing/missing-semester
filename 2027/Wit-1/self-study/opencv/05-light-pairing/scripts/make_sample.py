#!/usr/bin/env python3
"""Create two isolated pairs: parallel tilted bars and opposite tilted bars."""
import math
from pathlib import Path
import struct
import zlib

WIDTH, HEIGHT = 640, 360
BARS = ((100, 25), (200, 25), (430, 25), (530, -25))


def main():
    pixels = bytearray(bytes((20, 20, 20)) * WIDTH * HEIGHT)
    for center_x, angle in BARS:
        sine = math.sin(math.radians(angle))
        cosine = math.cos(math.radians(angle))
        for y in range(HEIGHT):
            for x in range(WIDTH):
                dx, dy = x + 0.5 - center_x, y + 0.5 - 180
                if abs(dx * sine - dy * cosine) <= 50 and abs(dx * cosine + dy * sine) <= 8:
                    offset = (y * WIDTH + x) * 3
                    pixels[offset:offset + 3] = bytes((240, 20, 20))

    def chunk(kind, data):
        return (struct.pack('!I', len(data)) + kind + data +
                struct.pack('!I', zlib.crc32(kind + data) & 0xffffffff))

    raw = b''.join(b'\x00' + pixels[row * WIDTH * 3:(row + 1) * WIDTH * 3]
                   for row in range(HEIGHT))
    png = (b'\x89PNG\r\n\x1a\n' +
           chunk(b'IHDR', struct.pack('!2I5B', WIDTH, HEIGHT, 8, 2, 0, 0, 0)) +
           chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b''))
    destination = Path(__file__).resolve().parents[1] / 'assets/pairing_scene.png'
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(png)
    print(f'Created synthetic pairing image: {destination}')


if __name__ == '__main__':
    main()
