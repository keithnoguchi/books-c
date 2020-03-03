/* SPDX-License-Identifier: GPL-2.0 */
#define KBUILD_MODNAME "drop_tcp_or_rewrite"
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>

int drop_tcp_or_rewrite(struct xdp_md *ctx)
{
	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;
	struct ethhdr *eth = data;
	struct iphdr *ip;
	struct tcphdr *tcp;
	int ipsize = 0;

	ipsize = sizeof(*eth);
	ip = data + ipsize;
	ipsize += sizeof(*ip);
	if (data + ipsize > data_end)
		return XDP_DROP;
	if (ip->protocol != IPPROTO_TCP)
		return XDP_PASS;

	tcp = (struct tcphdr *)(ip + 1);
	if ((void *)(tcp + 1) > data_end)
		return XDP_DROP;
	if (tcp->dest != htons(9090))
		return XDP_DROP;

	/* rewrite a DA and forward it */
	eth->h_dest[0] = 0x08;
	eth->h_dest[1] = 0x00;
	eth->h_dest[2] = 0x27;
	eth->h_dest[3] = 0xdd;
	eth->h_dest[4] = 0x38;
	eth->h_dest[5] = 0x2a;
	return XDP_TX;
}
