#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <limits.h>
# include <ctype.h>
# include <string.h>
# include <stdbool.h>
# include <stdint.h>
# include <ifaddrs.h>
# include <arpa/inet.h>
# include <net/if.h>
# include <netinet/in.h>
# include <linux/if_packet.h>
# include <linux/if_ether.h>
# include <linux/if_arp.h>
# include <time.h>

# include "libft.h"
# include "colors.h"
# include "structs.h"

# define DEBUG 1

/* check.c */
int		check(int valid_condition, const char *ok_msg, const char *ko_msg, const char *val1);
int		first_checks(int argc, char **env);

/* validations.c */
bool	ft_ishex(char c);
bool	is_local_ip(const char *ip);

/* initializer.c */
int		init_data(t_data *data, char **argv);

/* interface.c */
int		iface_from_ip(const char *ip_str, t_interface *iface);

/* debug.c */
void	debug_print_data(t_data *data);
void	debug_print_args(t_args *args);
void	debug_print_interface(t_interface *iface);
void	debug_print_request(t_request *req);

#endif
