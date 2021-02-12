#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
/* include pour l'heure */
#include <time.h>
#include <locale.h>
/* include pour stat() */
#include <sys/types.h> 
#include <sys/stat.h>
/* include pour opendir() */
#include <dirent.h>
#include <unistd.h>

/* --- déclaration des constantes --- */
#define GARE 100
#define DATE 11

/* === déclaration des types globaux === */
struct UnTrajet {
  int  idservice ;
  char idtrajet[100] ;
  int  numtrain ;
  int  direction ;
};

struct UnCalendrier {
  int idservice ;
  int lun ;
  int mar ;
  int mer ;
  int jeu ;
  int ven ;
  int sam ;
  int dim ;
  int start ;
  int end ;
};

struct UnStop {
  char idtrajet[100] ;
  char ha[9] ;
  char hd[9] ;
  char idgare[100] ;
  int sequence ;
};

struct UneGare {
  char idgare[100] ;
  char nomgare[100] ;
  char gareattache[100] ;
};

struct UneRame {
    int wagon  ; // n° de wagon
    int classe  ; // 1re classe, 2e classe
    int salle ; // 1 ou 2
    int siege ; // numéro de siège
    int position ; // fenêtre, couloir, place isolée
    int etat ; // à supprimer si on teste sur billet
    int billet ; // numéro unique de billet
  };
  
struct UneSeq {
  char type[5] ; /* Si c'est Car, TER, TGV, ça reprend le type de trajet, mais ça permet
  1. d'aller chercher l'info moins loin
  2/ de nuancer les types de rames de TGV (un jour lointain) */
  char gd[GARE] ;
  char ga[GARE] ;
  int  hd             ;
  int  ha             ;
  int  nbsalle ; // simplex, duplex
  struct UneRame *rame;
} ;

struct UneDate {
  int date ;
  struct UneSeq *sequence ;
} ;

struct UnVoyage {
  int  id ;
  struct UneDate *date ;
} ;

/* version 2D */
struct UnVoyage2 {
  char idtrajet[100] ;
  int  date          ;
  char type[5]       ;
  char garedep[GARE] ;
  char garearr[GARE] ;
  int seqdep         ;
  int seqarr         ;
  char hd[9]         ;
  char ha[9]         ;
  int  wagon    ; // n° de wagon
  int  classe   ; // 1re classe, 2e classe
  int  salle    ; // 0 bas, 1 haut
  int  siege    ; // numéro de siège
  int  position ; // 0 fenêtre, 1 couloir, 2 place isolée
  int  etat     ; // à supprimer si on teste sur billet
  int  billet   ; // numéro unique de billet
} ;
struct UnVoyage2 *tab_voyages2 ;

struct date {
  int jhebdo;
  int jour  ;
  int mois  ;
  int annee ;
  int date  ;
};

struct UnHoraire {
  char idtrajet[100];
  int idservice;
  int numtrain;
  int lun;
  int mar;
  int mer;
  int jeu;
  int ven;
  int sam;
  int dim;
  int direction;
  char ha[9];
  char hd[9];
  int sequence;
  char idgare[100];
  char nomgare[GARE];
};

struct UnRes_nodate { /*--- Structure de résultats origine/destination sans date ---*/
 char idtrajet[100];
  int idservice;
  int numtrain;
  int lun;
  int mar;
  int mer;
  int jeu;
  int ven;
  int sam;
  int dim;
  int direction;
  char ha[9] ;
  char hd[9] ;
  int seqdep ;
  int seqarr ;
  char idgaredep[100];
  char garedep[GARE];
  char idgarearr[100];
  char garearr[GARE];
  // struct resultat_nodate *p_prec      ;
  // struct resultat_nodate *p_suiv      ;
} ;

struct UnRes { /*--- Structure de résultats origine/destination avec date ---*/
 char idtrajet[100];
  int idservice;
  int numtrain;
  char ha[9] ;
  char hd[9] ;
  int seqdep ;
  int seqarr ;
  char idgaredep[100];
  char garedep[GARE];
  char idgarearr[100];
  char garearr[GARE];
  // struct resultat_nodate *p_prec      ;
  // struct resultat_nodate *p_suiv      ;
} ;

/* --- déclaration des variables globales --- */
// données sncf
struct UnTrajet *trajets;
struct UnCalendrier *calendriers ;
struct UnStop *stops ;
struct UneGare *gares ;
// les places
struct UnVoyage *voyages ;
//nb données importées
int nbtrajet;
int nbcalendrier;
int nbstop=0;
int nbgare;
//les places
int nbvoyage=0;
//les dates ouvertes à la vente
struct date *tab_date_vente; /*--- tableau des dates prises en charge par le programme ---*/
int    nbdatevente ; /* --- nb dates ouvertes à la réservation --- */
// éléments de date
int  jhebdo_num_sys, jour_sys, mois_sys, annee_sys ; // éléments de la date du système
char jhebdo_alpha_sys[9] ;



/* === déclarations préliminaires === */
void convmaj(char chaine[]) ;
char * str_replace_all(char * chaine, char * motif_a_remplacer, char * motif_de_remplacement) ;
// void supprime_accent(char chaine[]) ;
char * supprime_accent(char * chaine);
void dump_buffer() ;
void date_sys(int *jour, int *mois, int *annee, int *jhebdo_num) ;
void interprete_jour_semaine(int jhebdo_num, char jhebdo_alpha[]) ;
// int  calcul_jour_semaine(int jour_rech, int mois_rech, int annee_rech, int jour, int mois, int annee) ;
int calcul_jour_semaine(int jour_rech, int mois_rech, int annee_rech, int jour, int mois, int annee, int jour_sem) ;
// void date_suivante_precedente(int jhebdo, int jour, int mois, int annee, int *jhebdo_rech, int *jour_rech, int *mois_rech, int *annee_rech, int increment) ;
void date_suivante_precedente(int *jhebdo_rech, int *jour_rech, int *mois_rech, int *annee_rech, int increment) ;
int valide_date(int * jour, int * mois, int * annee) ;
int date_anterieure(int jour, int mois, int annee, int jour_ref, int mois_ref, int annee_ref) ;
void crea_date_vente(int jour, int mois, int annee);

int lecture_choix(int deb, int fin, char lettre, int * erreur) ;

void chargement_horaires();
void chargement_trajet() ;
void chargement_calendrier() ;
void chargement_stop() ;
void chargement_gare() ;

void chargement_place5() ;
void suppression_places5() ;
void identifie_trajet_date_a_creer() ;
void creation_places(char idtrajet[100], int date);

void lance_recherche() ;
struct UnHoraire * recherche_horaire(char rechgare[], int *nbres) ;
struct UnRes_nodate * compare_nodate(struct UnHoraire gare_dep_trouve[], int nb_gare_dep_trouve, struct UnHoraire gare_arr_trouve[], int nb_gare_arr_trouve, int *nb_res_nodate);
// struct horaire * recherche_horaire(char rechgare[], int *nb_res_horaire) ;
// struct UnRes_nodate * compare_nodate2(struct UnHoraire gare_dep_trouve[], int nb_gare_dep_trouve, struct UnHoraire gare_arr_trouve[], int nb_gare_arr_trouve, int *nb_res_nodate);
struct UnRes * compare_avecdate(struct UnRes_nodate tab_res_nodate[], int *nb_res_nodate, int jhebdo, int *nb_res_date  /*, char date_rech[SizeDate]*/ );
void tri(struct UnRes tab_res[], int * nb_res_date);

// =========================== //
/* === Programme principal === */
// =========================== //

int main()
{
  char lettre   ;  // char lu au clavier
  int  choix=-1 ;  // choix utilisateur reconstitué
  int  erreur   ;  // code erreur pour expressions conditionnelles
  
  printf("Chargement des données en cours... \nVeuillez patienter, le programme va bientôt démarrer\n\n");

  // Date du système
  date_sys(&jour_sys, &mois_sys, &annee_sys, &jhebdo_num_sys) ; // récupère la date du système
  interprete_jour_semaine(jhebdo_num_sys, jhebdo_alpha_sys)   ; // interprète le jour de semaine
  printf("Nous sommes le %s %d/%d/%d\n", jhebdo_alpha_sys, jour_sys, mois_sys, annee_sys) ;

  chargement_horaires() ; // chargement des données horaires à partir des fichiers GTFS
  crea_date_vente(jour_sys, mois_sys, annee_sys) ;
  chargement_place5() ;

  // crea_date(jour_sys, mois_sys, annee_sys);
  // chargement_place5() ;

  // if(nbhoraire) // si le nombre d'horaire chargés est différent de 0
  if(1)
  {
    printf("===========================\n");
    printf("Bienvenue chez SNCF Voyages\n");
    printf("===========================\n");
  }
    
  while (choix != 0)
  {    
    // affichage du menu et lecture du choix
    erreur = 1;
    while(erreur==1)
    {
      printf("\n-1- Réserver\n")                ;
      printf("-2- Consulter les horaires\n")    ;
      printf("-3- Mes réservations\n")          ;
      printf("-0- Quitter\n")                   ;
      printf("\nChoix : ")                      ;
      scanf("%c", &lettre)                      ;
      choix = lecture_choix(0,3,lettre,&erreur) ; 
    }
  
    // traitement selon le choix
    switch (choix)
    {
      case 0: printf("À bientôt sur SNCF Voyages\n") ; break ;
      case 1: printf("à faire : réserver (=consulter (horaires + tarifs) + réserver)\n") ;
              lance_recherche() ;
              break ;
      case 2: printf("à faire : consulter les horaires\n") ;
              break ;
      case 3: printf("à faire : mes billets\n");
              break ;
    } /* Fin du switch */
  } /* Fin du while */
} /* Fin du main */

// ======================== //
/* === Sous-programmes  === */
// ======================== //

/* -------------------------------------------- */
/* --- Procédures de chargement des données --- */
/* -------------------------------------------- */

// ~~~~~~~~~~~
/* --- Chargement des données horaires --- */
// ~~~~~~~~~~~
void chargement_horaires()
{
  chargement_trajet() ;
  chargement_calendrier() ;
  chargement_stop() ;
  chargement_gare() ;
}

