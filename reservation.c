#include <stdio.h>
#include <string.h>
#include <ctype.h>

// define des tailles de tableaux
#define MAXCALENDAR 30000
#define MAXVOYAGE   30000
#define MAXHORAIRE  30000
#define MAXGARE     3000

// define des longueurs de champs
#define MAXstrROUTE        40
#define MAXstrTRIP         40
#define MAXstrNUMTRAIN     10
#define MAXstrTIME          9
#define MAXstrSTOPHEADSIGN 10
#define MAXstrGAREID       30
#define MAXstrGARENOM      50

#define ERREUR_CHARGEMENT "Erreur de chargement des données\n"
#define NON_TROUVE -1

/* --------------------------- */
/*--- Déclaration des types ---*/
/* --------------------------- */

/*--- Fichier calendar ---*/
	struct typecalendar // une petite variable en enregistrement pour la lecture des fichiers originaux
	{
		int service_id ;
		int monday     ;
		int tuesday    ;
		int wednesday  ;
		int thursday   ;
		int friday     ;
		int saturday   ;
		int sunday     ;
		int start_date ;
		int end_date   ;
	} acalendar ;

	struct typecalendrier
	{
		int service_id ;
		int lundi      ;
		int mardi      ;
		int mercredi   ;
		int jeudi      ;
		int vendredi   ;
		int samedi     ;
		int dimanche   ;
	} ;

/*--- Fichier trips ---*/
	struct typetrips
	{
		char route_id[MAXstrROUTE]         ;
		int  service_id                    ;
		char trip_id[MAXstrTRIP]           ;
		char trip_headsign[MAXstrNUMTRAIN] ;
		int  direction_id                  ;
		int  block_id                      ;
		int  shape_id                      ;
	} atrip ;

	struct typevoyage
	{
		int  service_id                    ;
		char trip_id[MAXstrTRIP]           ;
		char num_train[MAXstrNUMTRAIN] ;
	} ;

/*--- Fichier stop_times ---*/
	struct typestoptimes
	{
		char trip_id[MAXstrTRIP]               ;
		char arrival_time[MAXstrTIME]          ;
		char departure_time[MAXstrTIME]        ;
		char stop_id[MAXstrGAREID]             ;
		int  stop_sequence                     ;
		char stop_headsign[MAXstrSTOPHEADSIGN] ;
		int  pickup_type                       ;
		int  drop_off_type                     ;
		int  shape_dist_traveled               ;
	} astoptime ;

	struct typehoraire
	{
		char trip_id[MAXstrTRIP]   ;
		char heure_arr[MAXstrTIME] ;
		char heure_dep[MAXstrTIME] ;
		char gare_id[MAXstrGAREID] ;
	} ;

/*--- Fichier stops ---*/
	struct typestops
	{
		char  stop_id[MAXstrGAREID]        ;
		char  stop_name[MAXstrGARENOM]  ;
		int   stop_desc                  ;
		float stop_lat                   ;
		float stop_lon                   ;
		int   zone_id                    ;
		int   stop_url                   ;
		int   location_type              ;
		char  parent_station[MAXstrGAREID] ;
	} astop ;

	struct typegare
	{
		char  gare_id[MAXstrGAREID]      ;
		char  gare_nom[MAXstrGARENOM] ;
	} ;

/* ---------------------------------------- */
/*--- Déclaration des variables globales ---*/
/* ---------------------------------------- */
char trips[MAXVOYAGE] ; // à partir de la table trips // À REVOIR
int nbcalendar,nbvoyages,nbhoraires,nbgares ;

/* --- tableaux de données --- */
struct typecalendrier calendrier[MAXCALENDAR] ;
struct typevoyage voyages[MAXVOYAGE]          ;
struct typehoraire horaires[MAXHORAIRE]       ;
struct typegare gares[MAXGARE]                ; 

/* ------------------------------- */
/*--- Déclaration préliminaires ---*/
/* ------------------------------- */
void chargement()                ;
void chargement_data()           ;
void lance_recherche()           ;

