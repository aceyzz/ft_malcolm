#include "ft_malcolm.h"

/*
	exemple : "01:23:45:67:89:ab"
	resultat : 0x01, 0x23, 0x45, 0x67, 0x89, 0xab
*/
static int	parse_hex_byte(const char *s, uint8_t *byte)
{
	int v = 0;
	if (!ft_ishex(s[0]) || !ft_ishex(s[1]))
		return (1);
	for (int i = 0; i < 2; i++)
	{
		char c = ft_tolower(s[i]);
		int n = (c >= '0' && c <= '9') ? c - '0' : c - 'a' + 10;
		v = (v << 4) | n; // decale de 4 bits et ajoute la valeur pour le chiffre courant, ex "1A" -> 0x1A
	}
	*byte = (uint8_t)v;
	return (0);
}

static int	parse_mac(const char *src, mac_addr dst)
{
	int i = 0;
	if (!src)
		return (1);
	while (i < 6)
	{
		if (parse_hex_byte(src, &dst[i]))
			return (1);
		src += 2;
		if (i < 5)
		{
			if (*src != ':')
				return (1);
			src++;
		}
		i++;
	}
	return (*src != '\0');
}

static int	parse_ipv4(const char *src, ipv4 *dst)
{
	if (!src || !dst)
		return (1);
	return (inet_pton(AF_INET, src, dst) != 1);
}

static int	init_args(t_args *args, char **argv)
{
	ft_memset(args, 0, sizeof(*args));
	ft_strlcpy(args->ip_source_str, argv[1], IP_STR_LEN);
	ft_strlcpy(args->mac_source_str, argv[2], MAC_STR_LEN);
	ft_strlcpy(args->ip_target_str, argv[3], IP_STR_LEN);
	ft_strlcpy(args->mac_target_str, argv[4], MAC_STR_LEN);

	if (check(parse_ipv4(argv[1], &args->ip_source) == 0,
		"Source IPv4 parsed successfully: (" LIME "%s" RST ")\n",
		"Invalid source IPv4 address: (" REDD "%s" RST ")\n", argv[1]) != EXIT_SUCCESS)
		return (1);

	if (check(parse_ipv4(argv[3], &args->ip_target) == 0,
		"Target IPv4 parsed successfully: (" LIME "%s" RST ")\n",
		"Invalid target IPv4 address: (" REDD "%s" RST ")\n", argv[3]) != EXIT_SUCCESS)
		return (1);

	if (check(parse_mac(argv[2], args->mac_source) == 0,
		"Source MAC parsed successfully: (" LIME "%s" RST ")\n",
		"Invalid source MAC address: (" REDD "%s" RST ")\n", argv[2]) != EXIT_SUCCESS)
		return (1);

	if (check(parse_mac(argv[4], args->mac_target) == 0,
		"Target MAC parsed successfully: (" LIME "%s" RST ")\n",
		"Invalid target MAC address: (" REDD "%s" RST ")\n", argv[4]) != EXIT_SUCCESS)
		return (1);

	if (check(is_local_ip(args->ip_source_str),
		"IP source address belongs to a local interface (" LIME "%s" RST ")\n",
		"IP source address does not belong to any local interface (" REDD "%s" RST ")\n",
		args->ip_source_str) != EXIT_SUCCESS)
		return (1);

	if (check(args->ip_source != args->ip_target,
		"Source and target IP addresses are different\n",
		"Source and target IP addresses must be different (" REDD "%s" RST ")\n",
		args->ip_source_str) != EXIT_SUCCESS)
		return (1);

	args->validated = true;
	return (0);
}

static void	init_interface(t_interface *iface)
{
	ft_memset(iface, 0, sizeof(*iface));
	iface->socket_fd = -1;
	iface->if_index = 0;
	iface->if_ip = 0;
	ft_memset(iface->if_name, 0, IFNAMSIZ);
	ft_memset(iface->if_mac, 0, MAC_BIN_LEN);
	ft_memset(&iface->sll, 0, sizeof(iface->sll));
	iface->sll.sll_family = AF_PACKET;
	iface->sll.sll_protocol = htons(ETH_P_ARP);
	iface->sll.sll_hatype = ARPHRD_ETHER;
	iface->sll.sll_pkttype = 0;
	iface->sll.sll_halen = MAC_BIN_LEN;
	iface->sll.sll_ifindex = 0;
	ft_memset(iface->sll.sll_addr, 0, MAC_BIN_LEN);
}

static void	init_request(t_request *req)
{
	ft_memset(req, 0, sizeof(*req));
	req->timestamp = 0;
	req->raw_len = 0;
	req->eth_dst_broadcast = 0;
	ft_memset(req->sender_mac, 0, MAC_BIN_LEN);
	req->sender_ip = 0;
}

int	init_data(t_data *data, char **argv)
{
	if (!data || !argv)
		return (1);
	ft_memset(data, 0, sizeof(*data));
	if (init_args(&data->args, argv))
		return (1);
	init_interface(&data->interface);
	init_request(&data->request);
	data->already_sent = false;
	data->timeout_seconds = 0;
	data->start_time = time(NULL);

	if (DEBUG)
		debug_print_data(data);
	
	return (0);
}
