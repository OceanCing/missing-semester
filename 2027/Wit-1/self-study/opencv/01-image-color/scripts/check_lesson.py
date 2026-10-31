#!/usr/bin/env python3
"""Exercise the actual C++ executable on known colors and threshold boundaries."""
import argparse
from pathlib import Path
import subprocess

import cv2
import numpy as np


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--binary', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    sample = Path(__file__).resolve().parents[1] / 'assets' / 'color_scene.png'
    image = cv2.imread(str(sample))
    assert image is not None and image.shape == (360, 640, 3)
    counts = []
    masks = {}
    for threshold in (20, 80, 180, 220, 255):
        destination = args.output / f'threshold-{threshold}'
        destination.mkdir(parents=True, exist_ok=True)
        subprocess.run([str(args.binary), f'--image={sample}',
                        f'--output={destination}', f'--threshold={threshold}',
                        '--headless'], check=True)
        mask = cv2.imread(str(destination / 'mask.png'), cv2.IMREAD_GRAYSCALE)
        selected = cv2.imread(str(destination / 'selected.png'))
        assert mask is not None and mask.shape == (360, 640)
        assert set(np.unique(mask)).issubset({0, 255})
        assert np.array_equal(selected[mask != 0], image[mask != 0])
        assert np.count_nonzero(selected[mask == 0]) == 0
        masks[threshold] = mask
        counts.append(cv2.countNonZero(mask))

    assert all(a >= b for a, b in zip(counts, counts[1:])), counts
    assert counts[0] > counts[1] > counts[2] > counts[3] == counts[4] == 0, counts
    standard = masks[80]
    assert standard[180, 112] == 255  # Red bar.
    assert standard[180, 322] == 0    # Blue bar.
    assert standard[110, 510] == 0    # White patch.
    assert standard[110, 590] == 255  # Yellow is an intentional false positive.
    assert standard[250, 510] == 255  # Dim red at threshold 80.
    assert masks[180][250, 510] == 0  # Dim red disappears as threshold rises.

    green_masks = {}
    for green_threshold in (19, 20, 50, 90, 220):
        destination = args.output / f'green-{green_threshold}'
        destination.mkdir(parents=True, exist_ok=True)
        subprocess.run([str(args.binary), f'--image={sample}',
                        f'--output={destination}', '--threshold=80',
                        f'--green-threshold={green_threshold}', '--use-green',
                        '--headless'], check=True)
        green_masks[green_threshold] = cv2.imread(
            str(destination / 'mask.png'), cv2.IMREAD_GRAYSCALE)
    green = green_masks[50]
    assert green[180, 112] == 255  # Bright red retained.
    assert green[250, 510] == 255  # Dim red retained.
    assert green[110, 590] == 0    # Yellow now rejected.
    assert green[180, 322] == 0 and green[110, 510] == 0  # Blue / white rejected.
    assert cv2.countNonZero(green) == 10528
    assert green_masks[19][110, 590] == 255
    assert green_masks[20][110, 590] == 0  # Equality does not pass the rule.
    assert green_masks[90][250, 510] == 0
    assert cv2.countNonZero(green_masks[220]) == 0
    assert np.all((green == 0) | (standard != 0))
    for invalid in ('-1', '256', 'not-a-number'):
        for parameter in ('threshold', 'green-threshold'):
            result = subprocess.run([str(args.binary), f'--image={sample}',
                                     f'--{parameter}={invalid}', '--headless'],
                                    stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            assert result.returncode != 0, (parameter, invalid)
    print(f'CHECK PASSED | thresholds 20,80,180,220,255 -> selected pixels {counts}',
          flush=True)
    print('Checked known colors, dim targets, false positives, mask output and invalid input.',
          flush=True)
    print('GREEN CHECK PASSED | R-B>80 AND R-G>50 keeps bright/dim red, rejects yellow; '
          'selected=10528. Boundary checks at R-G=20 and 90 passed.', flush=True)


if __name__ == '__main__':
    main()