void convmaj(char chaine[])      ;

/* =========================== */
/* === Programme principal === */
/* =========================== */

int main()
{
	int choix=-1 ; /* valeur lue au clavier (choix utilisateur) */

	//chargement() ;
	chargement_data() ;

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
							lance_recherche() ;
							break ;
			case 3: printf("à faire : mes billets\n");
							break ;
		} /* Fin du switch */
	} /* Fin du while */
} /* Fin du main */

/* ======================= */
/* === Sous-programmes === */
/* ======================= */

/* --------------------------------- */
/* --- Autre fonction Chargement --- */
/* --------------------------------- */

// void chargement()
// {
// 	int i, retour ;
// 	int nbhoraires ;
// 	FILE *fdatahoraire ;

// 	struct file_horaire // pour lire le fichier
// 	{
// 		char trip_id[30]           ;
// 		char arrival_time[8]   ; // il manque l'heure d'arrivée dans le fichier
// 		char departure_time[8] ;
// 		char stop_id[30]       ; // il faut le nom de la gare
// 		int  stop_sequence     ;
// 		int  service_id        ;
// 		int  monday            ;
// 		int  tuesday           ;
// 		int  wednesday         ;
// 		int  thursday          ;
// 		int  friday            ;
// 		int  saturday          ;
// 		int  sunday            ;
// 		int  start_date        ; // pas besoin
// 		int  end_date          ; // pas besoin
// 	} unhoraire ;

// 	struct horaire // pour garder ce qu'on veut
// 	{
// 		char trip_id[30]   ;
// 		char heure_arr[8]  ;
// 		char heure_dep[8]  ;
// 		char gare_id[30]   ;
// 		int  stop_sequence ;
// 		int  num_train     ;
// 		int  lundi         ;
// 		int  mardi         ;
// 		int  mercredi      ;
// 		int  jeudi         ;
// 		int  vendredi      ;
// 		int  samedi        ;
// 		int  dimanche      ;
// 		} ;

// 	struct horaire horaires[300000] ;


// 	fdatahoraire=fopen("./data_sncf/fill_sncf2.csv","r") ;
// 	if (fdatahoraire == NULL)
// 	{
// 		printf(ERREUR_CHARGEMENT) ;
// 	}
// 	else
// 	{
// 		printf("Fichier csv bien ouvert\n");
// 		char buffer[300000];
// 		fgets(buffer, sizeof(buffer), fdatahoraire); //skip the first line
// 		i=0 ;
// 		while (! feof(fdatahoraire) )
// 		{
// 			// on scanne le fichier ligne par ligne
// 			retour=fscanf(fdatahoraire,"%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
// 				unhoraire.trip_id,
// 				unhoraire.arrival_time,
// 				unhoraire.departure_time,
// 				unhoraire.stop_id,
// 				&unhoraire.stop_sequence,
// 				&unhoraire.service_id,
// 				&unhoraire.monday,
// 				&unhoraire.tuesday,
// 				&unhoraire.wednesday,
// 				&unhoraire.thursday,
// 				&unhoraire.friday,
// 				&unhoraire.saturday,
// 				&unhoraire.sunday,
// 				&unhoraire.start_date,
// 				&unhoraire.end_date) ;
			
// 			// on remplit le tableau horaires case par case (seulement les champs qui nous intéressent)
// 			strcpy(horaires[i].trip_id,  unhoraire.trip_id)        ;
// 			strcpy(horaires[i].heure_arr,unhoraire.arrival_time)   ;
// 			strcpy(horaires[i].heure_dep,unhoraire.departure_time) ;
// 			strcpy(horaires[i].gare_id,  unhoraire.stop_id)        ;
// 			horaires[i].stop_sequence =  unhoraire.stop_sequence   ;
// 			horaires[i].num_train     =  unhoraire.service_id      ;
// 			horaires[i].lundi         =  unhoraire.monday          ;
// 			horaires[i].mardi         =  unhoraire.tuesday         ;
// 			horaires[i].mercredi      =  unhoraire.wednesday       ;
// 			horaires[i].jeudi         =  unhoraire.thursday        ;
// 			horaires[i].vendredi      =  unhoraire.friday          ;
// 			horaires[i].samedi        =  unhoraire.sunday          ;
// 			horaires[i].dimanche      =  unhoraire.sunday          ;

