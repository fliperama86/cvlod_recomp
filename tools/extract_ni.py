#!/usr/bin/env python3
"""
Extract NI (Nisitenma-Ichigo) files from Castlevania: Legacy of Darkness ROM.
Properly decodes CI4 textures and geometry from GBI sub-display lists.

Usage:
    python3 tools/extract_ni.py resources/castlevania_legacy_of_darkness.z64 /tmp/ni_extract/
"""

import sys, os, zlib, struct
from pathlib import Path

NI_TABLE_OFFSET = 0xB1B98

def write_tga(path, rgba, w, h):
    hdr = bytearray(18)
    hdr[2] = 2; hdr[12] = w & 0xFF; hdr[13] = (w >> 8) & 0xFF
    hdr[14] = h & 0xFF; hdr[15] = (h >> 8) & 0xFF; hdr[16] = 32; hdr[17] = 0x28
    bgra = bytearray(w * h * 4)
    for i in range(0, w * h * 4, 4):
        bgra[i] = rgba[i+2]; bgra[i+1] = rgba[i+1]; bgra[i+2] = rgba[i]; bgra[i+3] = rgba[i+3]
    with open(path, 'wb') as f:
        f.write(hdr); f.write(bgra)

def read_ni_table(rom):
    entries = []
    pos = NI_TABLE_OFFSET
    while pos + 3 < len(rom):
        offset = struct.unpack_from('>I', rom, pos)[0]
        if offset == 0: break
        entries.append(offset)
        pos += 4
    return entries

def decompress_ni(rom, offset):
    if offset + 4 >= len(rom): return None
    try: return zlib.decompress(rom[offset + 4:offset + 4 + 512 * 1024])
    except: return None

def find_gbi_regions(data):
    """Find GBI display list regions by scanning for ENDDL (0xDF) and working backwards."""
    VALID_GBI = {0x01, 0x04, 0x05, 0x06, 0x07, 0xD7, 0xD9, 0xDA, 0xDB, 0xDC, 0xDE, 0xDF,
                 0xE2, 0xE3, 0xE4, 0xE5, 0xE7, 0xE8, 0xE9, 0xED,
                 0xF2, 0xF3, 0xF5, 0xF6, 0xF7, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF}
    regions = []
    i = 0
    while i + 7 < len(data):
        op = data[i]
        if op in VALID_GBI and op >= 0xD7:  # Start with a state-setup command
            start = i
            count = 0
            j = i
            while j + 7 < len(data):
                cmd_op = data[j]
                if cmd_op in VALID_GBI or cmd_op == 0x00:
                    count += 1
                    if cmd_op == 0xDF:  # ENDDL
                        if count >= 3:
                            regions.append((start, j + 8))
                        j += 8
                        break
                    j += 8
                else:
                    break
            i = j
        else:
            i += 8
    return regions

def extract_geometry_from_gbi(data, regions):
    """Extract geometry from GBI regions."""
    all_verts = []
    faces = []
    vtx_buf = [None] * 64

    for start, end in regions:
        i = start
        while i + 7 < end and i + 7 < len(data):
            w0 = struct.unpack_from('>I', data, i)[0]
            w1 = struct.unpack_from('>I', data, i + 4)[0]
            op = (w0 >> 24) & 0xFF

            if op == 0x01:  # VTX
                n = (w0 >> 12) & 0xFF
                v0_plus_n = (w0 >> 1) & 0x7F
                v0 = v0_plus_n - n
                addr = w1 & 0xFFFFFF
                if addr + n * 16 <= len(data):
                    for j in range(n):
                        o = addr + j * 16
                        x, y, z = struct.unpack_from('>hhh', data, o)
                        slot = (v0 + j) % 64
                        idx = len(all_verts)
                        all_verts.append((x, y, z))
                        vtx_buf[slot] = idx

            elif op == 0x05:  # TRI1
                i0 = ((w0 >> 16) & 0xFF) // 2
                i1 = ((w0 >> 8) & 0xFF) // 2
                i2 = (w0 & 0xFF) // 2
                if all(0 <= x < 64 and vtx_buf[x] is not None for x in [i0, i1, i2]):
                    faces.append((vtx_buf[i0], vtx_buf[i1], vtx_buf[i2]))

            elif op == 0x06:  # TRI2
                for tri in [((w0>>16)&0xFF, (w0>>8)&0xFF, w0&0xFF),
                            ((w1>>16)&0xFF, (w1>>8)&0xFF, w1&0xFF)]:
                    i0, i1, i2 = tri[0]//2, tri[1]//2, tri[2]//2
                    if all(0 <= x < 64 and vtx_buf[x] is not None for x in [i0, i1, i2]):
                        faces.append((vtx_buf[i0], vtx_buf[i1], vtx_buf[i2]))

            i += 8

    return all_verts, faces

