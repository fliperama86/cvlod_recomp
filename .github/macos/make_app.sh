#!/usr/bin/env bash
# Bundle the built LodRecomp binary into LodRecomp.app and ad-hoc sign it.
#
# Usage: .github/macos/make_app.sh <path-to-LodRecomp-binary> <version> [output-dir]
set -euo pipefail

BINARY="${1:?usage: make_app.sh <binary> <version> [outdir]}"
VERSION="${2:?usage: make_app.sh <binary> <version> [outdir]}"
OUTDIR="${3:-.}"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
ICON_PNG="$ROOT_DIR/assets/lod_icon-iOS-Default-1024x1024@1x.png"

APP="$OUTDIR/LodRecomp.app"
rm -rf "$APP"
mkdir -p "$APP/Contents/MacOS" "$APP/Contents/Resources"

cp "$BINARY" "$APP/Contents/MacOS/LodRecomp"
chmod +x "$APP/Contents/MacOS/LodRecomp"

ASSET_DEST="$APP/Contents/Resources/assets"
mkdir -p "$ASSET_DEST"
cp "$ROOT_DIR"/assets/*.rml "$ASSET_DEST/"
cp "$ROOT_DIR"/assets/*.rcss "$ASSET_DEST/"
cp "$ROOT_DIR"/assets/*.ttf "$ASSET_DEST/"
cp "$ROOT_DIR"/assets/*.otf "$ASSET_DEST/"
cp -R "$ROOT_DIR/assets/icons" "$ASSET_DEST/"
cp -R "$ROOT_DIR/assets/promptfont" "$ASSET_DEST/"
cp -R "$ROOT_DIR/assets/config_menu" "$ASSET_DEST/"

ICONSET="$(mktemp -d "${TMPDIR:-/tmp}/lodrecomp-iconset.XXXXXX")/LodRecomp.iconset"
trap 'rm -rf "$(dirname "$ICONSET")"' EXIT
mkdir -p "$ICONSET"

make_icon_png() {
    local size="$1"
    local output="$2"

    sips -s format png -z "$size" "$size" "$ICON_PNG" --out "$ICONSET/$output" >/dev/null
}

make_icon_png 16 "icon_16x16.png"
make_icon_png 32 "icon_16x16@2x.png"
make_icon_png 32 "icon_32x32.png"
make_icon_png 64 "icon_32x32@2x.png"
make_icon_png 128 "icon_128x128.png"
make_icon_png 256 "icon_128x128@2x.png"
make_icon_png 256 "icon_256x256.png"
make_icon_png 512 "icon_256x256@2x.png"
make_icon_png 512 "icon_512x512.png"
make_icon_png 1024 "icon_512x512@2x.png"
iconutil -c icns "$ICONSET" -o "$APP/Contents/Resources/LodRecomp.icns"

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
    <key>CFBundleIconFile</key>
    <string>LodRecomp.icns</string>
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
