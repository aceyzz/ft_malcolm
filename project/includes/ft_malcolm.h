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

# include "libft.h"
# include "colors.h"

# define DEBUG 0

void	exit_msg(char *msg, int fd, int exit_code);
void	checks(int argc, char **argv);

/* validations.c */
bool	ft_ishex(char c);
bool	is_valid_ip(const char *ip);
bool	is_valid_mac(const char *mac);

/* test.c */
void	test_args(void);

/* exit.c */
void	exit_msg(char *msg, int fd, int exit_code);

#endif
