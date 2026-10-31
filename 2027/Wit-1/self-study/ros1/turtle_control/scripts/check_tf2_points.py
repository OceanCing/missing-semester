#!/usr/bin/env python3
"""Check the actual launch file and C++ TF2 point conversion on a private master."""
import math
import os
from pathlib import Path
import re
import signal
import socket
import subprocess
import tempfile
import time
import xmlrpc.client


def world_point(output):
    match = re.search(r'World point: frame=lesson_world x=([-\d.]+) y=([-\d.]+) z=([-\d.]+)', output)
    if not match:
        raise AssertionError('No transformed point in output: ' + output[-4000:])
    return tuple(map(float, match.groups()))


def assert_point(actual, expected):
    assert all(abs(a - b) < 0.001 for a, b in zip(actual, expected)), (actual, expected)


def stop(process):
    if process.poll() is None:
        os.killpg(process.pid, signal.SIGINT)
        try:
            process.wait(timeout=8)
        except subprocess.TimeoutExpired:
            os.killpg(process.pid, signal.SIGKILL)
            process.wait()


def main():
    with socket.socket() as sock:
        sock.bind(('127.0.0.1', 0))
        port = sock.getsockname()[1]
    os.environ['ROS_MASTER_URI'] = 'http://127.0.0.1:{}'.format(port)
    os.environ['ROS_IP'] = '127.0.0.1'
    os.environ.pop('ROS_HOSTNAME', None)
    os.environ['PYTHONUNBUFFERED'] = '1'
    processes = []
    with tempfile.TemporaryDirectory(prefix='tf2-point-lesson-') as tmp:
        os.environ['ROS_LOG_DIR'] = str(Path(tmp) / 'ros-logs')
        logs = []
        try:
            master_path = Path(tmp) / 'master.log'
            logs.append(master_path)
            with master_path.open('w') as master_log:
                master_process = subprocess.Popen(['roscore', '-p', str(port)],
                                                   stdout=master_log, stderr=master_log,
                                                   start_new_session=True)
            processes.append(master_process)
            socket.setdefaulttimeout(1)
            master = xmlrpc.client.ServerProxy(os.environ['ROS_MASTER_URI'])
            for _ in range(60):
                try:
                    if master.getPid('/tf2_lesson_check')[0] == 1:
                        break
                except (OSError, xmlrpc.client.Error):
                    pass
                time.sleep(0.2)
            else:
                raise RuntimeError('Temporary ROS master did not start')

            cases = [(math.pi / 2, (5, 5, 0), (4, 3, 1)),
                     (0.0, (7, 3, 0), (5, 4, 1))]
            for index, (yaw, expected, extra_expected) in enumerate(cases):
                launch_path = Path(tmp) / 'launch-{}.log'.format(index)
                logs.append(launch_path)
                with launch_path.open('w') as launch_log:
                    launch = subprocess.Popen(['roslaunch', '--wait', 'turtle_control',
                                               'frames.launch', 'yaw:={}'.format(yaw)],
                                              stdout=launch_log, stderr=launch_log,
                                              start_new_session=True)
                processes.append(launch)
                try:
                    deadline = time.monotonic() + 15
                    while time.monotonic() < deadline:
                        output = launch_path.read_text(errors='replace')
                        if 'World point: frame=lesson_world' in output:
                            break
                        if launch.poll() is not None:
                            raise RuntimeError('Launch exited early: ' + output)
                        time.sleep(0.1)
                    actual = world_point(launch_path.read_text(errors='replace'))
                    assert_point(actual, expected)
                    print('PASS launch yaw={:.4f}: robot (2, 0, 0) -> world {}'.format(
                        yaw, actual), flush=True)

                    probe = subprocess.run(['rosrun', 'turtle_control', 'point_to_world',
                                            '__name:=point_to_world_probe', '_x:=0', '_y:=1', '_z:=1'],
                                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                           text=True, timeout=12)
                    assert probe.returncode == 0, probe.stdout
                    actual_extra = world_point(probe.stdout)
                    assert_point(actual_extra, extra_expected)
                    print('PASS point arguments: robot (0, 1, 1) -> world {}'.format(
                        actual_extra), flush=True)
                finally:
                    stop(launch)
                time.sleep(0.2)

            missing = subprocess.run(['rosrun', 'turtle_control', 'point_to_world',
                                      '__name:=point_without_transform'],
                                     stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                     text=True, timeout=12)
            assert missing.returncode == 1, missing.stdout
            assert 'Transform unavailable' in missing.stdout, missing.stdout
            print('PASS missing frame relationship reports a clear error.', flush=True)
            print('All TF2 point conversion checks passed.', flush=True)
        except BaseException:
            for path in logs:
                print(path.name + ':\n' + path.read_text(errors='replace')[-5000:], flush=True)
            raise
        finally:
            for process in reversed(processes):
                stop(process)


if __name__ == '__main__':
    main()
