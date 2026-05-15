#!/usr/bin/env bash
set -Eeuo pipefail

trap 'echo "Error: pipeline stopped because a step failed." >&2' ERR

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

PYTHON_CMD=()
if [[ -n "${PYTHON_BIN:-}" ]]; then
  PYTHON_CMD=("$PYTHON_BIN")
elif command -v python >/dev/null 2>&1; then
  PYTHON_CMD=(python)
elif command -v py >/dev/null 2>&1; then
  PYTHON_CMD=(py -3)
elif command -v python3 >/dev/null 2>&1; then
  PYTHON_CMD=(python3)
else
  echo "Error: no Python interpreter was found in PATH. Set PYTHON_BIN if needed." >&2
  exit 1
fi

TRAIN_GAMES="${TRAIN_GAMES:-200}"
COMPARE_GAMES="${COMPARE_GAMES:-20}"
DEMO_GAMES="${DEMO_GAMES:-5}"
LAYOUT="${LAYOUT:-smallClassic}"
DEPTH="${DEPTH:-2}"
RUN_TRAIN="${RUN_TRAIN:-0}"

run_step() {
  local title="$1"
  shift
  echo "============================================"
  echo "$title"
  echo "============================================"
  if "$@"; then
    return 0
  else
    local status=$?
    echo "Error: '$title' failed with exit code $status." >&2
    exit "$status"
  fi
}

if [[ "$RUN_TRAIN" == "1" ]]; then
  run_step "Step 1: Train RL weights" \
    "${PYTHON_CMD[@]}" train.py --num-games "$TRAIN_GAMES" --depth "$DEPTH" --layout "$LAYOUT"
else
  echo "============================================"
  echo "Step 1: Train RL weights skipped"
  echo "============================================"
  echo "Using existing final weights from weights.json."
  echo "Set RUN_TRAIN=1 if you really want to overwrite them."
fi

if [[ ! -s weights.json ]]; then
  echo "Error: train.py finished but weights.json was not created." >&2
  exit 1
fi

echo
run_step "Step 2: Compare agents" \
  "${PYTHON_CMD[@]}" compare.py --num-games "$COMPARE_GAMES" --layout "$LAYOUT" --depth "$DEPTH"

echo
run_step "Step 3: Demo RLMinimax" \
  "${PYTHON_CMD[@]}" pacman.py -p RLMinimaxAgent -l "$LAYOUT" -n "$DEMO_GAMES" -q

echo
echo "============================================"
echo "Pipeline completed successfully."
echo "Weights used from weights.json."
echo "============================================"