def extract_textures_from_file(data, file_idx, out_dir):
    """Extract textures from NI file using GBI SETTIMG info + brute-force CI4 decode."""
    regions = find_gbi_regions(data)
    textures = []

    # Collect SETTIMG references from GBI
    timg_info = []
    for start, end in regions:
        i = start
        cur_timg = None
        cur_width = 0
        cur_fmt = 0
        cur_siz = 0
        while i + 7 < end and i + 7 < len(data):
            w0 = struct.unpack_from('>I', data, i)[0]
            w1 = struct.unpack_from('>I', data, i + 4)[0]
            op = (w0 >> 24) & 0xFF
            if op == 0xFD:  # SETTIMG
                cur_fmt = (w0 >> 21) & 0x7
                cur_siz = (w0 >> 19) & 0x3
                cur_width = (w0 & 0xFFF) + 1
                cur_timg = w1 & 0xFFFFFF
            elif op == 0xF3 and cur_timg is not None:  # LOADBLOCK
                texels = ((w1 >> 12) & 0xFFF) + 1
                timg_info.append({
                    'addr': cur_timg, 'width': cur_width,
                    'fmt': cur_fmt, 'siz': cur_siz, 'texels': texels
                })
                cur_timg = None
            elif op == 0xF9:  # LOADTILE (also loads texture)
                if cur_timg is not None:
                    lrs = ((w1 >> 12) & 0xFFF)
                    lrt = (w1 & 0xFFF)
                    tw = (lrs >> 2) + 1
                    th = (lrt >> 2) + 1
                    timg_info.append({
                        'addr': cur_timg, 'width': tw,
                        'fmt': cur_fmt, 'siz': cur_siz, 'texels': tw * th
                    })
                    cur_timg = None
            i += 8

    # Decode each texture found
    for info in timg_info:
        addr = info['addr']
        w = info['width']
        fmt = info['fmt']
        siz = info['siz']
        texels = info['texels']
        h = max(1, texels // max(w, 1))
        if h < 2 or w < 2: continue
        bpp = [4, 8, 16, 32][min(siz, 3)]
        nbytes = (texels * bpp + 7) // 8
        if addr + nbytes > len(data): continue

        tex_data = data[addr:addr + nbytes]
        rgba = None
        fmt_name = ""

        if fmt == 0 and siz == 2:  # RGBA16
            fmt_name = "rgba16"
            rgba = bytearray(w * h * 4)
            for p in range(min(w*h, len(tex_data)//2)):
                px = struct.unpack_from('>H', tex_data, p*2)[0]
                rgba[p*4]   = ((px >> 11) & 0x1F) * 255 // 31
                rgba[p*4+1] = ((px >> 6) & 0x1F) * 255 // 31
                rgba[p*4+2] = ((px >> 1) & 0x1F) * 255 // 31
                rgba[p*4+3] = 255 if (px & 1) else 0

        elif fmt == 3 and siz == 1:  # IA8
            fmt_name = "ia8"
            rgba = bytearray(w * h * 4)
            for p in range(min(w*h, len(tex_data))):
                intensity = (tex_data[p] >> 4) * 255 // 15
                alpha = (tex_data[p] & 0xF) * 255 // 15
                rgba[p*4] = rgba[p*4+1] = rgba[p*4+2] = intensity
                rgba[p*4+3] = alpha

        elif fmt == 4 and siz == 0:  # I4
            fmt_name = "i4"
            rgba = bytearray(w * h * 4)
            for p in range(min(w*h, len(tex_data)*2)):
                v = (tex_data[p//2] >> (4 if p%2==0 else 0)) & 0xF
                v = v * 255 // 15
                rgba[p*4] = rgba[p*4+1] = rgba[p*4+2] = v; rgba[p*4+3] = 255

        elif siz == 0:  # CI4 — decode as grayscale indices
            fmt_name = "ci4"
            rgba = bytearray(w * h * 4)
            for p in range(min(w*h, len(tex_data)*2)):
                v = (tex_data[p//2] >> (4 if p%2==0 else 0)) & 0xF
                v = v * 255 // 15
                rgba[p*4] = rgba[p*4+1] = rgba[p*4+2] = v; rgba[p*4+3] = 255

        elif siz == 1:  # CI8 — decode as grayscale indices
            fmt_name = "ci8"
            rgba = bytearray(w * h * 4)
            for p in range(min(w*h, len(tex_data))):
                rgba[p*4] = rgba[p*4+1] = rgba[p*4+2] = tex_data[p]; rgba[p*4+3] = 255

        if rgba:
            path = os.path.join(out_dir, f'ni_{file_idx:03d}_{addr:06X}_{w}x{h}_{fmt_name}.tga')
            write_tga(path, rgba, w, h)
            textures.append(path)

    # Also try decoding the file START as a CI4 texture at common widths
    # (many NI files start with texture data before GBI)
    if regions and regions[0][0] > 64:
        tex_end = regions[0][0]
        for w in [16, 32, 64]:
            npix = tex_end * 2  # CI4: 2 pixels per byte
            h = npix // w
            if h < 4 or h > 256: continue
            rgba = bytearray(w * h * 4)
            for p in range(min(w*h, tex_end*2)):
                byte = data[p // 2] if p // 2 < len(data) else 0
                v = (byte >> (4 if p%2==0 else 0)) & 0xF
                v = v * 255 // 15
                rgba[p*4] = rgba[p*4+1] = rgba[p*4+2] = v; rgba[p*4+3] = 255
            path = os.path.join(out_dir, f'ni_{file_idx:03d}_start_ci4_{w}x{h}.tga')
            write_tga(path, rgba, w, h)
            textures.append(path)
            break  # Only one width

    return textures

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <rom.z64> [output_dir]")
        sys.exit(1)

    rom_path = sys.argv[1]
    out_dir = sys.argv[2] if len(sys.argv) > 2 else '/tmp/ni_extract'

    print(f"Loading ROM: {rom_path}")
    with open(rom_path, 'rb') as f:
        rom = f.read()

    entries = read_ni_table(rom)
    print(f"NI table: {len(entries)} entries")

    tex_dir = os.path.join(out_dir, 'textures')
    geo_dir = os.path.join(out_dir, 'geometry')
    raw_dir = os.path.join(out_dir, 'raw')
    os.makedirs(tex_dir, exist_ok=True)
    os.makedirs(geo_dir, exist_ok=True)
    os.makedirs(raw_dir, exist_ok=True)

    seen = set()
    stats = {'files': 0, 'geo_files': 0, 'verts': 0, 'faces': 0, 'textures': 0}

    for idx, offset in enumerate(entries):
        if offset in seen: continue
        seen.add(offset)

        data = decompress_ni(rom, offset)
        if data is None: continue
        stats['files'] += 1

        # Save raw
        with open(os.path.join(raw_dir, f'ni_{idx:03d}.bin'), 'wb') as f:
            f.write(data)

        # Find GBI regions
        regions = find_gbi_regions(data)

        # Extract geometry from GBI
        if regions:
            verts, faces = extract_geometry_from_gbi(data, regions)
            if verts and faces:
                obj_path = os.path.join(geo_dir, f'ni_{idx:03d}.obj')
                with open(obj_path, 'w') as f:
                    f.write(f"# {len(verts)} vertices, {len(faces)} faces\n")
                    for v in verts:
                        f.write(f"v {v[0]} {v[1]} {v[2]}\n")
                    for fa in faces:
                        f.write(f"f {fa[0]+1} {fa[1]+1} {fa[2]+1}\n")
                stats['geo_files'] += 1
                stats['verts'] += len(verts)
                stats['faces'] += len(faces)

        # Extract textures
        tex_files = extract_textures_from_file(data, idx, tex_dir)
        stats['textures'] += len(tex_files)

        if idx % 100 == 0:
            print(f"  {idx}/{len(entries)}... ({stats['geo_files']} models, {stats['textures']} textures)")

    print(f"\n=== Done ===")
    print(f"Files: {stats['files']}")
    print(f"Models: {stats['geo_files']} ({stats['verts']} verts, {stats['faces']} faces)")
    print(f"Textures: {stats['textures']} TGA files")
    print(f"Output: {out_dir}/")

if __name__ == '__main__':
    main()