// 			if (retour != EOF)
// 			{
// 				i++ ;
// 			}
// 		}
// 		i-- ;
// 		nbhoraires = i ;
// 		printf("%d\n",nbhoraires) ;
// 	}
// 	fclose(fdatahoraire) ;
// }

/* --------------------------- */
/* --- Fonction Chargement --- */
/* --------------------------- */
void chargement_data()
{
	int i,retour ;
	//int nbcalendar,nbvoyages,nbhoraires,nbgares ;
	FILE *fcalendar, *ftrips, *fstoptimes, *fstops ;
	char line[500] ; // pour lire ligne par ligne avec sscanf

/* -------------------------- */
/*--- Lecture des fichiers ---*/
/* -------------------------- */

/*--- Fichier calendar ---*/
	fcalendar=fopen("./data_sncf/export_gtfs_voyages/calendar.txt","r") ;
	if (fcalendar == NULL)
	{
		printf(ERREUR_CHARGEMENT) ;
	}
	else
	{
		printf("Fichier calendar bien ouvert\n");
		fgets(line, sizeof(line), fcalendar); // lire la 1re ligne et ne rien faire
		i=0 ;
		while (! feof(fcalendar) )
		{
			retour= fscanf(fcalendar,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&acalendar.service_id,
				&acalendar.monday,&acalendar.tuesday,&acalendar.wednesday,
				&acalendar.thursday,&acalendar.friday,
				&acalendar.saturday,&acalendar.sunday,
				&acalendar.start_date,&acalendar.end_date) ;

			calendrier[i].service_id = acalendar.service_id ;
			calendrier[i].lundi = acalendar.monday ;
			calendrier[i].mardi = acalendar.tuesday ;
			calendrier[i].mercredi = acalendar.wednesday ;
			calendrier[i].jeudi = acalendar.thursday ;
			calendrier[i].vendredi = acalendar.friday ;
			calendrier[i].samedi = acalendar.saturday ;
			calendrier[i].dimanche = acalendar.sunday ;
			
			if (retour != EOF)
			{
				i++ ;
			}
		}
		nbcalendar = i ;
	}
	fclose(fcalendar) ;

printf("Fichier Calendar : %d lignes chargées\n",i) ;
// printf("1re ligne (indice 0) %d %d %d %d %d %d %d %d\n",calendrier[0].service_id,calendrier[0].lundi,
//   calendrier[0].mardi,calendrier[0].mercredi,calendrier[0].jeudi,calendrier[0].vendredi,
// 	calendrier[0].samedi,calendrier[0].dimanche) ;
// printf("Avant-dernière ligne (indice %d) : %d %d %d %d %d %d %d %d\n",i-1,calendrier[i-1].service_id,calendrier[i-1].lundi,
// 	calendrier[i-1].mardi,calendrier[i-1].mercredi,calendrier[i-1].jeudi,calendrier[i-1].vendredi,
// 	calendrier[i-1].samedi,calendrier[i-1].dimanche) ;
// printf("Dernière ligne (indice %d) : %d %d %d %d %d %d %d %d\n",i,calendrier[i].service_id,calendrier[i].lundi,
// 	calendrier[i].mardi,calendrier[i].mercredi,calendrier[i].jeudi,calendrier[i].vendredi,
// 	calendrier[i].samedi,calendrier[i].dimanche) ;

/*--- Fichier trips ---*/
	ftrips=fopen("./data_sncf/export_gtfs_voyages/trips.txt","r") ;
	if (ftrips == NULL)
	{
		printf(ERREUR_CHARGEMENT) ;
	}
	else
	{
		printf("\nFichier trips bien ouvert\n");
		fgets(line, sizeof(line), ftrips); // lire la 1re ligne et ne rien faire
		i=0 ;
		while (fgets(line,sizeof(line),ftrips) != NULL)
		{
			retour=sscanf(line,"%41[^,],%d,%41[^,],%11[^,],%d,%d,%d",
			//retour=sscanf(line,"%41[^,],%d,%41[^,],%d,%d,%d,%d",	(parce que je me demandais si tripheadsign n'était pas un int)
				atrip.route_id,
				&atrip.service_id,
				atrip.trip_id,
				atrip.trip_headsign,
				&atrip.direction_id,
				&atrip.block_id,
				&atrip.shape_id ) ;

			// formattage des string
			// pas sure qu'on ait besoin de ceux qu'on ne prend pas dans voyages[]
			// atrip.route_id[MAXstrROUTE-1]             = '\0' ;
			// atrip.trip_id[MAXstrTRIP-1]               = '\0' ;
			// atrip.trip_headsign[MAXstrNUMTRAIN-1] = '\0' ;

			// remplissage du tableau voyages
			voyages[i].service_id = atrip.service_id         ;
			strcpy(voyages[i].trip_id,atrip.trip_id)         ;
			strcpy(voyages[i].num_train,atrip.trip_headsign) ;
			i++ ;
		}
	}
	nbvoyages = i ;

printf("Fichier trips : %d lignes chargées\n",nbvoyages) ; // pour vérifier
printf("Je ne sais pas pourquoi retour vaut %d\n",retour );
// printf("1re ligne (indice 0) : %d | %s\n", voyages[0].service_id, voyages[0].trip_id);
// printf("2e ligne (indice 1) : %d | %s\n", voyages[1].service_id, voyages[1].trip_id);
// printf("Ligne du milieu (indice %d) : %d | %s\n", nbvoyages/2,voyages[nbvoyages/2].service_id, voyages[nbvoyages/2].trip_id);
// printf("Avant-dernière ligne (indice %d) : %d | %s\n", nbvoyages-2,voyages[nbvoyages-2].service_id, voyages[nbvoyages-2].trip_id);
// printf("Dernière ligne (indice %d) : %d | %s\n",nbvoyages-1,voyages[nbvoyages-1].service_id, voyages[nbvoyages-1].trip_id);

	fclose(ftrips) ;

/*--- Fichier stop_times ---*/
	fstoptimes=fopen("./data_sncf/export_gtfs_voyages/stop_times.txt","r") ;
	if (fstoptimes == NULL)
	{
		printf(ERREUR_CHARGEMENT) ;
	}
	else
	{
		printf("\nFichier stop_times bien ouvert\n");
		fgets(line, sizeof(line), ftrips); // lire la 1re ligne et ne rien faire
		i=0 ;
		while (fgets(line,sizeof(line),ftrips) != NULL)
		{
			retour= sscanf(line,"%41[^,],%8s,%8s,%31[^,],%d,%11[^,],%d,%d,%d",
				astoptime.trip_id,
		 		astoptime.arrival_time,
		 		astoptime.departure_time,
		 		astoptime.stop_id,
		 		&astoptime.stop_sequence,
		 		astoptime.stop_headsign,
		 		&astoptime.pickup_type,
		 		&astoptime.drop_off_type,
		 		&astoptime.shape_dist_traveled) ;

			// formattage des string
			astoptime.arrival_time[MAXstrTIME-1]   = '\0' ;
			astoptime.departure_time[MAXstrTIME-1] = '\0' ;

			// remplissage du tableau horaires
			strcpy(horaires[i].trip_id,astoptime.trip_id) ;
			strcpy(horaires[i].heure_arr,astoptime.arrival_time) ;
			strcpy(horaires[i].heure_dep,astoptime.departure_time) ;
			strcpy(horaires[i].gare_id,astoptime.stop_id) ;

			if (retour != EOF)
			{
				i++ ;
			}
		}
		nbhoraires = i ;
	}
	fclose(fstoptimes) ;

printf("Fichier stop_times : %d lignes chargées\n",nbhoraires) ; // pour vérifier
printf("Je ne sais pas pourquoi retour vaut %d\n",retour );
// printf("1re ligne (indice 0) %s %s %s %s\n",horaires[0].trip_id,horaires[0].heure_arr,horaires[0].heure_dep,horaires[0].gare_id) ;
// printf("Avant-dernière ligne (indice %d) : %s %s %s %s\n",nbhoraires-2,horaires[nbhoraires-2].trip_id,horaires[nbhoraires-2].heure_arr,horaires[nbhoraires-2].heure_dep,horaires[nbhoraires-2].gare_id) ;
// printf("Dernière ligne (indice %d) : %s %s %s %s\n",nbhoraires-1,horaires[nbhoraires-1].trip_id,horaires[nbhoraires-1].heure_arr,horaires[nbhoraires-1].heure_dep,horaires[nbhoraires-1].gare_id) ;

/*--- Fichier stops ---*/
	fstops=fopen("./data_sncf/export_gtfs_voyages/stops.txt","r") ;
	if (fstops == NULL)
	{
		printf(ERREUR_CHARGEMENT) ;
	}
	else
	{
		printf("\nFichier stop bien ouvert\n");
		fgets(line, sizeof(line), ftrips); // lire la 1re ligne et ne rien faire
		i=0 ;
		while (fgets(line,sizeof(line),ftrips) != NULL)
		{
			retour= sscanf(line,"%41[^,],%41[^,],%d,%f,%f,%d,%d,%d,%s",
				astop.stop_id,
		 		astop.stop_name,
		 		&astop.stop_desc,
		 		&astop.stop_lat,
		 		&astop.stop_lon,
		 		&astop.zone_id,
		 		&astop.stop_url,
		 		&astop.location_type,
		 		astop.parent_station) ;

			// formattage des string
			astop.stop_id[MAXstrGAREID-1]      = '\0'   ;
			convmaj(astop.stop_id)                      ;
			astop.stop_name[MAXstrGARENOM-1]  = '\0'    ;
			convmaj(astop.stop_name)                    ;
			astop.parent_station[MAXstrGAREID-1] = '\0' ;
			convmaj(astop.parent_station)               ;

			// remplissage du tableau gares
			strcpy(gares[i].gare_id,astop.stop_id) ;
			strcpy(gares[i].gare_nom,astop.stop_name) ;

			if (retour != EOF)
			{
				i++ ;
			}
		}
		nbgares = i ;
	}
	fclose(fstops) ;

printf("Fichier stops : %d lignes chargées\n",nbgares) ; // pour vérifier
printf("Je ne sais pas pourquoi retour vaut %d\n",retour );
// printf("1re ligne (indice 0) %s %s\n",gares[0].gare_id,gares[0].gare_nom) ;
// printf("Avant-dernière ligne (indice %d) : %s %s\n",i-1,gares[i-1].gare_id,gares[i-1].gare_nom) ;
// printf("Dernière ligne (indice %d) : %s %s\n",i,gares[i].gare_id,gares[i].gare_nom) ;
}