// ~~~~~~~~~~~
/* --- Chargement des données trajets --- */
// ~~~~~~~~~~~
void chargement_trajet()
{
  FILE *f1;
  int  i, retour;
  char line[500] ; // pour lire ligne par ligne avec sscanf
  char dumpchar[100] ;
  char dumpcar;
  int dumpint ;

  /* --- Allocation de mémoire au tableau trajet --- */
  trajets = (struct UnTrajet *) malloc(sizeof(struct UnTrajet));

  /*--- Ouverture fichier horaires --- */
  //f1=fopen("./data_sncf/sncf_full_test.txt", "r") ;
  f1=fopen("./data/horaire/tgv/trips.txt","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement\n") ;
  }
  else
  {
    i=0 ;
    fgets(line, sizeof(line), f1); // lire la 1re ligne et ne rien faire (je l'enlève car il n'y a plus les entêtes dans le csv)
    while (fgets(line,sizeof(line),f1) != NULL)
    {     
      retour=sscanf(line,"%101[^,],%d,%101[^,],\"%d\",%d,%101[^,],%101[^\n]",
      // retour=sscanf(line,"%s,%d,%s,\"%d\",%d,%s,%s",
        dumpchar,
        &trajets[i].idservice,
         trajets[i].idtrajet,
        &trajets[i].numtrain,
        &trajets[i].direction,
        dumpchar,
        dumpchar);
      i++ ;
      nbtrajet = i ;
      trajets = (struct UnTrajet *) realloc(trajets,sizeof(struct UnTrajet) * (nbtrajet+1)) ;
    }
    fclose(f1) ;
  }
  // for (i=0;i<nbtrajet;i++)
  // {
  //   printf("%d \n",trajets[i].idservice);
  //   printf("%s \n",trajets[i].idtrajet);
  //   printf("%d \n",trajets[i].numtrain);
  //   printf("%d \n",trajets[i].direction);
  // }
  printf("nombre de trajets %d\n",nbtrajet);
}

// ~~~~~~~~~~~
/* --- Chargement des données calendrier de circulation --- */
// ~~~~~~~~~~~
void chargement_calendrier()
{
  FILE *f1;
  int  i, retour;
  char line[500] ; // pour lire ligne par ligne avec sscanf
  int dumpint ;

  /* --- Allocation de mémoire au tableau trajet --- */
  calendriers = (struct UnCalendrier *) malloc(sizeof(struct UnCalendrier));

  /*--- Ouverture fichier horaires --- */
  //f1=fopen("./data_sncf/sncf_full_test.txt", "r") ;
  f1=fopen("./data/horaire/tgv/calendar.txt","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement\n") ;
  }
  else
  {
    i=0 ;
    fgets(line, sizeof(line), f1); // lire la 1re ligne et ne rien faire (je l'enlève car il n'y a plus les entêtes dans le csv)
    while (fgets(line,sizeof(line),f1) != NULL)
    {     
      retour=sscanf(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
      // retour=sscanf(line,"%s,%d,%s,\"%d\",%d,%s,%s",
        &calendriers[i].idservice,
        &calendriers[i].lun,
        &calendriers[i].mar,
        &calendriers[i].mer,
        &calendriers[i].jeu,
        &calendriers[i].ven,
        &calendriers[i].sam,
        &calendriers[i].dim,
        &dumpint,
        &dumpint);
      i++ ;
      nbcalendrier = i ;
      calendriers = (struct UnCalendrier *) realloc(calendriers,sizeof(struct UnCalendrier) * (nbcalendrier+1)) ;
    }
    fclose(f1) ;
  }
  // for (i=0;i<nbcalendrier;i++)
  // {
  //   printf("%d \n",calendriers[i].idservice);
  //   printf("%d \n",calendriers[i].lun);
  // }
  printf("nombre de calendriers %d\n",nbcalendrier);
}

// ~~~~~~~~~~~
/* --- Chargement des données stop en gare --- */
// ~~~~~~~~~~~
void chargement_stop()
{
  FILE *f1;
  int  i, retour;
  char line[500] ; // pour lire ligne par ligne avec sscanf
  int dumpint ;
  char dumpchar ;

  /* --- Allocation de mémoire au tableau trajet --- */
  stops = (struct UnStop *) malloc(sizeof(struct UnStop));

  /*--- Ouverture fichier horaires --- */
  //f1=fopen("./data_sncf/sncf_full_test.txt", "r") ;
  f1=fopen("./data/horaire/tgv/stop_times.txt","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement\n") ;
  }
  else
  {
    i=0 ;
    fgets(line, sizeof(line), f1); // lire la 1re ligne et ne rien faire (je l'enlève car il n'y a plus les entêtes dans le csv)
    while (fgets(line,sizeof(line),f1) != NULL)
    {     
      retour=sscanf(line,"%101[^,],%10[^,],%10[^,],%101[^,],%d,%101[^,],%d,%d,%d",
      // retour=sscanf(line,"%s,%d,%s,\"%d\",%d,%s,%s",
        stops[i].idtrajet,
        stops[i].ha,
        stops[i].hd,
        stops[i].idgare,
        &stops[i].sequence,
        dumpchar,
        &dumpint,
        &dumpint,
        &dumpint);
      i++ ;
      nbstop = i ;
      stops = (struct UnStop *) realloc(stops,sizeof(struct UnStop) * (nbstop+1)) ;
    }
    fclose(f1) ;
  }
  // for (i=0;i<nbstop;i++)
  // {
  //   printf("%s \n",stops[i].idgare);
  //   printf("%d \n",stops[i].arrive);
  // }
  printf("nombre de stops %d\n",nbstop);
}

// ~~~~~~~~~~~
/* --- Chargement des données gares --- */
// ~~~~~~~~~~~
void chargement_gare()
{
  FILE *f1;
  int  i, retour;
  char line[500] ; // pour lire ligne par ligne avec sscanf
  int  dumpint ;
  char dumpchar ;

  /* --- Allocation de mémoire au tableau trajet --- */
  gares = (struct UneGare *) malloc(sizeof(struct UneGare));

  /*--- Ouverture fichier horaires --- */
  //f1=fopen("./data_sncf/sncf_full_test.txt", "r") ;
  f1=fopen("./data/horaire/tgv/stops.txt","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement\n") ;
  }
  else
  {
    i=0 ;
    fgets(line, sizeof(line), f1); // lire la 1re ligne et ne rien faire (je l'enlève car il n'y a plus les entêtes dans le csv)
    while (fgets(line,sizeof(line),f1) != NULL)
    {     
      retour=sscanf(line,"%101[^,],\"%101[^\"],%101[^,],%101[^,],%101[^,],%101[^,],%d,%101[^\n]",
      // retour=sscanf(line,"%s,%d,%s,\"%d\",%d,%s,%s",
        gares[i].idgare,
        gares[i].nomgare,
        dumpchar,
        dumpchar,
        dumpchar,
        dumpchar,
        &dumpint,
        dumpchar);
      // supprime_accent(gares[i].nomgare);
      convmaj(gares[i].nomgare);
      i++ ;
      nbgare = i ;
      gares = (struct UneGare *) realloc(gares,sizeof(struct UneGare) * (nbgare+1)) ;
    }
    fclose(f1) ;
  }
  // for (i=0;i<nbgare;i++)
  // {
  //   printf("%s \n",gares[i].idgare);
  //   printf("%s \n",gares[i].nomgare);
  // }
  printf("nombre de gares %d\n",nbgare);
}

