#!/usr/bin/env python3
"""Check actual C++ output: hue wrap, gray rejection and inclusive S/V limits."""
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
    sample = Path(__file__).resolve().parents[1] / 'assets/hsv_scene.png'
    image = cv2.imread(str(sample))
    assert image is not None and image.shape == (360, 640, 3)

    def run_case(name, h=0, radius=10, saturation=100, value=50):
        destination = args.output / name
        destination.mkdir(parents=True, exist_ok=True)
        subprocess.run([str(args.binary), f'--image={sample}',
                        f'--output={destination}', f'--h-center={h}',
                        f'--h-radius={radius}', f'--s-min={saturation}',
                        f'--v-min={value}', '--headless'], check=True)
        mask = cv2.imread(str(destination / 'mask.png'), cv2.IMREAD_GRAYSCALE)
        selected = cv2.imread(str(destination / 'selected.png'))
        assert mask is not None and mask.shape == (360, 640)
        assert set(np.unique(mask)).issubset({0, 255})
        assert np.array_equal(selected[mask != 0], image[mask != 0])
        assert np.count_nonzero(selected[mask == 0]) == 0
        return mask, destination

    red, destination = run_case('red-default')
    hue = cv2.imread(str(destination / 'hue.png'), cv2.IMREAD_GRAYSCALE)
    saturation = cv2.imread(str(destination / 'saturation.png'), cv2.IMREAD_GRAYSCALE)
    value = cv2.imread(str(destination / 'value.png'), cv2.IMREAD_GRAYSCALE)
    assert hue[180, 112] == 0 and hue[180, 222] == 179
    assert saturation[110, 510] == 0
    assert value[180, 112] == 240 and value[250, 510] == 110 and value[250, 590] == 60
    for y, x in [(180, 112), (180, 222), (250, 510), (250, 590)]:
        assert red[y, x] == 255
    for y, x in [(180, 322), (110, 510), (110, 590), (20, 20)]:
        assert red[y, x] == 0
    assert cv2.countNonZero(red) == 13856

    no_saturation, _ = run_case('allow-gray', saturation=0)
    assert no_saturation[110, 510] == 255  # White's H is also encoded as 0.
    bright, _ = run_case('bright-only', value=150)
    assert bright[180, 112] == 255 and bright[180, 222] == 255
    assert bright[250, 510] == 0 and bright[250, 590] == 0
    assert cv2.countNonZero(bright) == 7200
    at_boundary, _ = run_case('value-110', value=110)
    above_boundary, _ = run_case('value-111', value=111)
    assert at_boundary[250, 510] == 255 and above_boundary[250, 510] == 0
    blue, _ = run_case('blue', h=120)
    assert blue[180, 322] == 255 and blue[180, 432] == 255
    assert blue[180, 112] == 0 and cv2.countNonZero(blue) == 7200
    yellow, _ = run_case('yellow', h=30)
    assert yellow[110, 590] == 255 and yellow[180, 112] == 0
    upper_wrap, _ = run_case('upper-wrap', h=179, radius=1)
    assert upper_wrap[180, 112] == 255 and upper_wrap[180, 222] == 255
    zero_only, _ = run_case('hue-zero-only', radius=0)
    end_only, _ = run_case('hue-179-only', h=179, radius=0)
    assert zero_only[180, 112] == 255 and zero_only[180, 222] == 0
    assert end_only[180, 112] == 0 and end_only[180, 222] == 255
    for key, invalid in [('h-center', '-1'), ('h-center', '180'),
                         ('h-center', 'oops'), ('h-radius', '-1'),
                         ('h-radius', '41'), ('s-min', '256'), ('v-min', '-1')]:
        result = subprocess.run([str(args.binary), f'--image={sample}',
                                 f'--{key}={invalid}', '--headless'],
                                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        assert result.returncode != 0, (key, invalid)
    print('HSV CHECK PASSED | red at H=0/179, gray rejection, V=110 inclusive boundary, '
          'blue/yellow selection and invalid parameters.', flush=True)
    print('Expected samples: bright red V=240; dim red V=110; darker red V=60.', flush=True)


if __name__ == '__main__':
    main()