/* ------------------------------------------ */
/* -- Procédure de conversion en majuscule -- */
/* ------------------------------------------ */
void convmaj(char chaine[])
{
	int i ;

	for (i=0 ; i < strlen(chaine) ; i++)
	{
		chaine[i] = toupper(chaine[i]) ;
	}
}
/* ---------------------------- */
/* -- Fonctions de recherche -- */
/* ---------------------------- */
// à replacer là-haut :
void recherche_gare_depart(char rechgare[], char garedeptrouve[][MAXstrGAREID], int *nbgarestrouve);
void recherche_gare_arrivee() ;

void lance_recherche()
{
	int i, nbres=0;
	char garedep[MAXstrGARENOM]    ; // saisie utilisateur Gare de départ
	char garearr[MAXstrGARENOM]    ; // saisie utilisateur Gare d'arrivée
	char garedepres[20][MAXstrGAREID] ; // tableau de chaines quand il y a plusieurs gares dans la même ville

	/* Départ */
	printf("\nGare de départ : ");
	scanf("%s",garedep) ;
	convmaj(garedep) ;
	recherche_gare_depart(garedep,garedepres,&nbres) ;

	if(nbres==0)
	{
		printf ("\nIl n'y a pas de train au départ de %s\n",garedep) ;
	}
	else
	{
		for (i=0; i < nbres ; i++) // à revoir après (on n'aura pas besoin de la boucle ici)
		{
			printf("%s\n",garedepres[i] ); 
		}
		/* Arrivée */
			printf("Gare d'arrivée : ");
			scanf("%s",garearr) ;
			convmaj(garearr) ;
			//recherche_gare_arrivee() ;
	}
}