// ~~~~~~~~~~~
/* --- Chargement des données places à réserver --- */
// ~~~~~~~~~~~
void chargement_place5() // version 1 fichier 2D - 1 structure 2D
{
                // /* version 2D */
                // struct UnVoyage2 {
                  // char idtrajet[100] ;
                  // int  date           ;
                  // char type[5]        ;
                  // char garedep[GARE]  ;
                  // char garearr[GARE]  ;
                  // int seqdep ;
                  // int seqarr ;
                  // char hd[9]            ;
                  // char ha[9]             ;
                  // int  wagon  ; // n° de wagon
                  // int  classe  ; // 1re classe, 2e classe
                  // int  salle ; // 1 ou 2
                  // int  siege ; // numéro de siège
                  // int  position ; // fenêtre, couloir, place isolée
                  // int  etat ; // à supprimer si on teste sur billet
                  // int  billet ; // numéro unique de billet
                // } ;
                // struct UnVoyage2 *tab_voyages2 ;
  FILE *f1;
  char lettre;
  int  i, j, retour;
  char dump ;

  char idtrajet[100], type[5], garedep[GARE], garearr[GARE], hd[9], ha[9]  ;
  int date=0,seqdep=0,seqarr=0, wagon=0, classe=0, salle=0, siege=0, position=0, etat=0, billet=0;
  
  /* --- Allocation de mémoire au tableau tab_horaires --- */
  tab_voyages2 = (struct UnVoyage2 *) malloc(sizeof(struct UnVoyage2));

  /*--- Ouverture fichier places --- */
  f1=fopen("./data/place/places.txt","r") ;
  i=nbvoyage ;
  while ((! feof(f1)))
  {
    // while(retour!=(EOF))
    // {
    // lecture de la 1e colonne (idtrajet)
    j=0;
    retour=0;
    while((retour!=59) && (retour!=10)) // tant que different de LF et de ";"
    {
      fscanf(f1,"%c", &lettre) ;       // fscanf du caractere
      retour=lettre;
      if((retour!=59) && (retour!=10))  // si different de LF et de ";"
      {
        idtrajet[j++] = lettre  ;      // insertion
      }
      idtrajet[j] = '\0' ;             // cloture de la chaine
    }
    strcpy(tab_voyages2[i].idtrajet,idtrajet) ;

    // lecture de la 2e colonne (date)
    fscanf(f1,"%d;",&date);
    tab_voyages2[i].date = date ;

    // lecture de la 3e colonne (type)
    j=0;
    retour=0;
    while((retour!=59) &&(retour!=10)) // tant que different de LF et de ";"
    {
      fscanf(f1,"%c", &lettre) ;       // fscanf du caractere
      retour=lettre;
      if((retour!=59) &&(retour!=10))  // si different de LF et de ";"
      {
        type[j++] = lettre  ;          // insertion
      }
      type[j] = '\0' ;                 // cloture de la chaine
    }
    strcpy(tab_voyages2[i].type,type);

    // lecture de la 4e colonne (garedep)
    j=0;
    retour=0;
    while((retour!=59) &&(retour!=10)) // tant que different de LF et de ";"
    {
      fscanf(f1,"%c", &lettre) ;       // fscanf du caractere
      retour=lettre;
      if((retour!=59) &&(retour!=10))  // si different de LF et de ";"
      {
        garedep[j++] = lettre  ;          // insertion
      }
      garedep[j] = '\0' ;                 // cloture de la chaine
    }
    strcpy(tab_voyages2[i].garedep,garedep);

    // lecture de la 5e colonne (garearr)
    j=0;
    retour=0;
    while((retour!=59) &&(retour!=10)) // tant que different de LF et de ";"
    {
      fscanf(f1,"%c", &lettre) ;       // fscanf du caractere
  // printf("Ici ok 2, %c \n",lettre)  ;
      retour=lettre;
      if((retour!=59) &&(retour!=10))  // si different de LF et de ";"
      {
        garearr[j++] = lettre  ;          // insertion
      }
      garearr[j] = '\0' ;                 // cloture de la chaine
    }
    strcpy(tab_voyages2[i].garearr,garearr);

    fscanf(f1,"%d;%d;",&seqdep, &seqarr) ;
    tab_voyages2[i].seqdep = seqdep;
    tab_voyages2[i].seqarr = seqarr;  

    // lecture de la 8e colonne (hd)
    j=0;
    retour=0;
    while((retour!=59) &&(retour!=10)) // tant que different de LF et de ";"
    {
      fscanf(f1,"%c", &lettre) ;       // fscanf du caractere
  // printf("Ici ok 4, %c \n",lettre)  ;
      retour=lettre;
      if((retour!=59) &&(retour!=10))  // si different de LF et de ";"
      {
        hd[j++] = lettre  ;          // insertion
      }
      hd[j] = '\0' ;                 // cloture de la chaine
    }
    strcpy(tab_voyages2[i].hd, hd) ;
    
    // lecture de la 9e colonne (ha)
    j=0;
    retour=0;
    while((retour!=59) &&(retour!=10)) // tant que different de LF et de ";"
    {
      fscanf(f1,"%c", &lettre) ;       // fscanf du caractere
  // printf("Ici ok 9, %c \n",lettre)  ;
      retour=lettre;
      if((retour!=59) &&(retour!=10))  // si different de LF et de ";"
      {
        ha[j++] = lettre  ;          // insertion
      }
      ha[j] = '\0' ;                 // cloture de la chaine
    }    
    strcpy(tab_voyages2[i].ha, ha) ;

    retour=fscanf(f1,"%d;%d;%d;%d;%d;%d;%d",
      &wagon, &classe, &salle, &siege, &position, &etat, &billet) ;

    // strcpy(tab_voyages2[i].idtrajet,idtrajet) ;
    // tab_voyages2[i].date = date ;
    // strcpy(tab_voyages2[i].type,type);
    // strcpy(tab_voyages2[i].garedep,garedep);
    // strcpy(tab_voyages2[i].garearr,garearr);
    // tab_voyages2[i].seqdep = seqdep;
    // tab_voyages2[i].seqarr = seqarr;
    // strcpy(tab_voyages2[i].hd, hd) ;
    // strcpy(tab_voyages2[i].ha, ha) ;
    // tab_voyages2[i].nbsalle = nbsalle ;
    tab_voyages2[i].wagon = wagon ;
    tab_voyages2[i].classe = classe ;
    tab_voyages2[i].salle = salle ;
    tab_voyages2[i].siege = siege ;
    tab_voyages2[i].position = position ;
    tab_voyages2[i].etat = etat ;
    tab_voyages2[i].billet = billet ;

  //printf de contrôle
  // printf("%d: ",i) ;
  // printf("%s ",tab_voyages2[i].idtrajet);
  // printf("%d ",tab_voyages2[i].date);
  // printf("%s ",tab_voyages2[i].type);
  // printf("%s ",tab_voyages2[i].garedep);
  // printf("%s ",tab_voyages2[i].garearr);
  // printf("%d ",tab_voyages2[i].seqdep);
  // printf("%d ",tab_voyages2[i].seqarr);
  // printf("%s ",tab_voyages2[i].hd);
  // printf("%s ",tab_voyages2[i].ha);
  // printf("%d ",tab_voyages2[i].wagon);
  // printf("%d ",tab_voyages2[i].classe);
  // printf("%d ",tab_voyages2[i].salle);
  // printf("%d ",tab_voyages2[i].idtrajet);
  // printf("%d ",tab_voyages2[i].siege);
  // printf("%d ",tab_voyages2[i].position);
  // printf("%d ",tab_voyages2[i].etat);
  // printf("|%d| ",tab_voyages2[i].billet);
  // printf("\n") ;

    while((retour!=59) &&(retour!=10)) // tant que different de LF et de ";"
    {
      fscanf(f1,"%c", &dump) ;       // fscanf du caractere
      retour=dump;
    }

      i++ ;
      nbvoyage = i ;
      tab_voyages2 = (struct UnVoyage2 *) realloc(tab_voyages2,sizeof(struct UnVoyage2) * (nbvoyage+1));
    }
    

  // printf de contrôle
  for (i=0;i<nbvoyage;i++)
  {
    printf("nb voyages=%d;  date du voyage n°%d : %d\n",nbvoyage, i, tab_voyages2[i].date);
  }
  suppression_places5() ;
  // identifie_trajet_date_a_creer() ;
}

void suppression_places5()
{
  printf("nombre lus : %d\n",nbvoyage) ;
  int i,j,k ;

  i=0;
  while((i<nbvoyage-1)&&(tab_voyages2[i].date>=tab_date_vente[0].date))
  {
    i++;
    if (tab_voyages2[i].date<tab_date_vente[0].date)
    {
      for(j=i;j<nbvoyage-1;j++)
      {
        tab_voyages2[j] = tab_voyages2[j+1] ;
      }
      nbvoyage--;
      i--;
    }
  }
  tab_voyages2 = (struct UnVoyage2 *) realloc(tab_voyages2,sizeof(struct UnVoyage2) * (nbvoyage));
  //print de controle
  // for (i=0;i<nbvoyage;i++)
  // {
  //   printf("gardé : %s %d\n",tab_voyages2[i].idtrajet,tab_voyages2[i].date);
  // }
  printf("nombre gardés : %d\n",nbvoyage) ;
}

void identifie_trajet_date_a_creer()
{
  int i, j, k, existe=0;
  struct IdTrajet {
    char idtrajet[100] ;
  };
  struct IdTrajet *tab_idtrajet;
  int nbidtrajet=0;

  // // pour stocker les combinaisons de date + trajet qui n'ont pas encore de places ouvertes
  // struct TrajetDate {
  //   char idtrajet[100];
  //   int date ;
  // };
  // struct TrajetDate *tab_trajetdate;
  // int nbtrajetdate=0 ;

  // la liste des idtrajet
  tab_idtrajet = (struct IdTrajet *) malloc(sizeof(struct IdTrajet)) ;
  for (i=0;i<nbtrajet;i++)
  {
    // printf("l'id de la table trajets:%s\n",trajets[i].idtrajet);
    strcpy(tab_idtrajet[nbidtrajet].idtrajet,trajets[i].idtrajet );
    // printf("l'id du tableau:%s le res copié:%s\n",trajets[i].idtrajet,tab_idtrajet[nbidtrajet].idtrajet) ;
    // printf("nbidtrajet:%d\n",nbidtrajet);
    nbidtrajet++;
    tab_idtrajet = (struct IdTrajet *) realloc(tab_idtrajet,sizeof(struct IdTrajet) * (nbidtrajet+1));
    // printf("Ok 3\n")  ;
  }
  printf("nbidtrajet:%d\n",nbidtrajet);

  // pour chaque idtrajet
  for (i=0;i<nbidtrajet;i++)
  {
    // pour chacune des dates ouvertes
    for (j=0;j<nbdatevente;j++)
    {
      for (k=0;k<nbvoyage;k++)
      {
        // printf("id examiné=|%s| id voyage=|%s| ",tab_idtrajet[i].idtrajet,tab_voyages2[k].idtrajet);
        // printf("datevente examinée=%d datevoyage=|%d]\n",tab_date_vente[j].date,tab_voyages2[k].date);
        // printf("Le coupe date/id n'a pas été trouvé\n");
        if ((strcmp(tab_voyages2[k].idtrajet,tab_idtrajet[i].idtrajet)==0) && (tab_voyages2[k].date==tab_date_vente[j].date))          
        {
          existe=1;
        }
        // print de contrôle
        if(existe == 1)
        {
          printf("existe pour id=%s date=%d\n",tab_voyages2[k].idtrajet,tab_voyages2[k].date);
        }
        if (existe==0)
        {
          creation_places(tab_voyages2[k].idtrajet,tab_voyages2[k].date);
        }
        existe = 0;
      }
    }
  }
}

void creation_places(char idtrajet[100], int date)
{
  int i,j,k,l,m,n,o,p;
  int nbstop, nbsequence ;

  char type[5]="TGV" ;
  // int caracteristique[]; // l'idée là c'était d'aller chercher les caractéristiques par type de transport
  char nomfichier[50];
  FILE *f1;


  struct UnStop tab_stops[20];

  /* je mets if type = TGV parce que je n'ai que les données TGV, 
    mais si on importe d'autres ensembles de données gtfs, il faudrait 
    donner comme type à toutes les trajets, le type de l'ensemble de données"
  */
  if (strcmp(type,"TGV")==0)
  {
    strcpy(nomfichier,"./data/place/placement_tgv_duplex.txt");
  }

  // On prend tous les Stops de cet id
  for (i=0;i<nbstop;i++)
  {
    j=0 ;
    if (strcmp(idtrajet,stops[i].idtrajet)==0)
    {
      tab_stops[j++] = stops[i] ;
    }
    nbstop=j ;
  }

  nbsequence = nbstop-1;
  for (i=0;i<nbsequence;i++)
  {
    // tab_sequence[i].
  }


  f1=fopen(nomfichier,"r") ;


// struct UnStop {
//   char idtrajet[100] ;
//   char ha[9] ;
//   char hd[9] ;
//   char idgare[100] ;
//   int sequence ;
// };


  
  // {
  //   for (j=0;j<nbdatevente;j++)
  // }

          // struct UnVoyage2 {
          //   char idtrajet[100] ;
          //   int  date           ;
          //   char type[5]        ;
          //   char garedep[GARE]  ;
          //   char garearr[GARE]  ;
          //   int seqdep ;
          //   int seqarr ;
          //   char hd[9]            ;
          //   char ha[9]             ;
          //   int  nbsalle ; // simplex, duplex
          //   int  wagon  ; // n° de wagon
          //   int  classe  ; // 1re classe, 2e classe
          //   int  salle ; // 1 ou 2
          //   int  siege ; // numéro de siège
          //   int  position ; // fenêtre, couloir, place isolée
          //   int  etat ; // à supprimer si on teste sur billet
          //   int  billet ; // numéro unique de billet
          // } ;
}

