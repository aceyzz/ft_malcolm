#include "ft_malcolm.h"

static inline void	check_user(void)
{
	if (getuid())
		exit_msg("Error: You must be root to run this program.\n", 2, 1);
}

static inline void	check_args(int argc)
{
	if (argc != 5)
		exit_msg("Error: Invalid number of arguments.\n", 2, 1);
}

static inline void	check_ips(char **argv)
{
	if (!is_valid_ip(argv[1]) || !is_valid_ip(argv[3]))
		exit_msg("Error: Invalid IP address format.\n", 2, 1);
}

static inline void	check_mac(char **argv)
{
	if (!is_valid_mac(argv[2]) || !is_valid_mac(argv[4]))
		exit_msg("Error: Invalid MAC address format.\n", 2, 1);
}

void	checks(int argc, char **argv)
{
	if (DEBUG)
		test_args();
	check_user();
	check_args(argc);
	check_ips(argv);
	check_mac(argv);
	return ;
}
