"""Toggle the 0xE86AA0 patch in the live process. Usage: livepatch.py <pid> orig|stub"""
import sys, ctypes
from ctypes import wintypes

PID = int(sys.argv[1])
mode = sys.argv[2]
ADDR = 0x00E86AA0
ORIG = bytes([0x6A, 0xFF, 0x68, 0x80, 0x08])
STUB = bytes([0x31, 0xC0, 0xC2, 0x08, 0x00])
data = ORIG if mode == "orig" else STUB

k32 = ctypes.windll.kernel32
PROCESS_ALL_ACCESS = 0x1F0FFF
h = k32.OpenProcess(PROCESS_ALL_ACCESS, False, PID)
assert h, "OpenProcess failed"

old = wintypes.DWORD(0)
ok = k32.VirtualProtectEx(h, ctypes.c_void_p(ADDR), 5, 0x40, ctypes.byref(old))
assert ok, "VirtualProtectEx failed"

buf = ctypes.create_string_buffer(5)
n = ctypes.c_size_t(0)
k32.ReadProcessMemory(h, ctypes.c_void_p(ADDR), buf, 5, ctypes.byref(n))
print("before:", buf.raw.hex(" "))

ok = k32.WriteProcessMemory(h, ctypes.c_void_p(ADDR), data, 5, ctypes.byref(n))
assert ok and n.value == 5, "WriteProcessMemory failed"
k32.FlushInstructionCache(h, ctypes.c_void_p(ADDR), 5)

k32.ReadProcessMemory(h, ctypes.c_void_p(ADDR), buf, 5, ctypes.byref(n))
print("after: ", buf.raw.hex(" "))
k32.VirtualProtectEx(h, ctypes.c_void_p(ADDR), 5, old.value, ctypes.byref(old))
k32.CloseHandle(h)
