#include "ft_malcolm.h"

int	iface_from_ip(const char *ip_str, t_interface *iface)
{
	struct ifaddrs *ifaddr, *ifa;
	char buf[INET_ADDRSTRLEN];
	char sel_name[IFNAMSIZ] = {0};
	ipv4 ip = 0;
	int mac_ok = 0;

	if (!ip_str || !iface)
		return (1);
	if (inet_pton(AF_INET, ip_str, &ip) != 1)
		return (1);
	if (getifaddrs(&ifaddr) == -1)
		return (1);
	for (ifa = ifaddr; ifa; ifa = ifa->ifa_next)
	{
		if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET)
			continue;
		if (!inet_ntop(AF_INET, &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr, buf, sizeof(buf)))
			continue;
		if (ft_strcmp(buf, ip_str) == 0)
		{
			ft_memset(iface, 0, sizeof(*iface));
			ft_strlcpy(iface->if_name, ifa->ifa_name, IFNAMSIZ);
			iface->if_index = if_nametoindex(ifa->ifa_name);
			iface->if_ip = ip;
			ft_memset(iface->if_mac, 0, MAC_BIN_LEN);
			ft_strlcpy(sel_name, ifa->ifa_name, IFNAMSIZ);
			break;
		}
	}
	if (sel_name[0] == '\0')
	{
		freeifaddrs(ifaddr);
		return (1);
	}
	for (ifa = ifaddr; ifa; ifa = ifa->ifa_next)
	{
		if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_PACKET)
			continue;
		if (ft_strcmp(ifa->ifa_name, sel_name) != 0)
			continue;
		struct sockaddr_ll *sll = (struct sockaddr_ll *)ifa->ifa_addr;
		if (sll->sll_halen >= MAC_BIN_LEN)
		{
			ft_memcpy(iface->if_mac, sll->sll_addr, MAC_BIN_LEN);
			mac_ok = 1;
			break;
		}
	}
	freeifaddrs(ifaddr);
	iface->socket_fd = -1;
	ft_memset(&iface->sll, 0, sizeof(iface->sll));
	iface->sll.sll_family = AF_PACKET;
	iface->sll.sll_protocol = htons(ETH_P_ARP);
	iface->sll.sll_halen = MAC_BIN_LEN;
	iface->sll.sll_ifindex = iface->if_index;
	return (mac_ok ? 0 : 1);
}

int	open_iface_socket(t_interface *iface)
{
	int	fd;

	if (!iface)
		return (1);
	fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (fd == -1)
		return (1);
	if (bind(fd, (struct sockaddr *)&iface->sll, sizeof(iface->sll)) == -1)
	{
		close(fd);
		return (1);
	}
	iface->socket_fd = fd;
	if (check(iface->socket_fd != -1,
			"Raw socket opened successfully on interface (" LIME "%s" RST ")\n",
			"Failed to open raw socket on interface (" REDD "%s" RST ")\n",
			iface->if_name) != EXIT_SUCCESS)
	{
		close(fd);
		iface->socket_fd = -1;
		return (1);
	}
	return (0);
}
