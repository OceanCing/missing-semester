#!/usr/bin/env python3
"""Check actual C++ pairing results for known orientations and negative geometry cases."""
import argparse
import csv
import math
from pathlib import Path
import subprocess

import cv2
import numpy as np


def read_rows(path):
    with path.open() as stream:
        return list(csv.DictReader(stream))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--binary', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    sample = Path(__file__).resolve().parents[1] / 'assets/pairing_scene.png'

    def run_case(name, image=sample, limit=10):
        destination = args.output / name
        destination.mkdir(parents=True, exist_ok=True)
        subprocess.run([str(args.binary), f'--image={image}', f'--output={destination}',
                        f'--max-diff={limit}', '--headless'], check=True)
        return read_rows(destination / 'lights.csv'), read_rows(destination / 'pairs.csv')

    for limit, expected in [(10, {(0, 1)}), (45, {(0, 1)}),
                            (55, {(0, 1), (2, 3)}), (90, {(0, 1), (2, 3)})]:
        lights, pairs = run_case(f'diff-{limit}', limit=limit)
        assert len(lights) == 4 and len(pairs) == 6, (lights, pairs)
        for row, angle in zip(lights, (25, 25, 25, -25)):
            assert row['accepted'] == '1', row
            assert abs(float(row['signed_tilt']) - angle) < 1, row
        for row in pairs:
            assert all(math.isfinite(float(row[field])) for field in
                       ('length_ratio', 'y_offset_ratio', 'separation_ratio', 'angle_difference'))
        nearby = {(int(row['left']), int(row['right'])) for row in pairs
                  if row['geometry_passed'] == '1'}
        accepted = {(int(row['left']), int(row['right'])) for row in pairs
                    if row['accepted'] == '1'}
        assert nearby == {(0, 1), (2, 3)}, nearby
        assert accepted == expected, (limit, accepted)
        same = next(row for row in pairs if (row['left'], row['right']) == ('0', '1'))
        opposite = next(row for row in pairs if (row['left'], row['right']) == ('2', '3'))
        assert float(same['angle_difference']) < 0.1, same
        assert abs(float(opposite['angle_difference']) - 50) < 1, opposite

    geometry_cases = [
        ('valid-vertical', [(90, 120, 16, 100), (190, 120, 16, 100)], True),
        ('unequal-length', [(90, 120, 16, 120), (190, 150, 16, 60)], False),
        ('vertical-offset', [(90, 80, 16, 100), (190, 180, 16, 100)], False),
        ('too-far', [(90, 120, 16, 100), (490, 120, 16, 100)], False),
        ('too-close', [(90, 120, 16, 100), (120, 120, 16, 100)], False),
    ]
    for name, rectangles, should_pass in geometry_cases:
        image = np.full((360, 640, 3), 20, dtype=np.uint8)
        for x, y, width, height in rectangles:
            image[y:y + height, x:x + width] = (20, 20, 240)
        path = args.output / f'{name}.png'
        assert cv2.imwrite(str(path), image)
        lights, pairs = run_case(name, image=path)
        assert len(lights) == 2 and all(row['accepted'] == '1' for row in lights), lights
        assert len(pairs) == 1, pairs
        assert (pairs[0]['geometry_passed'] == '1') == should_pass, (name, pairs)
        assert (pairs[0]['accepted'] == '1') == should_pass, (name, pairs)

    for invalid in ('-1', '91', 'oops'):
        result = subprocess.run([str(args.binary), f'--image={sample}',
                                 f'--max-diff={invalid}', '--headless'],
                                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        assert result.returncode != 0, invalid
    print('PAIRING CHECK PASSED | 4 lights, 2 nearby pairs; max diff 10deg=1 pair, '
          '55deg=2 pairs.', flush=True)
    print('Checked signed tilt, opposite tilts, cross-group rejection, length mismatch, '
          'vertical offset, spacing and invalid parameters.', flush=True)


if __name__ == '__main__':
    main()
