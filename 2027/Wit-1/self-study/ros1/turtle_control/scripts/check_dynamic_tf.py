#!/usr/bin/env python3
"""Check live pose -> dynamic TF -> transformed point on a private ROS master."""
import math
import os
from pathlib import Path
import signal
import socket
import subprocess
import tempfile
import time
import xmlrpc.client


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
    processes = []
    with tempfile.TemporaryDirectory(prefix='dynamic-tf-lesson-') as tmp:
        os.environ['ROS_LOG_DIR'] = str(Path(tmp) / 'ros-logs')
        logs = []
        try:
            master_path = Path(tmp) / 'master.log'
            logs.append(master_path)
            with master_path.open('w') as master_log:
                processes.append(subprocess.Popen(
                    ['roscore', '-p', str(port)], stdout=master_log,
                    stderr=master_log, start_new_session=True))
            socket.setdefaulttimeout(1)
            master = xmlrpc.client.ServerProxy(os.environ['ROS_MASTER_URI'])
            for _ in range(60):
                try:
                    if master.getPid('/dynamic_tf_check')[0] == 1:
                        break
                except (OSError, xmlrpc.client.Error):
                    pass
                time.sleep(0.2)
            else:
                raise RuntimeError('Temporary ROS master did not start')

            import rospy
            import tf2_geometry_msgs  # Register PointStamped with the TF2 buffer.
            import tf2_ros
            from geometry_msgs.msg import PointStamped
            from turtlesim.msg import Pose

            rospy.init_node('dynamic_tf_check', disable_signals=True)
            buffer = tf2_ros.Buffer()
            listener = tf2_ros.TransformListener(buffer)
            publisher = rospy.Publisher('/test/pose', Pose, queue_size=1)
            launch_path = Path(tmp) / 'launch.log'
            logs.append(launch_path)
            with launch_path.open('w') as launch_log:
                launch = subprocess.Popen(
                    ['roslaunch', '--wait', 'turtle_control', 'dynamic_frames.launch',
                     'pose_topic:=/test/pose'], stdout=launch_log,
                    stderr=launch_log, start_new_session=True)
            processes.append(launch)

            previous_stamp = None
            cases = [(2.0, 3.0, 0.0, (4.0, 3.0)),
                     (5.0, 3.0, math.pi / 2, (5.0, 5.0))]
            for x, y, yaw, expected_point in cases:
                pose = Pose(x=x, y=y, theta=yaw)
                deadline = time.monotonic() + 12
                while time.monotonic() < deadline:
                    assert launch.poll() is None, 'TF broadcaster exited early'
                    publisher.publish(pose)
                    time.sleep(0.05)
                    try:
                        transform = buffer.lookup_transform(
                            'turtle_world', 'turtle_body', rospy.Time(0))
                    except (tf2_ros.LookupException, tf2_ros.ConnectivityException,
                            tf2_ros.ExtrapolationException):
                        continue
                    translation = transform.transform.translation
                    rotation = transform.transform.rotation
                    actual = (translation.x, translation.y, translation.z,
                              rotation.x, rotation.y, rotation.z, rotation.w)
                    expected = (x, y, 0.0, 0.0, 0.0,
                                math.sin(yaw / 2), math.cos(yaw / 2))
                    if all(abs(a - b) < 0.001 for a, b in zip(actual, expected)):
                        break
                else:
                    raise AssertionError('No matching transform for pose {}'.format((x, y, yaw)))

                stamp = transform.header.stamp
                assert stamp.to_sec() > 0
                if previous_stamp is not None:
                    assert stamp > previous_stamp, 'Dynamic TF timestamp did not advance'
                previous_stamp = stamp
                point = PointStamped()
                point.header.frame_id = 'turtle_body'
                point.header.stamp = rospy.Time(0)
                point.point.x = 2.0
                world_point = buffer.transform(point, 'turtle_world', rospy.Duration(1))
                assert abs(world_point.point.x - expected_point[0]) < 0.001, world_point
                assert abs(world_point.point.y - expected_point[1]) < 0.001, world_point
                assert abs(world_point.point.z) < 0.001, world_point
                print('PASS pose ({:.1f}, {:.1f}, yaw={:.4f}): body (2, 0) -> world {}'.format(
                    x, y, yaw, expected_point), flush=True)

            # Check that the documented observer command can query these frames.
            echo = subprocess.Popen(
                ['rosrun', 'tf', 'tf_echo', 'turtle_world', 'turtle_body'],
                stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                text=True, start_new_session=True)
            processes.append(echo)
            deadline = time.monotonic() + 3
            while time.monotonic() < deadline:
                publisher.publish(pose)
                time.sleep(0.05)
            stop(echo)
            output, _ = echo.communicate(timeout=3)
            assert 'Translation: [5.000, 3.000, 0.000]' in output, output
            print('PASS tf_echo observer and increasing dynamic TF timestamps.', flush=True)
            print('All dynamic TF checks passed.', flush=True)
        except BaseException:
            for path in logs:
                print(path.name + ':\n' + path.read_text(errors='replace')[-4000:], flush=True)
            raise
        finally:
            if 'rospy' in locals() and rospy.core.is_initialized():
                rospy.signal_shutdown('Dynamic TF check complete')
            for process in reversed(processes):
                stop(process)


if __name__ == '__main__':
    main()
