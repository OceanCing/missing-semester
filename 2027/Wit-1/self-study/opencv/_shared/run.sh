#!/usr/bin/env bash
set -euo pipefail
shared_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
course_build="${OPENCV_COURSE_BUILD:-$HOME/catkin_ws/.opencv-build/offline-course}"
course_records="${OPENCV_COURSE_RECORDS:-$HOME/catkin_ws/records/opencv/offline-course}"
course_assets="$course_records/assets"
topic="${1:-help}"
if [ "$#" -gt 0 ]; then shift; fi
case "$topic" in
  help|--help|-h)
    cat <<'HELP'
用法：bash _shared/run.sh <课程> [参数]
06 camera       相机成像模型
07 mat          Mat、浅拷贝、clone、ROI
08 preprocess   滤波、阈值、形态学；--variant=blur / threshold / morph
09 geometry     Canny、轮廓层次与外接矩形
10 warp         透视裁剪与角点顺序
11 video        离线视频、暂停与计时
12 calibration  棋盘格标定；--variant=distortion 显示畸变
13 pnp          PnP 与坐标变换；--variant=wrong-size 对照尺寸错误
14 pipeline     离线检测、裁剪、PnP 和丢失处理
15 ros          ROS 1 Noetic 离线图像发布与订阅
all             按顺序打开全部后续实验，Q 进入下一个
verify          一次构建、准备素材并验证全部后续例子（包括 ROS 1）
prepare         仅生成离线素材；--force 可重建自带的生成素材
通用实验参数：--headless、--value=N、--frames=N。
构建结果与生成素材都保存在本地 catkin_ws，不写入学习源码目录。
HELP
    exit 0 ;;
esac
if [ -f /opt/ros/noetic/setup.bash ]; then
  set +u
  source /opt/ros/noetic/setup.bash
  set -u
fi
mkdir -p "$course_build" "$course_records"
exec > >(tee -a "$course_records/session.log") 2>&1
cmake -S "$shared_dir" -B "$course_build" -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DPYTHON_EXECUTABLE=/usr/bin/python3
cmake --build "$course_build" --parallel 2
"$course_build/course_assets" --assets="$course_assets"
default_config=(--config="$shared_dir/config/detector.yaml")
for argument in "$@"; do
  if [[ "$argument" == --config=* ]]; then default_config=(); fi
done
run_one() {
  local key="$1" binary folder destination argument
  shift
  case "$key" in
    06|camera) binary=lesson06_camera; folder=06-camera-model ;;
    07|mat) binary=lesson07_mat; folder=07-mat-roi ;;
    08|preprocess) binary=lesson08_preprocess; folder=08-preprocess ;;
    09|geometry) binary=lesson09_geometry; folder=09-contour-geometry ;;
    10|warp) binary=lesson10_warp; folder=10-perspective-roi ;;
    11|video) binary=lesson11_video; folder=11-video-time ;;
    12|calibration) binary=lesson12_calibration; folder=12-camera-calibration ;;
    13|pnp) binary=lesson13_pnp; folder=13-pnp-coordinates ;;
    14|pipeline) binary=lesson14_pipeline; folder=14-offline-pipeline ;;
    15|ros)
      bash "$shared_dir/ros-run.sh" "$course_build" "$course_assets" "$course_records/15-ros-image" "$shared_dir/config/detector.yaml" "$@"
      return ;;
    *) echo "未知课程：$key。运行 bash _shared/run.sh help 查看列表。"; return 2 ;;
  esac
  destination="$course_records/$folder"
  for argument in "$@"; do
    if [[ "$argument" =~ ^--variant=([a-z][a-z-]*)$ ]]; then
      destination="$destination/${BASH_REMATCH[1]}"
    fi
  done
  mkdir -p "$destination"
  "$course_build/$binary" --assets="$course_assets" --output="$destination" "${default_config[@]}" "$@"
}
case "$topic" in
  all)
    run_one camera "$@"
    run_one mat "$@"
    run_one preprocess --variant=blur "$@"
    run_one preprocess --variant=threshold "$@"
    run_one preprocess --variant=morph "$@"
    run_one geometry "$@"
    run_one warp "$@"
    run_one video "$@"
    run_one calibration "$@"
    run_one calibration --variant=distortion "$@"
    run_one pnp "$@"
    run_one pnp --variant=wrong-size "$@"
    run_one pipeline "$@"
    run_one ros "$@" ;;
  verify)
    python3 "$shared_dir/checks.py" --build="$course_build" --assets="$course_assets" \
      --output="$course_records/checks" --config="$shared_dir/config/detector.yaml"
    bash "$shared_dir/ros-run.sh" "$course_build" "$course_assets" "$course_records/checks/ros" "$shared_dir/config/detector.yaml" --headless
    echo "ALL OFFLINE OPENCV AND ROS1 CHECKS PASSED" ;;
  prepare) "$course_build/course_assets" --assets="$course_assets" "$@" ;;
  *) run_one "$topic" "$@" ;;
esac
