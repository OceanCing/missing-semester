#!/usr/bin/env python3
"""Check C++ measurements against the fixture's independently specified angles."""
import argparse
import csv
import math
from pathlib import Path
import subprocess


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--binary', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    sample = Path(__file__).resolve().parents[1] / 'assets/direction_scene.png'
    expected_centers = (59.5, 159.5, 259.5, 359.5, 459.5, 569.5)
    expected_angles = (0, 25, 25, 60, 60, 90)

    def run_case(limit):
        destination = args.output / f'tilt-{limit}'
        destination.mkdir(parents=True, exist_ok=True)
        subprocess.run([str(args.binary), f'--image={sample}', f'--output={destination}',
                        f'--max-tilt={limit}', '--headless'], check=True)
        with (destination / 'directions.csv').open() as file:
            rows = list(csv.DictReader(file))
        assert len(rows) == 6, rows
        for row, center, angle in zip(rows, expected_centers, expected_angles):
            measured = float(row['tilt_degrees'])
            assert math.isfinite(measured) and 0 <= measured <= 90, row
            assert abs(float(row['center_x']) - center) < 0.6, row
            assert abs(measured - angle) < 1.0, row  # Rasterized edge tolerance.
            assert row['shape_passed'] == '1' and float(row['ratio']) > 5, row
        return rows

    for limit, expected_ids in [(0, {0}), (15, {0}), (30, {0, 1, 2}),
                                (65, {0, 1, 2, 3, 4}), (89, {0, 1, 2, 3, 4}),
                                (90, {0, 1, 2, 3, 4, 5})]:
        rows = run_case(limit)
        actual = {int(row['id']) for row in rows if row['accepted'] == '1'}
        assert actual == expected_ids, (limit, actual, expected_ids)
    for value in ('-1', '91', 'oops'):
        result = subprocess.run([str(args.binary), f'--image={sample}',
                                 f'--max-tilt={value}', '--headless'],
                                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        assert result.returncode != 0, value
    print('DIRECTION CHECK PASSED | 6 shape candidates; direction passed: '
          '15deg=1, 30deg=3, 65deg=5, 90deg=6.', flush=True)
    print('Checked mirrored tilts, 0/90-degree boundaries and invalid parameters.', flush=True)


if __name__ == '__main__':
    main()
