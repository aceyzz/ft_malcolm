#include "ft_malcolm.h"

void	exit_msg(char *msg, char *added, int fd, int exit_code)
{
	ft_putstr_fd(exit_code == 0 ? LIME "OK: " RST : REDD "Error (msg): " RST, fd);
	ft_putstr_fd(msg, fd);
	if (added)
	{
		ft_putstr_fd(" (", fd);
		ft_putstr_fd(exit_code == 0 ? LIME : REDD, fd);
		ft_putstr_fd(added, fd);
		ft_putstr_fd(RST ")", fd);
	}
	ft_putstr_fd("\n", fd);
	exit(exit_code);
}
