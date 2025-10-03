#include "ft_malcolm.h"

/*
	argv[1] : ip source
	argv[2] : mac source
	argv[3] : ip target
	argv[4] : mac target
*/

int main(int argc, char **argv)
{
	// verifications initiales
	if (checks(argc, argv))
		return (1);
	// init des structure de datas (a definir)
	// gestion des signaux
	// lancement du programme (boucle de wait requete ARP)
	// nettoyage et sortie

	printf("OK Brother\n");
    return (0);
}
