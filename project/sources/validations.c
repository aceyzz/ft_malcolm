#include "ft_malcolm.h"

bool	ft_ishex(char c)
{
	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

bool	is_local_ip(const char *ip)
{
	struct ifaddrs	*ifaddr, *ifa;
	char			buf[INET_ADDRSTRLEN];

	if (getifaddrs(&ifaddr) == -1)
		return (false);
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET)
			continue;
		if (inet_ntop(AF_INET, &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr,
				buf, sizeof(buf)) == NULL)
			continue;
		if (ft_strcmp(buf, ip) == 0)
		{
			freeifaddrs(ifaddr);
			return (true);
		}
	}
	freeifaddrs(ifaddr);
	return (false);
}
