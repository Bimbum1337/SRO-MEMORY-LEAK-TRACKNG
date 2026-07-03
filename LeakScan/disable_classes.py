"""
Experimental live patcher: force the creation factories of CRTModSet /
CRTModSound / CEFUpdater to return NULL, so those classes are never
instantiated. Fully reversible in RAM (does not touch the exe on disk).

Usage:
  python disable_classes.py <pid> modset|modsound|cefupdater|all  off   # apply NULL-return patch
  python disable_classes.py <pid> modset|modsound|cefupdater|all  on    # restore original bytes

Choke points (verified via IDA + RTTI vtable mapping, SRO_Client base 0x400000):
  CRTModSet    factory  sub_E83110  (else-branch already returns 0 -> callers null-safe)
  CRTModSound  factory  sub_E7FA90  (naturally returns 0 on alloc fail -> callers null-safe)
  CEFUpdater   factory  sub_EC86B0  (result stored w/o null check -> HIGH crash risk)
"""
import sys, ctypes
from ctypes import wintypes

TARGETS = {
    #            addr        original bytes            null-return patch
    "modset":    (0x00E83110, b"\x6A\xFF\x68",         b"\x31\xC0\xC3"),
    "modsound":  (0x00E7FA90, b"\x6A\xFF\x68",         b"\x31\xC0\xC3"),
    "cefupdater":(0x00EC86B0, b"\x53\x55\x56\x57\x8B", b"\x31\xC0\xC2\x04\x00"),
}

def patch(pid, name, apply_null):
    addr, orig, stub = TARGETS[name]
    data = stub if apply_null else orig
    n = len(orig)  # write region size = length of original sequence we know
    if apply_null and len(stub) < len(orig):
        data = stub + orig[len(stub):]  # keep trailing original bytes intact
    k32 = ctypes.windll.kernel32
    h = k32.OpenProcess(0x1F0FFF, False, pid)
    assert h, "OpenProcess failed (run as admin)"
    old = wintypes.DWORD(0)
    assert k32.VirtualProtectEx(h, ctypes.c_void_p(addr), n, 0x40, ctypes.byref(old)), "VP failed"
    buf = ctypes.create_string_buffer(n); cnt = ctypes.c_size_t(0)
    k32.ReadProcessMemory(h, ctypes.c_void_p(addr), buf, n, ctypes.byref(cnt))
    before = buf.raw
    assert k32.WriteProcessMemory(h, ctypes.c_void_p(addr), data, len(data), ctypes.byref(cnt)) and cnt.value == len(data), "write failed"
    k32.FlushInstructionCache(h, ctypes.c_void_p(addr), len(data))
    k32.ReadProcessMemory(h, ctypes.c_void_p(addr), buf, n, ctypes.byref(cnt))
    after = buf.raw
    k32.VirtualProtectEx(h, ctypes.c_void_p(addr), n, old.value, ctypes.byref(old))
    k32.CloseHandle(h)
    print(f"  {name:10s} @0x{addr:X}  before={before.hex(' ')}  after={after.hex(' ')}")

def main():
    pid = int(sys.argv[1]); which = sys.argv[2]; state = sys.argv[3]
    apply_null = (state == "off")
    names = list(TARGETS) if which == "all" else [which]
    print(f"{'DISABLE (NULL)' if apply_null else 'RESTORE'} -> {names}")
    for nm in names:
        patch(pid, nm, apply_null)

if __name__ == "__main__":
    main()