/* ---------------------------------------- */
/* --- Fonctions de recherche de voyage --- */
/* ---------------------------------------- */
void lance_recherche()
{
  char lettre    ; // char lu au clavier
  int  choix2=-1 ; // choix utilisateur reconstitué
  int  erreur2=0 ; // code erreur pour expressions conditionnelles
  int  erreur3=0 ; // postériorité d'une date à une date de ref (-1=égalité 0=postérieur 1=antérieur)
  int  erreur4=0 ;

  int  i ;
  int  nb_res_depart=0 ;
  int  nb_res_arrive=0 ;
  int  nb_res_nodate=0 ;
  int  nb_res_date=0   ;

  int jour, mois, annee, jhebdo ; // éléments de la date de voyage
  char jhebdo_alpha[9]          ; // élément de la date de voyage

  char garedep[GARE] ; // saisie utilisateur Gare de départ
  char garearr[GARE] ; // saisie utilisateur Gare d'arrivée
  char date_rech[DATE]    ; // saisie utilisateur Date de voyage

  struct UnHoraire *tab_res_depart=NULL ; // pointeur de struct horaire pour les résultats au départ d'une gare
  struct UnHoraire *tab_res_arrive=NULL ; // pointeur de struct horaire pour les résultats à l'arrivée d'une gare
  struct UnRes_nodate *tab_res_nodate=NULL ; // pointeur de struct resultat_nodate pour les résultats communs
  struct UnRes *tab_res=NULL ; // pointeur de struct resultat pour les résultats communs

  /* === Départ === */
  printf("\nGare de départ                      : ")     ; // invite de saisie
  scanf("%s",garedep)                                    ; // récupération saisie utilisateur Gare de départ
  convmaj(garedep)                                       ; // conversion en majuscule
  tab_res_depart=recherche_horaire(garedep,&nb_res_depart) ;
  if(nb_res_depart==0) // Cas : pas de résultat au départ de la gare saisie
  {
    printf ("\nIl n'y a pas de train au départ de %s\n",garedep) ;
    dump_buffer() ;
  }
  else // Cas : des résultats au départ de la gare saisie
  {    

  /* === Arrivée ===*/
    printf("Gare d'arrivée                      : ")       ; // invite de saisie
    scanf("%s",garearr)                                    ; // récupération saisie utilisateur gare d'arrivée
    convmaj(garearr)                                       ; // conversion en majuscule
    tab_res_arrive = recherche_horaire(garearr,&nb_res_arrive) ; // recherche_horaire reçoit la chaine saisie, le nombre de résultats et retourne un tableau de résultats  
    
    // tab_res_nodate = compare_nodate(res_depart,nb_res_depart,res_arrive,nb_res_arrive,&nb_res_nodate);
    tab_res_nodate = compare_nodate(tab_res_depart,nb_res_depart,tab_res_arrive,nb_res_arrive,&nb_res_nodate);
    if(nb_res_nodate==0) // Cas : pas de résultat entre la gare de départ et la gare d'arrivée
    {     
      printf ("\nIl n'y a pas de liaison entre %s et %s\n",garedep, garearr) ;
      dump_buffer() ;
    }
    else // Cas : des résultats entre la gare de départ et la gare d'arrivée
    {

      /* === Date === */
      dump_buffer() ;
      printf("Date du voyage (JJ/MM/AAAA)         : ") ; // invite de saisie
      erreur2 = valide_date(&jour, &mois, &annee)      ; // lecture saisie et contrôle format
      erreur3 = date_anterieure(jour, mois, annee, jour_sys, mois_sys, annee_sys) ;
      while (erreur2 | erreur3 == 1)
      {
        if (erreur2)
        {
          printf("\nDate incorrecte\n")                               ;
          dump_buffer() ;
        }
        if (erreur3 == 1)
        {
          printf("\nNous ne proposons pas de voyage dans le passé\n") ;
        }
        printf("\nGare de départ                      : %s\n",garedep); 
        printf("Gare d'arrivée                      : %s\n",garearr)  ;
        printf("Date du voyage (JJ/MM/AAAA)         : ")              ;
        erreur2 = valide_date(&jour, &mois, &annee)                   ;
        erreur3 = date_anterieure(jour, mois, annee, jour_sys, mois_sys, annee_sys) ;
      }
      
      jhebdo = calcul_jour_semaine(jour, mois, annee, jour_sys, mois_sys, annee_sys, jhebdo_num_sys) ; // calcul du jour de semaine de la date de voyage
      interprete_jour_semaine(jhebdo, jhebdo_alpha) ;
      
      tab_res = compare_avecdate(tab_res_nodate, &nb_res_nodate, jhebdo, &nb_res_date /*,date_rech*/);
      
      tri(tab_res,&nb_res_date) ;
      if(nb_res_nodate==0)
      {
        printf("\nAucun train ne circule entre %s et %s le %s %d/%d/%d\n",garedep, garearr, jhebdo_alpha, jour, mois, annee) ;
      }
      else
      {
        while (choix2 != 0)
        {
          printf("\n") ;
          printf("--------------------------------------------------------------------------------------------------------------\n") ;
          // printf(" n° | Gare de départ         | Gare d'arrivée         | numéro | hh:mm | hh:mm | Type\n") ;
          printf(" %3s | %-35s | %-35s | %6s | %8s | %8s | \n","n°","Gare de départ","Gare d'arrivée","Train","hh:mm:ss","hh:mm:ss","Type");
          printf("--------------------------------------------------------------------------------------------------------------\n") ;
          for(i=0;i<nb_res_date;i++)
          {
            // printf("%3d | %-22s | %-22s | %6d | %2d:%02d | %2d:%02d | %s\n", i+1, 
            printf("%3d | %-35s| %-35s| %6d | %8s | %8s | \n", i+1, 
              tab_res[i].garedep, tab_res[i].garearr, tab_res[i].numtrain, 
              // tab_res[i].heure_dep/100, (tab_res[i].heure_dep-tab_res[i].heure_dep/100*100), 
              // tab_res[i].heure_arr/100, (tab_res[i].heure_arr-tab_res[i].heure_arr/100*100), tab_res[i].type) ;
              // tab_res[i].hd/100, tab_res[i].heure_dep%100,tab_res[i].heure_arr/100, 
              // tab_res[i].heure_arr%100, tab_res[i].type) ;
              tab_res[i].hd, tab_res[i].ha);

          }
          printf("--------------------------------------------------------------------------------------------------------------\n") ;
          printf("\n") ;
          printf("-1- Choisir un train circulant le %s %d/%d/%d\n",jhebdo_alpha, jour, mois, annee) ; // faire une fonction qui actualise la date (mutualiser avec jour_semaine ?)
          printf("-2- Afficher les trains du jour précédent\n") ;
          printf("-3- Afficher les trains du jour suivant\n") ;
          printf("-4- Modifier la recherche\n") ;
          printf("-0- Retour à l'accueil\n") ;
          printf("\nChoix : ") ;
          scanf("%c",&lettre) ;
          choix2 = lecture_choix(0,4,lettre,&erreur4) ;
          switch (choix2)
          {
            case 1: printf("choisir un train (n°) : ") ;
                    break;
            case 2: date_suivante_precedente(&jhebdo, &jour, &mois, &annee, -1) ;
                    interprete_jour_semaine(jhebdo, jhebdo_alpha) ;
                    tab_res=compare_avecdate(tab_res_nodate, &nb_res_nodate, jhebdo, &nb_res_date) ;
                    tri(tab_res,&nb_res_date) ;
                    break;
            case 3: date_suivante_precedente(&jhebdo, &jour, &mois, &annee, 1) ;
                    interprete_jour_semaine(jhebdo, jhebdo_alpha) ;
                    tab_res=compare_avecdate(tab_res_nodate, &nb_res_nodate, jhebdo, &nb_res_date) ;
                    tri(tab_res,&nb_res_date) ;
                    break;
            case 4: printf("c'est peut-être pas la peine de faire cette entrée si c'est pour demander 'voulez vous changer le départ, oui, non, voulez-vous changer l'arrivée, oui, non etc.\n") ;
                    break;
            case 0: break;
            default:printf("\nDésolés, nous n'avons pas compris votre choix, recommencez\n") ; 
                    break ;
          }
        }
      }
    }
  }
}

// ~~~~~~~~~~~
/* Fonction de recherche d'horaires selon la gare saisie
 (retourne un tableau d'horaires qui matchent la gare saisie) */
// ~~~~~~~~~~~
struct UnHoraire * recherche_horaire(char rechgare[], int *nbres)
{
  struct idnomgare {
    char idgare[100];
    char nomgare[GARE];
  } ;
  struct idnomgare listgares[20] ;
  int nblistgares ;
        // struct UnHoraire {
          char idtrajet[100];
          int idservice;
          int numtrain;
          int lun;
          int mar;
          int mer;
          int jeu;
          int ven;
          int sam;
          int dim;
          int direction;
          char ha;
          char hd;
          int sequence;
          char idgare[100];
          char nomgare[GARE];
        // };
  int i, j, k=0, l=0, m,n ;
  // int i        ; // compteur des gares
  // int j        ; // compteur caractère chaine
  // int k        ; // compteur caractère sous-chaine
  // int l=0      ; // compteur xième gare correspondante trouvée
  int position ; // compteur de la position où le 1er caractère commun est trouvé
  struct UnHoraire unhoraire ;    // variable locale de type struct horaire
  struct UnHoraire *res_horaire ; // pointeur du tableau de résultats à retourner

  // res_horaire = (struct UnHoraire *) malloc(sizeof(struct UnHoraire)) ;

