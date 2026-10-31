#!/usr/bin/env bash
set -euo pipefail
opencv_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
exec bash "$opencv_dir/_shared/run.sh" verify "$@"
