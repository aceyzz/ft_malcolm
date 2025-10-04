#include "ft_malcolm.h"

static int	check_and_report(int valid_condition, const char *ok_msg, const char *ko_msg, const char *val)
{
	if (!valid_condition) {
		dprintf(STDERR_FILENO, REDD "[ERROR] " RST);
		dprintf(STDERR_FILENO, ko_msg ? ko_msg : "Unknown error", val);
		if (errno != 0)
			dprintf(STDERR_FILENO, REDD "(errno) -> %s\n" RST, strerror(errno) ? strerror(errno) : "Unknown error");
		errno = 0;
		return (EXIT_FAILURE);
	}
	if (DEBUG && ok_msg) {
		dprintf(STDOUT_FILENO, LIME "[DEBUG] " RST);
		dprintf(STDOUT_FILENO, ok_msg ? ok_msg : "Unknown message", val);
	}
	return (EXIT_SUCCESS);
}

int	check(int valid_condition, const char *ok_msg, const char *ko_msg, const char *val1)
{
	return (check_and_report(valid_condition, ok_msg, ko_msg, val1));
}

static char *get_env_value(char **env, const char *key)
{
	size_t key_len = ft_strlen(key);
	for (int i = 0; env[i] != NULL; i++) {
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=') {
			return (env[i] + key_len + 1);
		}
	}
	return (NULL);
}

int	first_checks(int argc, char **env)
{
	char *user = get_env_value(env, "USER");

	if (check(getuid() == 0,
			"Sufficient privileges (" LIME "user: %s" RST ")\n",
			"This program must be run as root (" REDD "user: %s" RST ")\n",
			user ? user : "unknown") != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (check(argc == 5,
			"Correct number of arguments\n",
			"Invalid number of arguments -> " REDD "%s" RST,
			"Usage: ./ft_malcolm <IP source> <MAC source> <IP target> <MAC target>\n"))
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}
