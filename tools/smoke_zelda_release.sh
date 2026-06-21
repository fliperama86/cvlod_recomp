#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$repo_root"

if ! command -v timeout >/dev/null 2>&1; then
    echo "error: timeout command not found" >&2
    exit 127
fi

if [[ $# -gt 0 ]]; then
    version="$1"
elif [[ -n "${LOD_RELEASE_VERSION:-}" ]]; then
    version="$LOD_RELEASE_VERSION"
else
    version="$(git describe --tags --abbrev=0 2>/dev/null | sed 's/^v//' || true)"
    version="${version:-0.0.0-dev}"
fi
build_dir="${LOD_PRIMARY_BUILD_DIR:-build}"
bundle_dir="${LOD_BUNDLE_DIR:-build}"
app_path="$bundle_dir/LodRecomp.app"
log_path="${TMPDIR:-/tmp}/lod_zelda_release_smoke.log"
config_log_path="${TMPDIR:-/tmp}/lod_zelda_release_config_smoke.log"

echo "[release-smoke] Configuring primary release build in $build_dir"
cmake -S . -B "$build_dir" -U LOD_USE_ZELDA_MENU -DCMAKE_BUILD_TYPE=Release

echo "[release-smoke] Building LodRecomp"
cmake --build "$build_dir" --target LodRecomp --parallel

echo "[release-smoke] Regenerating app bundle at $app_path"
rm -rf "$app_path"
bash .github/macos/make_app.sh "$build_dir/LodRecomp" "$version" "$bundle_dir"
codesign --verify --verbose=2 "$app_path"

echo "[release-smoke] Checking for stale LodRecomp processes before smoke"
pgrep -la LodRecomp || true

echo "[release-smoke] Running bounded app-bundle smoke from /tmp"
set +e
(
    cd /tmp
    timeout 10 "$repo_root/$app_path/Contents/MacOS/LodRecomp" > "$log_path" 2>&1
)
bundle_status=$?
set -e
echo "[release-smoke] bundle smoke status: $bundle_status"
tail -180 "$log_path"

echo "[release-smoke] Checking for stale LodRecomp processes after bundle smoke"
pgrep -la LodRecomp || true

echo "[release-smoke] Running bounded config-open smoke"
set +e
RECOMP_UI_OPEN_ON_START=1 LOD_ZELDA_WAIT_FOR_START=1 timeout 10 "$build_dir/LodRecomp" > "$config_log_path" 2>&1
config_status=$?
set -e
echo "[release-smoke] config smoke status: $config_status"
tail -180 "$config_log_path"

echo "[release-smoke] Checking for stale LodRecomp processes after config smoke"
pgrep -la LodRecomp || true

if [[ "$bundle_status" != "124" || "$config_status" != "124" ]]; then
    echo "error: expected both smoke runs to exit by timeout status 124" >&2
    exit 1
fi

echo "[release-smoke] OK"
