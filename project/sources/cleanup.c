#include "ft_malcolm.h"

void	cleanup(t_data *data)
{
	if (!data)
		return;
	if (data->interface.socket_fd != -1)
	{
		int rc = close(data->interface.socket_fd);
		check(rc != -1, "Socket closed successfully\n", "Failed to close socket\n", NULL);
		data->interface.socket_fd = -1;
	}
}
