#!/usr/bin/env python3
"""Check real turtlesim motion using an independent temporary ROS master."""
import argparse
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
    parser = argparse.ArgumentParser()
    parser.add_argument('--show', action='store_true')
    args = parser.parse_args()
    with socket.socket() as sock:
        sock.bind(('127.0.0.1', 0))
        port = sock.getsockname()[1]
    os.environ['ROS_MASTER_URI'] = 'http://127.0.0.1:{}'.format(port)
    os.environ['ROS_IP'] = '127.0.0.1'
    os.environ.pop('ROS_HOSTNAME', None)
    env = os.environ.copy()
    if not args.show:
        env['QT_QPA_PLATFORM'] = 'offscreen'
    processes = []
    with tempfile.TemporaryDirectory(prefix='turtle-lesson-') as tmp:
        env['ROS_LOG_DIR'] = str(Path(tmp) / 'ros-logs')
        log_path = Path(tmp) / 'processes.log'
        with log_path.open('w') as log:
            def start(command):
                process = subprocess.Popen(command, env=env, stdout=log, stderr=log,
                                           start_new_session=True)
                processes.append(process)
                return process
            try:
                start(['roscore', '-p', str(port)])
                ready = False
                socket.setdefaulttimeout(1)
                for _ in range(60):
                    try:
                        result = xmlrpc.client.ServerProxy(env['ROS_MASTER_URI']).getPid('/lesson_check')
                        if result[0] == 1:
                            ready = True
                            break
                    except (OSError, xmlrpc.client.Error):
                        pass
                    time.sleep(0.2)
                if not ready:
                    raise RuntimeError('Temporary ROS master did not start')
                import rospy
                from geometry_msgs.msg import Twist
                from std_srvs.srv import Empty
                from turtlesim.msg import Pose
                rospy.init_node('turtle_lesson_check', anonymous=True, disable_signals=True)
                start(['rosrun', 'turtlesim', 'turtlesim_node'])
                rospy.wait_for_service('/reset', timeout=15)
                reset = rospy.ServiceProxy('/reset', Empty)
                commands = []
                observed = rospy.Subscriber('/turtle1/cmd_vel', Twist,
                                            lambda msg: commands.append(msg), queue_size=100)
                poses = []
                observed_pose = rospy.Subscriber('/turtle1/pose', Pose,
                                                 lambda msg: poses.append(msg), queue_size=500)
                controller_command = ['rosrun', 'turtle_control', 'turtle_motion']
                cases = [('straight', 1.0, 0.0, 3.0 if args.show else 1.0),
                         ('turn', 0.0, 1.0, 3.0 if args.show else 1.0),
                         ('circle', 1.0, 1.0, 2 * math.pi if args.show else 1.0)]
                for name, linear, angular, duration in cases:
                    reset()
                    rospy.sleep(0.15)
                    before = rospy.wait_for_message('/turtle1/pose', Pose, timeout=3)
                    commands.clear()
                    poses.clear()
                    print('Running {}: linear={} angular={} duration={:.2f}'.format(
                        name, linear, angular, duration), flush=True)
                    started = time.monotonic()
                    process = start(controller_command + ['_linear:={}'.format(linear),
                                     '_angular:={}'.format(angular), '_duration:={}'.format(duration)])
                    if process.wait(timeout=duration + 8) != 0:
                        raise RuntimeError('{} controller failed'.format(name))
                    elapsed = time.monotonic() - started
                    assert duration <= elapsed < duration + 3, (name, 'duration', elapsed)
                    rospy.sleep(0.2)
                    after = rospy.wait_for_message('/turtle1/pose', Pose, timeout=3)
                    dx, dy = after.x - before.x, after.y - before.y
                    path = [before] + list(poses) + [after]
                    distance = 0.0
                    rotation = 0.0
                    for previous, current in zip(path, path[1:]):
                        distance += math.hypot(current.x - previous.x, current.y - previous.y)
                        delta = current.theta - previous.theta
                        rotation += math.atan2(math.sin(delta), math.cos(delta))
                    # GUI scheduling can slow the simulator's updates. Check the
                    # measured path, while separately checking controller wall time.
                    if linear == 0:
                        assert distance < 0.02, (name, 'unexpected translation', distance)
                    else:
                        assert 0.7 * abs(linear) * duration < distance < 1.25 * abs(linear) * duration
                    if angular == 0:
                        assert abs(rotation) < 0.02, (name, 'unexpected rotation', rotation)
                    else:
                        assert 0.7 < rotation / (angular * duration) < 1.25
                    for pose in path:
                        px, py = pose.x - before.x, pose.y - before.y
                        local_x = math.cos(before.theta) * px + math.sin(before.theta) * py
                        local_y = -math.sin(before.theta) * px + math.cos(before.theta) * py
                        if angular == 0:
                            assert abs(local_y) < 0.03, (name, 'not straight', local_y)
                            assert local_x * linear >= -0.001, (name, 'wrong direction')
                        else:
                            angle = pose.theta - before.theta
                            radius = linear / angular
                            assert abs(local_x - radius * math.sin(angle)) < 0.04
                            assert abs(local_y - radius * (1 - math.cos(angle))) < 0.04
                    assert any(abs(p.linear_velocity - linear) < 1e-3 and
                               abs(p.angular_velocity - angular) < 1e-3 for p in path)
                    assert len(commands) >= 10, (name, 'missing repeated messages')
                    assert any(abs(msg.linear.x - linear) < 1e-6 and
                               abs(msg.angular.z - angular) < 1e-6 for msg in commands)
                    assert commands[-1].linear.x == 0 and commands[-1].angular.z == 0
                    rospy.sleep(0.3)
                    stopped = rospy.wait_for_message('/turtle1/pose', Pose, timeout=3)
                    assert math.hypot(stopped.x - after.x, stopped.y - after.y) < 0.02
                    assert abs(math.atan2(math.sin(stopped.theta - after.theta),
                                          math.cos(stopped.theta - after.theta))) < 0.02
                    print('PASS {}: dx={:.3f} dy={:.3f}; path={:.3f} turn={:.3f}; {} messages; stopped'.format(
                        name, dx, dy, distance, rotation, len(commands)), flush=True)
                observed.unregister()
                observed_pose.unregister()
                print('All motion checks passed.', flush=True)
                if args.show:
                    time.sleep(2)
            except BaseException:
                log.flush()
                print(log_path.read_text(errors='replace')[-6000:], flush=True)
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
