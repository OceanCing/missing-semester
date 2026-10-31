#!/usr/bin/env bash
set -euo pipefail
lesson_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
lesson_build="${OPENCV_DIRECTION_BUILD:-$HOME/catkin_ws/.opencv-build/04-light-direction}"
lesson_output="${OPENCV_DIRECTION_OUTPUT:-$HOME/catkin_ws/records/opencv/04-light-direction}"
lesson_image="${OPENCV_DIRECTION_IMAGE:-$lesson_dir/assets/direction_scene.png}"
lesson_verify=0
if [ "${1:-}" = --verify ]; then lesson_verify=1; shift; fi
mkdir -p "$lesson_output"
exec > >(tee "$lesson_output/run.log") 2>&1
cmake -S "$lesson_dir" -B "$lesson_build" -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build "$lesson_build" --parallel 2
if [ "$lesson_verify" -eq 1 ]; then
    python3 "$lesson_dir/scripts/check_lesson.py" \
        --binary "$lesson_build/light_direction" --output "$lesson_output/checks"
fi
exec "$lesson_build/light_direction" --image="$lesson_image" --output="$lesson_output" "$@"
