#include "ft_malcolm.h"

static int eq_mac(const uint8_t *a, const uint8_t *b)
{
	for (int i = 0; i < 6; i++) if (a[i] != b[i]) return 0;
	return 1;
}

static char*	packet_to_string(const uint8_t *buf, ssize_t len)
{
	static char str[2048];
	ssize_t pos = 0;
	for (ssize_t i = 0; i < len && pos < (ssize_t)(sizeof(str) - 3); i++)
		pos += snprintf(str + pos, sizeof(str) - pos, "%02X ", buf[i]);
	if (pos < (ssize_t)(sizeof(str) - 1))
		str[pos] = '\0';
	else
		str[sizeof(str) - 1] = '\0';
	return (str);
}

int	parse_arp_request(const uint8_t *buf, ssize_t len, t_data *data)
{
	const struct ethhdr *eth;
	const struct arphdr *arp;
	const uint8_t *p;
	ipv4 spa, tpa;

	if (!buf || len < (ssize_t)(sizeof(struct ethhdr) + sizeof(struct arphdr)))
		return 1;
	eth = (const struct ethhdr *)buf;
	if (ntohs(eth->h_proto) != ETH_P_ARP)
		return 1;
	arp = (const struct arphdr *)(buf + sizeof(struct ethhdr));
	if (ntohs(arp->ar_hrd) != ARPHRD_ETHER) return 1;
	if (ntohs(arp->ar_pro) != ETH_P_IP) return 1;
	if (arp->ar_hln != MAC_BIN_LEN) return 1;
	if (arp->ar_pln != 4) return 1;
	if (ntohs(arp->ar_op) != ARPOP_REQUEST) return 1;

	if (len < (ssize_t)(sizeof(struct ethhdr) + sizeof(struct arphdr) + 2*MAC_BIN_LEN + 2*4))
		return 1;

	p = (const uint8_t *)(arp + 1);
	const uint8_t *sha = p;               p += MAC_BIN_LEN;
	const uint8_t *spa_p = p;             p += 4;
	const uint8_t *tha = p;               p += MAC_BIN_LEN; // tha inutile car broadcast donc MAC dst = ff:ff:ff:ff:ff:ff
	(void)tha;
	const uint8_t *tpa_p = p;             p += 4;

	ft_memcpy(&spa, spa_p, 4);
	ft_memcpy(&tpa, tpa_p, 4);

	int is_broadcast = 1;
	for (int i = 0; i < 6; i++) if (eth->h_dest[i] != 0xFF) { is_broadcast = 0; break; }

	if (!is_broadcast) return (1);
	if (tpa != data->args.ip_source) return (1);
	if (spa != data->args.ip_target) return (1);
	if (!eq_mac(sha, data->args.mac_target)) return (1);

	ft_memcpy(data->request.sender_mac, sha, MAC_BIN_LEN);
	data->request.sender_ip = spa;
	data->request.eth_dst_broadcast = 1;
	data->request.raw_len = len;
	data->request.timestamp = time(NULL);

	if (DEBUG)
	{
		char *raw_packet = packet_to_string(buf, len);
		printf(GOLD "[DEBUG] ARP request received:\n  %s\n" RST, raw_packet);
		debug_print_request(&data->request);
	}

	return (check(true, "An ARP request has been broadcast on the network\n", NULL, NULL));
}

int	send_arp_reply(t_data *data)
{
	struct
	{
		struct ethhdr eth;
		struct arphdr ah;
		uint8_t sha[MAC_BIN_LEN];
		uint8_t spa[4];
		uint8_t tha[MAC_BIN_LEN];
		uint8_t tpa[4];
	} __attribute__((packed)) packet;
	struct sockaddr_ll dst;
	ssize_t sent;

	ft_memset(&packet, 0, sizeof(packet));

	ft_memcpy(packet.eth.h_dest, data->args.mac_target, MAC_BIN_LEN);
	ft_memcpy(packet.eth.h_source, data->args.mac_source, MAC_BIN_LEN);
	packet.eth.h_proto = htons(ETH_P_ARP);

	packet.ah.ar_hrd = htons(ARPHRD_ETHER);
	packet.ah.ar_pro = htons(ETH_P_IP);
	packet.ah.ar_hln = MAC_BIN_LEN;
	packet.ah.ar_pln = 4;
	packet.ah.ar_op  = htons(ARPOP_REPLY);

	ft_memcpy(packet.sha, data->args.mac_source, MAC_BIN_LEN);
	ft_memcpy(packet.spa, &data->args.ip_source, 4);
	ft_memcpy(packet.tha, data->args.mac_target, MAC_BIN_LEN);
	ft_memcpy(packet.tpa, &data->args.ip_target, 4);

	dst = data->interface.sll;
	ft_memcpy(dst.sll_addr, data->args.mac_target, MAC_BIN_LEN);

	char *raw_packet = packet_to_string((const uint8_t *)&packet, sizeof(packet));
	sent = sendto(data->interface.socket_fd, &packet, sizeof(packet), 0, (struct sockaddr *)&dst, sizeof(dst));
	return (check(sent == (ssize_t)sizeof(packet), "Sent ARP reply to target\nRaw packet sent:\n  %s\n", "Failed to send ARP reply\nRaw packet:\n  %s\n", raw_packet));
}
