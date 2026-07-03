import sys, struct

class FileView:
    def __init__(self, f): self.f = f
    def __getitem__(self, sl):
        self.f.seek(sl.start); return self.f.read(sl.stop - sl.start)
    def unpack(self, fmt, off):
        self.f.seek(off); return struct.unpack(fmt, self.f.read(struct.calcsize(fmt)))

dump, va, n = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
m = FileView(open(dump, "rb"))
magic, ver, nstreams, dir_rva = m.unpack("<4sIII", 0)
for i in range(nstreams):
    st, sz, rva = m.unpack("<III", dir_rva + 12 * i)
    if st == 9:
        cnt, base_rva = m.unpack("<QQ", rva)
        foff = base_rva
        off = rva + 16
        for j in range(cnt):
            start, dsize = m.unpack("<QQ", off)
            if start <= va < start + dsize:
                data = m[foff + (va - start): foff + (va - start) + n]
                print(" ".join(f"{b:02X}" for b in data))
                sys.exit(0)
            foff += dsize; off += 16
print("VA not in dump")
