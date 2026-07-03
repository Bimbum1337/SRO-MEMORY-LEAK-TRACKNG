"""
Minidump RTTI vtable instance counter (x86 MSVC).

Parses a full minidump of SRO_Client, discovers all vtables via RTTI
CompleteObjectLocators inside the main module image, then counts how many
times each vtable address appears (4-aligned) across all non-module memory
(i.e. heaps/stacks). Diffing two snapshots names the leaking classes.

Usage: python vtscan.py <dump.dmp> <out.csv>
"""
import sys, struct, json
import numpy as np

MDMP = b"MDMP"
ModuleListStream = 4
Memory64ListStream = 9

class FileView:
    """Slice-only view over a file, avoids mmap (32-bit address space)."""
    def __init__(self, f):
        self.f = f
    def __getitem__(self, sl):
        self.f.seek(sl.start)
        return self.f.read(sl.stop - sl.start)
    def unpack(self, fmt, off):
        self.f.seek(off)
        return struct.unpack(fmt, self.f.read(struct.calcsize(fmt)))

def parse(path):
    m = FileView(open(path, "rb"))
    magic, ver, nstreams, dir_rva = m.unpack("<4sIII", 0)
    assert magic == MDMP, "not a minidump"
    streams = {}
    for i in range(nstreams):
        st, sz, rva = m.unpack("<III", dir_rva + 12 * i)
        streams.setdefault(st, []).append((sz, rva))
    return m, streams

def read_mstring(m, rva):
    (ln,) = m.unpack("<I", rva)
    return m[rva + 4 : rva + 4 + ln].decode("utf-16-le", "replace")

def get_modules(m, streams):
    sz, rva = streams[ModuleListStream][0]
    (n,) = m.unpack("<I", rva)
    mods = []
    off = rva + 4
    for i in range(n):
        base, size, cksum, ts = m.unpack("<QIII", off)
        (name_rva,) = m.unpack("<I", off + 20)
        name = read_mstring(m, name_rva)
        mods.append((base, size, name))
        off += 108
    return mods

def get_mem64(m, streams):
    sz, rva = streams[Memory64ListStream][0]
    n, base_rva = m.unpack("<QQ", rva)
    regions = []
    off = rva + 16
    file_off = base_rva
    for i in range(n):
        start, dsize = m.unpack("<QQ", off)
        regions.append((start, dsize, file_off))
        file_off += dsize
        off += 16
    return regions

def read_va(m, regions, va, size):
    """Read bytes at virtual address va (must be within one region)."""
    for start, dsize, foff in regions:
        if start <= va and va + size <= start + dsize:
            o = foff + (va - start)
            return m[o : o + size]
    return None

def main():
    dump_path, out_path = sys.argv[1], sys.argv[2]
    m, streams = parse(dump_path)
    mods = get_modules(m, streams)
    regions = get_mem64(m, streams)
    regions_sorted = sorted(regions)

    # main module = the .exe with sro/client in name, else first module
    main_mod = None
    for base, size, name in mods:
        if "sro_client" in name.lower():
            main_mod = (base, size, name)
            break
    if main_mod is None:
        main_mod = mods[0]
    mbase, msize, mname = main_mod
    print(f"main module: {mname} base=0x{mbase:X} size=0x{msize:X}")

    # pull whole module image into one contiguous buffer
    img = bytearray(msize)
    for start, dsize, foff in regions:
        if start >= mbase and start + dsize <= mbase + msize:
            img[start - mbase : start - mbase + dsize] = m[foff : foff + dsize]
    img = bytes(img)

    lo, hi = mbase, mbase + msize
    arr = np.frombuffer(img[: len(img) & ~3], dtype="<u4")
    # candidate COL pointers: dwords pointing into module
    in_mod = (arr >= lo) & (arr < hi)
    idxs = np.nonzero(in_mod)[0]
    print(f"{len(idxs)} in-module dword pointers to test")

    def u32(off):
        if off < 0 or off + 4 > len(img):
            return None
        return struct.unpack_from("<I", img, off)[0]

    vtables = {}   # va -> (class_name, col_offset)
    col_cache = {}  # col va -> (name, offset) or None
    for i in idxs:
        col = int(arr[i])
        res = col_cache.get(col)
        if res is None and col not in col_cache:
            res = None
            co = col - mbase
            sig = u32(co)
            offset = u32(co + 4)
            ptd = u32(co + 12)
            if sig == 0 and offset is not None and offset < 0x10000 and ptd is not None and lo <= ptd < hi:
                nameoff = ptd - mbase + 8
                nm = img[nameoff : nameoff + 256]
                if nm[:4] == b".?AV" or nm[:4] == b".?AU":
                    end = nm.find(b"\0")
                    if end > 4:
                        res = (nm[:end].decode("ascii", "replace"), offset)
            col_cache[col] = res
        if res:
            vt_va = mbase + int(i) * 4 + 4
            # vtable slot 0 must point into module (code)
            slot0 = u32(vt_va - mbase)
            if slot0 is not None and lo <= slot0 < hi:
                vtables[vt_va] = res
    print(f"{len(vtables)} RTTI vtables found")

    # count vtable occurrences in all memory OUTSIDE any module image
    mod_ranges = sorted((b, b + s) for b, s, _ in mods)
    def in_any_module(start, end):
        for b, e in mod_ranges:
            if start < e and end > b:
                return True
        return False

    vt_arr = np.array(sorted(vtables.keys()), dtype="<u4")
    counts = np.zeros(len(vt_arr), dtype=np.int64)
    scanned = 0
    for start, dsize, foff in regions_sorted:
        if in_any_module(start, start + dsize):
            continue
        scanned += dsize
        CH = 1 << 26
        for coff in range(0, dsize, CH):
            csz = min(CH, dsize - coff) & ~3
            if csz <= 0:
                continue
            buf = m[foff + coff : foff + coff + csz]
            a = np.frombuffer(buf, dtype="<u4")
            # cheap range prefilter then exact match
            cand = a[(a >= vt_arr[0]) & (a <= vt_arr[-1])]
            if len(cand) == 0:
                continue
            pos = np.searchsorted(vt_arr, cand)
            pos[pos >= len(vt_arr)] = 0
            hitmask = vt_arr[pos] == cand
            hitpos = pos[hitmask]
            if len(hitpos):
                binc = np.bincount(hitpos, minlength=len(vt_arr))
                counts += binc
    print(f"scanned {scanned/1e6:.0f} MB of non-module memory")

    # aggregate per class (offset-0 subobject counts = true instance counts)
    per_class = {}
    for va, cnt in zip(vt_arr, counts):
        name, off = vtables[int(va)]
        d = per_class.setdefault(name, {"total": 0, "off0": 0, "vts": {}})
        d["total"] += int(cnt)
        if off == 0:
            d["off0"] += int(cnt)
        if cnt:
            d["vts"][f"0x{int(va):X}"] = int(cnt)
    with open(out_path, "w") as f:
        json.dump(per_class, f)
    top = sorted(per_class.items(), key=lambda kv: -kv[1]["off0"])[:40]
    print(f"{'count(off0)':>12} {'total':>8}  class")
    for name, d in top:
        print(f"{d['off0']:>12} {d['total']:>8}  {name}")

if __name__ == "__main__":
    main()