  for (i=0;i<nbgare;i++)                          // pour chaque ligne de gares
  {
    for (j=0;gares[i].nomgare[j]!='\0';j++)       // pour le caractère du nom de la gare, tant qu'il est différent de \O
    {
      k=0;                                        // et du premier caractère de la saisie (sous-chaine)
      if(gares[i].nomgare[j] == rechgare[k])      // si le caractère du nom de la gare est égal au caractère de la saisie
      {
        position = j+1 ;                          // on mémorise la position de recherche.
        while (gares[i].nomgare[j] == rechgare[k])  // tant que les deux caractères sont égaux...
        {
          j++ ;                                   // ... on passe au caractère suivant pour le nom de la gare
          k++ ;                                   // ... et pour la saisie
        }
        if(rechgare[k]=='\0')                     // si la saisie (sous-chaine) arrive à la fin (= toute la sous-chaine a été trouvée dans la chaine)
        {
                                                      // realloc de mémoire au tableau res_horaire
          strcpy(listgares[l].nomgare,gares[i].nomgare) ;
          strcpy(listgares[l++].idgare, gares[i].idgare) ;
          nblistgares = l ;
    // printf de contrôle
    // printf("gare trouvée %d : %s %s\n",i, gares[i].nomgare, gares[i].idgare);
        }
        else                                          // sinon (les caractères comparés de la chaine et de la sous-chaine sont différents)
        {
          j=position ;                                // on passe au caractère suivant dans la chaine (retour à la position retenue)
          position=0 ;                                // la position est réinitialisée
        }
      } /* fin du if caractère chaine = caractère sous-chaine (on passe au caractère de la chaine suivant en repartant du 1er caractère de la sous-chaine) */
    } /* fin du for chaque caractère de la chaine */
  } /* fin du for chaque ligne de tab_horaire */
  
  // on va chercher les autres infos
  // l=0;k=0;
  res_horaire = (struct UnHoraire *) malloc(sizeof(struct UnHoraire)) ;        
  for (i=0;i<nblistgares;i++) // pour chaque gare trouvée
  {
    for (j=0;j<nbgare;j++) // pour chaque ligne de stops
    {
      if (strcmp(listgares[i].idgare,gares[j].idgare)==0)
      {
        for (l=0;l<nbstop;l++)
        {
          if (strcmp(listgares[i].idgare,stops[l].idgare)==0)
          {

            for (m=0;m<nbtrajet;m++)
            {
              if (strcmp(stops[l].idtrajet,trajets[m].idtrajet)==0)
              {
                for (n=0;n<nbcalendrier;n++)
                {
                  if (trajets[m].idservice == calendriers[n].idservice)
                  {
                    res_horaire = (struct UnHoraire *) realloc(res_horaire,sizeof(struct UnHoraire) * (*nbres+1));
  // printf("idgare copié n°%d : %s\n",k,gares[j].idgare);
        strcpy(res_horaire[k].idgare,listgares[i].idgare);
        strcpy(res_horaire[k].nomgare,listgares[i].nomgare);
            strcpy(res_horaire[k].idtrajet,stops[l].idtrajet);
            strcpy(res_horaire[k].ha, stops[l].ha) ;
            strcpy(res_horaire[k].hd,stops[l].hd) ;
            res_horaire[k].sequence = stops[l].sequence ;
                res_horaire[k].idservice = trajets[m].idservice;
                res_horaire[k].numtrain = trajets[m].numtrain ;              
                    res_horaire[k].lun = calendriers[n].lun;
                    res_horaire[k].mar = calendriers[n].mar;
                    res_horaire[k].mer = calendriers[n].mer;
                    res_horaire[k].jeu = calendriers[n].jeu;
                    res_horaire[k].ven = calendriers[n].ven;
                    res_horaire[k].sam = calendriers[n].sam;
                    res_horaire[k].dim = calendriers[n].dim;
                    k++;
                    *nbres=k;
                    // res_horaire = (struct UnHoraire *) realloc(res_horaire,sizeof(struct UnHoraire) * (*nbres+1));
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  // printf de contrôle
  // for (k=0; k<*nbres;k++)
  // {
  //   printf("%d : ",k);
  //   printf("%s ",res_horaire[k].idtrajet);
  //   printf("%d ",res_horaire[k].idservice);
  //   printf("%d ",res_horaire[k].numtrain);
  //   printf("%d ",res_horaire[k].lun);
  //   printf("%d ",res_horaire[k].mar);
  //   printf("%d ",res_horaire[k].mer);
  //   printf("%d ",res_horaire[k].jeu);
  //   printf("%d ",res_horaire[k].ven);
  //   printf("%d ",res_horaire[k].sam);
  //   printf("%d ",res_horaire[k].dim);
  //   printf("%d ",res_horaire[k].direction);
  //   printf("%s ",res_horaire[k].ha);
  //   printf("%s ",res_horaire[k].hd);
  //   printf("%d ",res_horaire[k].sequence);
  //   printf("%s ",res_horaire[k].idgare);
  //   printf("%s ",res_horaire[k].nomgare);
  //   printf("\n");
  // }
  return res_horaire;
}

// ~~~~~~~~~~~
/* Fonction de comparaison des résultats départ/arrivée 
  (retourne un tableau des résultats, construit à partir des match) */
struct UnRes_nodate * compare_nodate(struct UnHoraire gare_dep_trouve[], int nb_gare_dep_trouve, struct UnHoraire gare_arr_trouve[], int nb_gare_arr_trouve, int *nb_res_nodate)
{
  int i=0 ; // compteur résultats à l'arrivée
  int j=0 ; // compteur résultats au départ
  int k=0 ; // compteur match arrivée/départ
  struct UnRes_nodate *tab_resultats_nodate ; // pointeur du tableau de résultats communs à retourner
  
  /* allocation de mémoire au tableau de résultats tab_resultats_nodate */
  tab_resultats_nodate = (struct UnRes_nodate *) malloc(sizeof(struct UnRes_nodate));

  for(i=0 ; i<nb_gare_arr_trouve ; i++)                                          // pour chaque arrivee
  {             
    for (j=0 ; j<nb_gare_dep_trouve ; j++)
    {                                                                           // pour chaque départ
      if( strcmp(gare_dep_trouve[j].idtrajet, gare_arr_trouve[i].idtrajet)==0)  // condition id départ = id arrivée
      {
        if( gare_dep_trouve[j].sequence < gare_arr_trouve[i].sequence)           // condition depart avant arrivee
        {
          strcpy(tab_resultats_nodate[k].idtrajet,gare_dep_trouve[j].idtrajet); // copie des infos dans la structure UnRes_nodate
          tab_resultats_nodate[k].idservice = gare_dep_trouve[j].idservice ;
          tab_resultats_nodate[k].numtrain  = gare_dep_trouve[j].numtrain  ;
          tab_resultats_nodate[k].lun       = gare_dep_trouve[j].lun       ;
          tab_resultats_nodate[k].mar       = gare_dep_trouve[j].mar       ;
          tab_resultats_nodate[k].mer       = gare_dep_trouve[j].mer       ;
          tab_resultats_nodate[k].jeu       = gare_dep_trouve[j].jeu       ;
          tab_resultats_nodate[k].ven       = gare_dep_trouve[j].ven       ;
          tab_resultats_nodate[k].sam       = gare_dep_trouve[j].sam       ;
          tab_resultats_nodate[k].dim       = gare_dep_trouve[j].dim       ;
          tab_resultats_nodate[k].direction = gare_dep_trouve[j].direction ;
          strcpy(tab_resultats_nodate[k].hd,  gare_dep_trouve[j].hd)       ;
          strcpy(tab_resultats_nodate[k].ha,  gare_arr_trouve[i].ha)       ;
          tab_resultats_nodate[k].seqdep    = gare_dep_trouve[j].sequence  ;
          tab_resultats_nodate[k].seqarr    = gare_arr_trouve[i].sequence  ;
          strcpy(tab_resultats_nodate[k].idgaredep, gare_dep_trouve[j].idgare) ;
          strcpy(tab_resultats_nodate[k].idgarearr, gare_arr_trouve[i].idgare) ;
          strcpy(tab_resultats_nodate[k].garedep,   gare_dep_trouve[j].nomgare) ;
          strcpy(tab_resultats_nodate[k].garearr,   gare_arr_trouve[i].nomgare) ;
  // printf de contrôle à supprimer
  // printf("dépar:%s | arrivée:%s | num_train:%d | hdep:%d | harr:%d | type:%s\n",
  //  tab_resultats_nodate[k].dep_gare, 
  //  tab_resultats_nodate[k].arr_gare, 
  //  tab_resultats_nodate[k].num_train, 
  //  tab_resultats_nodate[k].heure_dep, 
  //  tab_resultats_nodate[k].heure_arr,
  //  tab_resultats_nodate[k].type
  // );
          k++;
          tab_resultats_nodate = (struct UnRes_nodate *) realloc(tab_resultats_nodate,sizeof(struct UnRes_nodate) * (k+1));          
        } // fin du if sur l'idtrajet
      } // fin du if sur les stop_sequence
    } // fin du for Gare de départ
  } // fin du for Gare d'arrivée     
  *nb_res_nodate = k ;
  return tab_resultats_nodate ;
}

// ~~~~~~~~~~~
/* Fonction de selection des resultats en fonction du jour de la semaine desire 
  (retourne un tableau des résultats, construit à partir des match) */
// ~~~~~~~~~~~
struct UnRes * compare_avecdate(struct UnRes_nodate tab_res_nodate[], int *nb_res_nodate, int jhebdo, int *nb_res_date  /*, char date_rech[SizeDate]*/ )       //la plus belle des fonctions
{  
  // printf("passage dans date : jour de la semaine %d\n", jhebdo);
  // printf("passage dans date : numéro de train du 2e résultat du tableau de résultats no date %d\n", tab_res_nodate[1].num_train);     // ces 2 printf ne s'allument pas, la fonction n'arrive donc pas a s'executer
  int i=0 ; // compteur de tab_resultat_nodate                    // j'ai mis en commentaire toutes les mentions de la date dans la structure resultat jusqu'a trouver comment convertir proprement
  int j=0 ; // compteur de tab_resultat
  struct UnRes *tab_resultats ; // pointeur du tableau de résultats communs à retourner
  
  /* allocation de mémoire au tableau de résultats tab_resultats */
  tab_resultats = (struct UnRes *) malloc(sizeof(struct UnRes));
  
  switch(jhebdo)        // en fonction de jhebdo, on cherche dans la bonne colonne
  {
    case 0: 
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].dim)
        {
          strcpy(tab_resultats[j].garedep, tab_res_nodate[i].garedep) ;
          strcpy(tab_resultats[j].garearr, tab_res_nodate[i].garearr) ;
          // strcpy(tab_resultats[j].date    , date_rech)                   ;
          // strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].numtrain = tab_res_nodate[i].numtrain      ;
          strcpy(tab_resultats[j].hd, tab_res_nodate[i].hd)      ;
          strcpy(tab_resultats[j].ha, tab_res_nodate[i].ha)      ;
          tab_resultats[j].seqdep    = tab_res_nodate[i].seqdep  ;
          tab_resultats[j].seqarr    = tab_res_nodate[i].seqarr  ;
                
          j++;
          tab_resultats = (struct UnRes *) realloc(tab_resultats,sizeof(struct UnRes) * (j+1));
        }
      }
      break;
    case 1:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].lun)
        {
          strcpy(tab_resultats[j].garedep, tab_res_nodate[i].garedep) ;
          strcpy(tab_resultats[j].garearr, tab_res_nodate[i].garearr) ;
          // strcpy(tab_resultats[j].date    , date_rech)                   ;
          // strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].numtrain = tab_res_nodate[i].numtrain      ;
          strcpy(tab_resultats[j].hd, tab_res_nodate[i].hd)     ;
          strcpy(tab_resultats[j].ha, tab_res_nodate[i].ha)      ;
                
          j++;
          tab_resultats = (struct UnRes *) realloc(tab_resultats,sizeof(struct UnRes) * (j+1));
        }
      }
      break;
    case 2:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].mar)
        {
          strcpy(tab_resultats[j].garedep, tab_res_nodate[i].garedep) ;
          strcpy(tab_resultats[j].garearr, tab_res_nodate[i].garearr) ;
          // strcpy(tab_resultats[j].date    , date_rech)                   ;
          // strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].numtrain = tab_res_nodate[i].numtrain      ;
          strcpy(tab_resultats[j].hd, tab_res_nodate[i].hd)     ;
          strcpy(tab_resultats[j].ha, tab_res_nodate[i].ha)      ;
                
          j++;
          tab_resultats = (struct UnRes *) realloc(tab_resultats,sizeof(struct UnRes) * (j+1));
        }
      }
      break;
    case 3:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].mer)
        {
          strcpy(tab_resultats[j].garedep, tab_res_nodate[i].garedep) ;
          strcpy(tab_resultats[j].garearr, tab_res_nodate[i].garearr) ;
          // strcpy(tab_resultats[j].date    , date_rech)                   ;
          // strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].numtrain = tab_res_nodate[i].numtrain      ;
          strcpy(tab_resultats[j].hd, tab_res_nodate[i].hd)     ;
          strcpy(tab_resultats[j].ha, tab_res_nodate[i].ha)      ;
                
          j++;
          tab_resultats = (struct UnRes *) realloc(tab_resultats,sizeof(struct UnRes) * (j+1));
        }
      }
      break;
    case 4:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].jeu)
        {
          strcpy(tab_resultats[j].garedep, tab_res_nodate[i].garedep) ;
          strcpy(tab_resultats[j].garearr, tab_res_nodate[i].garearr) ;
          // strcpy(tab_resultats[j].date    , date_rech)                   ;
          // strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].numtrain = tab_res_nodate[i].numtrain      ;
          strcpy(tab_resultats[j].hd, tab_res_nodate[i].hd)     ;
          strcpy(tab_resultats[j].ha, tab_res_nodate[i].ha)      ;
                
          j++;
          tab_resultats = (struct UnRes *) realloc(tab_resultats,sizeof(struct UnRes) * (j+1));
        }
      }
      break;
    case 5:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].ven)
        {
          strcpy(tab_resultats[j].garedep, tab_res_nodate[i].garedep) ;
          strcpy(tab_resultats[j].garearr, tab_res_nodate[i].garearr) ;
          // strcpy(tab_resultats[j].date    , date_rech)                   ;
          // strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].numtrain = tab_res_nodate[i].numtrain      ;
          strcpy(tab_resultats[j].hd, tab_res_nodate[i].hd)     ;
          strcpy(tab_resultats[j].ha, tab_res_nodate[i].ha)      ;
                
          j++;
          tab_resultats = (struct UnRes *) realloc(tab_resultats,sizeof(struct UnRes) * (j+1));
        }
      }
      break;
    case 6:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].sam)
        {
          strcpy(tab_resultats[j].garedep, tab_res_nodate[i].garedep) ;
          strcpy(tab_resultats[j].garearr, tab_res_nodate[i].garearr) ;
          // strcpy(tab_resultats[j].date    , date_rech)                   ;
          // strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].numtrain = tab_res_nodate[i].numtrain      ;
          strcpy(tab_resultats[j].hd, tab_res_nodate[i].hd)     ;
          strcpy(tab_resultats[j].ha, tab_res_nodate[i].ha)      ;
                
          j++;
          tab_resultats = (struct UnRes *) realloc(tab_resultats,sizeof(struct UnRes) * (j+1));
        }
      }
      break;
  }     
  *nb_res_date = j   ;
  return tab_resultats ;
}

