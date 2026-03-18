#!/usr/bin/env python3
"""Visualize N64 vertex data from the game's NI file sub-DL at 0x06008600."""

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np

# 31 vertices from seg6:0x060083C0 (decoded from RDRAM)
# Format: (x, y, z, r, g, b, a)
verts = [
    ( -7, 24,  -7, 129,  63, 125, 255),  #  0
    (-17, 16,   0, 255, 255, 255, 255),  #  1
    ( -8, 16, -14,  80,  80,  80, 255),  #  2
    (  7, 24,   7, 129,  63, 125, 255),  #  3
    ( -8, 16,  14,  80,  80,  80, 255),  #  4
    ( -7, 24,   7, 129,  63, 125, 255),  #  5
    ( -7, 33,   7, 255, 255, 255, 255),  #  6
    ( -7, 33,  -7, 250, 250, 250, 255),  #  7
    (  7, 33,  -7, 110, 110, 110, 255),  #  8
    (  7, 33,   7, 255, 255, 255, 255),  #  9
    ( -7, 24,   7, 129,  63, 125, 255),  # 10
    ( -8, 16,  14,  80,  80,  80, 255),  # 11
    ( 17, 16,   0, 255, 255, 255, 255),  # 12
    (  8, 16,  14,  80,  80,  80, 255),  # 13
    (  7, 24,  -7, 129,  63, 125, 255),  # 14
    (  8, 16, -14,  80,  80,  80, 255),  # 15
    (-17,  0,   0, 255, 255, 255, 255),  # 16
    ( -8,  0,  14,  40,  40,  40, 255),  # 17
    ( -8,  0,  14,  40,  40,  40, 255),  # 18
    (  8,  0,  14,  40,  40,  40, 255),  # 19
    ( 17,  0,   0, 255, 255, 255, 255),  # 20
    (  8, 16, -14,  80,  80,  80, 255),  # 21
    (  7, 24,  -7, 129,  63, 125, 255),  # 22
    (  7, 24,   7, 129,  63, 125, 255),  # 23
    (  7, 33,   7, 255, 255, 255, 255),  # 24
    (  7, 33,  -7, 110, 110, 110, 255),  # 25
    (  7, 24,  -7, 110, 110, 110, 255),  # 26
    ( -7, 24,   7, 110, 110, 110, 255),  # 27
    ( -7, 33,   7, 255, 255, 255, 255),  # 28
    (  7, 33,   7, 255, 255, 255, 255),  # 29
    (  7, 24,   7, 110, 110, 110, 255),  # 30
]

# TRI2 commands from the sub-DL (each TRI2 = 2 triangles)
# Indices are byte-divided: p0(17,7), p0(9,7), p0(1,7) for tri A
#                            p1(17,7), p1(9,7), p1(1,7) for tri B
# But the raw values are vertex_index * 2 in the command encoding.
# Decoded from the sub-DL:
tri2_cmds = [
    # cmd23: 06040200 000A0806
    (0x06040200, 0x000A0806),
    # cmd24: 06100E0C 0012100C
    (0x06100E0C, 0x0012100C),
    # cmd25: 06021614 001A1806
    (0x06021614, 0x001A1806),
    # cmd26: 06181E1C 00160220
    (0x06181E1C, 0x00160220),
    # cmd27: 06221620 001A0824
    (0x06221620, 0x001A0824),
    # cmd28: 06261A24 00181A26
    (0x06261A24, 0x00181A26),
    # cmd29: 06281826 001A0608
    (0x06281826, 0x001A0608),
    # cmd30: 06140002 001C0618
    (0x06140002, 0x001C0618),
    # cmd31: 0604002A 002C2A00
    (0x0604002A, 0x002C2A00),
    # cmd32: 0632302E 0034322E
    (0x0632302E, 0x0034322E),
    # cmd33: 063A3836 003C3A36
    (0x063A3836, 0x003C3A36),
]

def decode_tri2(w0, w1):
    """Decode F3DEX2 TRI2 command into 6 vertex indices (2 triangles)."""
    # TRI2: same as two TRI1 packed in w0 and w1
    # tri A: p0(17,7), p0(9,7), p0(1,7) from w0
    # tri B: p1(17,7), p1(9,7), p1(1,7) from w1
    a0 = (w0 >> 17) & 0x7F
    a1 = (w0 >> 9) & 0x7F
    a2 = (w0 >> 1) & 0x7F
    b0 = (w1 >> 17) & 0x7F
    b1 = (w1 >> 9) & 0x7F
    b2 = (w1 >> 1) & 0x7F
    return [(a0, a1, a2), (b0, b1, b2)]

# Decode all triangles
triangles = []
for w0, w1 in tri2_cmds:
    tris = decode_tri2(w0, w1)
    triangles.extend(tris)

print(f"Total triangles: {len(triangles)}")
for i, (a, b, c) in enumerate(triangles):
    va, vb, vc = verts[a], verts[b], verts[c]
    print(f"  tri[{i:2d}]: v{a:2d}({va[0]:4d},{va[1]:4d},{va[2]:4d}) "
          f"v{b:2d}({vb[0]:4d},{vb[1]:4d},{vb[2]:4d}) "
          f"v{c:2d}({vc[0]:4d},{vc[1]:4d},{vc[2]:4d})")

# 3D visualization
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Draw each triangle as a polygon
polys = []
colors = []
for a, b, c in triangles:
    va, vb, vc = verts[a], verts[b], verts[c]
    poly = [(va[0], va[2], va[1]), (vb[0], vb[2], vb[1]), (vc[0], vc[2], vc[1])]
    polys.append(poly)
    # Average vertex color for face color
    r = (va[3] + vb[3] + vc[3]) / 3 / 255
    g = (va[4] + vb[4] + vc[4]) / 3 / 255
    b_col = (va[5] + vb[5] + vc[5]) / 3 / 255
    colors.append((r, g, b_col, 0.8))

mesh = Poly3DCollection(polys, alpha=0.8)
mesh.set_facecolors(colors)
mesh.set_edgecolor('black')
mesh.set_linewidth(0.3)
ax.add_collection3d(mesh)

# Set axis limits
ax.set_xlim(-20, 20)
ax.set_ylim(-20, 20)
ax.set_zlim(-5, 40)
ax.set_xlabel('X')
ax.set_ylabel('Z')
ax.set_zlabel('Y (up)')
ax.set_title('Castlevania LoD - NI File Model (seg6 sub-DL 0x06008600)')

plt.tight_layout()
plt.savefig('/tmp/lod_model.png', dpi=150)
print(f"\nSaved to /tmp/lod_model.png")
plt.show()
