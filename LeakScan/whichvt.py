"""Find which RTTI vtable contains given slot addresses. Usage: whichvt.py dump.dmp addr1 addr2 ..."""
import sys, struct
import numpy as np

class FileView:
    def __init__(self, f): self.f = f
    def __getitem__(self, sl):
        self.f.seek(sl.start); return self.f.read(sl.stop - sl.start)
    def unpack(self, fmt, off):
        self.f.seek(off); return struct.unpack(fmt, self.f.read(struct.calcsize(fmt)))

dump = sys.argv[1]
targets = [int(a, 16) for a in sys.argv[2:]]
m = FileView(open(dump, "rb"))
magic, ver, nstreams, dir_rva = m.unpack("<4sIII", 0)
streams = {}
for i in range(nstreams):
    st, sz, rva = m.unpack("<III", dir_rva + 12 * i)
    streams.setdefault(st, []).append((sz, rva))

# main module
sz, rva = streams[4][0]
(nmod,) = m.unpack("<I", rva)
off = rva + 4
mbase = msize = None
for i in range(nmod):
    base, size, ck, ts = m.unpack("<QIII", off)
    (name_rva,) = m.unpack("<I", off + 20)
    (ln,) = m.unpack("<I", name_rva)
    name = m[name_rva+4:name_rva+4+ln].decode("utf-16-le", "replace")
    if "sro_client" in name.lower():
        mbase, msize = base, size
    off += 108

sz, rva = streams[9][0]
n, base_rva = m.unpack("<QQ", rva)
img = bytearray(msize)
off = rva + 16
foff = base_rva
for i in range(n):
    start, dsize = m.unpack("<QQ", off)
    if start >= mbase and start + dsize <= mbase + msize:
        img[start-mbase:start-mbase+dsize] = m[foff:foff+dsize]
    foff += dsize
    off += 16
img = bytes(img)
lo, hi = mbase, mbase + msize

def u32(o):
    return struct.unpack_from("<I", img, o)[0] if 0 <= o and o+4 <= len(img) else None

arr = np.frombuffer(img[:len(img) & ~3], dtype="<u4")
idxs = np.nonzero((arr >= lo) & (arr < hi))[0]
vts = []  # (start_va, name, col_offset)
cache = {}
for i in idxs:
    col = int(arr[i])
    if col not in cache:
        res = None
        co = col - mbase
        sig, offset = u32(co), u32(co+4)
        ptd = u32(co+12)
        if sig == 0 and offset is not None and offset < 0x10000 and ptd and lo <= ptd < hi:
            nm = img[ptd-mbase+8:ptd-mbase+8+256]
            if nm[:4] in (b".?AV", b".?AU"):
                e = nm.find(b"\0")
                if e > 4: res = (nm[:e].decode("ascii","replace"), offset)
        cache[col] = res
    r = cache[col]
    if r:
        vt = mbase + int(i)*4 + 4
        s0 = u32(vt - mbase)
        if s0 is not None and lo <= s0 < hi:
            vts.append((vt, r[0], r[1]))
vts.sort()
starts = [v[0] for v in vts]
import bisect
for t in targets:
    j = bisect.bisect_right(starts, t) - 1
    if j >= 0:
        vt, name, coloff = vts[j]
        print(f"0x{t:X}: slot {(t-vt)//4} of vtable 0x{vt:X}  class={name}  col_offset={coloff}")
    else:
        print(f"0x{t:X}: no vtable found before it")