// ~~~~~~~~~~~
/* Tri des resultats par heure de depart */
// ~~~~~~~~~~~
void tri(struct UnRes tab_res[], int * nb_res_date)
{
  int i ; // compteur du tableau d'entrée
  int j ; // compteur pour trouver l'indice à partir duquel décaler
  struct UnRes unresultat ;

  for (i=0 ; i < *nb_res_date ; i++)
  {
    unresultat = tab_res[i] ;
    j = i ;
    while ((j>0) && (strcmp(unresultat.hd, tab_res[j-1].hd)<0))
    {
      tab_res[j]=tab_res[j-1];
      j-- ;
    }
    tab_res[j] = unresultat ;
  }
}


// ============================================= //
/* === Fonctions et procédures sur les dates === */
// ============================================= //

/* --------------------- */
/* -- Date du système -- */
/* --------------------- */
// void date_sys(int *jour, int *mois, int *annee)
void date_sys(int *jour, int *mois, int *annee, int *jhebdo_num)
{
  time_t nb_sec_1970, temps ;
  struct tm date ;

  /* -- met la date en francais -- */
  setlocale(LC_ALL,"");

  /*-- Récupère la date système -- */
  nb_sec_1970 = time(&temps);      // Secondes depuis 01/01/1970
  date = *localtime(&nb_sec_1970); // Conversion en date

  /* Éléments intelligibles de la date du système */
  *jour       = date.tm_mday       ; // jour du système
  *mois       = date.tm_mon  +1    ; // mois du système
  *annee      = date.tm_year +1900 ; // année du système
  *jhebdo_num = date.tm_wday       ; // jour de semaine du système (0 à 6)
}

/* ----------------------------- */
/* -- Postériorité d'une date -- */
/* ----------------------------- */
int date_anterieure(int jour, int mois, int annee, int jour_ref, int mois_ref, int annee_ref)
{
  int erreur=0;

  if (annee < annee_ref)
  {
    erreur=1;
  }
  else if (annee == annee_ref)
  {
    if (mois < mois_ref)
    {
      erreur=1;
    }
    else if (mois == mois_ref)
    {
      if (jour < jour_ref)
      {
        erreur=1;
      }
      else if (jour = jour_ref)
      {
        erreur = -1;
      }
    }
  }
  return erreur ;
}

/* --------------------------------- */
/* -- Tableau des dates à traiter -- */
/* --------------------------------- */
void crea_date_vente(int jour, int mois, int annee)
{
  int jour_end, mois_end, annee_end;
  int i, j, m, a, date;
  // int i, nbdate; // je l'ai passé en global
  char chdate[9];
  jour_end=jour;
  mois_end=mois;
  annee_end=annee;
  
  j=jour;
  m=mois;
  a=annee;
  
  for(i=0; i<4;i++)
  {
    mois_end++;
    if(mois_end>12)
    {
      mois_end=1;
      annee_end++;
    }
  }
  // calcule la date la plus loin des ventes ouvertes (dans 4 mois)
  switch(mois_end)
  {
    case 4: case 6: case 9: case 11:
      while(jour_end>30)
      {
        jour_end--;
      }
      break;
    case 2 :
      if((mois_end % 4 == 0 && mois_end % 100 != 0) || mois_end % 400 == 0)
      {
        while(jour_end>29)
        {
          jour_end--;
        }
      }
      else
      {
        while(jour_end>28)
        {
          jour_end--;
        }
      }
      break;
    default :
      break ;
  }

  // calcule toutes les dates intermédiaires
  tab_date_vente = (struct date *) malloc(sizeof(struct date));
  
  i=0;
  while( (j!=jour_end) | (m!=mois_end ) | (a!=annee_end))
  {
    tab_date_vente[i].jhebdo=calcul_jour_semaine(j,m,a,jour_sys,mois_sys,annee_sys,jhebdo_num_sys) ;
    tab_date_vente[i].jour=j;
    tab_date_vente[i].mois=m;
    tab_date_vente[i].annee=a;
    
    chdate[0]=(a/1000)+48; 
    chdate[1]=((a%1000)/100)+48;
    chdate[2]=((a%100)/10)+48;
    chdate[3]=(a%10)+48;
    chdate[4]=(m/10)+48;
    chdate[5]=(m%10)+48;
    chdate[6]=(j/10)+48;
    chdate[7]=(j%10)+48;
    chdate[8]='\0';
    // strcpy(tab_date_vente[i].date,chdate); // je remplace par un int, c'est plus simple à traiter
    date = atoi(chdate) ;
    tab_date_vente[i].date = date ;
    
 // printf("%d\n", tab_date[i].date);
    
    j++;
    
    i++;
    nbdatevente=i;
    tab_date_vente = (struct date *) realloc(tab_date_vente,sizeof(struct date) * (nbdatevente+1)) ;
    
    switch(m)
    {
      case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        if( j>31)
        {
         j=1;
         m++;
        }
        break;
      case 4: case 6: case 9: case 11:
        if( j>30)
        {
         j=1;
         m++;
        }
        break;
      case 2 :
        if((m % 4 == 0 && m % 100 != 0) || m % 400 == 0)
        {
          if( j>29)
          {
           j=1;
           m++;
          }
        }
        else
        {
          if( j>28)
          {
           j=1;
           m++;
          }
        }
        break;
      default :
        break ;
    }
    
    if(m>12)
    {
      m=1;
      a++;
    }
  } 
}

