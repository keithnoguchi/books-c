/* SPDX-License-Identifier: GPL-2.0 */
#define KBUILD_MODNAME "drop_tcp_count"
#include <linux/bpf.h>
#include <linux/in.h>
#include <linux/ip.h>

BPF_TABLE("percpu_array", uint32_t, long, packetcnt, 256);

int drop_tcp_count(struct xdp_md *ctx)
{
	int ipsize = 0;
	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;
	struct ethhdr *eth = data;
	struct iphdr *ip;
	long *cnt;
	__u32 idx;

	ipsize = sizeof(*eth);
	ip = data + ipsize;
	ipsize += sizeof(*ip);

	if (data + ipsize > data_end)
		return XDP_DROP;

	idx = ip->protocol;
	cnt = packetcnt.lookup(&idx);
	if (cnt)
		*cnt += 1;

	if (ip->protocol == IPPROTO_TCP)
		return XDP_DROP;

	return XDP_PASS;
}
