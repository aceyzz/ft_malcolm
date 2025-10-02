#include "ft_malcolm.h"

void	exit_msg(char *msg, int fd, int exit_code)
{
	char	*err_msg;

	if (exit_code == 2 && errno)
	{
		err_msg = strerror(errno);
		ft_putstr_fd(REDD "Error (errno): " RST, fd);
		ft_putstr_fd(err_msg, fd);
		ft_putstr_fd("\n", fd);
	}
	ft_putstr_fd(REDD "Error (msg): " RST, fd);
	ft_putstr_fd(msg, fd);
	ft_putstr_fd("\n", fd);
	exit(exit_code);
}