/* ------------------------------- */
/* -- Calcul du jour de semaine -- */
/* ------------------------------- */
// incrémente les 4 variables d'une date donnéee connue 
// (jour de semaine, jour, mois, année) jusqu'à atteindre
// la date recherchée pour en connaitre le jour de semaine
int calcul_jour_semaine(int jour_rech, int mois_rech, int annee_rech, int jour, int mois, int annee, int jhebdo)
{
  int jhebdo_rech=jhebdo ; // jour de semaine à retourner (commence au jour de semaine fourni)
  int i, j=0 ;
  int annee_bi[10];        // tableau d'années bissextiles

  /* -- Construction du tableau des prochaines annees bissextiles -- */
  for(i=annee; i<=annee_rech;i++)  /* Est-ce qu'on a vraiment besoin de ça ? 
  On n'a qu'à juste utiliser le if qui dit si annee_rech est bissextile*/
  {                              
    if((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
    {
      annee_bi[j++]=i;
    }
  }
  j=0;

  /* -- Incrementation des jours -- */
  while((jour != jour_rech) | (mois != mois_rech) | (annee != annee_rech)) // tant qu'on n'atteint pas la date recherchée
  {                                                                        // on incrémente les 4 variables de date
    switch(mois)                                                           // (jour et mois selon le nombre de jours du mois)
    {
      case 1 : case 3 : case 5 : case 7 : case 8 : case 10 : case 12 : // les mois de 31 jours
        if(jour<31)            // si le jour n'est pas le 31
        {
          jour++;              // incrémentation du jour
        }
        else                   // si le jour est le 31 (dernier du mois)
        {
          jour=1;              // le prochain jour est le 1er
          mois++;              // du mois suivant
        }
        break;
      case 2 :                 // en février
        if(annee==annee_bi[j]) // si l'année est bissextile
        {
          if(jour<29)          // si le jour n'est pas 29 (dernier du mois)
          {
            jour++;            // incrémentation du jour
          }
          else                 // sinon (le jour est le dernier du mois)
          {
            jour=1;            // le prochain jour est le 01/03
            mois++;
            // j++;
          }
        }
        else                   // si l'année n'est pas bissextile
        {
          if(jour<28)          // si le jour n'est pas 28 (dernier du mois)
          {         
            jour++;            // incrémentation du jour
          }
          else                 // sinon (le jour est le dernier du mois)
          {
            jour=1;            // le prochain jour est le 01/03
            mois++;
          }
        }
        break;
      default :                // pour tous les autres mois (ceux de 30 jours)
        if(jour<30)            // si le jour n'est pas le 30 (dernier du mois)
        {
          jour++;              // incrémentation du jour
        }
        else                   // sinon (le jour est le dernier du mois)
        {
          jour=1;              // le prochain jour est le 1er
          mois++;              // du mois suivant
        }
        break;
    } // fin du switch (selon le nombre de jours du mois)
    if(mois==13)                                                          // (année si on a changé d'année dans le switch)
    {
      mois=1;
      annee++;
    }
    jhebdo_rech++;                                                        // (jour de semaine)
    if (jhebdo_rech==7)
      jhebdo_rech=0;                                                      // remise à 0 si jour de la semaine 7 (convention : 0 à 6)          
  } /* fin du while d'incrémentation d'un jour hebdo */
  return jhebdo_rech; // renvoie un int (0 à 6)
}

/* --------------------------------------- */
/* -- Interprétation du jour de semaine -- */
/* --------------------------------------- */
void interprete_jour_semaine(int jhebdo_num, char jhebdo_alpha[])
{
  switch(jhebdo_num)
  {
    case 0: strcpy(jhebdo_alpha,"dimanche") ; break ;
    case 1: strcpy(jhebdo_alpha,"lundi")    ; break ;
    case 2: strcpy(jhebdo_alpha,"mardi")    ; break ;
    case 3: strcpy(jhebdo_alpha,"mercredi") ; break ;
    case 4: strcpy(jhebdo_alpha,"jeudi")    ; break ;
    case 5: strcpy(jhebdo_alpha,"vendredi") ; break ;
    case 6: strcpy(jhebdo_alpha,"samedi")   ; break ;
  }
}

/* ------------------------------------------------------- */
/* -- Lecture et contrôle de validité d'une date saisie -- */
/* ------------------------------------------------------- */
int valide_date(int * jour, int * mois, int * annee)
{
  int mois_switch ;
  int erreur = 0;

  int i=0 ;
  char lettre=50 ; // caractère lu
  #define MAX_DIGIT 12 // taille de la chaine
  char digit[MAX_DIGIT] ; // chaine constituée par ajout de caractère lu
  int compteur_saisie=0, compteur_element_date=0;

  // lecture saisie, controle du format
  while(compteur_saisie < MAX_DIGIT && lettre != '\n' && ((lettre>46) && (lettre<58)) && ! erreur)
  {
    scanf("%c", &lettre);
    if( lettre!='\n') // si on n'a pas rencontré LF
    {
      if( (lettre>47) && (lettre<58) )    // si la saisie est un chiffre
      {
        digit[compteur_saisie++]=lettre ; // lire la saisie   
      }
      else if (lettre == 47) // si la saisie est le / séparateur d'éléments de date
      {
        switch(compteur_element_date)
        {
          case 0: // conversion du jour en int
            if(compteur_saisie > 0 && compteur_saisie < 3)
            {
              *jour = atoi(digit) ;
              compteur_saisie=0 ;
              compteur_element_date++ ;
              digit[0]='\0' ;
            }
            else
            {
              erreur = 1;
            }
            break;
          case 1: // conversion du mois en int
            if(compteur_saisie > 0 && compteur_saisie < 3)
              {
                *mois = atoi(digit) ;
                compteur_saisie=0 ;
                compteur_element_date++ ;
                digit[0]='\0' ;
              }
            else
            {
              erreur = 1;
            }
            break;
          default: erreur=1 ; break ;
        }
      }
    }
    else if (lettre == '\n' && (compteur_saisie == 4) && compteur_element_date == 2)
    {
      digit[compteur_saisie] = '\0' ;
      *annee = atoi(digit) ;
      compteur_saisie=0 ;
      compteur_element_date++ ;
    }
    else
    {
      erreur = 1;
    }
  } // fin du while lecture saisie et contrôle du format

  // contrôle de cohérence de date
  if(compteur_saisie>=MAX_DIGIT)
  {
    erreur=1;
  }
  switch(*mois)
  {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      if( (*jour<1 || *jour>31) )
      {
        erreur=1;
      }
      break;
    case 4: case 6: case 9: case 11:
      if( (*jour<1 || *jour>30) )
      {
        erreur=1;
      }
      break;
    case 2 :
      if((*mois % 4 == 0 && *mois % 100 != 0) || *mois % 400 == 0)
      {
        if( (*jour<1 || *jour>29) )
        {
          erreur=1;
        }
      }
      else
      {
        if( (*jour<0 || *jour>28) )
        {
          erreur=1;
        }
      }
      break;
    default :
      erreur=1;
      break ;
  }
  return erreur ;
}


/* ---------------------------------------------- */
/* -- Incrémentation/décrémentation d'une date -- */
/* ---------------------------------------------- */
void date_suivante_precedente(int *jhebdo_rech, int *jour_rech, int *mois_rech, int *annee_rech, int increment)
{
  int i ;
  int jhebdo, jour, mois, annee ;

  jhebdo = *jhebdo_rech ;
  jour   = *jour_rech   ;
  mois   = *mois_rech   ;
  annee  = *annee_rech  ;

  if (increment == 0)
  {
    *jhebdo_rech = jhebdo ;
    *jour_rech   = jour   ;
    *mois_rech   = mois   ;
    *annee_rech  = annee  ;
  }

  // incrementation
  else if (increment > 0)
  {    
    for (i=0 ; i < increment ; i++) // de 0 à l'incrément (nombre de jours de décalage)
    {                               // on incrémente les 4 variables de date :
      switch(mois)                  // jour et mois selon le nombre de jours du mois
      {
        case 1 : case 3 : case 5 : case 7 : case 8 : case 10 : case 12 : // les mois de 31 jours
          if(jour<31)               // si le jour n'est pas le 31
          {
            jour++;                 // incrémentation du jour
          }
          else                      // si le jour est le 31 (dernier du mois)
          {
            jour=1;                 // le prochain jour est le 1er
            mois++;                 // du mois suivant
          }
          break;
        case 2 :                    // en février
          if((annee % 4 == 0 && annee % 100 != 0) || annee % 400 == 0) // si l'année est bissextile
          {
            if(jour<29)             // si le jour n'est pas 29 (dernier du mois)
            {
              jour++;               // incrémentation du jour
            }
            else                    // sinon (le jour est le dernier du mois)
            {
              jour=1;               // le prochain jour est le 01/03
              mois++;
            }
          }
          else                      // si l'année n'est pas bissextile
          {
            if(jour<28)             // si le jour n'est pas 28 (dernier du mois)
            {         
              jour++;               // incrémentation du jour
            }
            else                    // sinon (le jour est le dernier du mois)
            {
              jour=1;               // le prochain jour est le 01/03
              mois++;
            }
          }
          break;
        default :                  // pour tous les autres mois (ceux de 30 jours)
          if(jour<30)              // si le jour n'est pas le 30 (dernier du mois)
          {
            jour++;                // incrémentation du jour
          }
          else                     // sinon (le jour est le dernier du mois)
          {
            jour=1;                // le prochain jour est le 1er
            mois++;                // du mois suivant
          }
          break;
      } // fin du switch (selon le nombre de jours du mois)
      if(mois==13)                 // si on a changé d'année dans le switch
      {
        mois=1;                    // le mois est janvier
        annee++;                   // incrémentation de l'année
      }
      jhebdo++;                    // incrémentation du jour de semaine
      if (jhebdo==7)
      {       
        jhebdo=0;                  // remise à 0 si jour de semaine 7 (convention : 0 à 6)          
      }
    } // fin du for d'incrémentation d'un jour hebdo
  } // fin du if d'incrémentation

  // décrémentation
  else
  {
    for (i=0 ; i > increment ; i--) // de 0 à l'incrément (nombre de jours de décalage)
    {                               // on décrémente les 4 variables de date :
      switch(mois)                  // jour et mois selon le nombre de jours du mois précédent
      {
        case 1 : case 2 : case 4 : case 6 : case 7 : case 9 : case 11 : // le mois précédent a 31 jours
          if(jour>1)                // si le jour n'est pas le 1er
          {
            jour--;                 // decrémentation du jour
          }
          else                      // sinon (le jour est le premier du mois)
          {
            jour=31;                // le jour précédent est le 31
            mois--;                 // du mois précédent
          }
          break;
        case 3 :                   // en mars
          if((annee % 4 == 0 && annee % 100 != 0) || annee % 400 == 0) // si l'année est bissextile
          {
            if(jour>1)             // si le jour n'est pas le 1er
            {
              jour--;              // décrémentation du jour
            }
            else                   // sinon (le jour est le premier du mois)
            {
              jour=29;             // le jour précédent est le 29
              mois--;              // ddu mois précédent (février)
            }
          }
          else                     // si l'année n'est pas bissextile
          {
            if(jour>1)             // si le jour n'est pas le 1er
            {         
              jour--;              // decrémentation du jour
            }
            else                   // sinon (le jour est le premier du mois)
            {
              jour=28;             // le jour précédent est le 28
              mois--;              // du mois précédent (février)
            }
          }
          break;
        default :                  // pour tous les autres mois (ceux de 30 jours)
          if(jour>1)               // si le jour n'est pas le 1er
          {
            jour--;                // décrémentation du jour
          }
          else                     // sinon (le jour est le premier du mois)
          {
            jour=30;               // le jour précédent est le 30
            mois--;                // du mois précent
          }
          break;
      } // fin du switch (selon le nombre de jours du mois)
      if(mois==0)                  // si on a changé d'année dans le switch
      {
        mois=12;                   // le mois est décembre
        annee--;                   // décrémentation de l'année
      }
      jhebdo--;                    // décrémentation du jour de semaine
      if (jhebdo==-1)
      {
        jhebdo=6;                  // remise à 0 si jour de la semaine -1 (convention : 0 à 6)
      } 
    } // fin du for de décrémentation d'un jour hebdo
  }  // fin du else de décrémentation
  *jhebdo_rech = jhebdo ;
  *jour_rech   = jour   ;
  *mois_rech   = mois   ;
  *annee_rech  = annee  ;
}


// ====================================== //
/* === Fonctions et procédures outils === */
// ====================================== //

/* ------------------------------------------ */
/* -- Conversion d'une chaine en majuscule -- */
/* ------------------------------------------ */
void convmaj(char chaine[])
{
  int i ;

  for (i=0 ; i < strlen(chaine) ; i++)
  {
    chaine[i] = toupper(chaine[i]) ;
  }
}

/* ------------------------------------------------------------------------------ */
/* Remplace toutes les occurrences de motif_a_remplacer par motif_de_remplacement */
/* ------------------------------------------------------------------------------ */
char * str_replace_all(char * chaine, char * motif_a_remplacer, char * motif_de_remplacement)
{
  char * pt_motif_a_remplacer = strstr(chaine, motif_a_remplacer), * chaine_retour = NULL;
  int taille_pt_motif_a_remplacer, taille_motif_a_remplacer, taille_motif_de_remplacement ;
  while (pt_motif_a_remplacer != NULL)
  {
    taille_pt_motif_a_remplacer = strlen(pt_motif_a_remplacer) ;
    taille_motif_a_remplacer = strlen(motif_a_remplacer) ;
    taille_motif_de_remplacement = strlen(motif_de_remplacement) ;
    if (taille_motif_a_remplacer != taille_motif_de_remplacement)
    {
      /* Ajuster la taille pour pouvoir placer motif_de_remplacement */
      memmove(pt_motif_a_remplacer + taille_motif_de_remplacement,
      pt_motif_a_remplacer + taille_motif_a_remplacer, taille_pt_motif_a_remplacer);
    }
    /* Remplacer par motif_de_remplacement */
    strncpy(pt_motif_a_remplacer, motif_de_remplacement,
    taille_motif_de_remplacement);
    /* On prépare pour la prochaine itération */
    pt_motif_a_remplacer = strstr(chaine, motif_a_remplacer);
    /* On met a jour la chaine de retour */
    chaine_retour = chaine;
  }
  return chaine_retour;
}

//* ------------------------------------------------ */
// --- fonction outil de suppression des accents --- */
/* ------------------------------------------------- */
char * supprime_accent(char * chaine)
// void supprime_accent(char chaine[])
{
  char * chaine_retour = chaine, *motif_a_remplacer, *motif_de_remplacement ;
  size_t nb_elements ;
  int i ;
  /* tableau de tous les caractères accentués à remplacer */
  const char * tab_caracteres_a_remplacer[]
   = {"À", "Á", "Â", "Ã", "Ä", "Å",
  "Æ", "Ç", "È", "É", "Ê", "Ë", "Ì", "Í", "Î", "Ï", "Ð", "Ñ", "Ò", "Ó", "Ô", "Õ",
  "Ö", "Ø", "Ù", "Ú", "Û", "Ü", "Ý", "ß", "à", "á", "â", "ã", "ä", "å", "æ", "ç",
  "è", "é", "ê", "ë", "ì", "í", "î", "ï", "ñ", "ò", "ó", "ô", "õ", "ö", "ø", "ù",
  "ú", "û", "ü", "ý", "ÿ", "Ā", "ā", "Ă", "ă", "Ą", "ą", "Ć", "ć", "Ĉ", "ĉ", "Ċ",
  "ċ", "Č", "č", "Ď", "ď", "Đ", "đ", "Ẽ", "ẽ", "Ē", "ē", "Ĕ", "ĕ", "Ė", "ė", "Ę",
  "ę", "Ě", "ě", "Ĝ", "ĝ", "Ğ", "ğ", "Ġ", "ġ", "Ģ", "ģ", "Ĥ", "ĥ", "Ħ", "ħ", "Ĩ",
  "ĩ", "Ī", "ī", "Ĭ", "ĭ", "Į", "į", "İ", "ı", "IJ", "ij", "Ĵ", "ĵ", "Ķ", "ķ", "Ĺ",
  "ĺ", "Ļ", "ļ", "Ľ", "ľ", "L·", "l·", "Ł", "ł", "Ń", "ń", "Ņ", "ņ", "Ň", "ň", "ʼn",
  "Ō", "ō", "Ŏ", "ŏ", "Ő", "ő", "Œ", "œ", "Ŕ", "ŕ", "Ŗ", "ŗ", "Ř", "ř", "Ś", "ś",
  "Ŝ", "ŝ", "Ş", "ş", "Š", "š", "Ţ", "ţ", "Ť", "ť", "Ŧ", "ŧ", "Ũ", "ũ", "Ū", "ū",
  "Ŭ", "ŭ", "Ů", "ů", "Ű", "ű", "Ų", "ų", "Ŵ", "ŵ", "Ŷ", "ŷ", "Ÿ", "ÿ", "Ź", "ź",
  "Ż", "ż", "Ž", "ž", "s", "ƒ", "Ơ", "ơ", "Ư", "ư", "Ǎ", "ǎ", "Ǐ", "ǐ", "Ǒ", "ǒ",
  "Ǔ", "ǔ", "Ǖ", "ǖ", "Ǘ", "ǘ", "Ǚ", "ǚ", "Ǜ", "ǜ", "Ǻ", "ǻ", "Ǽ", "ǽ", "Ǿ", "ǿ"};
  /* tableau de tous les caractères sans accent de remplacement */
  const char * tab_caracteres_de_remplacement[] = {"A", "A", "A", "A", "A", "A",
  "AE", "C", "E", "E", "E", "E", "I", "I", "I", "I", "D", "N", "O", "O", "O", "O",
  "O", "O", "U", "U", "U", "U", "Y", "s", "a", "a", "a", "a", "a", "a", "ae", "c",
  "e", "e", "e", "e", "i", "i", "i", "i", "n", "o", "o", "o", "o", "o", "o", "u",
  "u", "u", "u", "y", "y", "A", "a", "A", "a", "A", "a", "C", "c", "C", "c", "C",
  "c", "C", "c", "D", "d", "D", "d", "E", "e", "E", "e", "E", "e", "E", "e", "E",
  "e", "E", "e", "G", "g", "G", "g", "G", "g", "G", "g", "H", "h", "H", "h", "I",
  "i", "I", "i", "I", "i", "I", "i", "I", "i", "IJ", "ij", "J", "j", "K", "k",
  "L", "l", "L", "l", "L", "l", "L", "l", "l", "l", "N", "n", "N", "n", "N", "n",
  "n", "O", "o", "O", "o", "O", "o", "OE", "oe", "R", "r", "R", "r", "R", "r",
  "S", "s", "S", "s", "S", "s", "S", "s", "T", "t", "T", "t", "T", "t", "U", "u",
  "U", "u", "U", "u", "U", "u", "U", "u", "U", "u", "W", "w", "Y", "y", "Y", "y",
  "Z", "z", "Z", "z", "Z", "z", "s", "f", "O", "o", "U", "u", "A", "a", "I", "i",
  "O", "o", "U", "u", "U", "u", "U", "u", "U", "u", "U", "u", "A", "a", "AE",
  "ae", "O", "o"};
  nb_elements = sizeof(tab_caracteres_de_remplacement) / sizeof(*tab_caracteres_de_remplacement) ;
  /* boucle de traitement de tous les caractères */
  for (i = 0 ; i < nb_elements ; i++)
  {
    motif_a_remplacer = (char * ) tab_caracteres_a_remplacer[i] ;
    motif_de_remplacement= (char * ) tab_caracteres_de_remplacement[i] ;
    chaine_retour = str_replace_all(chaine, motif_a_remplacer,
    motif_de_remplacement) ;
    if (chaine_retour != NULL)
    {
      chaine = chaine_retour;
    }
  }
  return chaine;
}


/* ------------------------------ */
/* -- Lecture/vidage du buffer -- */
/* ------------------------------ */
void dump_buffer() 
{
  char dump ;
  while(dump!='\n')
  {
    scanf("%c", &dump);
  }
}

/* ---------------------------------------- */
/* -- Conversion du choix de char en int -- */
/* ---------------------------------------- */
// La lecture en char permet de mettre des contrôles
// sur la validité de la saisie
int lecture_choix(int deb, int fin, char lettre, int * erreur)
{
  int  choix ;              // choix reconstitué à retourner
  char debalpha, finalpha ; // bornes des choix du menu

  debalpha = deb + 48 ; // conversion des bornes en char
  finalpha = fin + 48 ;

  // si saisie dans les bornes du choix
  if (((lettre >= debalpha) && (lettre <= finalpha)) && lettre != '\n')
  {
    choix = lettre - 48 ; // conversion du char en int
    dump_buffer() ;
    *erreur=0     ;
    return choix  ;
  }
  // si saisie hors des bornes du choix
  else
  {
    dump_buffer() ;
    *erreur=1     ;
    printf("Veuillez saisir un choix valide (%d à %d)\n", deb, fin);
  }
}

