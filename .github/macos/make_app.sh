#!/usr/bin/env bash
# Bundle the built LodRecomp binary into LodRecomp.app and ad-hoc sign it.
#
# Usage: .github/macos/make_app.sh <path-to-LodRecomp-binary> <version> [output-dir]
set -euo pipefail

BINARY="${1:?usage: make_app.sh <binary> <version> [outdir]}"
VERSION="${2:?usage: make_app.sh <binary> <version> [outdir]}"
OUTDIR="${3:-.}"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

APP="$OUTDIR/LodRecomp.app"
rm -rf "$APP"
mkdir -p "$APP/Contents/MacOS" "$APP/Contents/Resources"

cp "$BINARY" "$APP/Contents/MacOS/LodRecomp"
chmod +x "$APP/Contents/MacOS/LodRecomp"

cat > "$APP/Contents/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>LodRecomp</string>
    <key>CFBundleIdentifier</key>
    <string>com.fliperama86.lodrecomp</string>
    <key>CFBundleName</key>
    <string>LodRecomp</string>
    <key>CFBundleDisplayName</key>
    <string>Castlevania: Legacy of Darkness Recompiled</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleShortVersionString</key>
    <string>${VERSION#v}</string>
    <key>CFBundleVersion</key>
    <string>${VERSION#v}</string>
    <key>LSMinimumSystemVersion</key>
    <string>11.0</string>
    <key>NSHighResolutionCapable</key>
    <true/>
</dict>
</plist>
EOF

# Ad-hoc sign with the LoD entitlements (RWX memory for the recomp runtime).
codesign --force --sign - --entitlements "$SCRIPT_DIR/entitlements.plist" "$APP"

echo "Bundled and signed: $APP"
