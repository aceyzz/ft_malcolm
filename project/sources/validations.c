#include "ft_malcolm.h"

bool	ft_ishex(char c)
{
	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

bool	is_valid_ip(const char *ip)
{
	int		num = 0, dots = 0, digits = 0;
	const char *octet_start = ip;

	if (!ip)
		return (false);
	while (*ip)
	{
		if (*ip == '.')
		{
			if (digits == 0)
				return (false);
			if (digits > 1 && *octet_start == '0')
				return (false);
			dots++;
			num = 0;
			digits = 0;
			ip++;
			octet_start = ip;
			continue;
		}
		if (!ft_isdigit(*ip))
			return (false);
		num = num * 10 + (*ip - '0');
		if (num > 255)
			return (false);
		digits++;
		ip++;
	}
	if (digits > 1 && *octet_start == '0')
		return (false);
	return (dots == 3 && digits > 0);
}

bool	is_valid_mac(const char *mac)
{
	int		colons = 0, digits = 0;

	if (!mac)
		return (false);
	while (*mac)
	{
		if (*mac == ':')
		{
			if (digits != 2)
				return (false);
			colons++;
			digits = 0;
			mac++;
			continue;
		}
		if (!ft_ishex(*mac))
			return (false);
		digits++;
		mac++;
	}
	return (colons == 5 && digits == 2);
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
		if (strcmp(buf, ip) == 0)
		{
			freeifaddrs(ifaddr);
			return (true);
		}
	}
	freeifaddrs(ifaddr);
	return (false);
}
