#!/usr/bin/env bash
set -euo pipefail
course_build="$1"; course_assets="$2"; course_output="$3"; course_config="$4"
shift 4
shared_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
if [ ! -x "$course_build/lesson15_ros" ]; then
  echo '未构建 ROS 1 图像示例；请在已配置的 Noetic 容器中运行。'
  exit 1
fi
mkdir -p "$course_output"
export ROS_MASTER_URI="http://127.0.0.1:${OPENCV_COURSE_ROS_PORT:-11341}"
export ROS_IP=127.0.0.1
unset ROS_HOSTNAME
python3 - <<'PY'
import os, socket
from urllib.parse import urlparse
address = urlparse(os.environ['ROS_MASTER_URI'])
connection = socket.socket()
try:
    connection.bind(('127.0.0.1', address.port))
except OSError:
    raise SystemExit('课程 ROS 端口已被占用；不要停止其他程序，可改 OPENCV_COURSE_ROS_PORT。')
finally:
    connection.close()
PY
owned_pids=()
cleanup() {
  for pid in "${owned_pids[@]}"; do kill -INT "$pid" 2>/dev/null || true; done
  for pid in "${owned_pids[@]}"; do wait "$pid" 2>/dev/null || true; done
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM
roscore -p "${OPENCV_COURSE_ROS_PORT:-11341}" > "$course_output/master.log" 2>&1 &
owned_pids+=("$!")
python3 - <<'PY'
import os, time, xmlrpc.client
master = xmlrpc.client.ServerProxy(os.environ['ROS_MASTER_URI'])
for _ in range(100):
    try:
        if master.getUri('/offline_course_ready')[0] == 1:
            break
    except OSError:
        pass
    time.sleep(0.1)
else:
    raise SystemExit('课程 ROS master 未启动，请查看 master.log。')
PY
"$course_build/lesson15_ros" --mode=publish --assets="$course_assets" --output="$course_output" > "$course_output/publisher.log" 2>&1 &
owned_pids+=("$!")
if [[ " $* " == *" --headless "* ]]; then
  python3 "$shared_dir/check_ros.py" --output="$course_output" > "$course_output/output-subscriber.log" 2>&1 &
  monitor_pid="$!"; owned_pids+=("$monitor_pid")
  timeout 25s "$course_build/lesson15_ros" --mode=process --assets="$course_assets" \
    --output="$course_output" --config="$course_config" --headless --frames=30
  wait "$monitor_pid"
  python3 - "$course_output/metrics.json" <<'PY'
import json, sys
data = json.load(open(sys.argv[1]))
assert data['received'] >= 30 and data['stamp_preserved'] == 1 and data['frame_preserved'] == 1
assert data['encoding_bgr8'] == 1 and data['width'] == 640 and data['height'] == 480
print('ROS IMAGE CHECK PASSED | offline image -> C++ cv_bridge callback -> output subscriber')
PY
else
  "$course_build/lesson15_ros" --mode=process --assets="$course_assets" \
    --output="$course_output" --config="$course_config"
fi
