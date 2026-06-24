Castlevania: Legacy of Darkness Recompiled - Linux x64
======================================================

Setup
-----
1. Extract this folder anywhere.
2. Place your legally dumped stock Castlevania: Legacy of Darkness ROM
   in the same folder as the LodRecomp binary, named:  rom.z64
   (If no ROM is found, the app will ask you to select one and remember it.)
3. Make sure the binary is executable, then run it from this folder:
      chmod +x LodRecomp
      ./LodRecomp

Desktop launcher/icon
---------------------
The archive includes LodRecomp.desktop, lodrecomp.png, and installable
desktop assets under share/. To install the launcher for the current user
after extracting the archive, run these commands from the extracted folder:

  install -Dm755 LodRecomp "$HOME/.local/bin/LodRecomp"
  install -Dm644 share/applications/lodrecomp.desktop \
    "$HOME/.local/share/applications/lodrecomp.desktop"
  install -Dm644 share/icons/hicolor/1024x1024/apps/lodrecomp.png \
    "$HOME/.local/share/icons/hicolor/1024x1024/apps/lodrecomp.png"

Make sure $HOME/.local/bin is in your PATH before using the desktop launcher.

Runtime dependencies
--------------------
This build links against your system libraries and a Vulkan driver.
On Debian/Ubuntu install:

  sudo apt-get install -y \
    libsdl2-2.0-0 libfreetype6 zlib1g libvulkan1 \
    libgtk-3-0 libx11-6 libxrandr2 libxi6 libxcursor1 libxinerama1 \
    mesa-vulkan-drivers

(Use your GPU vendor's Vulkan driver if you have one; mesa-vulkan-drivers
provides open-source and software-fallback ICDs.) A working Vulkan device
is required; the renderer and the F1 settings menu both use Vulkan.

Hotkeys
-------
F1   Show/hide the settings menu
F11  Toggle windowed/fullscreen
F5   Cycle internal resolution (Original, Original2x, Auto)
F6   Cycle aspect ratio (Original, Expand, Manual)
F7   Cycle antialiasing (None, MSAA2X, MSAA4X, MSAA8X)
F8   Cycle refresh-rate mode (Original, Display, Manual)

Keyboard: Enter = A, Right Shift = B, Z = Z, Space = Start,
arrows = D-pad, WASD = analog stick. SDL game controllers are
supported. Controller Back/Select shows or hides the settings menu;
Start/Options remains mapped to N64 Start.

Config files and the current-session log are stored in ~/.lodrecomp.
For crash reports, attach ~/.lodrecomp/LodRecomp.log. It is truncated
on each launch and keeps the most recent 512 KiB by default. Use
LOD_LOG_MAX_KB=4096 only when asked for a larger debug log.
For portable mode, create an empty portable.txt next to LodRecomp;
config files and saves will stay in that same folder.

This is a fan recompilation runtime; it contains no game assets and
requires your own ROM. Not affiliated with Konami.
