#!/usr/bin/env python3
"""Exercise a running simulation. Writes measured evidence; returns nonzero on failure."""
import argparse
import json
import math
import time
from pathlib import Path

import rospy
from control_msgs.msg import JointControllerState
from controller_manager_msgs.srv import ListControllers, SwitchController, LoadController, UnloadController
from dynamic_reconfigure.client import Client
from gazebo_msgs.msg import ModelStates
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
from tf.transformations import euler_from_quaternion
import tf

LABELS = ["front_left", "front_right", "back_left", "back_right"]
RADIUS, LEVER = 0.07625, 0.4


def yaw(q):
    return euler_from_quaternion([q.x, q.y, q.z, q.w])[2]


def body_velocity(w):
    return [sum(w) * RADIUS / 4,
            (-w[0] + w[1] + w[2] - w[3]) * RADIUS / 4,
            (-w[0] + w[1] - w[2] + w[3]) * RADIUS / (4 * LEVER)]


class Recorder:
    def __init__(self):
        self.wheels, self.odom, self.truth = {}, None, None
        self.rows, self.results = [], {}
        self.publisher = rospy.Publisher('/cmd_vel', Twist, queue_size=1)
        for label in LABELS:
            rospy.Subscriber('/hero_chassis_controller/' + label + '/state',
                             JointControllerState, self.wheel, callback_args=label)
        rospy.Subscriber('/odom', Odometry, self.odometry)
        rospy.Subscriber('/gazebo/model_states', ModelStates, self.model)

    def wheel(self, msg, label):
        self.wheels[label] = msg

    def odometry(self, msg):
        self.odom = msg

    def model(self, msg):
        if 'hero' in msg.name:
            self.truth = msg.pose[msg.name.index('hero')]

    def ready(self):
        deadline = time.monotonic() + 30
        while time.monotonic() < deadline:
            if len(self.wheels) == 4 and self.odom and self.truth and self.publisher.get_num_connections():
                return
            time.sleep(0.05)
        raise RuntimeError('No complete wheel/odom/Gazebo feedback within 30 seconds')

    def phase(self, name, velocity, duration):
        start, deadline = rospy.Time.now().to_sec(), time.monotonic() + 60
        begin = len(self.rows)
        msg = Twist()
        if velocity is not None:
            msg.linear.x, msg.linear.y, msg.angular.z = velocity
        while rospy.Time.now().to_sec() - start < duration:
            if time.monotonic() > deadline:
                raise RuntimeError('Simulation clock stopped during ' + name)
            if velocity is not None:
                self.publisher.publish(msg)
            states = [self.wheels[x] for x in LABELS]
            p = self.truth
            self.rows.append(dict(phase=name, t=rospy.Time.now().to_sec(),
                                  stamp=states[0].header.stamp.to_sec(),
                                  target=[x.set_point for x in states],
                                  measured=[x.process_value for x in states],
                                  error=[x.error for x in states],
                                  effort=[x.command for x in states],
                                  truth=[p.position.x, p.position.y, yaw(p.orientation)],
                                  odom=[self.odom.pose.pose.position.x,
                                        self.odom.pose.pose.position.y,
                                        yaw(self.odom.pose.pose.orientation)]))
            time.sleep(0.025)
        return self.rows[begin:]

    def check(self, name, passed, **details):
        self.results[name] = dict(passed=bool(passed), **details)
        print(('PASS ' if passed else 'FAIL ') + name + ' ' + json.dumps(details), flush=True)

    def motion(self, name, velocity, expected_signs, axis):
        rows = self.phase(name, velocity, 3)
        end = rows[-1]
        a, b = rows[0]['truth'], end['truth']
        dx, dy = b[0] - a[0], b[1] - a[1]
        relative = [math.cos(a[2]) * dx + math.sin(a[2]) * dy,
                    -math.sin(a[2]) * dx + math.cos(a[2]) * dy,
                    math.atan2(math.sin(b[2] - a[2]), math.cos(b[2] - a[2]))]
        signs_ok = all(x * s > 0.05 for x, s in zip(end['target'], expected_signs))
        displacement_ok = relative[axis] * velocity[axis] > 0.01
        late = [r for r in rows if r['t'] > rows[-1]['t'] - 1]
        mean_error = sum(abs(e) for r in late for e in r['error']) / (4 * len(late))
        self.check(name, signs_ok and displacement_ok and mean_error < 1.0,
                   relative_motion=relative, target=end['target'], mean_abs_error=mean_error)
        self.phase(name + '_stop', (0, 0, 0), 1.5)

    def run(self):
        self.ready()
        rospy.wait_for_service('/controller_manager/list_controllers', timeout=10)
        controllers = rospy.ServiceProxy('/controller_manager/list_controllers', ListControllers)()
        states = {x.name: x.state for x in controllers.controller}
        self.check('plugin_load', states.get('hero_chassis_controller') == 'running', states=states)
        self.check('odom_frames', self.odom.header.frame_id == 'odom' and
                   self.odom.child_frame_id == 'base_link',
                   parent=self.odom.header.frame_id, child=self.odom.child_frame_id)
        listener = tf.TransformListener()
        listener.waitForTransform('odom', 'base_link', rospy.Time(0), rospy.Duration(5))
        _, rotation = listener.lookupTransform('odom', 'base_link', rospy.Time(0))
        self.check('tf_available', all(math.isfinite(x) for x in rotation))
        settings = Client('/hero_chassis_controller', timeout=10)
        settings.update_configuration({'world_mode': False})
        self.phase('settle', (0, 0, 0), 1)
        self.motion('forward', (0.2, 0, 0), [1, 1, 1, 1], 0)
        self.motion('backward', (-0.2, 0, 0), [-1, -1, -1, -1], 0)
        self.motion('left', (0, 0.15, 0), [-1, 1, 1, -1], 1)
        self.motion('rotate', (0, 0, 0.5), [-1, 1, -1, 1], 2)
        self.phase('timeout_drive', (0.2, 0, 0), 1)
        timeout_rows = self.phase('timeout', None, 1)
        late = timeout_rows[-1]
        self.check('timeout_stop', max(map(abs, late['target'])) < 1e-8,
                   target_after_one_second=late['target'])
        pid = Client('/hero_chassis_controller/pid/front_left', timeout=10)
        original = pid.get_configuration()
        pid.update_configuration({'p': 0.7})
        self.phase('online_tune', (0.15, 0, 0), 1)
        observed = self.wheels['front_left'].p
        pid.update_configuration({'p': original['p']})
        self.check('online_pid', abs(observed - 0.7) < 1e-6, observed_p=observed,
                   restored_p=original['p'])
        self.phase('tune_stop', (0, 0, 0), 1)
        clients = [Client('/hero_chassis_controller/pid/' + label, timeout=10) for label in LABELS]
        originals = [client.get_configuration() for client in clients]
        try:
            for client in clients:
                client.update_configuration({'p': 20.0, 'i': 0.0, 'd': 0.0})
            settings.update_configuration({'max_acceleration_x': 5.0})
            saturation = self.phase('saturation', (0.6, 0, 0), 0.6)
            peak = max(abs(x) for r in saturation for x in r['effort'])
            self.check('saturation_clamped', 4.99 <= peak <= 5.000001, peak_effort=peak)
            self.phase('saturation_stop', (0, 0, 0), 1)
        finally:
            for client, cfg in zip(clients, originals):
                client.update_configuration({key: cfg[key] for key in ['p', 'i', 'd']})
            settings.update_configuration({'max_acceleration_x': 0.5})
        # Same command at the same heading, first relative to the body, then to odom.
        body = self.phase('body_x_after_turn', (0.2, 0, 0), 3)[-1]
        self.phase('body_stop', (0, 0, 0), 1)
        settings.update_configuration({'world_mode': True})
        world = self.phase('world_x_after_turn', (0.2, 0, 0), 3)[-1]
        def global_target(row):
            vx, vy, _ = body_velocity(row['target'])
            angle = row['odom'][2]
            return [math.cos(angle) * vx - math.sin(angle) * vy,
                    math.sin(angle) * vx + math.cos(angle) * vy]
        bv, wv = global_target(body), global_target(world)
        self.check('world_vs_body', wv[0] > 0.15 and abs(wv[1]) < 0.04 and abs(bv[1]) > 0.07,
                   body_command_in_odom=bv, world_command_in_odom=wv, heading=world['odom'][2])
        settings.update_configuration({'world_mode': False})
        self.phase('final_stop', (0, 0, 0), 1)
        peak = max(abs(x) for r in self.rows for x in r['effort'])
        self.check('torque_limit', peak <= 5.0 + 1e-9, max_abs_effort=peak)
        # Consecutive controller timestamps verify the ramp, ignoring stop/watchdog transitions.
        forward = [r for r in self.rows if r['phase'] == 'forward']
        slopes = []
        for a, b in zip(forward, forward[1:]):
            dt = b['stamp'] - a['stamp']
            if dt > 0:
                slopes.append(abs(body_velocity(b['target'])[0] - body_velocity(a['target'])[0]) / dt)
        self.check('acceleration_limit', slopes and max(slopes) <= 0.55,
                   max_target_acceleration=max(slopes, default=0))
        self.test_tf_failure()

    def test_tf_failure(self):
        name = 'hero_chassis_controller'
        switch = rospy.ServiceProxy('/controller_manager/switch_controller', SwitchController)
        unload = rospy.ServiceProxy('/controller_manager/unload_controller', UnloadController)
        load = rospy.ServiceProxy('/controller_manager/load_controller', LoadController)
        def reload_controller(frame, mode):
            if not switch([], [name], 2, False, 3.0).ok or not unload(name).ok:
                raise RuntimeError('Could not stop and unload controller')
            rospy.set_param('/' + name + '/world_frame', frame)
            rospy.set_param('/' + name + '/cmd_vel_mode', mode)
            rospy.set_param('/' + name + '/world_mode', mode == 'world')
            if not load(name).ok or not switch([name], [], 2, False, 3.0).ok:
                raise RuntimeError('Could not reload controller')
        try:
            reload_controller('missing_test_frame', 'world')
            rows = self.phase('missing_tf', (0.2, 0, 0), 1)
            target = rows[-1]['target']
            self.check('missing_tf_stops', max(map(abs, target)) < 1e-8, target=target)
        finally:
            reload_controller('odom', 'base_link')
            self.phase('restored_stop', (0, 0, 0), 0.5)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', default='/tmp/hero-simulation.json')
    args = parser.parse_args(rospy.myargv()[1:])
    rospy.init_node('verify_hero_simulation', anonymous=True)
    recorder = Recorder()
    completed = False
    try:
        recorder.run()
        completed = True
    finally:
        recorder.publisher.publish(Twist())
        output = dict(checks=recorder.results, samples=recorder.rows,
                      all_passed=completed and bool(recorder.results) and
                      all(x['passed'] for x in recorder.results.values()))
        Path(args.output).write_text(json.dumps(output, indent=2))
    return 0 if output['all_passed'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