void recherche_gare_depart(char rechgare[], char garedeptrouve[][MAXstrGAREID], int *nbgarestrouve)
{
	struct typegare unegare ;
	int i ; // compteur gares
	int j ; // compteur caractère chaine
	int k ; // compteur caractère sous-chaine
	int position ;
	int l=0 ; // compteur xième gare correspondante trouvée

	for (i=0;i<nbgares;i++)
	{
		unegare = gares[i] ;
		for (j=0;unegare.gare_nom[j]!='\0';j++)
		{
			k=0;
			if(unegare.gare_nom[j] == rechgare[k])
			{
				position = j+1 ;
				while (unegare.gare_nom[j] == rechgare[k])
				{
					j++ ; // on va se faire engueuler de modifier le compteur dans la boucle
					k++ ;
				}

				if(rechgare[k]=='\0')
				{
					strcpy(garedeptrouve[l++],unegare.gare_id) ;
					*nbgarestrouve=l ;
				}
				else
				{
					j=position ;
					position=0 ;
				}
			}
		}
	}
}

/* ------------------------------------------ */
/* Une tentative avec strstr */
/* ------------------------------------------ */
// void recherche_gare(const char rechgare[], char garedep_id[])
// {
// 	struct typegare unegare ;
// 	int i,j=0 ;
// 	char *pchaine ;

