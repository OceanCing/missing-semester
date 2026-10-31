#!/usr/bin/env python3
"""Check the actual C++ contour features and filter decisions on known shapes."""
import argparse
import csv
import math
from pathlib import Path
import subprocess

import cv2


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--binary', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    sample = Path(__file__).resolve().parents[1] / 'assets/contour_scene.png'

    def run_case(name, area=200, ratio=30):
        destination = args.output / name
        destination.mkdir(parents=True, exist_ok=True)
        subprocess.run([str(args.binary), f'--image={sample}', f'--output={destination}',
                        f'--min-area={area}', f'--ratio-times10={ratio}', '--headless'],
                       check=True)
        with (destination / 'contours.csv').open() as file:
            rows = list(csv.DictReader(file))
        for row in rows:
            assert all(math.isfinite(float(row[key])) for key in
                       ['area', 'short_side', 'long_side', 'ratio'])
        assert len(rows) == 7, rows
        return rows, destination

    def accepted(rows):
        return [row for row in rows if row['accepted'] == '1']

    standard, destination = run_case('default')
    selected = accepted(standard)
    assert len(selected) == 2, selected
    centers = sorted(float(row['center_x']) for row in selected)
    assert abs(centers[0] - 111.5) < 0.1 and abs(centers[1] - 221.5) < 0.1
    for row in selected:
        assert abs(float(row['area']) - 3427) < 0.1
        assert 6.4 < float(row['ratio']) < 6.6
    single_pixel = next(row for row in standard if abs(float(row['center_x']) - 50) < 0.1)
    assert float(single_pixel['area']) == 0 and single_pixel['accepted'] == '0'
    mask = cv2.imread(str(destination / 'mask.png'), cv2.IMREAD_GRAYSCALE)
    assert mask is not None
    assert cv2.countNonZero(mask[110:260, 100:124]) == 3600
    for y, x in [(180, 322), (110, 510), (110, 590)]:
        assert mask[y, x] == 0  # Blue, white, yellow rejected before contours.
    small_allowed, _ = run_case('allow-small', area=0)
    assert len(accepted(small_allowed)) == 4  # Two bars plus two narrow noise regions.
    large_required, _ = run_case('area-4000', area=4000)
    assert len(accepted(large_required)) == 0
    blocks_allowed, _ = run_case('allow-wide', ratio=10)
    assert len(accepted(blocks_allowed)) == 4  # Two bars plus two broad red blocks.
    thin_required, _ = run_case('ratio-7', ratio=70)
    assert len(accepted(thin_required)) == 0
    equal_area, _ = run_case('area-3427', area=3427)
    above_area, _ = run_case('area-3428', area=3428)
    assert len(accepted(equal_area)) == 2 and len(accepted(above_area)) == 0
    for key, invalid in [('min-area', '-1'), ('min-area', '5001'),
                         ('min-area', 'oops'), ('ratio-times10', '-1'),
                         ('ratio-times10', '101')]:
        result = subprocess.run([str(args.binary), f'--image={sample}',
                                 f'--{key}={invalid}', '--headless'],
                                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        assert result.returncode != 0, (key, invalid)
    print('CONTOUR CHECK PASSED | 7 outer contours; candidates: default=2, '
          'min-area 0=4, min-area 4000=0, min-ratio 1.0=4.', flush=True)
    print('Checked contour area vs pixel count, inclusive area boundary, '
          'zero-size contour and invalid input.', flush=True)


if __name__ == '__main__':
    main()
