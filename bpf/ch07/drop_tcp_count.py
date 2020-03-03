# SPDX-License-Identifier: GPL-2.0
#!/usr/bin/python
from bcc import BPF
import time
import sys

device = "lo"
b = BPF(src_file="drop_tcp_count.c")
fn = b.load_func("drop_tcp_count", BPF.XDP)
b.attach_xdp(device, fn, 0)
packetcnt = b.get_table("packetcnt")

prev = [0] * 256
print("Printing packet counts per IP protocol-number, hit CTRL-C to stop")
while 1:
    try:
        for k in packetcnt.keys():
            val = packetcnt.sum(k).value
            i = k.value
            if val:
                delta = val - prev[i]
                prev[i] = val
                if delta > 0:
                    print("{}: {} pkt/s".format(i, delta))
        time.sleep(1)
    except KeyboardInterrupt:
        print("Removing filter from device")
        break

b.remove_xdp(device, 0)
