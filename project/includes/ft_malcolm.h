#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <limits.h>
# include <ctype.h>
# include <string.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdint.h>
# include <ifaddrs.h>
# include <arpa/inet.h>
# include <net/if.h>
# include <netinet/in.h>
# include <linux/if_packet.h>
# include <time.h>

# include "libft.h"
# include "colors.h"
# include "structs.h"

# define DEBUG 1

/* check.c */
int		checks(int argc, char **argv);

/* validations.c */
bool	ft_ishex(char c);
bool	is_valid_ip(const char *ip);
bool	is_valid_mac(const char *mac);
bool	is_local_ip(const char *ip);

/* exit.c */
void	exit_msg(char *msg, char *added, int fd, int exit_code);

#endif
