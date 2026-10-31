#!/usr/bin/env python3
"""Generate known colors, including red pixels on both ends of OpenCV's H range."""
from pathlib import Path
import struct
import zlib

WIDTH, HEIGHT = 640, 360


def main():
    pixels = bytearray(bytes((20, 20, 20)) * WIDTH * HEIGHT)

    def rectangle(x, y, width, height, bgr):
        row_data = bytes(reversed(bgr)) * width
        for row in range(y, y + height):
            offset = (row * WIDTH + x) * 3
            pixels[offset:offset + width * 3] = row_data

    rectangle(100, 110, 24, 150, (20, 20, 240))  # H near 0.
    rectangle(210, 110, 24, 150, (27, 20, 240))  # H near 179, still visibly red.
    for x in (310, 420):
        rectangle(x, 110, 24, 150, (240, 40, 20))
    rectangle(490, 90, 52, 64, (230, 230, 230))
    rectangle(566, 90, 52, 64, (20, 220, 240))
    rectangle(490, 220, 52, 64, (20, 20, 110))
    rectangle(566, 220, 52, 64, (20, 20, 60))

    def chunk(kind, data):
        return (struct.pack('!I', len(data)) + kind + data +
                struct.pack('!I', zlib.crc32(kind + data) & 0xffffffff))

    raw = b''.join(b'\x00' + pixels[row * WIDTH * 3:(row + 1) * WIDTH * 3]
                   for row in range(HEIGHT))
    png = (b'\x89PNG\r\n\x1a\n' +
           chunk(b'IHDR', struct.pack('!2I5B', WIDTH, HEIGHT, 8, 2, 0, 0, 0)) +
           chunk(b'IDAT', zlib.compress(raw, 9)) + chunk(b'IEND', b''))
    destination = Path(__file__).resolve().parents[1] / 'assets/hsv_scene.png'
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(png)
    print(f'Created synthetic HSV image: {destination}')


if __name__ == '__main__':
    main()