// 	for (i=0;i<nbgares;i++)
// 	{
// 		printf("\n1\n");
// 		unegare = gares[i] ;
// 		printf("%s\n",unegare.gare_nom );
// 		pchaine = strstr(unegare.gare_nom,rechgare) ;
// 		printf("%s %s\n",pchaine,unegare.gare_nom );
// 		if (pchaine)//(strcmp(pchaine,rechgare) == 0)
// 		{
// 			printf("%s",unegare.gare_nom) ;
// 			//printf("ça marche pas\n");
// 		//	strcpy(garedep_id[j++],unegare.gare_id) ;
// 		}

// 		// if (strcmp(pchaine,NULL) != 0)
// 		// {
// 		// 	printf("2\n");
// 		// 	return unegare.gare_id ;
// 		// }
// 		// else 
// 		// {
// 		// 	printf("3\n");
// 		// 	return NULL ;
// 		// }
// 	}
// }
// /* ------------------------------- */
// /* -- Procédure lance recherche -- */
// /* ------------------------------- */
// void lance_recherche()
// {
// 	int numero ;
// 	char nom_a_rechercher[MAX_NOM] ;
// 	struct etudiant eleve         ;

// 	printf("Nom à rechercher : ") ;
// 	scanf("%s",nom_a_rechercher)  ;
// 	convmaj(nom_a_rechercher)     ;
// 	numero=recherche(nom_a_rechercher) ;
	
// 	if(numero == NON_TROUVE)
// 	{
// 		printf("L'élève %s n'a pas été trouvé\n",nom_a_rechercher);
// 	}
// 	else
// 	{
// 		printf("--------------------------\n");
// 		printf("%-10s %-10s %4s\n","Nom", "Prenom", "age");
// 		printf("--------------------------\n");
// 		eleve = tabeleves[numero] ;
// 		printf("%-10s %-10s %4d\n",eleve.nom, eleve.prenom, eleve.age);
// 	}
// }