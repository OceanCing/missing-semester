#!/usr/bin/env python3
"""Check the C++ pose callback against real turtlesim on a temporary master."""
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


def main():
    with socket.socket() as sock:
        sock.bind(('127.0.0.1', 0))
        port = sock.getsockname()[1]
    os.environ['ROS_MASTER_URI'] = 'http://127.0.0.1:{}'.format(port)
    os.environ['ROS_IP'] = '127.0.0.1'
    os.environ.pop('ROS_HOSTNAME', None)
    os.environ['QT_QPA_PLATFORM'] = 'offscreen'
    processes = []
    with tempfile.TemporaryDirectory(prefix='turtle-pose-lesson-') as tmp:
        os.environ['ROS_LOG_DIR'] = str(Path(tmp) / 'ros-logs')
        listener_path = Path(tmp) / 'listener.log'
        process_path = Path(tmp) / 'processes.log'
        with process_path.open('w') as log, listener_path.open('w') as listener_log:
            def start(command, output=log):
                process = subprocess.Popen(command, stdout=output, stderr=output,
                                           start_new_session=True)
                processes.append(process)
                return process

            def wait_for_logged_pose(x, y, theta):
                deadline = time.monotonic() + 6
                while time.monotonic() < deadline:
                    content = listener_path.read_text(errors='replace')
                    matches = re.findall(r'Pose: x=([-\d.]+) y=([-\d.]+) theta=([-\d.]+)', content)
                    for values in matches:
                        actual = tuple(map(float, values))
                        if all(abs(a - b) < 0.03 for a, b in zip(actual, (x, y, theta))):
                            return actual
                    if listener.poll() is not None:
                        raise RuntimeError('Listener exited unexpectedly: ' + content)
                    time.sleep(0.1)
                raise AssertionError('Callback did not log expected pose {}: {}'.format(
                    (x, y, theta), listener_path.read_text(errors='replace')))

            try:
                start(['roscore', '-p', str(port)])
                socket.setdefaulttimeout(1)
                master = xmlrpc.client.ServerProxy(os.environ['ROS_MASTER_URI'])
                for _ in range(60):
                    try:
                        if master.getPid('/pose_lesson_check')[0] == 1:
                            break
                    except (OSError, xmlrpc.client.Error):
                        pass
                    time.sleep(0.2)
                else:
                    raise RuntimeError('Temporary ROS master did not start')

                import rospy
                from turtlesim.msg import Pose
                from turtlesim.srv import TeleportAbsolute
                rospy.init_node('pose_lesson_check', anonymous=True, disable_signals=True)
                simulator = start(['rosrun', 'turtlesim', 'turtlesim_node'])
                rospy.wait_for_service('/turtle1/teleport_absolute', timeout=15)
                initial = rospy.wait_for_message('/turtle1/pose', Pose, timeout=5)
                listener = start(['stdbuf', '-oL', '-eL', 'rosrun', 'turtle_control',
                                  'turtle_pose_listener'], output=listener_log)
                first = wait_for_logged_pose(initial.x, initial.y, initial.theta)
                print('PASS initial pose received by C++ callback: {}'.format(first), flush=True)

                teleport = rospy.ServiceProxy('/turtle1/teleport_absolute', TeleportAbsolute)
                teleport(7.0, 3.0, 0.5)
                moved = wait_for_logged_pose(7.0, 3.0, 0.5)
                print('PASS callback received the updated position: {}'.format(moved), flush=True)

                os.killpg(listener.pid, signal.SIGINT)
                listener.wait(timeout=5)
                assert listener.returncode == 0, listener.returncode
                assert simulator.poll() is None, 'Stopping the listener stopped turtlesim'
                after = rospy.wait_for_message('/turtle1/pose', Pose, timeout=5)
                assert math.hypot(after.x - 7.0, after.y - 3.0) < 0.03
                print('PASS stopping the listener leaves turtlesim publishing normally.', flush=True)
                print('All pose callback checks passed.', flush=True)
            except BaseException:
                print(process_path.read_text(errors='replace')[-5000:], flush=True)
                print(listener_path.read_text(errors='replace')[-3000:], flush=True)
                raise
            finally:
                for process in reversed(processes):
                    if process.poll() is None:
                        os.killpg(process.pid, signal.SIGINT)
                        try:
                            process.wait(timeout=4)
                        except subprocess.TimeoutExpired:
                            os.killpg(process.pid, signal.SIGKILL)
                            process.wait()


if __name__ == '__main__':
    main()
