#include "ft_malcolm.h"

static int	check_and_report(int valid_condition, const char *ok_msg, const char *ko_msg, const char *val)
{
	if (!valid_condition)
	{
		dprintf(STDERR_FILENO, REDD "[ERROR] " RST);
		dprintf(STDERR_FILENO, ko_msg, val);
		return (EXIT_FAILURE);
	}
	if (DEBUG && ok_msg)
	{
		dprintf(STDOUT_FILENO, LIME "[DEBUG] " RST);
		dprintf(STDOUT_FILENO, ok_msg, val);
	}
	return (EXIT_SUCCESS);
}

static int	check(int valid_condition, const char *ok_msg, const char *ko_msg, const char *val1)
{
	return (check_and_report(valid_condition, ok_msg, ko_msg, val1));
}

int	checks(int argc, char **argv)
{
	if (check(getuid() == 0,
			"Running as root\n",
			"This program must be run as root\n",
			NULL))
		return (EXIT_FAILURE);

	if (check(argc == 5,
			"Correct number of arguments\n",
			"Invalid number of arguments -> Usage: ./ft_malcolm <IP source> <MAC source> <IP target> <MAC target>\n",
			NULL))
		return (EXIT_FAILURE);

	if (check(is_valid_ip(argv[1]),
			"Valid IP source address format (" LIME "%s" RST ")\n",
			"Invalid IP source address format (" REDD "%s" RST ")\n",
			argv[1]))
		return (EXIT_FAILURE);

	if (check(is_valid_ip(argv[3]),
			"Valid IP target address format (" LIME "%s" RST ")\n",
			"Invalid IP target address format (" REDD "%s" RST ")\n",
			argv[3]))
		return (EXIT_FAILURE);

	if (check(is_valid_mac(argv[2]),
			"Valid MAC source address format (" LIME "%s" RST ")\n",
			"Invalid MAC source address format (" REDD "%s" RST ")\n",
			argv[2]))
		return (EXIT_FAILURE);

	if (check(is_valid_mac(argv[4]),
			"Valid MAC target address format (" LIME "%s" RST ")\n",
			"Invalid MAC target address format (" REDD "%s" RST ")\n",
			argv[4]))
		return (EXIT_FAILURE);

	if (check(is_local_ip(argv[1]),
			"IP source address belongs to a local interface (" LIME "%s" RST ")\n",
			"IP source address does not belong to any local interface (" REDD "%s" RST ")\n",
			argv[1]))
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}
