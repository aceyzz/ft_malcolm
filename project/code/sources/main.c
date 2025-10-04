#include "ft_malcolm.h"

/*
	argv[1] : ip source
	argv[2] : mac source
	argv[3] : ip target
	argv[4] : mac target
*/

int main(int argc, char **argv, char **env)
{
	t_data	data;

	// verifs initiales
	if (first_checks(argc, env))
		return (1);

	// init structs de data - args - interface - request + verifs
	if (init_data(&data, argv))
		return (1);

	// gestion des signaux
	if (setup_signals())
		return (1);

	// lancement du programme (boucle de wait requete ARP)
	while (!g_stop)
	{
		uint8_t buf[2048];
		ssize_t n = recvfrom(data.interface.socket_fd, buf, sizeof(buf), 0, NULL, NULL);
		if (n < 0)
		{
			if (errno == EINTR)
				continue;
			break;
		}
		if (parse_arp_request(buf, n, &data) == 0)
		{
			send_arp_reply(&data);
			data.already_sent = true;
			break;
		}
	}

	// nettoyage et sortie
	cleanup(&data);

	printf(LIME SOUL "\n[END] TOUT EST OK BROTHER\n\n" RST );
    return (0);
}
