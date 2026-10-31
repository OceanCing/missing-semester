#!/usr/bin/env bash
set -euo pipefail
lesson_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
lesson_build="${OPENCV_LESSON_BUILD:-$HOME/catkin_ws/.opencv-build/01-image-color}"
lesson_output="${OPENCV_LESSON_OUTPUT:-$HOME/catkin_ws/records/opencv/01-image-color}"
lesson_image="${OPENCV_LESSON_IMAGE:-$lesson_dir/assets/color_scene.png}"

cmake -S "$lesson_dir" -B "$lesson_build" -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build "$lesson_build" --parallel 2
mkdir -p "$lesson_output"
exec "$lesson_build/image_color" \
  --image="$lesson_image" --output="$lesson_output" "$@"
