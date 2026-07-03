import json, sys, re

def demangle(n):
    # ".?AVCRTModSet@@" -> "CRTModSet"; keep templates readable-ish
    n = n[4:] if n.startswith((".?AV", ".?AU")) else n
    return n.rstrip("@")

a = json.load(open(sys.argv[1]))
b = json.load(open(sys.argv[2]))
names = set(a) | set(b)
rows = []
for n in names:
    c1 = a.get(n, {}).get("off0", 0)
    c2 = b.get(n, {}).get("off0", 0)
    if c2 - c1 != 0:
        rows.append((c2 - c1, c1, c2, demangle(n)))
rows.sort(key=lambda r: -r[0])
print(f"{'delta':>9} {'dump1':>9} {'dump2':>9}  class   (dumps ~2.9 min apart)")
for d, c1, c2, n in rows[:45]:
    print(f"{d:>+9} {c1:>9} {c2:>9}  {n}")
print("...")
for d, c1, c2, n in rows[-10:]:
    print(f"{d:>+9} {c1:>9} {c2:>9}  {n}")
