#!/usr/bin/env bash
set -euo pipefail
lesson_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
lesson_build="${OPENCV_PAIRING_BUILD:-$HOME/catkin_ws/.opencv-build/05-light-pairing}"
lesson_output="${OPENCV_PAIRING_OUTPUT:-$HOME/catkin_ws/records/opencv/05-light-pairing}"
lesson_image="${OPENCV_PAIRING_IMAGE:-$lesson_dir/assets/pairing_scene.png}"
lesson_verify=0
if [ "${1:-}" = --verify ]; then lesson_verify=1; shift; fi
mkdir -p "$lesson_output"
exec > >(tee "$lesson_output/run.log") 2>&1
cmake -S "$lesson_dir" -B "$lesson_build" -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build "$lesson_build" --parallel 2
if [ "$lesson_verify" -eq 1 ]; then
    python3 "$lesson_dir/scripts/check_lesson.py" \
        --binary "$lesson_build/light_pairing" --output "$lesson_output/checks"
fi
exec "$lesson_build/light_pairing" --image="$lesson_image" --output="$lesson_output" "$@"
