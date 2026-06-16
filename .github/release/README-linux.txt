Castlevania: Legacy of Darkness Recompiled — Linux x64
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
is required — the renderer and the F1 settings overlay both use Vulkan.

Hotkeys
-------
F1   Show/hide the settings overlay
F11  Toggle windowed/fullscreen
F5   Cycle internal resolution (Original, Original2x, Auto)
F6   Cycle aspect ratio (Original, Expand, Manual)
F7   Cycle antialiasing (None, MSAA2X, MSAA4X, MSAA8X)
F8   Cycle refresh-rate mode (Original, Display, Manual)

Keyboard: Enter = A, Right Shift = B, Z = Z, Space = Start,
arrows = D-pad, WASD = analog stick. SDL game controllers are
supported; see the repository README for bindings and controls.json.

Config files are stored in ~/.lodrecomp.
For portable mode, create an empty portable.txt next to LodRecomp;
config files and saves will stay in that same folder.

This is a fan recompilation runtime; it contains no game assets and
requires your own ROM. Not affiliated with Konami.
