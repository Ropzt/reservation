#include <stdio.h>

/* --- déclaration des variables globales --- */

/* --- déclarations préliminaires --- */

/* =========================== */
/* === Programme principal === */
/* =========================== */

int main()
{

	int choix=-1 ; /* valeur lue au clavier (choix utilisateur) */

	while (choix != 0)
	{
		printf("\n-1- Réserver\n") ;
		printf("-2- Consulter les horaires\n") ;
		printf("-3- Mes réservations\n") ;
		printf("-0- Quitter\n");
		printf("\nChoix : ") ;
		scanf("%d",&choix) ;

		switch (choix)
		{
			case 1:	printf("à faire : réserver (=consulter (horaires + tarifs) + réserver)\n") ;
							break ;
			case 2:	printf("à faire : consulter les horaires\n") ;
							break ;
			case 3: printf("à faire : mes billets\n");
							break ;
		} /* Fin du switch */
	} /* Fin du while */
} /* Fin du main */