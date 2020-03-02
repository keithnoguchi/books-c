/* SPDX-License-Identifier: GPL-2.0 */
/*
 * TCP packet drop example.
 *
 * # Examples
 *
 * Load the program against the loopback as below:
 *
 * ```sh
 * $ sudo ip link set lo xdp obj drop_tcp.o sec ch07
 * ```
 *
 * Check it with `ip address` command:
 *
 * ```sh
 * $ ip address show lo
 * 1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 xdpgeneric/id:12 qdisc noqueue state UNKNOWN group default qlen 1000
 *   link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
 *   inet 127.0.0.1/8 scope host lo
 *      valid_lft forever preferred_lft forever
 *   inet6 ::1/128 scope host
 *      valid_lft forever preferred_lft forever
 * ```
 *
 * And run the `nmap` from the other machine, just to see if it's
 * not reporting anything with TCP.
 *
 * ```sh
 * $ sudo nmap -sS 127.0.0.1
 * Starting Nmap 7.80 ( https://nmap.org ) at 2020-03-02 15:18 PST
 * mass_dns: warning: Unable to determine any DNS servers. Reverse DNS is disabled. Try using --system-dns
 * or specify valid servers with --dns-servers
 * ```
 *
 * You should be able to see the result after removing the program:
 *
 * ```sh
 * $ sudo ip link set lo xdp off
 * ```
 *
 * ```sh
 * $ sudo nmap -sS 127.0.0.1
 * Starting Nmap 7.80 ( https://nmap.org ) at 2020-03-02 15:20 PST
 * mass_dns: warning: Unable to determine any DNS servers. Reverse DNS is disabled. Try using --system-dns
 * or specify valid servers with --dns-servers
 * Nmap scan report for localhost.local (127.0.0.1)
 * Host is up (0.0000060s latency).
 * Not shown: 998 closed ports
 * PORT     STATE SERVICE
 * 22/tcp   open  ssh
 * 8000/tcp open  http-alt
 *
 * Nmap done: 1 IP address (1 host up) scanned in 0.08 seconds
 * ```
 */
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/ip.h>

#define SEC(NAME) __attribute__((section(NAME), used))

SEC("ch07")
int drop_tcp(struct xdp_md *ctx)
{
	int ipsize = 0;
	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;
	struct ethhdr *eth = data;
	struct iphdr *ip;

	ipsize = sizeof(*eth);
	ip = data + ipsize;
	ipsize += sizeof(struct iphdr);

	/* too small */
	if (data + ipsize > data_end)
		return XDP_DROP;

	/* drop TCP */
	if (ip->protocol == IPPROTO_TCP)
		return XDP_DROP;

	return XDP_PASS;
}
