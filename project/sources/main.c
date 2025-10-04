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
	// nettoyage et sortie

	printf(LIME "\n[DEBUG] Tout est OK Brother\n\n" RST );
    return (0);
}
