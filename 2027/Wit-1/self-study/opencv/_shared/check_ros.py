#!/usr/bin/env python3
import argparse
import json
from pathlib import Path
import rospy
from sensor_msgs.msg import Image

parser = argparse.ArgumentParser()
parser.add_argument('--output', type=Path, required=True)
args = parser.parse_args()
rospy.init_node('offline_course_output_check', anonymous=True)
message = rospy.wait_for_message('/vision_course/image_debug', Image, timeout=20)
assert message.width == 640 and message.height == 480 and message.encoding == 'bgr8'
assert message.step == 640 * 3 and len(message.data) == message.step * message.height
assert message.header.stamp.to_sec() > 0 and message.header.frame_id == 'camera_optical_frame'
(args.output / 'output-message.json').write_text(json.dumps({
    'width': message.width, 'height': message.height, 'encoding': message.encoding,
    'frame_id': message.header.frame_id, 'stamp': message.header.stamp.to_sec(),
    'bytes': len(message.data)
}, indent=2) + '\n')
print('Output Image received and checked')
