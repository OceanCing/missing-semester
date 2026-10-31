#!/usr/bin/env python3
"""Exercise position feedback and stopping with a real, isolated turtlesim."""
import math
import os
from pathlib import Path
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
    with tempfile.TemporaryDirectory(prefix='turtle-feedback-lesson-') as tmp:
        os.environ['ROS_LOG_DIR'] = str(Path(tmp) / 'ros-logs')
        log_path = Path(tmp) / 'processes.log'
        with log_path.open('w') as log:
            def start(command):
                process = subprocess.Popen(command, stdout=log, stderr=log,
                                           start_new_session=True)
                processes.append(process)
                return process

            try:
                start(['roscore', '-p', str(port)])
                socket.setdefaulttimeout(1)
                master = xmlrpc.client.ServerProxy(os.environ['ROS_MASTER_URI'])
                for _ in range(60):
                    try:
                        if master.getPid('/feedback_lesson_check')[0] == 1:
                            break
                    except (OSError, xmlrpc.client.Error):
                        pass
                    time.sleep(0.2)
                else:
                    raise RuntimeError('Temporary ROS master did not start')

                import rospy
                from geometry_msgs.msg import Twist
                from std_srvs.srv import Empty
                from turtlesim.msg import Pose
                from turtlesim.srv import TeleportAbsolute
                rospy.init_node('feedback_lesson_check', anonymous=True, disable_signals=True)
                start(['rosrun', 'turtlesim', 'turtlesim_node'])
                rospy.wait_for_service('/reset', timeout=15)
                rospy.wait_for_service('/turtle1/teleport_absolute', timeout=5)
                reset = rospy.ServiceProxy('/reset', Empty)
                teleport = rospy.ServiceProxy('/turtle1/teleport_absolute', TeleportAbsolute)
                commands = []
                observer = rospy.Subscriber('/turtle1/cmd_vel', Twist,
                                             lambda msg: commands.append(msg), queue_size=500)
                rospy.sleep(0.2)
                controller_command = ['rosrun', 'turtle_control', 'turtle_go_to_x']

                def pose():
                    return rospy.wait_for_message('/turtle1/pose', Pose, timeout=3)

                def check_stopped():
                    rospy.sleep(0.2)
                    assert commands, 'No velocity commands received'
                    assert commands[-1].linear.x == 0 and commands[-1].angular.z == 0
                    before = pose()
                    rospy.sleep(0.3)
                    after = pose()
                    assert math.hypot(after.x - before.x, after.y - before.y) < 0.01
                    assert abs(after.theta - before.theta) < 0.01
                    return after

                for speed_cap in (1.0, 2.0):
                    reset()
                    rospy.sleep(0.2)
                    initial = pose()
                    commands.clear()
                    started = time.monotonic()
                    process = start(controller_command + ['_target_x:=8',
                                     '_max_speed:={}'.format(speed_cap)])
                    assert process.wait(timeout=25) == 0, 'Controller failed to reach target'
                    elapsed = time.monotonic() - started
                    actual = check_stopped()
                    assert abs(actual.x - 8.0) <= 0.06, ('target error', actual.x)
                    assert abs(actual.y - initial.y) < 0.02, ('unexpected sideways motion', actual.y)
                    moving = [msg.linear.x for msg in commands if msg.linear.x > 0.001]
                    assert moving and max(moving) > speed_cap - 0.05
                    assert max(moving) <= speed_cap + 1e-6
                    assert any(0.001 < speed < 0.2 for speed in moving), 'Did not slow near target'
                    assert all(abs(msg.angular.z) < 1e-6 for msg in commands)
                    print('PASS speed cap {}: stopped at x={:.3f}, time={:.2f}s, peak={:.2f}'.format(
                        speed_cap, actual.x, elapsed, max(moving)), flush=True)

                reset()
                teleport(8.0, 5.5, 0.0)
                rospy.sleep(0.2)
                commands.clear()
                process = start(controller_command + ['_target_x:=8', '_max_speed:=1'])
                assert process.wait(timeout=5) == 0
                check_stopped()
                assert all(msg.linear.x == 0 for msg in commands)
                print('PASS already at target: no movement requested.', flush=True)

                reset()
                rospy.sleep(0.2)
                commands.clear()
                process = start(controller_command + ['_target_x:=9', '_max_speed:=1'])
                deadline = time.monotonic() + 5
                while not any(msg.linear.x > 0.3 for msg in commands):
                    if process.poll() is not None or time.monotonic() > deadline:
                        raise AssertionError('No movement before interrupt check')
                    time.sleep(0.05)
                os.killpg(process.pid, signal.SIGINT)
                assert process.wait(timeout=5) == 0
                check_stopped()
                print('PASS Ctrl+C sends zero velocity and stops movement.', flush=True)

                reset()
                teleport(5.5, 5.5, 1.0)
                rospy.sleep(0.2)
                commands.clear()
                process = start(controller_command + ['_target_x:=8', '_max_speed:=1'])
                assert process.wait(timeout=5) == 1
                check_stopped()
                assert all(msg.linear.x == 0 for msg in commands)
                print('PASS incorrect starting heading requests reset without moving.', flush=True)
                observer.unregister()
                print('All position feedback checks passed.', flush=True)
            except BaseException:
                print(log_path.read_text(errors='replace')[-7000:], flush=True)
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
