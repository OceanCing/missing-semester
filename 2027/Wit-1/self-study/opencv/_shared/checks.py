#!/usr/bin/env python3
"""Run actual C++ programs and compare with known offline teaching fixtures."""
import argparse
import csv
import json
from pathlib import Path
import subprocess


def main():
    parser = argparse.ArgumentParser()
    for name in ('build', 'assets', 'output', 'config'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    completed = []

    def run(name, target, *extra):
        destination = args.output / name
        destination.mkdir(parents=True, exist_ok=True)
        subprocess.run([str(args.build / target), f'--assets={args.assets}',
                        f'--output={destination}', f'--config={args.config}',
                        '--headless', *extra], check=True)
        metrics = json.loads((destination / 'metrics.json').read_text())
        completed.append(name)
        return metrics

    near = run('camera-1m', 'lesson06_camera', '--value=100')
    far = run('camera-2m', 'lesson06_camera', '--value=200')
    assert near['height_px'] == 80 and far['height_px'] == 40
    assert abs(near['x_at_u400_m'] - 0.1) < 1e-8
    mat = run('mat', 'lesson07_mat')
    assert mat['shallow_shares_data'] == 1 and mat['clone_independent'] == 1
    assert mat['parent_changed_L1'] > 0 and mat['clone_changed_L1'] == 0
    assert mat['roi_continuous'] == 0
    static_view = run('static-view-unused-value', 'lesson07_mat', '--value=60')
    assert static_view['clone_changed_L1'] == 0
    morph = run('morph-3', 'lesson08_preprocess', '--variant=morph', '--value=1')
    assert morph['noise_after_open'] == 0 and morph['gap_after_close'] == 255
    assert morph['cleaned_components'] == 2 and morph['raw_components'] > 2
    identity = run('morph-1', 'lesson08_preprocess', '--variant=morph', '--value=0')
    assert identity['cleaned_components'] == identity['raw_components']
    oversized = run('morph-33', 'lesson08_preprocess', '--variant=morph', '--value=16')
    assert oversized['cleaned_components'] == 0
    blur = run('blur', 'lesson08_preprocess', '--variant=blur')
    assert blur['median_L1'] < blur['noisy_L1']
    thresholds = run('thresholds', 'lesson08_preprocess', '--variant=threshold')
    assert 0 < thresholds['otsu_threshold'] < 255
    geometry = run('geometry', 'lesson09_geometry')
    assert geometry['external_contours'] == 3 and geometry['tree_contours'] == 4
    assert geometry['child_contours'] == 1 and geometry['edge_pixels'] > 0
    warp = run('warp', 'lesson10_warp')
    assert warp['correct_mean_absolute_error'] < 15
    assert warp['correct_mean_absolute_error'] < warp['wrong_mean_absolute_error']
    assert warp['roi_width'] == 160 and warp['roi_height'] == 80
    calibration = run('calibration', 'lesson12_calibration')
    assert calibration['views'] >= 15 and calibration['rms_px'] < 1.5, calibration
    assert abs(calibration['fx'] - 800) < 120 and abs(calibration['fy'] - 800) < 120, calibration
    distortion = run('distortion', 'lesson12_calibration', '--variant=distortion')
    assert distortion['corrected_MAE'] < distortion['distorted_MAE'], distortion
    pnp = run('pnp-clean', 'lesson13_pnp')
    assert abs(pnp['z_m'] - 2) < 0.001 and pnp['translation_error_m'] < 0.001, pnp
    assert pnp['reprojection_rms_px'] < 0.001
    assert abs(pnp['body_x_m'] - 2.2) < 0.001 and abs(pnp['body_y_m'] + 0.1) < 0.001
    scaled = run('pnp-wrong-size', 'lesson13_pnp', '--variant=wrong-size')
    assert abs(scaled['z_m'] - 4) < 0.002 and scaled['reprojection_rms_px'] < 0.001, scaled
    noisy = run('pnp-noise', 'lesson13_pnp', '--value=2')
    assert noisy['z_m'] > 0 and noisy['reprojection_rms_px'] < 10
    video = run('video', 'lesson11_video', '--frames=30')
    assert video['frames'] == 30 and video['detected_frames'] >= 27, video
    pipeline = run('pipeline', 'lesson14_pipeline', '--frames=180')
    assert pipeline['frames'] == 180 and pipeline['pose_frames'] >= 140, pipeline
    with (args.assets / 'truth.csv').open() as stream:
        truth = {int(row['frame']): row for row in csv.DictReader(stream)}
    with (args.output / 'pipeline/frames.csv').open() as stream:
        observations = list(csv.DictReader(stream))
    errors, depth_errors, false_positive = [], [], 0
    for observation in observations:
        expected = truth[int(observation['frame'])]
        visible = expected['visible'] == '1'
        if not visible:
            assert observation['has_pose'] == '0', observation
            if observation['center_u']:
                false_positive += 1
            continue
        if observation['center_u']:
            du = float(observation['center_u']) - float(expected['u'])
            dv = float(observation['center_v']) - float(expected['v'])
            errors.append((du * du + dv * dv) ** 0.5)
        if observation['has_pose'] == '1':
            depth_errors.append(abs(float(observation['z_m']) - float(expected['z_m'])))
    assert len(errors) >= 140 and sum(errors) / len(errors) < 4, errors[:10]
    assert len(depth_errors) >= 140 and sum(depth_errors) / len(depth_errors) < 0.2, depth_errors[:10]
    assert false_positive == 0
    for target, extra in [('lesson07_mat', '--input=/nonexistent/offline-course.png'),
                           ('lesson08_preprocess', '--value=99'),
                           ('lesson11_video', '--input=/nonexistent/offline-course.avi'),
                           ('lesson13_pnp', '--frames=0')]:
        result = subprocess.run([str(args.build / target), f'--assets={args.assets}',
                                 f'--output={args.output / "invalid"}', '--headless', extra],
                                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        assert result.returncode != 0, (target, extra)
    report = {'passed_cases': completed, 'mean_center_error_px': sum(errors) / len(errors),
              'mean_depth_error_m': sum(depth_errors) / len(depth_errors),
              'note': 'Generated offline fixtures; not a real camera calibration or field performance score.'}
    (args.output / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    print(f'OFFLINE COURSE CHECK PASSED | {len(completed)} cases; center MAE '
          f'{report["mean_center_error_px"]:.3f}px, depth MAE {report["mean_depth_error_m"]:.3f}m')


if __name__ == '__main__':
    main()
