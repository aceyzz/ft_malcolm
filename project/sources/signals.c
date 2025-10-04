#include "ft_malcolm.h"

volatile sig_atomic_t	g_stop = 0;

static void	on_signal(int sig)
{
	(void)sig;
	g_stop = 1;
	const char msg[] = "\033[0;38;2;255;255;0m[SIGNAL] Exiting...\n\033[0m";
	write(STDERR_FILENO, msg, sizeof(msg) - 1);
}

int	setup_signals(void)
{
	struct sigaction sa;
	int ret = 0;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = on_signal;
	sigemptyset(&sa.sa_mask);

	ret |= check(sigaction(SIGINT, &sa, NULL) != -1,
		"SIGINT handler set for %s\n", "Failed to set SIGINT handler for %s\n", "SIGINT");
	ret |= check(sigaction(SIGTERM, &sa, NULL) != -1,
		"SIGTERM handler set for %s\n", "Failed to set SIGTERM handler for %s\n", "SIGTERM");
	ret |= check(sigaction(SIGQUIT, &sa, NULL) != -1,
		"SIGQUIT handler set for %s\n", "Failed to set SIGQUIT handler for %s\n", "SIGQUIT");
	ret |= check(sigaction(SIGHUP, &sa, NULL) != -1,
		"SIGHUP handler set for %s\n", "Failed to set SIGHUP handler for %s\n", "SIGHUP");

	return (ret ? 1 : 0);
}
