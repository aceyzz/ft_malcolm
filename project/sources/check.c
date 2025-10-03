#include "ft_malcolm.h"

static int	check_and_report(int condition, const char *err_fmt, const char *ok_fmt, const char *val1, const char *val2)
{
	if (condition)
	{
		dprintf(STDOUT_FILENO, REDD "[ERROR] " RST);
		dprintf(STDERR_FILENO, err_fmt, val1, val2);
		return (EXIT_FAILURE);
	}
	if (DEBUG && ok_fmt)
	{
		dprintf(STDOUT_FILENO, LIME "[DEBUG] " RST);
		dprintf(STDOUT_FILENO, ok_fmt, val1, val2);
	}
	return (EXIT_SUCCESS);
}

static int	check_user(void)
{
	if (geteuid())
		return check_and_report(1, "This program must be run as root (" REDD "euid = %d" RST ")\n", NULL, (const char *)(intptr_t)geteuid(), NULL);
	return check_and_report(0, NULL, "Running as root (" LIME "euid = %d" RST ")\n", (const char *)(intptr_t)geteuid(), NULL);
}

static int	check_args(int argc)
{
	if (argc != 5)
		return check_and_report(argc != 5, "Invalid number of arguments -> Usage: ./ft_malcolm <IP source> <MAC source> <IP target> <MAC target>\n", NULL, NULL, NULL);
	return check_and_report(0, NULL, "Correct number of arguments (" LIME "%d" RST ")\n", (const char *)(intptr_t)argc, NULL);
}

static int	check_ips(char **argv)
{
	if (!is_valid_ip(argv[1]) || !is_valid_ip(argv[3]))
	{
		if (!is_valid_ip(argv[1]))
			return check_and_report(1, "Invalid IP source address format (" REDD "%s" RST ")\n", NULL, argv[1], NULL);
		return check_and_report(1, "Invalid IP target address format (" REDD "%s" RST ")\n", NULL, argv[3], NULL);
	}
	return check_and_report(0, NULL, "Valid IP address formats (" LIME "%s" RST ", " LIME "%s" RST ")\n", argv[1], argv[3]);
}

static int	check_macs(char **argv)
{
	if (!is_valid_mac(argv[2]) || !is_valid_mac(argv[4]))
	{
		if (!is_valid_mac(argv[2]))
			return check_and_report(1, "Invalid MAC source address format (" REDD "%s" RST ")\n", NULL, argv[2], NULL);
		return check_and_report(1, "Invalid MAC target address format (" REDD "%s" RST ")\n", NULL, argv[4], NULL);
	}
	return check_and_report(0, NULL, "Valid MAC address formats (" LIME "%s" RST ", " LIME "%s" RST ")\n", argv[2], argv[4]);
}

static int	check_local_ip(char **argv)
{
	if (!is_local_ip(argv[1]))
		return check_and_report(1, "IP source address does not belong to any local interface (" REDD "%s" RST ")\n", NULL, argv[1], NULL);
	return check_and_report(0, NULL, "IP source address belongs to a local interface (" LIME "%s" RST ")\n", argv[1], NULL);
}

int	checks(int argc, char **argv)
{
	return (check_user() || check_args(argc) || check_ips(argv) || check_macs(argv) || check_local_ip(argv));
}
