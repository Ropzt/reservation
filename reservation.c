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

// ==================== //
/* === Déclaration === */
// ==================== //

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Déclaration des constantes ~~~ 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAX_GARE   100
#define MAX_ID     100
#define MAX_NOM     50
#define MAX_DATENUM  9 // pour les dates 20210211
#define MAX_DATE    12 // pour les dates 15/02/2021 // c'est pas plutôt 11 ?
#define MAX_HEURE    9 // pour les horaires 14:15:00
#define MAX_DIGIT   20
#define MAX_PLACES  45
#define MAX_RESERVATION 45

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Déclaration des types globaux ~~~ 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct UnTrajet { // fichier trips.txt
  int  idservice        ;
  char idtrajet[MAX_ID] ;
  int  numtrain         ;
  int  direction        ;
};

struct UnCalendrier { // fichier calendar.txt
  int idservice ;
  int lun       ;
  int mar       ;
  int mer       ;
  int jeu       ;
  int ven       ;
  int sam       ;
  int dim       ;
  int start     ;
  int end       ;
};

struct UnStop { // fichier stops_times.txt
  char idtrajet[MAX_ID] ;
  char ha[MAX_HEURE]    ;
  char hd[MAX_HEURE]    ;
  char idgare[MAX_GARE] ;
  int  sequence         ;
};

struct UneGare { // fichier stops.txt
  char idgare[MAX_ID]      ;
  char nomgare[MAX_GARE]   ;
  char gareattache[MAX_ID] ;
};

struct UnTarif { // fichier tarifs-tgv-par-od.csv
  char  garedep[MAX_GARE] ;
  char  garearr[MAX_GARE] ;
  float prix0             ;
  float prix2             ;
  float prix1             ;
};

struct UnePlace { // niveau place de tab_places
  int wagon    ; // numéro de wagon
  int classe   ; // 1 1re classe, 2 2e classe
  int salle    ; // 0 bas 1 haut
  int siege    ; // numéro de siège
  int position ; // 0 fenêtre, 1 couloir, 2 place isolee
  int billet   ; // numéro unique de billet
};
  
struct UneSeq { // niveau sequence de tab_places
  char gd[MAX_GARE]  ; // gare de départ
  char ga[MAX_GARE]  ; // gare d'arrivée
  int  seqdep        ; // numéro de séquence départ dans le trajet
  int  seqarr        ; // numéro de séquence arrivee dans le trajet
  char hd[MAX_HEURE] ; // heure de départ
  char ha[MAX_HEURE] ; // heure d'arrivée
  struct UnePlace place[45];
};

struct UneDate { // niveau date de tab_places
  int date ;
  struct UneSeq sequence[30] ;
};

struct UnVoyage { // niveau idtrajet de tab_places
  char idtrajet[MAX_ID] ;
  char type[5] ;  // type de train
  struct UneDate date[200] ;
};

struct SeqModif { // niveau sequence de a_sauvegarder
  int modifie ; 
};

struct DateModif { // niveau date de a_sauvegarder
  struct SeqModif sequence[30] ;
};

struct TrajetModif { // niveau idtrajet de a_sauvegarder
  struct DateModif date[200] ;
};

struct date { // éléments d'une date composée
  int jhebdo ;
  int jour   ;
  int mois   ;
  int annee  ;
  int date   ;
};

struct UnHoraire { // horaire de passage d'un trajet dans une gare
  char idtrajet[MAX_ID]  ;
  int  idservice         ;
  int  numtrain          ;
  char ha[MAX_HEURE]     ;
  char hd[MAX_HEURE]     ;
  int  sequence          ;
  char idgare[MAX_ID]    ;
  char nomgare[MAX_GARE] ;
};

struct UnRes_nodate { // résultat origine/destination sans date
  char idtrajet[MAX_ID];
  int  idservice;
  int  numtrain;
  char hd[MAX_HEURE] ;
  char ha[MAX_HEURE] ;
  int  seqdep ;
  int  seqarr ;
  char idgaredep[MAX_ID];
  char garedep[MAX_GARE];
  char idgarearr[MAX_ID];
  char garearr[MAX_GARE];
};

struct UnRes { // résultat origine/destination avec date
  char  idtrajet[MAX_ID]  ;
  int   idservice         ;
  int   numtrain          ;
  char  ha[MAX_HEURE]     ;
  char  hd[MAX_HEURE]     ;
  int   seqdep            ;
  int   seqarr            ;
  char  idgaredep[MAX_ID] ;
  char  garedep[MAX_GARE] ;
  char  idgarearr[MAX_ID] ;
  char  garearr[MAX_GARE] ;
  int   dispo             ;
  int   dispo_1ere        ;
  int   dispo_2nde        ;
  float prix0             ;
  float prix2             ;
  float prix1             ;
};

struct Stockage { // compteurs de chaque niveau de tab_places
    char idtrajet[MAX_ID] ;
    int  max_date         ;
    int  max_seq          ;
    int  max_place        ;
};

struct UnBillet{
  int  place           ; // n dans le tableau des places de : tab_places_dispo.tab_dispo[n] 
  char nom[MAX_NOM]    ;
  char prenom[MAX_NOM] ;
  int  age             ;
  int  billet          ; // code billet (date*10)+i
  float prix           ;
};
  
struct UnBilletRegistre { // billets des voyages réservées
  int   date              ; 
  char  idtrajet[MAX_ID]  ; 
  char  garedep[MAX_GARE] ;
  char  garearr[MAX_GARE] ;
  char  nom[MAX_NOM]      ;
  char  prenom[MAX_NOM]   ;
  int   age               ;
  int   classe            ;
  int   wagon             ;
  int   salle             ;
  int   siege             ;
  int   position          ;
  float prix              ;
  int   billet            ; // code billet (date*10)+i
};

struct position { // quantité de places dispo pour chaque position, par salle
  int fenetre ;
  int couloir ;
  int isole   ;
  int salle   ;
};

struct caract { // tableau de quantité de places dispo pour chaque position, par salle
  struct position position[2]; // 2 car 2 classes
};

struct UnResPlaceDispo {
  char   id[MAX_ID] ;
  struct UnePlace tab_dispo[MAX_PLACES] ;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Déclaration des variables globales ~~~ 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// données sncf
struct UnTrajet *trajets         ;
struct UnCalendrier *calendriers ;
struct UnStop *stops             ;
struct UneGare *gares            ;
struct UnTarif *tarifs           ;
int    nbtrajet     ;
int    nbcalendrier ;
int    nbstop=0     ;
int    nbgare       ;
int    nbtarif      ;
// places
struct UnVoyage *tab_places ;
// places modifiées
struct TrajetModif *a_sauvegarder ;
// dates ouvertes à la vente
struct date *tab_date_vente; // tableau des dates prises en charge par le programme
int    nbdatevente         ; // nb dates ouvertes à la réservation
// compteurs
struct Stockage *stockage_nb_rep; // 
int nbreservationjournee=0;
// billets
struct UnBilletRegistre *registre_billets;
int nb_registre_billet;
// éléments de date système
int    jhebdo_num_sys, jour_sys, mois_sys, annee_sys ;
char   jhebdo_alpha_sys[9] ;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Déclaration préliminaires ~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~ Procédures de chargement/sauvegarde de données ~~~
void chargement()            ;
void chargement_horaires()   ;
void chargement_trajet()     ;
void chargement_calendrier() ;
void chargement_stop()       ;
void chargement_gare()       ;
void chargement_tarifs()     ;
void chargement_places()     ;
void chargement_billets()    ;
void quitter()               ;
void sauvegarde()            ;
void sauvegarde_places()     ;
void sauvegarde_billets()   ;

// ~~~ Procédures et fonctions métier ~~~
void lance_recherche();
struct UnHoraire * recherche_horaire(char rechgare[], int *nbres);
struct UnRes_nodate * compare_nodate(struct UnHoraire gare_dep_trouve[], int nb_gare_dep_trouve, struct UnHoraire gare_arr_trouve[], int nb_gare_arr_trouve, int *nb_res_nodate);
struct UnRes * compare_avecdate(struct UnRes_nodate tab_res_nodate[], int nb_res_nodate, int jhebdo, int *nb_res_date  /*, char date_rech[SizeDate]*/ );
// void calcule_tarif(char garedep[], char garearr[], struct UnTarif *untarif)
void trouve_tarif(char garedep[], char garearr[], /*char type[],*/ struct UnTarif *untarif);
void tri(struct UnRes tab_res[], int * nb_res_date);

int  sequence(char idtrajet[MAX_ID], struct UneSeq tab_sequence[]);
void verification_res_dispo(struct UnRes tab_res[], int nb_res, struct UnResPlaceDispo tab_places_dispo[], int date);
int  circule_date(char idtrajet[MAX_ID], int date);
void crea_date_vente(int jour, int mois, int annee);
int  circule_jhebdo(char idtrajet[MAX_ID], int jhebdo);
void ecriture_resa_in_tab_places( struct UnRes tab_res ,struct UnePlace tab_dispo, int date, int billet);
void reservation(struct UnRes tab_res, int nb_res, struct UnePlace tab_dispo[], int date);
void creation_fichier_billet(struct UnBilletRegistre tab_registre[], int nb_nouveau);
void faux_paiement();
void interprete_position(int pos, char position[]);
void interprete_salle(int etage, char salle[]) ;

// ~~~ Outils de traitement des dates ~~~
int  valide_date(int * jour, int * mois, int * annee) ;
int  calcul_jour_semaine(int jour_rech, int mois_rech, int annee_rech, int jour, int mois, int annee, int jour_sem) ;
void interprete_jour_semaine(int jhebdo_num, char jhebdo_alpha[]) ;
void date_suivante_precedente(int *jhebdo_rech, int *jour_rech, int *mois_rech, int *annee_rech, int increment) ;
int  date_anterieure(int jour, int mois, int annee, int jour_ref, int mois_ref, int annee_ref) ;
void decoupe_date(int date, int *jour, int *mois, int *annee) ;
int  assemble_date(int jour, int mois, int annee) ;

// ~~~ Outils de traitement des choix
int  lecture_choix(int deb, int fin, char lettre, int * erreur);
void saisie_int(char invite[], int min,int max, int *mon_int);
void saisie_text(char invite[], char sortie[]);

// ~~~ Outils de traitement des chaines ~~~
void convmaj(char chaine[]) ;
void tiret_to_space(char chaine[]) ;
void coupe_chaine_au_caractere(char chaine[], char sschaine1[], char sschaine2[], char sep) ;
void nettoie_debut_chaine(char chaine[], char pattern[]) ;
void trim(char *ch) ;
char * str_replace_all(char * chaine, char * motif_a_remplacer, char * motif_de_remplacement) ;
// void supprime_accent(char chaine[]) ;
char * supprime_accent(char * chaine);
void dump_buffer() ;

// ~~~ Outils d'interface système ~~~
void date_sys(int *jour, int *mois, int *annee, int *jhebdo_num) ;
void AfficheObjetInfo(char NomObjetInfo[]);
int  ListerRep(char NomRep[], char ListObjet[4000][100]);
void supprime_repertoire(char nomrep[]) ;

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

  chargement() ;

  // test();

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
      erreur=0;
      printf("\n-1- Réserver\n")                ;
      //printf("-2- Consulter les horaires\n")    ;
      //printf("-3- Mes réservations\n")          ;
      printf("-0- Quitter\n")                   ;
      // saisie_int("\nChoix : ",0,1,&choix)       ;
      printf("\nChoix : ")                      ;
      scanf("%c", &lettre)                      ;
      choix = lecture_choix(0,3,lettre,&erreur) ; 
    }
  
    // traitement selon le choix
    switch (choix)
    {
      case 0: quitter();
              break ;
      case 1: lance_recherche() ;
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
/* --- Chargement des données --- */
// ~~~~~~~~~~~
void chargement()
{
  chargement_horaires() ; // chargement des données horaires à partir des fichiers GTFS
  chargement_tarifs()   ; // chargement des données tarifs à partir des fichiers tarifs_... .csv
  crea_date_vente(jour_sys, mois_sys, annee_sys) ;
  // printf("nbdatevente=%d\n",nbdatevente);
  chargement_places()   ; // chargement des places ouvertes à la vente
  // chargement_billets()  ; // chargement des billets de voyages réservés
}

// ~~~~~~~~~~~
/* --- Chargement des données horaires --- */
// ~~~~~~~~~~~
void chargement_horaires()
{
  chargement_trajet()     ; // fichier trips.txt
  chargement_calendrier() ; // fichier calendar.txt
  chargement_stop()       ; // fichier stop_times.txt
  chargement_gare()       ; // fichier stops.txt
}

// ~~~~~~~~~~~
/* --- Chargement des données trajets --- */
// ~~~~~~~~~~~
void chargement_trajet()
{
  FILE *f1           ;
  int  i, retour     ;
  char line[500]     ; // pour lire ligne par ligne avec fgets et sscanf
  char dumpchar[100] ; // pour les champs non gardés
  char dumpcar       ;
  int  dumpint       ;

  /* --- Allocation de mémoire au tableau trajet --- */
  trajets = (struct UnTrajet *) malloc(sizeof(struct UnTrajet));

  /*--- Ouverture fichier horaires --- */
  f1=fopen("./data/horaire/tgv_echantillon/trips.txt","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement des trajets\n") ;
  }
  else
  {
    i=0 ;
    fgets(line, sizeof(line), f1); // lire la 1re ligne et ne rien faire
    while (fgets(line,sizeof(line),f1) != NULL)
    {     
      retour=sscanf(line,"%101[^,],%d,%101[^,],%d,%d,%101[^,],%101[^\n]",
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
  // printf("nombre de trajets %d\n",nbtrajet);
}

// ~~~~~~~~~~~
/* --- Chargement des données calendrier de circulation --- */
// ~~~~~~~~~~~
void chargement_calendrier()
{
  FILE *f1       ;
  int  i, retour ;
  char line[500] ; // pour lire ligne par ligne avec fgets et sscanf
  int  dumpint   ; // pour les champs non gardés

  /* --- Allocation de mémoire au tableau trajet --- */
  calendriers = (struct UnCalendrier *) malloc(sizeof(struct UnCalendrier));

  /*--- Ouverture fichier horaires --- */
  f1=fopen("./data/horaire/tgv_echantillon/calendar.txt","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement des calendriers de circulation\n") ;
  }
  else
  {
    i=0 ;
    fgets(line, sizeof(line), f1);
    while (fgets(line,sizeof(line),f1) != NULL)
    {     
      retour=sscanf(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
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
  // printf("nombre de calendriers %d\n",nbcalendrier);
}

// ~~~~~~~~~~~
/* --- Chargement des données stop en gare --- */
// ~~~~~~~~~~~
void chargement_stop()
{
  FILE *f1       ;
  int  i, retour ;
  char line[500] ; // pour lire ligne par ligne avec fgets et sscanf
  int  dumpint   ; // pour les champs non gardés
  char dumpchar  ;

  /* --- Allocation de mémoire au tableau trajet --- */
  stops = (struct UnStop *) malloc(sizeof(struct UnStop));

  /*--- Ouverture fichier horaires --- */
  f1=fopen("./data/horaire/tgv_echantillon/stop_times.txt","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement des horaires de passage en gare\n") ;
  }
  else
  {
    i=0 ;
    fgets(line, sizeof(line), f1);
    while (fgets(line,sizeof(line),f1) != NULL)
    {     
      retour=sscanf(line,"%101[^,],%10[^,],%10[^,],%101[^,],%d,%101[^,],%d,%d,%d",
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
  // printf("nombre de stops %d\n",nbstop);
}

// ~~~~~~~~~~~
/* --- Chargement des données gares --- */
// ~~~~~~~~~~~
void chargement_gare()
{
  FILE *f1       ;
  int  i, retour ; // pour lire ligne par ligne avec fgets et sscanf
  char line[500] ; // pour lire ligne par ligne avec sscanf
  int  dumpint   ; // pour les champs non gardés
  char dumpchar  ;
  char pattern1[10]="GARE DE " ;

  /* --- Allocation de mémoire au tableau trajet --- */
  gares = (struct UneGare *) malloc(sizeof(struct UneGare));

  /*--- Ouverture fichier horaires --- */
  f1=fopen("./data/horaire/tgv_echantillon/stops.txt","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement des gares\n") ;
  }
  else
  {
    i=0 ;
    fgets(line, sizeof(line), f1);
    while (fgets(line,sizeof(line),f1) != NULL)
    {     
      retour=sscanf(line,"%101[^,],\"%101[^\"],%101[^,],%101[^,],%101[^,],%101[^,],%d,%101[^\n]",
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
      nettoie_debut_chaine(gares[i].nomgare,pattern1) ;
      trim(gares[i].nomgare) ;
      tiret_to_space(gares[i].nomgare);
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
  // printf("nombre de gares %d\n",nbgare);
}

// ~~~~~~~~~~~
/* --- Chargement des données de tarifs --- */
// ~~~~~~~~~~~
void chargement_tarifs()
{
  FILE *f1       ;
  int  i, retour ;
  char line[500] ; // pour lire ligne par ligne avec sscanf
  char od[200]   ; // origine-destination
  char dumpchar[MAX_GARE];
  char sep='-';
  char prix0[MAX_GARE],prix2[MAX_GARE],prix1[MAX_GARE];

  /* --- Allocation de mémoire au tableau trajet --- */
  tarifs = (struct UnTarif *) malloc(sizeof(struct UnTarif));

  /*--- Ouverture fichier horaires --- */
  f1=fopen("./data/tarif/tarifs-tgv-par-od.csv","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement des tarifs\n") ;
  }
  else
  {
    i=0 ;
    fgets(line, sizeof(line), f1);
    while (fgets(line,sizeof(line),f1) != NULL)
    {
      retour=sscanf(line,"%202[^;];%101[^;];%101[^;];%101[^;];%101[^\n]",
        od,
        prix0,
        prix2,
        prix1,
        dumpchar);

        tarifs[i].prix0=atof(prix0);
        tarifs[i].prix2=atof(prix2);
        tarifs[i].prix1=atof(prix1);
      
      coupe_chaine_au_caractere(od,tarifs[i].garedep,tarifs[i].garearr,sep);
      convmaj(tarifs[i].garedep);
      tiret_to_space(tarifs[i].garedep) ;
      convmaj(tarifs[i].garedep);
      tiret_to_space(tarifs[i].garedep) ;
      // supprime_accent(tarifs[i].garedep);
      // supprime_accent(tarifs[i].garearr);
      i++ ;
      nbtarif = i ;
      tarifs = (struct UnTarif *) realloc(tarifs,sizeof(struct UnTarif) * (nbtarif+1)) ;
    }
    fclose(f1) ;
  }
  // printf("nombre de tarifs %d\n",nbtarif);
}

// ~~~~~~~~~~~
/* --- Chargement des données places à réserver --- */
// ~~~~~~~~~~~
void chargement_places()
{
  int i ; // compteur trajets
  int j ; // compteur date
  int k ; // compteur sequence
  int l=0;// compteur places
  int m,retour,existe;  // compteur à tout faire

  // chemin des répertoires
  char nomrep[MAX_ID];
  char nomrepid[MAX_ID];
  char nomrepdate[MAX_ID];
  char nomrepseq[MAX_ID];
  char fichiertrain[MAX_ID];
  char fichierplace[MAX_ID] ;

  // convertir les int en chaine
  char datechar[MAX_DATENUM] ;
  char seqdepchar[3];

  // tableau des noms de répertoire
  char rep_idtrajet[4000][MAX_ID];
  char rep_date[200][MAX_ID];
  char rep_seq[30][MAX_ID];
  char rep_places[1][MAX_ID];

  // bornes des tableaux des noms de répertoire (=nb répertoires)
  int nbrep_idtrajet;
  int nbrep_date ;
  int nbrep_seq ;

  // variables isolées identiques à celles présentes dans les structures
  char idtrajet[MAX_ID] ;
  int  wagon, classe, salle, siege, position, billet;

  struct UneSeq tab_sequence[30] ;
  int  nbsequence ;
  struct UnePlace placeslues[50] ; // c'est 50 pour la démo, sinon c'est le nb de places d'un train
  int nbplacestrain=0 ;
  int nbplaces=0 ;

  FILE *f1 ;

  // PARTIE TRAITEMENT

  tab_places = (struct UnVoyage *) malloc(sizeof(struct UnVoyage)) ;
  stockage_nb_rep = (struct Stockage *) malloc (sizeof(struct Stockage));
  strcpy(nomrep,"./data/place");
  a_sauvegarder = (struct TrajetModif *) malloc(sizeof(struct TrajetModif)) ;

  // On liste le contenu de data/place/ (sous-dossiers id)
  nbrep_idtrajet=ListerRep(nomrep,rep_idtrajet);

  // Pour chaque idtrajet possible
  existe=0;
  for (i=0;i<nbtrajet;i++)
  {
    // On compare avec ceux qui sont présents et on crée les dossiers manquants
    if (nbrep_idtrajet>0)
    {
      for (m=0;m<nbrep_idtrajet;m++)
      {
        if (strcmp(trajets[i].idtrajet,rep_idtrajet[m])==0)
        {
          existe=1;
        }
        // printf("existe=%d %d/%d\n",existe,nbrep_idtrajet,nbtrajet);
        if (existe == 0)
        {
          strcpy(nomrepid,nomrep);
          strcat(nomrepid,"/");
          strcat(nomrepid,trajets[i].idtrajet);
          mkdir(nomrepid,0700);
        }
      }
      existe=0;
    }
    // S'il n'y avait aucun dossier, on crée tous les dossiers
    else
    {
      strcpy(nomrepid,nomrep);
      strcat(nomrepid,"/");
      strcat(nomrepid,trajets[i].idtrajet);
      mkdir(nomrepid,0700);
    }
  }

  // On RE-liste le contenu de place (sous-dossiers id)
  nbrep_idtrajet = ListerRep(nomrep,rep_idtrajet);
  stockage_nb_rep = (struct Stockage *) realloc(stockage_nb_rep, sizeof(struct Stockage) * (nbrep_idtrajet+1)) ;

  // POUR CHAQUE DOSSIER NIVEAU IDTRAJET
  for (i=0;i<nbrep_idtrajet;i++)
  {
    strcpy(stockage_nb_rep[i].idtrajet, rep_idtrajet[i]) ;
    // printf("i commencé = %d/%d\n",i,nbrep_idtrajet);

    // On récupère la liste des places de ce type de train
    // if (strcmp(type,"TGV")==0) // À adapter si on gère plusieurs types de train
    // {
      strcpy(fichiertrain,"./data/train/placement_tgv_duplex_echantillon.txt");
    // }
    f1=fopen(fichiertrain,"r") ;
    if (f1 == NULL)
    {
      printf("Problème d'ouverture de %s\n",fichiertrain) ;
    }
    else
    {
      m=0;
      while (! feof(f1))
      {
        retour=fscanf(f1,"%d,%d,%d,%d,%d",&wagon,&classe,&salle,&siege,&position) ;
        if (retour != EOF)
        {
          placeslues[m].wagon = wagon  ;
          placeslues[m].classe = classe;
          placeslues[m].salle = salle ;
          placeslues[m].siege = siege ;
          placeslues[m++].position = position ;
        }
      }
      nbplacestrain=m;
      // printf("nbplacestrain au chargement=%d\n",nbplacestrain);
    }
    fclose(f1);
    stockage_nb_rep[i].max_place = nbplacestrain ;
    // printf("%d\n",stockage_nb_rep[i].max_place );

    // on reconstruit les séquences pour récupérer certaines infos
    strcpy(idtrajet,rep_idtrajet[i]);
    nbsequence = sequence(idtrajet, tab_sequence);
    stockage_nb_rep[i].max_seq = nbsequence ;

    // printf("nbsequence %d de idtrajet=%s\n",nbsequence,trajets[i].idtrajet) ;

    // On liste le contenu de id (sous-dossiers date)
    strcpy(nomrepid,nomrep);
    strcat(nomrepid,"/");
    strcat(nomrepid,rep_idtrajet[i]);
    nbrep_date=ListerRep(nomrepid,rep_date);

    // Pour chaque date de voyage ouverte à la vente
    existe=0;
    for (m=0;m<nbdatevente;m++)
    {
      // Si ce train circule ce jour-là
      if (circule_date(rep_idtrajet[i],tab_date_vente[m].date))
      {
        // On compare avec ceux qui sont déjà présents et on crée les dossiers date manquants
        if (nbrep_date>0)
        {
          for (j=0;j<nbrep_date;j++)
          {
            if (tab_date_vente[m].date==atoi(rep_date[j]))
            {
              existe=1;
            }
            if (existe == 0)
            {
              strcpy(nomrepdate,nomrep);
              strcat(nomrepdate,"/");
              strcat(nomrepdate,rep_idtrajet[i]);
              strcat(nomrepdate,"/");
              sprintf(datechar,"%d",tab_date_vente[m].date);
              strcat(nomrepdate,datechar);
              mkdir(nomrepdate,0700);
            }
          }
          existe=0;
        }
        // Sinon, il n'y avait aucun dossier, on crée tous les dossiers
        else
        {
          strcpy(nomrepdate,nomrep);
          strcat(nomrepdate,"/");
          strcat(nomrepdate,rep_idtrajet[i]);
          strcat(nomrepdate,"/");
          sprintf(datechar,"%d",tab_date_vente[m].date);
          strcat(nomrepdate,datechar);
          mkdir(nomrepdate,0700);
        }
      }
    }

    // On RE-liste le contenu de id (sous-dossiers date)
    nbrep_date=ListerRep(nomrepid,rep_date);
    stockage_nb_rep[i].max_date = nbrep_date ;

    // POUR CHAQUE DOSSIER NIVEAU DATE
    for (j=0;j<nbrep_date;j++)
    {
      // On construit leur path
      strcpy(nomrepdate,nomrep);
      strcat(nomrepdate,"/");
      strcat(nomrepdate,rep_idtrajet[i]);
      strcat(nomrepdate,"/");
      strcat(nomrepdate,rep_date[j]);

      // On supprime ceux qui ont une date passée
      if (atoi(rep_date[j])<tab_date_vente[0].date)
      {
        // printf("%d\n",tab_date_vente[0].date);
        supprime_repertoire(nomrepdate) ;
      }
      else
      {
        // On liste le contenu de date (sous-dossiers sequence)
        nbrep_seq=ListerRep(nomrepdate,rep_seq);

        // S'il est vide, on crée les dossiers sequence
        if (nbrep_seq == 0)
        {
          if (nbsequence>=1)
          {
            for (k=0;k<nbsequence;k++)
            {
              strcpy(nomrepseq,nomrep);
              strcat(nomrepseq,"/");
              strcat(nomrepseq,rep_idtrajet[i]);
              strcat(nomrepseq,"/");
              strcat(nomrepseq,rep_date[j]);
              strcat(nomrepseq,"/");
              sprintf(seqdepchar,"%d",tab_sequence[k].seqdep);
              strcat(nomrepseq,seqdepchar);
              mkdir(nomrepseq,0700);
            }
          }
        }

        // On RE-liste le contenu de date (sous-dossiers sequence)
        nbrep_seq=ListerRep(nomrepdate,rep_seq);
        // printf("nbrep_seq ici : %d\n");
        // printf("%s %s nbrepseq=%d\n", rep_idtrajet[i],rep_date[j],nbrep_seq);

        // POUR CHAQUE DOSSIER NIVEAU SEQUENCE
        for (k=0;k<nbrep_seq;k++)
        {
          // On construit le path
          strcpy(nomrepseq,nomrep);
          strcat(nomrepseq,"/");
          strcat(nomrepseq,rep_idtrajet[i]);
          strcat(nomrepseq,"/");
          strcat(nomrepseq,rep_date[j]);
          strcat(nomrepseq,"/");
          sprintf(seqdepchar,"%d",tab_sequence[k].seqdep);
          strcat(nomrepseq,seqdepchar);
        
          // On liste le contenu de sequence (fichier places.txt)
          existe=ListerRep(nomrepseq,rep_places);

          // Si le dossier sequence est vide, on crée le fichier places.txt
          if(existe == 0)
          {
            // On construit le path du fichier
            strcpy(fichierplace,nomrepseq);
            strcat(fichierplace,"/");
            strcat(fichierplace,"places.txt");

            f1=fopen(fichierplace,"w");
            if (f1 == NULL)
            {
              printf("Problème d'ouverture de %s\n",fichierplace) ;
            }
            else
            {
              for (m=0;m<nbplacestrain;m++)
              {
                fprintf(f1,"%d;%d;%d;%d;%d;0",placeslues[m].wagon,placeslues[m].classe,placeslues[m].salle,placeslues[m].siege,placeslues[m].position);
                if (m<nbplacestrain-1)
                {
                  fprintf(f1,"\n");
                }
              }
              fclose(f1);
            }
          }

          // On est maintenant sûr d'avoir un fichier, on le lit et on charge tab_places
          strcpy(fichierplace,nomrep);
          strcat(fichierplace,"/");
          strcat(fichierplace,rep_idtrajet[i]);
          strcat(fichierplace,"/");
          strcat(fichierplace,rep_date[j]);
          strcat(fichierplace,"/");
          sprintf(seqdepchar,"%d",tab_sequence[k].seqdep);
          strcat(fichierplace,seqdepchar);
          strcat(fichierplace,"/");
          strcat(fichierplace,"places.txt");

          f1 = fopen(fichierplace,"r");
          if (f1 == NULL)
          {
            printf("\nFichier place %s pas trouvé\n",fichierplace);
          }
          else
          {
            l=0;
            while (! feof(f1) )
            {
              retour=fscanf(f1,"%d;%d;%d;%d;%d;%d",&wagon,&classe,&salle,&siege,&position,&billet) ;
              // printf("c'est pas là %d;%d;%d;%d;%d;%d\n",wagon,classe,salle,siege,position,billet);
              if (retour != EOF)
              {
                /* Création d'une nouvelle case à la fin du tableau */
                tab_places = (struct UnVoyage *) realloc(tab_places, sizeof(struct UnVoyage) * (nbplaces + 1)) ;
                a_sauvegarder = (struct TrajetModif *) realloc(a_sauvegarder, sizeof(struct TrajetModif) * (nbplaces + 1)) ;

                strcpy(tab_places[i].idtrajet,rep_idtrajet[i]);
                strcpy(tab_places[i].type,"TGV"); // remplacer par un type selon l'idtrajet
               tab_places[i].date[j].date = atoi(rep_date[j]);
                strcpy(tab_places[i].date[j].sequence[k].gd,tab_sequence[k].gd);
                strcpy(tab_places[i].date[j].sequence[k].ga,tab_sequence[k].ga);
               tab_places[i].date[j].sequence[k].seqdep = tab_sequence[k].seqdep ;
               tab_places[i].date[j].sequence[k].seqarr = tab_sequence[k].seqarr ;
                strcpy(tab_places[i].date[j].sequence[k].hd,tab_sequence[k].hd);
                strcpy(tab_places[i].date[j].sequence[k].ha,tab_sequence[k].ha);

                tab_places[i].date[j].sequence[k].place[l].wagon  = wagon ;
                tab_places[i].date[j].sequence[k].place[l].classe = classe ;
                tab_places[i].date[j].sequence[k].place[l].salle  = salle ;
                tab_places[i].date[j].sequence[k].place[l].siege  = siege ;
                tab_places[i].date[j].sequence[k].place[l].position = position ;
                tab_places[i].date[j].sequence[k].place[l].billet = billet ;

                a_sauvegarder[i].date[j].sequence[k].modifie = 0 ;

                l++ ;
                // printf("Ça va i=%d=%s j=%d=%s k=%d=%d nbsequence=%d l=%d %d;%d;%d;%d;%d\n",i,rep_idtrajet[i],j,rep_date[j],k,tab_sequence[k].seqdep,nbsequence,l,wagon,classe,salle,siege,position) ;
              }
            }
            nbplaces+=l ;
            // printf("nbplaces=%d\n",nbplaces);
            fclose(f1) ;
          }
        }
      }
    }
  // printf("i terminé = %d\n",i);
  // printf("dans stockage idtrajet=%s maxdate=%d maxseq=%d maxplace=%d\n",stockage_nb_rep[i].idtrajet,stockage_nb_rep[i].max_date, stockage_nb_rep[i].max_seq,stockage_nb_rep[i].max_place);
  }
  // printf("%d places chargées tous trains et toutes dates confondues\n",nbplaces);
}

/* ---------------------------------------------- */
/* --- Chargement des informations de billets --- */
/* ---------------------------------------------- */
void chargement_billets()
{  
  FILE *f1;
  int i;
  
  registre_billets = (struct UnBilletRegistre *) malloc(sizeof(struct UnBilletRegistre));
  //registre_billets = (struct UnBilletRegistre *) realloc(registre_billets,sizeof(struct UnBilletRegistre) * (i+1)) ;
      
  f1=fopen("data/registre_billet.txt","r");
  
  i=0;
  //Chargement des donnees
  if(f1==NULL)
  {
    strcpy(registre_billets[i].idtrajet,"\0");
    strcpy(registre_billets[i].garedep,"\0");
    strcpy(registre_billets[i].garearr,"\0");
    strcpy(registre_billets[i].nom,"\0");
    strcpy(registre_billets[i].prenom,"\0");
    registre_billets[i].date    = 0;
    registre_billets[i].age     = 0;
    registre_billets[i].classe  = 0;
    registre_billets[i].wagon   = 0;
    registre_billets[i].salle   = 0;
    registre_billets[i].siege   = 0;
    registre_billets[i].position= 0;
    registre_billets[i].billet  = 0;
  }
  else
  {
    while((! feof(f1)))
    {
      fscanf(f1,"%101[^;];%101[^;];%101[^;];%51[^;];%51[^;];"
        , registre_billets[i].idtrajet
        , registre_billets[i].garedep
        , registre_billets[i].garearr
        , registre_billets[i].nom
        , registre_billets[i].prenom);
                                  
      fscanf(f1, "%d;%d;%d;%d;%d;%d;%d;%d;%f"
        , &registre_billets[i].date
        , &registre_billets[i].age
        , &registre_billets[i].classe
        , &registre_billets[i].wagon
        , &registre_billets[i].salle
        , &registre_billets[i].siege
        , &registre_billets[i].position
        , &registre_billets[i].billet   
        , &registre_billets[i].prix);
      
      i++;
      registre_billets = (struct UnBilletRegistre *) realloc(registre_billets,sizeof(struct UnBilletRegistre) * (i+1)) ;
    }
  }
  nb_registre_billet=i;
  fclose(f1);
}

/* ---------------------------- */
/* --- Quitter le programme --- */
/* ---------------------------- */
void quitter()
{
  sauvegarde();
  printf("À bientôt sur SNCF Voyages\n") ; 
} 

/* ---------------------------------------------- */
/* --- Sauvegarder les données de réservation --- */
/* ---------------------------------------------- */
void sauvegarde()
{
  sauvegarde_places()  ;
  // sauvegarde_billets() ;
}

/* ---------------------------------------------------------------- */
/* --- Sauvegarder les données de réservation (de chaque train) --- */
/* ---------------------------------------------------------------- */
void sauvegarde_places()
{
  int i, j, k, l;
  FILE *f1 ;
  struct UnVoyage places;

  // chemin des répertoires
  char nomrep[MAX_ID] = "./data/place/";
  char fichierplace[MAX_ID] ;
  // int en chaine
  char datechar[MAX_DATENUM] ;
  char seqdepchar[3];
  int  nbplacestrain;

  // pour chaque idtrajet
  for (i=0;i<nbtrajet;i++)
  {
    // chaque date ouverte de ce trajet
    for (j=0;j<stockage_nb_rep[i].max_date;j++)
    {
      // chaque sequence
      for (k=0;k<stockage_nb_rep[i].max_seq;k++)
      {
        // si les données ont ete modifiees au niveau de la sequence
        // ecrire le fichier
        if (a_sauvegarder[i].date[j].sequence[k].modifie == 1 )
        {
          strcpy(fichierplace,nomrep);
          strcat(fichierplace,"/");
          strcat(fichierplace,tab_places[i].idtrajet);
          strcat(fichierplace,"/");
          sprintf(datechar,"%d",tab_places[i].date[j].date);
          strcat(fichierplace,datechar);
          strcat(fichierplace,"/");
          sprintf(seqdepchar,"%d",tab_places[i].date[j].sequence[k].seqdep);
          strcat(fichierplace,seqdepchar);
          strcat(fichierplace,"/");
          strcat(fichierplace,"places.txt");

          f1 = fopen(fichierplace,"w");
          if(f1 == NULL)
          {
            printf("\nImpossible d'écrire le fichier %s\n",fichierplace);
          }
          else
          {
            for (l=0;l<stockage_nb_rep[i].max_place;l++)
            {
              fprintf(f1,"%d;%d;%d;%d;%d;%d",
               tab_places[i].date[j].sequence[k].place[l].wagon,
               tab_places[i].date[j].sequence[k].place[l].classe,
               tab_places[i].date[j].sequence[k].place[l].salle,
               tab_places[i].date[j].sequence[k].place[l].siege,
               tab_places[i].date[j].sequence[k].place[l].position,
               tab_places[i].date[j].sequence[k].place[l].billet);
              if (l<stockage_nb_rep[i].max_place-1)
              {
                fprintf(f1,"\n");
              }
            }
            fclose(f1) ;
          }
        }
      }
    }
  }
}

/* ------------------------------------------------------------------- */
/* --- Sauvegarder les données de réservation (de chaque voyageur) --- */
/* ------------------------------------------------------------------- */
void sauvegarde_billets()
{
  FILE *f1;
  int i;
  char fichier[MAX_ID] = "data/registre_billet.txt" ;

  f1=fopen(fichier,"w");
  if(f1 == NULL)
  {
    printf("\nImpossible d'écrire le fichier %s\n",fichier);
  }
  else
  {
    //Ecriture du registre
    for(i=0;i<nb_registre_billet;i++)
    {
      fprintf(f1, "%s;%s;%s;%s;%s;%d;%d;%d;%d;%d;%d;%d;%d;%f\n"
        , registre_billets[i].idtrajet
        , registre_billets[i].garedep
        , registre_billets[i].garearr
        , registre_billets[i].nom
        , registre_billets[i].prenom
        , registre_billets[i].date
        , registre_billets[i].age
        , registre_billets[i].classe
        , registre_billets[i].wagon
        , registre_billets[i].salle
        , registre_billets[i].siege
        , registre_billets[i].position
        , registre_billets[i].billet
        , registre_billets[i].prix);
    }  
    fclose(f1);
  }
}

/* ---------------------------------------- */
/* --- Fonctions de recherche de voyage --- */
/* ---------------------------------------- */
void lance_recherche()
{
  char lettre    ; // char lu au clavier
  int  choix2=-1 ; // choix utilisateur reconstitué
  int  choix_resultat; // choix utilisateur menu resultat
  // codes erreur pour expressions conditionnelles
  int  erreur2=0 ; // la date est valide
  int  erreur3=0 ; // antériorité de la date de voyage / date système (0=égale 1=antérieure -1=postérieure)
  int  erreur4=0 ; // antériorité de la dernière date de voyage à la vente / date de voyage
  int  erreur5=0 ;

  int  i ;
  int  nb_res_depart=0 ;
  int  nb_res_arrive=0 ;
  int  nb_res_nodate=0 ;
  int  nb_res_date=0   ;

  int jour, mois, annee, jhebdo ; // éléments de la date de voyage
  char jhebdo_alpha[9]          ; // élément de la date de voyage
  int date_int;
  int jour_test,mois_test,annee_test,jhebdo_test;

  char garedep[MAX_GARE]   ; // saisie utilisateur Gare de départ
  char garearr[MAX_GARE]   ; // saisie utilisateur Gare d'arrivée
  char date_rech[MAX_DATE] ; // saisie utilisateur Date de voyage

  struct UnHoraire *tab_res_depart=NULL ; // pointeur de struct horaire pour les résultats au départ d'une gare
  struct UnHoraire *tab_res_arrive=NULL ; // pointeur de struct horaire pour les résultats à l'arrivée d'une gare
  struct UnRes_nodate *tab_res_nodate=NULL ; // pointeur de struct resultat_nodate pour les résultats communs
  struct UnRes *tab_res=NULL ; // pointeur de struct resultat pour les résultats communs
  struct UnResPlaceDispo *tab_places_dispo; // creation du tableau de places disponibles, dependant du nombre de resultats
  tab_places_dispo = (struct UnResPlaceDispo *) malloc(sizeof(struct UnResPlaceDispo));
  
   
  /* === Départ === */
  
  saisie_text("Gare de départ", garedep) ; // récupération saisie utilisateur Gare de départ
  convmaj(garedep)                       ; // conversion en majuscule
  tab_res_depart=recherche_horaire(garedep,&nb_res_depart) ; // recherche des passages dans cette gare
  
  if(nb_res_depart==0) // Cas : pas de résultat au départ de la gare saisie
  {
    printf ("\nIl n'y a pas de train au départ de %s\n",garedep) ;
  }
  else // Cas : des résultats au départ de la gare saisie
  {

  /* === Arrivée ===*/
    
    saisie_text("Gare d'arrivée", garearr) ; // récupération saisie utilisateur gare d'arrivée
    convmaj(garearr)                       ; // conversion en majuscule
    tab_res_arrive = recherche_horaire(garearr,&nb_res_arrive) ; // recherche des passages dans cette gare
    
    // recherche de trajets entre gare de depart et gare d'arrivee
    tab_res_nodate = compare_nodate(tab_res_depart,nb_res_depart,tab_res_arrive,nb_res_arrive,&nb_res_nodate);

    if(nb_res_nodate==0) // Cas : pas de train identique entre la gare de départ et la gare d'arrivée
    {     
      printf ("\nIl n'y a pas de liaison entre %s et %s\n",garedep, garearr) ;
    }
    else // Cas : des trains identiques entre la gare de départ et la gare d'arrivée
    {

      /* === Date === */

      printf("Date du voyage (JJ/MM/AAAA)         : ") ; // invite de saisie
      erreur2 = valide_date(&jour, &mois, &annee)      ; // lecture saisie et contrôle format + date existante
      erreur3 = date_anterieure(jour, mois, annee, jour_sys, mois_sys, annee_sys) ;
      while (erreur2 | erreur3 == 1)
      {
        if (erreur2)
        {
          printf("\nDate incorrecte\n")                ;
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
      date_int = assemble_date(jour,mois,annee) ;
      
      // recherche de trajets à la date donnée
      tab_res = compare_avecdate(tab_res_nodate, nb_res_nodate, jhebdo, &nb_res_date);
      tab_places_dispo = (struct UnResPlaceDispo *) realloc(tab_places_dispo,sizeof(struct UnResPlaceDispo) * (nb_res_date)) ;
      
      // complete tab_res avec les nombres de places disponibles
      verification_res_dispo(tab_res, nb_res_date, tab_places_dispo, date_int);
      tri(tab_res,&nb_res_date) ;

      if(nb_res_nodate==0)
      {
        printf("\nAucun train ne circule entre %s et %s le %s %d/%d/%d\n",garedep, garearr, jhebdo_alpha, jour, mois, annee) ;
      }
      else
      {
        while (choix2 != 0)
        {
          erreur4 = date_anterieure(tab_date_vente[nbdatevente-1].jour,
            tab_date_vente[nbdatevente-1].mois,
            tab_date_vente[nbdatevente-1].annee,
            jour, mois, annee) ;
          if(erreur4 == 1)
          {
            printf("\nLes billets pour la date selectionnee ne sont pas encore ouverts a la vente.\n");
          }

          printf("\n") ;
          printf("----------------------------------------------------------------------------------------------------------------------------------------------\n") ;
          printf(" %3s | %-30s | %-30s | %6s | %8s | %8s | %5s | %7s | %10s | %10s\n",
            "n°","Gare de départ","Gare d'arrivée","Train","hh:mm:ss","hh:mm:ss","Type","Prems","2de classe","1re classe");
          printf("----------------------------------------------------------------------------------------------------------------------------------------------\n") ;
          
          // Affichage des résultats
          for(i=0;i<nb_res_date;i++)
          {
            // Modification des tarifs selon la disponibilité des places à réserver
            // Si les dates sont fermées
            if(erreur4 == 1)
            {
              printf("%3d | %-30s| %-30s| %6d | %8s | %8s |  TGV  | %7s | %10s | %10s\n", i+1, 
                tab_res[i].garedep, tab_res[i].garearr, tab_res[i].numtrain,
                tab_res[i].hd, tab_res[i].ha,"XXXXX","XXXXX","XXXXX");
            }
            // Si les dates sont ouvertes
            else
            {
              // S'il y a des places disponibles
              if(tab_res[i].dispo)
              {
                // En premiere classe
                if(tab_res[i].dispo_1ere)
                {
                  // Et en seconde classe
                  if(tab_res[i].dispo_2nde)
                  {
                    printf("%3d | %-30s| %-30s| %6d | %8s | %8s |  TGV  | %3.2f € | %8.2f € | %8.2f €\n", i+1, 
                    tab_res[i].garedep, tab_res[i].garearr, tab_res[i].numtrain, 
                    tab_res[i].hd, tab_res[i].ha,tab_res[i].prix0,tab_res[i].prix2,tab_res[i].prix1);
                  }
                  // Ou pas en seconde classe
                  else
                  {
                    printf("%3d | %-30s| %-30s| %6d | %8s | %8s |  TGV  | %7s | %10s | %8.2f €\n", i+1, 
                    tab_res[i].garedep, tab_res[i].garearr, tab_res[i].numtrain, 
                    tab_res[i].hd, tab_res[i].ha,"COMPLET","COMPLET",tab_res[i].prix1);
                  }
                }
                // Ou déjà pas en premiere classe
                else
                {
                  printf("%3d | %-30s| %-30s| %6d | %8s | %8s |  TGV  | %3.2f € | %8.2f € | %10s\n", i+1, 
                    tab_res[i].garedep, tab_res[i].garearr, tab_res[i].numtrain, 
                    tab_res[i].hd, tab_res[i].ha,tab_res[i].prix0,tab_res[i].prix2,"COMPLET");
                }
              }
              // S'il n'y a aucune place disponible
              else
              {
                printf("%3d | %-30s| %-30s| %6d | %8s | %8s |  TGV  | %7s | %10s | %10s\n", i+1, 
                tab_res[i].garedep, tab_res[i].garearr, tab_res[i].numtrain, 
                tab_res[i].hd, tab_res[i].ha,"COMPLET","COMPLET","COMPLET");
              }
            }  
          }
          // Affichage des menus
          // Dates non ouvertes (pour consultation des horaires)
          if(erreur4 == 1)
          {
            printf("---------------------------------------------------------------------------------------------------------------------------------------------\n") ;
            printf("\n") ;
            printf("-1- Afficher les trains du jour précédent\n") ;
            printf("-2- Afficher les trains du jour suivant\n") ;
            printf("-0- Retour à l'accueil\n") ;
            saisie_int("\nChoix : ",0,2,&choix2) ;
            // pour correspondre aux valeurs de choix des autres menus
            if(choix2>0)
            {
              choix2++;
            }
          }
          // Dates ouvertes à la vente
          else
          {
            erreur3=0;
            erreur4=0;
            jour_test=jour;
            mois_test=mois;
            annee_test=annee;
            jhebdo_test=jhebdo;
            // décrémente la date (sur une variable de test) pour verifier qu'elle n'est pas antérieure à la date système
            date_suivante_precedente(&jhebdo_test, &jour_test, &mois_test, &annee_test, -1) ;
            erreur3 = date_anterieure(jour_test, mois_test, annee_test, jour_sys, mois_sys, annee_sys) ;
            
            jour_test=jour;
            mois_test=mois;
            annee_test=annee;
            jhebdo_test=jhebdo;
            // incrémente la date (sur une variable de test) pour vérifier qu'elle ne dépasse pas la date d'ouverte des ventes
            date_suivante_precedente(&jhebdo_test, &jour_test, &mois_test, &annee_test, 1) ;
            erreur4 = date_anterieure(tab_date_vente[nbdatevente-1].jour,
                                tab_date_vente[nbdatevente-1].mois,
                                tab_date_vente[nbdatevente-1].annee,
                                jour_test, mois_test, annee_test) ;
            // Si c'est la date du jour, on ne propose pas les trains de la veille
            if(erreur3 == 1)
            {
              printf("---------------------------------------------------------------------------------------------------------------------------------------------\n") ;
              printf("\n") ;
              printf("-1- Choisir un train circulant le %s %d/%d/%d\n",jhebdo_alpha, jour, mois, annee) ; // faire une fonction qui actualise la date (mutualiser avec jour_semaine ?)
              printf("-2- Afficher les trains du jour suivant\n") ;
              printf("-0- Retour à l'accueil\n") ;
              saisie_int("\nChoix : ",0,2,&choix2) ;
              if(choix2>1)
              {
                choix2++;
              }
            }
            // Si c'est la dernière date ouverte à la vente, on ne propose pas les trains du lendemain
            else if(erreur4 == 1)
            {
              printf("---------------------------------------------------------------------------------------------------------------------------------------------\n") ;
              printf("\n") ;
              printf("-1- Choisir un train circulant le %s %d/%d/%d\n",jhebdo_alpha, jour, mois, annee) ;
              printf("-2- Afficher les trains du jour précédent\n") ;
              printf("-0- Retour à l'accueil\n") ;
              saisie_int("\nChoix : ",0,2,&choix2) ;
            }
            // Dans les autres cas, tous les menus s'affichent
            else
            {
              printf("---------------------------------------------------------------------------------------------------------------------------------------------\n") ;
              printf("\n") ;
              printf("-1- Choisir un train circulant le %s %d/%d/%d\n",jhebdo_alpha, jour, mois, annee) ;
              printf("-2- Afficher les trains du jour précédent\n") ;
              printf("-3- Afficher les trains du jour suivant\n") ;
              printf("-0- Retour à l'accueil\n") ;
              saisie_int("\nChoix : ",0,3,&choix2) ;
            }
          }
          
          // printf("\nChoix : ") ;
          // scanf("%c",&lettre) ;
          // choix2 = lecture_choix(0,4,lettre,&erreur4) ;
          switch (choix2)
          {
            case 1: saisie_int("                                 Choisir un train (n°)       : ",1, nb_res_date, &choix_resultat);
                    // printf("Choisir un train (n°) : ") ;
                    // scanf("%c",&lettre) ; // modifier, il peut y avoir plus de 10 trains
                    // choix_resultat=lecture_choix(1,nb_res_date,lettre,&erreur4);
                    // while (erreur4)
                    // {
                    //   dump_buffer() ;
                    //   printf("Choisir un train (n°) : ") ;
                    //   scanf("%c",&lettre) ;
                    //   choix_resultat=lecture_choix(1,nb_res_date,lettre,&erreur4);
                    // }
                    reservation(tab_res[choix_resultat-1],nb_res_date,tab_places_dispo[choix_resultat-1].tab_dispo,date_int);
                    free(tab_places_dispo);
                    tab_places_dispo = NULL ;
                    free(tab_res);
                    tab_res = NULL ;
                    free(tab_res_nodate);
                    tab_res_nodate = NULL ;
                    free(tab_res_arrive);
                    tab_res_arrive = NULL ;
                    free(tab_res_depart);
                    tab_res_depart = NULL ;
                    choix2=0;
                    break;
            case 2: date_suivante_precedente(&jhebdo, &jour, &mois, &annee, -1) ;
                    interprete_jour_semaine(jhebdo, jhebdo_alpha) ;
                    date_int=assemble_date(jour, mois, annee) ;
                    tab_res=compare_avecdate(tab_res_nodate, nb_res_nodate, jhebdo, &nb_res_date) ;
                    free(tab_places_dispo);
                    tab_places_dispo = NULL ;
                    tab_places_dispo = (struct UnResPlaceDispo *) realloc(tab_places_dispo,sizeof(struct UnResPlaceDispo) * (nb_res_date)) ;
                    verification_res_dispo(tab_res, nb_res_date, tab_places_dispo, date_int);
                    tri(tab_res,&nb_res_date) ;
                    break;
            case 3: date_suivante_precedente(&jhebdo, &jour, &mois, &annee, 1) ;
                    interprete_jour_semaine(jhebdo, jhebdo_alpha) ;
                    date_int=assemble_date(jour, mois, annee) ;
                    tab_res=compare_avecdate(tab_res_nodate, nb_res_nodate, jhebdo, &nb_res_date) ;
                    free(tab_places_dispo);
                    tab_places_dispo = NULL ;
                    tab_places_dispo = (struct UnResPlaceDispo *) realloc(tab_places_dispo,sizeof(struct UnResPlaceDispo) * (nb_res_date)) ;
                    verification_res_dispo(tab_res, nb_res_date, tab_places_dispo, date_int);
                    tri(tab_res,&nb_res_date) ;
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
  struct idnomgare 
  {
    char idgare[MAX_ID];
    char nomgare[MAX_GARE];
  } ;
  struct idnomgare listgares[20] ;
  int nblistgares ;

// struct UnHoraire {
  char idtrajet[MAX_ID];
  int idservice;
  int numtrain;
  int direction;
  char ha;
  char hd;
  int sequence;
  char idgare[MAX_ID];
  char nomgare[MAX_GARE];
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
        else                                      // sinon (les caractères comparés de la chaine et de la sous-chaine sont différents)
        {
          j=position ;                            // on passe au caractère suivant dans la chaine (retour à la position retenue)
          position=0 ;                            // la position est réinitialisée
        }
      } /* fin du if caractère chaine = caractère sous-chaine (on passe au caractère de la chaine suivant en repartant du 1er caractère de la sous-chaine) */
    } /* fin du for chaque caractère de la chaine */
  } /* fin du for chaque ligne de tab_horaire */
  
  // on va chercher les autres infos
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
                    strcpy(res_horaire[k].idtrajet,stops[l].idtrajet);
                    res_horaire[k].idservice = trajets[m].idservice;
                    res_horaire[k].numtrain = trajets[m].numtrain ;
                    strcpy(res_horaire[k].ha,stops[l].ha) ;
                    strcpy(res_horaire[k].hd,stops[l].hd) ;
                    res_horaire[k].sequence = stops[l].sequence ;
                    strcpy(res_horaire[k].idgare,listgares[i].idgare);
                    strcpy(res_horaire[k].nomgare,listgares[i].nomgare);
                    k++;
                    *nbres=k;
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
          strcpy(tab_resultats_nodate[k].hd,  gare_dep_trouve[j].hd)       ;
          strcpy(tab_resultats_nodate[k].ha,  gare_arr_trouve[i].ha)       ;
          tab_resultats_nodate[k].seqdep    = gare_dep_trouve[j].sequence  ;
          tab_resultats_nodate[k].seqarr    = gare_arr_trouve[i].sequence  ;
          strcpy(tab_resultats_nodate[k].idgaredep, gare_dep_trouve[j].idgare) ;
          strcpy(tab_resultats_nodate[k].idgarearr, gare_arr_trouve[i].idgare) ;
          strcpy(tab_resultats_nodate[k].garedep,   gare_dep_trouve[j].nomgare) ;
          strcpy(tab_resultats_nodate[k].garearr,   gare_arr_trouve[i].nomgare) ;
          // printf de contrôle
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
struct UnRes * compare_avecdate(struct UnRes_nodate tab_res_nodate[], int nb_res_nodate, int jhebdo, int *nb_res_date)
{  
  // printf("passage dans date : jour de la semaine %d\n", jhebdo);
  // printf("passage dans date : numéro de train du 2e résultat du tableau de résultats no date %d\n", tab_res_nodate[1].num_train);     // ces 2 printf ne s'allument pas, la fonction n'arrive donc pas a s'executer
  int i=0 ; // compteur de tab_resultat_nodate
  int j=0 ; // compteur de tab_resultat
  struct UnRes *tab_resultats ; // pointeur du tableau de résultats communs à retourner
  struct UnTarif untarif ; 

  /* allocation de mémoire au tableau de résultats tab_resultats */
  tab_resultats = (struct UnRes *) malloc(sizeof(struct UnRes));

  for (i=0;i<nb_res_nodate;i++)
  {
    if (circule_jhebdo(tab_res_nodate[i].idtrajet,jhebdo))
    {
      strcpy(tab_resultats[j].idtrajet, tab_res_nodate[i].idtrajet)   ;
          tab_resultats[j].idservice = tab_res_nodate[i].idservice    ;
          tab_resultats[j].numtrain = tab_res_nodate[i].numtrain      ;
          tab_resultats[j].numtrain = tab_res_nodate[i].numtrain      ;
          // strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)   ;
          strcpy(tab_resultats[j].hd, tab_res_nodate[i].hd)           ;
          strcpy(tab_resultats[j].ha, tab_res_nodate[i].ha)           ;
          tab_resultats[j].seqdep    = tab_res_nodate[i].seqdep       ;
          tab_resultats[j].seqarr    = tab_res_nodate[i].seqarr       ;
          strcpy(tab_resultats[j].garedep, tab_res_nodate[i].garedep) ;
          strcpy(tab_resultats[j].garearr, tab_res_nodate[i].garearr) ;

      trouve_tarif(tab_res_nodate[i].garedep,tab_res_nodate[i].garearr, &untarif) ; 

      tab_resultats[j].prix0 = untarif.prix0 ;
      tab_resultats[j].prix2 = untarif.prix2 ;
      tab_resultats[j].prix1 = untarif.prix1 ;

      j++;
      tab_resultats = (struct UnRes *) realloc(tab_resultats,sizeof(struct UnRes) * (j+1));
    }
  }
  
  *nb_res_date = j   ;
  return tab_resultats ;
}

// ~~~~~~~~~~~
/* Calcule les tarifs pour une gare de départ et une gare d'arrivée */
// ~~~~~~~~~~~
// pour prévoir de moduler en fonction des réductions
/*
void calcule_tarif(char garedep[], char garearr[], struct UnTarif *untarif)
{
  trouve_tarif(garedep,garearr,&untarif) ;
}
*/
// ~~~~~~~~~~~
/* Trouve les tarifs de base pour une gare de départ et une gare d'arrivée */
// ~~~~~~~~~~~
// Prend la gare des tarifs comme sous-chaine car elle ne contient pas le nom de la gare, seulement le nom de la ville
// void trouve_tarif(char garedep[], char garearr[], /*char type[],*/ struct UnTarif * untarif)
void trouve_tarif(char garedep[], char garearr[], /*char type[],*/ struct UnTarif *untarif)
{
  int i,j,k ;
  int l,m,n ;
  int position, position2;
  int trouve=0;

  for (i=0;i<nbtarif;i++)                           // pour chaque ligne de tarif
  {
    for (j=0;garedep[j]!='\0';j++)                  // pour le caractère du nom de la gare, tant qu'il est différent de \O
    {
      //printf("%c\n",garedep[j]);
      k=0;                                          // et du premier caractère de la sous-chaine
      if (garedep[j] == tarifs[i].garedep[k])       // si le caractère du nom de la gare est égal au caractère de la gare saisie
      {
       
        position = j+1 ;                            // on mémorise la position de recherche
        while (garedep[j] == tarifs[i].garedep[k])  // tant que les deux caractères sont égaux...
        {
          j++ ;                                     // ... on passe au caractère suivant pour le nom de la gare
          k++ ;                                     // ... et pour la saisie
        }
        if (tarifs[i].garedep[k]=='\0')             // si la sous-chaine arrive à la fin (= toute la sous-chaine a été trouvée dans la chaine)
        {
          l=0;
          while(garearr[l]!='\0' && l<MAX_GARE)     // on cherche si la gare de départ correspond
          {
            // printf("%s\n",tarifs[i].garedep);
            m=0;
            if (garearr[l] == tarifs[i].garearr[m])
            {
              
              position2 = l+1;
              while (garearr[l] == tarifs[i].garearr[m])
              {
                l++;
                m++;
                // printf("%s\n",tarifs[i].garearr);
              }
              if(tarifs[i].garearr[m]=='\0')
              {
                trouve=1;
                untarif->prix0 = tarifs[i].prix0 ;
                untarif->prix2 = tarifs[i].prix2 ;
                untarif->prix1 = tarifs[i].prix1 ;
              }
              else
              {
                l=position2 ;
                position2=0 ;
              }
            }
            l++;
          } // fin du while gare d'arrivée
        }
        else
        {
          j=position ;
          position=0 ;
        }
      }
    }
  }

  if (! trouve) // on cherche dans l'autre sens (origine/destination => destination/origine)
  {
    for (i=0;i<nbtarif;i++)                           // pour chaque ligne de tarif
    {
      for (j=0;garearr[j]!='\0';j++)                  // pour le caractère du nom de la gare, tant qu'il est différent de \O
      {
        k=0;                                          // et du premier caractère de la saisie (sous-chaine)
        if (garearr[j] == tarifs[i].garedep[k])       // si le caractère du nom de la gare est égal au caractère de la gare saisie
        {
          position = j+1 ;                            // on mémorise la position de recherche
          while (garearr[j] == tarifs[i].garedep[k])  // tant que les deux caractères sont égaux...
          {
            j++ ;                                     // ... on passe au caractère suivant pour le nom de la gare
            k++ ;                                     // ... et pour la saisie
          }
          if (tarifs[i].garedep[k]=='\0')             // si la sous-chaine arrive à la fin (= toute la sous-chaine a été trouvée dans la chaine)
          {
            l=0;
            while(garedep[l]!='\0' && l<MAX_GARE)     // on cherche si la gare de départ correspond         
            {
              m=0;
              if (garedep[l] == tarifs[i].garearr[m])
              {
                position2 = l+1;
                while (garedep[l] == tarifs[i].garearr[m])
                {
                  l++;
                  m++;
                }
                if(tarifs[i].garearr[m]=='\0')
                {
                  untarif->prix0 = tarifs[i].prix0 ;
                  untarif->prix2 = tarifs[i].prix2 ;
                  untarif->prix1 = tarifs[i].prix1 ;
                }
                else
                {
                  l=position2 ;
                  position2=0 ;
                }
              } // fin du while gare d'arrivée
            l++;
            }
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
}

// ~~~~~~~~~~~
/* Verification de la quantite de places disponibles (Totales/1ere/2nde) tout au long du trajet, pour chaque resultat */
// ~~~~~~~~~~~
void verification_res_dispo(struct UnRes tab_res[], int nb_res_date, struct UnResPlaceDispo tab_places_dispo[], int date_int)
{
  int i, j, k, l, m, t, z;
  int itrouve, jtrouve, ktrouve, ttrouve;
  int nbplace, nbpremiere, nbseconde;
  int max_date, max_seq;
  
  // printf de controle
  // int cpt;
  // for (cpt=0;cpt<nb_res_date;cpt++)
  // {
  //   printf("%s\n",tab_res[cpt].idtrajet);
  //   printf("%d\n",tab_res[cpt].numtrain);
  //   printf("%d\n",tab_res[cpt].seqdep);
  //   printf("%d\n",tab_res[cpt].seqarr);
  // }

  m=0;
  for(z=0; z < nb_res_date; z++)  // pour chacun des resultats de tab_res
  {
    nbplace=0;
    nbpremiere = 0;
    nbseconde = 0;
    i=0;
    itrouve=0;
    while(!itrouve && i<nbtrajet) // rechercher toutes les places dans la limite des id existants, jusqu'à avoir trouvé l'id desiré
    {
      if(!strcmp(tab_res[z].idtrajet, tab_places[i].idtrajet)) //trouver l'id desiré
      {
        itrouve=i;
        
        strcpy(tab_places_dispo[m].id, tab_places[i].idtrajet);

        t=0;
        ttrouve=0;
        while(!ttrouve && t<nbtrajet) // rechercher les bons compteurs dans stockage_nb_rep
        {
          if (!strcmp(stockage_nb_rep[t].idtrajet, tab_places[i].idtrajet))
          {
            ttrouve=t;

            max_date = stockage_nb_rep[t].max_date;
            max_seq  = stockage_nb_rep[t].max_seq ;
          }
          t++ ;
        }
      
        j=0;
        jtrouve=0;
        while(!jtrouve && j<max_date) //rechercher dans cet id la date desiree dans la limite des dates par id 
        { 
          // printf("j=%d dans tab_place %d (comparé avec %d)\n",j,tab_places[i].date[j].date,date_int);
          if(tab_places[i].date[j].date == date_int) //trouver la date desirée dans l'id desiré (en int)
          {
            // printf("date trouvée=%d\n",tab_places[i].date[j].date);
            jtrouve=j;
            
            k=0;
            ktrouve=0;
            while(!ktrouve && k<max_seq) //rechercher la sequence correspondante à la gare de depart 
            {
              // printf("k=%d seqdep dans tab_place=%d (comparé avec %d\n",k,tab_places[i].date[j].sequence[k].seqdep,tab_res[z].seqdep);
              if(tab_places[i].date[j].sequence[k].seqdep == tab_res[z].seqdep) //trouver la sequence correspondante à votre gare de depart
              {
                // printf("sequence de tab_places_dispo=%d\n",tab_res[z].seqdep);
                ktrouve=k;

                for(l=0;l<MAX_PLACES;l++) //compter les places parmi la limite de places definie par la capacité
                {
                  //transferer les informations sur la place dans le tableau approprié
                  tab_places_dispo[m].tab_dispo[l].wagon    = tab_places[i].date[j].sequence[k].place[l].wagon    ;
                  tab_places_dispo[m].tab_dispo[l].classe   = tab_places[i].date[j].sequence[k].place[l].classe   ;
                  tab_places_dispo[m].tab_dispo[l].salle    = tab_places[i].date[j].sequence[k].place[l].salle    ;
                  tab_places_dispo[m].tab_dispo[l].siege    = tab_places[i].date[j].sequence[k].place[l].siege    ; 
                  tab_places_dispo[m].tab_dispo[l].position = tab_places[i].date[j].sequence[k].place[l].position ; 
                  
                  // Concernant l'état de réservation
                  if(!tab_places[i].date[j].sequence[k].place[l].billet) // si la place est disponible
                  {              
                    tab_places_dispo[m].tab_dispo[l].billet = 0  ; 
                    nbplace++;
                    // printf("dans train %d nbplace=%d\n",z,nbplace) ;
                    
                    if(tab_places_dispo[m].tab_dispo[l].classe==1)
                    {
                      nbpremiere++;
                    }
                    else
                    {
                      nbseconde++;
                    }      
                  }
                  else // sinon (la place est occupée)
                  {
                    tab_places_dispo[m].tab_dispo[l].billet = 1  ; // tranfère l'info mais n'incrémente pas les places dispo
                  }//fin du if place disponible    
                }//fin du for des places    
              }//fin du if sequence trouve
              k++;            
            }//fin du while des sequences
          }//fin du if date trouve
          j++;
        }//fin du while des dates
      }//fin du if id trouve
      i++;  
    }//fin du while des id
      
    i=0;
    j=0;
    k=ktrouve;
    l=0;
      
    //tant que l'on a pas trouvé notre sequence d'arrivée
    while (tab_places[itrouve].date[jtrouve].sequence[k].seqarr != tab_res[z].seqarr  && k<max_seq) 
    {
      k++; //prochaine sequence

      for(l=0;l<MAX_PLACES;l++)
      {
        //si la place est prise dans cette nouvelle sequence
        if(tab_places[itrouve].date[jtrouve].sequence[k].place[l].billet) 
        {
          //on notifie que la place n'est plus disponible
          tab_places_dispo[m].tab_dispo[l].billet = 1; 
                        
          nbplace--; //decrémenter le nombre de places trouvées
          if(tab_places_dispo[m].tab_dispo[l].classe==1)
          {
            nbpremiere--;
          }
          else
          {
            nbseconde--;
          }
        }//fin du if place disponible
      }//fin du for des places
    }  
    tab_res[z].dispo=nbplace;         // il y a tant de places disponibles sur ce trajet
    tab_res[z].dispo_1ere=nbpremiere; // il y a tant de places disponibles en premiere sur ce trajet
    tab_res[z].dispo_2nde=nbseconde;  // il y a tant de places disponibles en seconde sur ce trajet
    m++;
  }//fin du for pour les resultats

  // for (z=0;z<nb_res_date;z++)
  // {
  //   printf("nombre de places dans ce train n°%d : en tout=%d 1re=%d 2nd=%d\n",z,tab_res[z].dispo=nbplace,tab_res[z].dispo_1ere=nbpremiere,tab_res[z].dispo_2nde=nbseconde);
  // }
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

// ~~~~~~~~~~~
/* Procedure Contenant les choix utilisateurs quant a la selection des places + lance ecriture dans tab_places et sauvegarde dans le registre des billets */
// ~~~~~~~~~~~
void reservation(struct UnRes tab_res, int nb_res, struct UnePlace tab_dispo[], int date_int)
{
  int  i, j, k, z;
  int  jtrouve;
  int  nbplace;
  int  classe, nbpassagers, choix_salle, choix_position, choix_place;
  int  annulation, continuer, erreur4, deja_utilise;
  char lettre;
  char nom[MAX_NOM], prenom[MAX_NOM], position[10], salle[6];
  int  age;
  float montant_total=0.0;
  
  z=nbreservationjournee;
  
  struct caract tab_caract[2];  // 2 car 2 classes
  
  struct UnBillet tab_billet[MAX_RESERVATION];
  struct UnBilletRegistre tab_registre[MAX_RESERVATION];

  //Initialisation du tableau des caracteristiques
  for(i=0;i<2;i++) // pour chaque classe
  {
    for(j=0;j<2;j++) // pour chaque salle
    {
      tab_caract[i].position[j].fenetre=0;
      tab_caract[i].position[j].couloir=0;
      tab_caract[i].position[j].isole=0;
      tab_caract[i].position[j].salle=0; 
    }
  }
  /*
  printf("%s\n", tab_res.idtrajet);

  for(i=0;i<MAX_PLACES;i++)
  {
    printf("i=%d ; wagon=%d ; classe=%d ; salle=%d ; siege=%d ; position=%d ; billet=%d\n", i,tab_dispo[i].wagon, tab_dispo[i].classe,tab_dispo[i].salle,tab_dispo[i].siege,tab_dispo[i].position,tab_dispo[i].billet );
  }
  */

  //Remplissage des caracteristiques
  for(j=0;j<MAX_PLACES;j++)
  {
    if( ! tab_dispo[j].billet)
    {
      switch(tab_dispo[j].position)
      {
        case 0 : 
          tab_caract[tab_dispo[j].classe-1].position[tab_dispo[j].salle].fenetre++;
          tab_caract[tab_dispo[j].classe-1].position[tab_dispo[j].salle].salle++;
          break;
        case 1 :
          tab_caract[tab_dispo[j].classe-1].position[tab_dispo[j].salle].couloir++;
          tab_caract[tab_dispo[j].classe-1].position[tab_dispo[j].salle].salle++;
          break;
        case 2 :
          tab_caract[tab_dispo[j].classe-1].position[tab_dispo[j].salle].isole++;
          tab_caract[tab_dispo[j].classe-1].position[tab_dispo[j].salle].salle++;
          break;
      }
    }
  }  

  /*-----------------------------------------------
  -------Debut des choix utilisateur---------------
  -----------------------------------------------*/
  if(tab_res.dispo)
  {
    if(tab_res.dispo_1ere && tab_res.dispo_2nde)
    {
      saisie_int("(0 pour choisir un autre train)  Voyager en classe (1 ou 2)  : ",0, 2, &classe);
    }
    else
    {
      if(tab_res.dispo_1ere && !tab_res.dispo_2nde)
      {
        classe=1;
        printf("Vous voyagerez en Première Classe.\n");
      }
      else
      {
        classe=2;
        printf("Vous voyagerez en Deuxième Classe.\n");
      }
    }

    //ici
    if(classe)
    {
      if(classe==2)
      {
        saisie_int("(0 pour choisir un autre train)  Nombre de voyageurs         : ",0, tab_res.dispo_2nde, &nbpassagers);
      }
      else
      {
        saisie_int("(0 pour choisir un autre train)  Nombre de voyageurs         : ",0, tab_res.dispo_1ere, &nbpassagers); 
      }
      
      if(nbpassagers)
      {
        //initialisation de .billet qui servira de controle a la fin
        for(i=0;i<nbpassagers;i++)
        {
          tab_billet[i].billet = 0;
          tab_billet[i].place = -1;
        }
        
        i=0;
        annulation=0;
        while(i<nbpassagers && !annulation)
        {
          printf("INFORMATIONS VOYAGEUR %d        ",i+1) ;
          saisie_text("  Nom   ", nom);
          saisie_text("                                 Prénom", prenom);
          saisie_int("                                 Age                         : ",1, 200, &age);

          // En seconde classe
          if(classe==2)
          {
            // choix de la salle
            if(tab_caract[classe-1].position[0].salle && tab_caract[classe-1].position[1].salle) // s'il y a des places aux deux etages
            {
              saisie_int("(0=salle basse, 1=salle haute)   Salle                       : ",0,1,&choix_salle);
            }

            else if(tab_caract[classe-1].position[0].salle) // si il n'y a de places qu'au rdc
            {
              printf("Places disponibles en salle basse seulement\n");
              choix_salle=0;
            }

            else // si il n'y de places qu'a l'etage
            { 
              printf("Places disponibles en salle haute seulement\n");
              choix_salle=1;
            }
            
            // choix du placement
            if(  tab_caract[classe-1].position[choix_salle].fenetre
              && tab_caract[classe-1].position[choix_salle].couloir
              // && tab_caract[classe-1].position[choix_salle].isole 
              )     // si il reste les trois options, proposer
            {
              saisie_int("(0=fenetre, 1=couloir)           Placement                   : ",0,1,&choix_position);
            }
            else
            {
              if(tab_caract[classe-1].position[choix_salle].fenetre)
              {
                printf("Il ne reste plus que des places fenetres.\n");
                choix_position=0;
              }
              else
              {
                printf("Il ne reste plus que des places couloirs.\n");
                choix_position=1;
              }
            }

            saisie_int("(0=retour 1=continuer)           Voulez-vous continuer ?     : ",0,1,&continuer);
            if(continuer)
            {
              j=0;
              jtrouve=0;
              while(j<MAX_PLACES && !jtrouve)
              {
                if(   !tab_dispo[j].billet 
                  && tab_dispo[j].classe   == classe
                  && tab_dispo[j].salle    == choix_salle
                  && tab_dispo[j].position == choix_position)
                  {
                  deja_utilise=0;
                  for(k=0;k<=i;k++)
                  {
                    if(tab_billet[k].place==j)
                    {
                      deja_utilise=1;
                    }
                  }

                  if(!deja_utilise)
                  {
                    tab_billet[i].place = j;
                    strcpy(tab_billet[i].nom,nom);
                    strcpy(tab_billet[i].prenom,prenom);
                    tab_billet[i].age = age;
                    tab_billet[i].billet = (date_int*100)+z;
                    tab_billet[i].prix = tab_res.prix0;
                    montant_total=montant_total+tab_res.prix0;
                  }//fin du if deja_utilise
                }//fin du if a quatre conditions
                j++;
              }// fin du while des places dispo
            }
            else
            {
              annulation=1;
            }
          }
          
          // Première classe
          else
          {
            erreur4=1;
            while(erreur4)
            {
              printf("--------------------------------------------------\n");
              printf("| %11s | %5s | %5s | %5s | %8s |\n","Identifiant","Wagon","Salle","Siege","Position");
              printf("--------------------------------------------------\n");
              
              for(j=0;j<MAX_PLACES;j++)
              {
                if(!tab_dispo[j].billet && tab_dispo[j].classe==1)
                {
                  deja_utilise=0;
                  for(k=0;k<=i;k++)
                  {
                    if(tab_billet[k].place==j)
                    {
                      deja_utilise=1;
                    }
                  }
                  
                  if(!deja_utilise)
                  {
                    interprete_position(tab_dispo[j].position, position);
                    interprete_salle(tab_dispo[j].salle, salle) ;
                    printf("| %11d | %5d | %5s | %5d | %8s |\n"
                                       ,j+1
                                       ,tab_dispo[j].wagon
                                       ,salle
                                       ,tab_dispo[j].siege
                                       ,position);
                  }
                } 
              }
              printf("--------------------------------------------------\n");

              saisie_int("Veuillez selectionner un Identifiant de place\n(0 pour revenir au choix des resultats) : ",0, j, &choix_place);
              // saisie_int("Veuillez selectionner un siege \n(0 pour revenir au choix des resultats) : ",0, j, &choix_place);
              // choix_place--;
              if(choix_place>0)
              {
                choix_place--;
                if(!tab_dispo[choix_place].billet && tab_dispo[choix_place].classe==1)
                {
                  deja_utilise=0;
                  for(k=0;k<=i;k++)
                  {
                    if(tab_billet[k].place==j)
                    {
                      deja_utilise=1;
                    }
                  }
                  
                  if(!deja_utilise)
                  {
                    tab_billet[i].place = choix_place;
                    strcpy(tab_billet[i].nom,nom);
                    strcpy(tab_billet[i].prenom,prenom);
                    tab_billet[i].age = age;
                    tab_billet[i].billet = (date_int*100)+z;
                    tab_billet[i].prix = tab_res.prix1;
                    montant_total = montant_total+tab_res.prix1;
                    erreur4=0;
                  }
                  else
                  {
                    erreur4=1;
                    printf("Veuillez saisir un des identifiants proposé\n");
                  }
                }
                else
                {
                  erreur4=1;
                  printf("Veuillez saisir un des identifiants proposé\n");
                }
              }
              else
              {
                erreur4=0;
                annulation=1;
              }
              
            }//fin du while erreur  
          }// fin du if classe 1 ou 2 
          i++; //incrementation compteur passagers
          z++;
        }// fin du while nbpassagers et potentielle sortie
          
      }
      else
      {
        annulation=1;
      } //fin du if nbpassagers et potentielle sortie
    }
    else
    {
      annulation=1;
    }//fin du if classe et potentielle sortie
  }
  else
  {
    printf("Il ne reste aucune place disponible pour ce train.\n");
    annulation=1;
  }

  if(!annulation)
  {    
    printf("\n                      Le montant total de votre commande est : %6.2f\n\n", montant_total);

    faux_paiement();
    
    for(i=0;i<nbpassagers;i++)
    {
      tab_registre[i].date = date_int;
      strcpy(tab_registre[i].idtrajet, tab_res.idtrajet);
      strcpy(tab_registre[i].garedep,  tab_res.garedep);
      strcpy(tab_registre[i].garearr,  tab_res.garearr);
      strcpy(tab_registre[i].nom,    tab_billet[i].nom);
      strcpy(tab_registre[i].prenom, tab_billet[i].prenom);
      tab_registre[i].age = tab_billet[i].age;
      
      tab_registre[i].classe   = tab_dispo[tab_billet[i].place].classe;
      tab_registre[i].wagon    = tab_dispo[tab_billet[i].place].wagon;
      tab_registre[i].salle    = tab_dispo[tab_billet[i].place].salle;
      tab_registre[i].siege    = tab_dispo[tab_billet[i].place].siege;
      tab_registre[i].position = tab_dispo[tab_billet[i].place].position;
      tab_registre[i].billet   = tab_billet[i].billet;
      
      tab_registre[i].prix  = tab_billet[i].prix;
      
      // printf("%d %s %s %s %s %s classe=%d siege=%d",tab_registre[i].date,tab_registre[i].idtrajet,
      //   tab_registre[i].garedep,tab_registre[i].garearr,tab_registre[i].nom,tab_registre[i].prenom,
      //   tab_registre[i].classe,tab_registre[i].siege);

      ecriture_resa_in_tab_places(tab_res ,tab_dispo[tab_billet[i].place], date_int, tab_billet[i].billet);
    }
    
    // creation_fichier_billet(tab_registre, nbpassagers);
    
    nbreservationjournee=z;
  }
  else // je sais plus si c'est utile cette partie remise a zero
  {
    i=0;
    while(i<nbpassagers && tab_billet[i].billet )
    {
      tab_billet[i].place  = 0;
      strcpy(tab_billet[i].nom,   "\0");
      strcpy(tab_billet[i].prenom,"\0");
      tab_billet[i].age    = 0;
      tab_billet[i].billet = 0;
      i++;
    }
    //tout remettre a zero
  }
}

/* ---------------------------------------------- */
/* -- Interpretation du int position en string -- */
/* ---------------------------------------------- */
void interprete_position(int pos, char position[])
{  
  switch(pos)
  {
    case 0:
      strcpy(position, "fenetre");
      break;
    case 1:
      strcpy(position, "couloir");
      break;
    case 2:
      strcpy(position, "isole");
      break;
  }
}

/* ------------------------------------------- */
/* -- Interpretation du int salle en string -- */
/* ------------------------------------------- */
void interprete_salle(int etage, char salle[])
{
  switch(etage)
  {
    case 0:
      strcpy(salle, "basse");
      break;
    case 1:
      strcpy(salle, "haute");
      break;
  }
}

// ~~~~~~~~~~~
/* Procedure simulant la saisie de coordonnees bancaires */
// ~~~~~~~~~~~
void faux_paiement()
{
  int  j;
  int  i;
  int  erreur, booleen, test;
  char lettre;
  char dump;
  char digit[MAX_DIGIT];

  int num_bancaire, date_bancaire, cvc_bancaire;
  int len_num;

  /*-- Carte Bancaire -- */
  erreur=1;
  while(erreur)
  {
    i=0;
    erreur = 1;
    while(erreur)
    {
      i=0;
      erreur=0;
      lettre=50;

      printf("                                 Numero de carte bancaire    : ");
      while(!erreur && i<MAX_DIGIT && lettre!='\n' && ( (lettre<58) && (lettre>47)) )    
      {
        scanf("%c", &lettre);

        if( lettre!='\n')
        {
          if( (lettre<58) && (lettre>47) ) 
          {
            digit[i]=lettre;
            i++;
            erreur=0;
          }
          else
          {
            erreur=1;
          } 
        }
        else
        {
          if(strlen(digit)!=16)
          {
            erreur=1;
          } 
        }
      }
      
      if(i>=MAX_DIGIT)
      {
        erreur=1;
        printf("Saisie superieure a 16 caracteres. ");
      }
      
      if(erreur)
      {
          i=0;
          test=0;
          for(i=0;i<MAX_DIGIT;i++)
          {
            digit[i]='\0';
          }
          
          if(lettre != '\n')
          {
            while(dump!='\n')
            {
              scanf("%c", &dump);
            }
          }
          lettre=50;
          dump='a';
          printf("Veuillez saisir un numero de carte bancaire valide.\n");
      }
    }//Fin du while(erreur) de num_bancaire
    num_bancaire=atoi(digit);
    digit[i]='\0';
    len_num =strlen(digit);
    for(i=0;i<MAX_DIGIT;i++)
    {
      digit[i]='\0';
    }
    if(len_num==16)
    {
    
      i=0;
      erreur = 1;
      while(erreur)
      {
        i=0;
        erreur=0;
        lettre=50;
        printf("                                 Date d'expiration (MMAA)    : ");
        while(!erreur && i<MAX_DIGIT && lettre!='\n' && ( (lettre<58) && (lettre>47)) )    
        {
          scanf("%c", &lettre);
        
          if( lettre!='\n')
          {
            if( (lettre<58) && (lettre>47) ) 
            {
              digit[i]=lettre;
              i++;
            }
            else
            {
              erreur=1;
            } 
          }
          else 
          {
            digit[i]='\0';
            test = atoi(digit);
            if(test>1299 || test < 121)   // expiration min pour Jan 2021 -> 01/21 -> 121 -> EXP_MIN_CARTE a creer
            {
              erreur=1;
            }
          }
        }
        
        if(i>=MAX_DIGIT)
        {
          erreur=1;
          printf("Saisie superieure a 25 caracteres. ");
        }
        
        if(erreur)
        {
          i=0;
          test=0;
          for(i=0;i<MAX_DIGIT;i++)
          {
            digit[i]='\0';
          }
          
          if(lettre != '\n')
          {
            while(dump!='\n')
            {
              scanf("%c", &dump);
            }
          }
          dump='a';
          printf("Veuillez saisir une date d'expiration valide.\n");
        }
      }//Fin du while(erreur) de date_bancaire
      date_bancaire=test;
      
      for(i=0;i<MAX_DIGIT;i++)
      {
        digit[i]='\0';
      }
      if(date_bancaire<=1299 && date_bancaire >= 121)
      {
        
        i=0;
        erreur = 1;
        while(erreur)
        {
          i=0;
          erreur=0;
          lettre=50;
          printf("                                 Code de sécurité (au dos)   : ");
          while(!erreur && i<MAX_DIGIT && lettre!='\n' && ( (lettre<58) && (lettre>47)) )
          {
            scanf("%c", &lettre);
        
            if(lettre!='\n')
            {
              if( (lettre<58) && (lettre>47) ) 
              {
                digit[i]=lettre;
                i++;
              }
              else
              {
                erreur=1;
                
              } 
            }
            else 
            {
              digit[i]='\0';
              test=atoi(digit);
              if(test>1000 || test<99)
              {
                erreur=1;
              }
            }
            
          }
          
          if(i>=MAX_DIGIT)
          {
            erreur=1;
            printf("Saisie superieure a 25 caracteres. ");
          }
          
          if(erreur)
          {
            i=0;
            test=0;
            for(i=0;i<MAX_DIGIT;i++)
            {
              digit[i]='\0';
            }
            
            if(lettre != '\n')
            {
              while(dump!='\n')
              {
                scanf("%c", &dump);
              }
            }
            lettre=50;
            dump='a';
            printf("Veuillez saisir un code de sécurité valide.\n");
          }
        }//Fin du while(erreur) de cvc_bancaire
        cvc_bancaire=test;
        for(i=0;i<MAX_DIGIT;i++)
        {
          digit[i]='\0';
        }
        if(cvc_bancaire<1000 && cvc_bancaire>99)
        {
          erreur=0;
          printf("Paiement effectue avec success.\n");
        }//Fin du if( date_bancaire )
        else
        {
          erreur=1;
          printf("Carte bancaire non valide.\n");
        }
      }//Fin du if( date_bancaire )
      else
      {
        erreur=1;
        printf("Carte bancaire non valide.\n");
      }
    }//Fin du if( num_bancaire )
    else
    {
      erreur=1;
      printf("Carte bancaire non valide.\n");
    }
  }//Fin grande boucle while(erreur)
}

// ~~~~~~~~~~~
/* Ecriture des reservation dans tab_places */
// ~~~~~~~~~~~
void ecriture_resa_in_tab_places( struct UnRes tab_res ,struct UnePlace tab_dispo, int date_int, int billet)
{  
  int i, j, k, l, t, z;
  int itrouve, jtrouve, ktrouve, ttrouve;
  int nbplace;
  int max_date,max_seq;
  
  nbplace=0;
  i=0;
  itrouve=0;
  while(!itrouve && i<nbtrajet) // rechercher des tous les id dans la limite des id existants, jusqu'?avoir trouv?l'id desir?
  {  
    if(!strcmp(tab_places[i].idtrajet, tab_res.idtrajet)) //trouver l'id desir?
    {  
      itrouve=i;
      /*
      t=0;
      while(strcmp(stockage_nb_rep[t].idtrajet, tab_places[i].idtrajet) && t<nbtrajet)
      {
        if(!strcmp(stockage_nb_rep[t].idtrajet, tab_places[i].idtrajet))
        {
          max_date = stockage_nb_rep[t].max_date ;
          max_seq  = stockage_nb_rep[t].max_seq  ;
        }
        t++;
      }
      */
        t=0;
        ttrouve=0;
        while(!ttrouve && t<nbtrajet) // rechercher les bons compteurs dans stockage_nb_rep
        {
          if (!strcmp(stockage_nb_rep[t].idtrajet, tab_places[i].idtrajet))
          {
            ttrouve=t;
            
            // printf("idtrajet de stockage_nb_rep=%s ",stockage_nb_rep[t].idtrajet);max_date = stockage_nb_rep[t].max_date;
            max_date = stockage_nb_rep[t].max_date;
            max_seq  = stockage_nb_rep[t].max_seq ;
            // printf("max_date=%d max_seq=%d\n",max_date,max_seq);
          }
          t++ ;
        }

      
      j=0;
      jtrouve=0;
      while(!jtrouve && j<max_date) //rechercher dans cet id la date desiree dans la limite des dates par id (nbdate pas forcement la bonne limite car nbdate>=nbdate_par_id)
      {      
        if(tab_places[i].date[j].date == date_int)//trouver la date desir? dans l'id desir? (en char)
        {        
          jtrouve=j;
          /*
          while(!ktrouve && k< stock.max_seq)
          {
            
          }
          */
          for(k=tab_res.seqdep; k<tab_res.seqarr;k++)
          {        
            for(l=0;l<MAX_PLACES;l++) //compter les places parmi la limite de places definie par la capacit?
            {
              if(tab_places[i].date[j].sequence[k].place[l].wagon == tab_dispo.wagon
                && tab_places[i].date[j].sequence[k].place[l].siege == tab_dispo.siege)
              {
                tab_places[i].date[j].sequence[k].place[l].billet = billet ;
                a_sauvegarder[i].date[j].sequence[k].modifie = 1           ;
                //printf("%d\n",tab_places[i].date[j].sequence[k].place[l].billet);
              }
            }//fin du for des places
          }//fin du for des sequences
        }//fin du if date trouve
        j++;
      }//fin du while des dates
    }//fin du if id trouve
    i++;
  }//fin du while des id
}


// ~~~~~~~~~~~
/* Copie des nouvelles reservations dans le registre */
// ~~~~~~~~~~~
void creation_fichier_billet(struct UnBilletRegistre tab_registre[], int nb_nouveau)
{
  int i;

  //Ajout des nouveaux billets
  for(i=0;i<nb_nouveau;i++)
  {
    registre_billets = (struct UnBilletRegistre *) realloc(registre_billets,sizeof(struct UnBilletRegistre) * (nb_registre_billet+1)) ;
    strcpy(registre_billets[nb_registre_billet+1].idtrajet, tab_registre[i].idtrajet);
    strcpy(registre_billets[nb_registre_billet+1].garedep, tab_registre[i].garedep);
    strcpy(registre_billets[nb_registre_billet+1].garearr,tab_registre[i].garearr);
    strcpy(registre_billets[nb_registre_billet+1].nom, tab_registre[i].nom);
    strcpy(registre_billets[nb_registre_billet+1].prenom,tab_registre[i].prenom);
    registre_billets[nb_registre_billet+1].date = tab_registre[i].date;
    registre_billets[nb_registre_billet+1].age = tab_registre[i].age;
    registre_billets[nb_registre_billet+1].classe = tab_registre[i].classe;
    registre_billets[nb_registre_billet+1].wagon = tab_registre[i].wagon;
    registre_billets[nb_registre_billet+1].salle = tab_registre[i].salle;
    registre_billets[nb_registre_billet+1].siege = tab_registre[i].siege;
    registre_billets[nb_registre_billet+1].position = tab_registre[i].position;
    registre_billets[nb_registre_billet+1].billet = tab_registre[i].billet;
    
    registre_billets[nb_registre_billet+i].prix = tab_registre[i].prix;
    nb_registre_billet = nb_registre_billet+1;
  }
}

/* --------------------------------- */
/* -- Tableau des dates à traiter -- */
/* --------------------------------- */
void crea_date_vente(int jour, int mois, int annee)
{
  int jour_end, mois_end, annee_end;
  int i, j, m, a, date;
  // int i, nbdate; // je l'ai passé en global
  char chdate[MAX_DATENUM];
  jour_end=jour;
  mois_end=mois;
  annee_end=annee;
  
  j=jour;
  m=mois;
  a=annee;
  
  for(i=0; i<1;i++) // mettre i<4 pour ouvrir les places à la vente sur 4 mois 
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


// ~~~~~~~~~~~
/* Calcule si un train circule pour un trajet et une date */
// ~~~~~~~~~~~
int circule_date(char idtrajet[MAX_ID], int date)
{
  int i,j;
  int circule=0;
  int jour, mois, annee, jhebdo;

  decoupe_date(date, &jour, &mois, &annee) ;
  jhebdo = calcul_jour_semaine(jour, mois, annee, jour_sys, mois_sys, annee_sys, jhebdo_num_sys);
  circule=circule_jhebdo(idtrajet,jhebdo) ;
  return circule ;
}

// ~~~~~~~~~~~
/* Calcule si un train circule pour un trajet et un jour de semaine */
// ~~~~~~~~~~~
int circule_jhebdo(char idtrajet[MAX_ID], int jhebdo)
{
  int i,j;
  int circule=0;

  for (i=0;i<nbtrajet;i++)
  {
    if (strcmp(trajets[i].idtrajet, idtrajet) == 0)
    {
      for (j=0;j<nbcalendrier;j++)
      {
        if (trajets[i].idservice == calendriers[j].idservice)
        {
          switch (jhebdo)
          {
            case 0: 
              if (calendriers[j].dim == 1)
              {
                circule = 1;
              }
              break;
            case 1:
              if (calendriers[j].lun == 1)
                {
                  circule = 1;
                }
                break;
            case 2:
              if (calendriers[j].mar == 1)
                {
                  circule = 1;
                }
                break;
            case 3: 
              if (calendriers[j].mer == 1)
                {
                  circule = 1;
                }
                break;
            case 4:
              if (calendriers[j].jeu == 1)
                {
                  circule = 1;
                }
                break;
            case 5:
              if (calendriers[j].ven == 1)
                {
                  circule = 1;
                }
                break;
            case 6:
              if (calendriers[j].sam == 1)
                {
                  circule = 1;
                }
                break;
            }
          }
        }
      }
    }
  return circule ;
}

// ~~~~~~~~~~~
/* Construit un tableau de séquences UneSeq pour un idtrajet */
// ~~~~~~~~~~~
int sequence(char idtrajet[MAX_ID], struct UneSeq tab_sequence[])
{
  int i,j=0 ;
  struct UnStop tab_stops[30] ;
  struct UnStop unstop ; // pour le tri

  int nbarret,nbsequence ;

  // pour un id, on recherche tous les match dans stops
  for (i=0;i<nbstop;i++)
  {
    if (strcmp(idtrajet,stops[i].idtrajet)==0)
    {
      tab_stops[j++] = stops[i] ;
    }
    nbarret=j ;
  }

  // On remplace idgare par le nom de la gare
  for (i=0;i<nbarret;i++)
  {
    for (j=0;j<nbgare;j++)
    {
      if (strcmp(tab_stops[i].idgare,gares[j].idgare)==0)
      {
        strcpy(tab_stops[i].idgare,gares[j].nomgare);
      }
    }
  }

  // On trie les arrêts/stops par numéro de séquence croissant
  for (i=0;i<nbarret;i++)
  {
    unstop = tab_stops[i] ;
    j=i ;
    while ((j>0) && unstop.sequence<tab_stops[j-1].sequence)
    {
      tab_stops[j]=tab_stops[j-1] ;
      j--;
    }
    tab_stops[j]=unstop;
  }

  // On reconstitue les sequences en croisant les infos de 2 stops consecutifs
  if (nbarret==0)
  {
    nbsequence = 0;
  }
  else
  {
    nbsequence = nbarret-1 ;
  }
  for (i=0;i<nbsequence;i++)
  {
    strcpy(tab_sequence[i].gd,tab_stops[i].idgare)         ;
    strcpy(tab_sequence[i].ga,tab_stops[i+1].idgare)       ;
           tab_sequence[i].seqdep= tab_stops[i].sequence   ;
           tab_sequence[i].seqarr= tab_stops[i+1].sequence ;
    strcpy(tab_sequence[i].hd,tab_stops[i].hd)             ;
    strcpy(tab_sequence[i].ha,tab_stops[i+1].ha)           ;
  }
  return nbsequence ;
}

// ============================================= //
/* === Fonctions et procédures sur les dates === */
// ============================================= //

/* ------------------------------------------------------- */
/* -- Lecture et contrôle de validité d'une date saisie -- */
/* ------------------------------------------------------- */
int valide_date(int * jour, int * mois, int * annee)
{
  int mois_switch ;
  int erreur = 0;

  int i=0 ;
  char lettre=50 ; // caractère lu
  
  char digit[MAX_DATE] ; // chaine constituée par ajout de caractère lu
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
  if(compteur_saisie>=MAX_DATE)
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

/* ----------------------------- */
/* -- Antériorité d'une date -- */
/* ----------------------------- */
// compare la premiere date passee et la deuxième
// erreur  1 : la première date est antérieure 
// erreur  0 : les dates sont identiques
// erreur -1 : la première date est postérieure
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

/* --------------------------------------------------------------- */
/* -- Convertit une date AAAAMMJJ en éléments jour, mois, annee -- */
/* --------------------------------------------------------------- */
void decoupe_date(int date, int *jour, int *mois, int *annee)
{
  char datechar[MAX_DATENUM],anneechar[5], moischar[3], jourchar[3];

  // convertit la date AAAAMMJJ en chaine
  sprintf(datechar,"%d",date);

  // construit les chaines jourchar, moischar, anneechar
  anneechar[0]=datechar[0];
  anneechar[1]=datechar[1];
  anneechar[2]=datechar[2];
  anneechar[3]=datechar[3];
  anneechar[4]='\0';
  moischar[0]=datechar[4];
  moischar[1]=datechar[5];
  moischar[2]='\0';
  jourchar[0]=datechar[6];
  jourchar[1]=datechar[7];
  jourchar[2]='\0';

  // convertit en int
  *annee = atoi(anneechar);
  *mois  = atoi(moischar);
  *jour  = atoi(jourchar);
}

/* ------------------------------------------------------------------- */
/* -- Convertit les élements jour, mois, annee en une date AAAAMMJJ -- */
/* ------------------------------------------------------------------- */
int assemble_date(int jour, int mois, int annee)
{
  int date_int;

  date_int= annee*10000;
  date_int= date_int+(mois*100);
  date_int= date_int+jour;

  return date_int ;
}

// ====================================== //
// === Outils de traitement des choix === //
// ====================================== //

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

/* -------------------------------- */
/* -- Saisie Textuelle Securisee -- */
/* -------------------------------- */
void saisie_text(char invite[], char sortie[])
{    
  int i;
  char dump, lettre;                          //securisation des saisies
  int val_choix, erreur;                      //securisation des saisies
  int compteur_saisie, compteur_saisie_date;  //securisation des saisies
  int choix_reserver, date_prob;
  int jour_prec, mois_prec, annee_prec;

  compteur_saisie=0;
  erreur = 1;
  while(erreur==1)
  {
    erreur=0;
    lettre='a';
    printf("%s                      : ", invite);     // invite de saisie
    while(!erreur && compteur_saisie<MAX_GARE && lettre!='\n' && ( ((lettre<91) && (lettre>64)) || ((lettre<123) && (lettre>96)) || (lettre==32) || (lettre==45) ) )
    {                             // si la saisie n'est pas une lettre maj ou min, un LF, un espace ou un -, ou qu'elle est trop grande, erreur
      scanf("%c", &lettre);
      
      if(compteur_saisie<MAX_GARE && lettre!='\n')          // si la saisie est trop grande ou un LF, sortir, si il n'y avait pas d'erreur avant, fin de la saisie avec success
      {
        if( ((lettre<91) && (lettre>64)) || ((lettre<123) && (lettre>96)) || (lettre==32) || (lettre==45) ) 
        {                         // si la saisie n'est pas une lettre maj ou min, un espace ou un -
          sortie[compteur_saisie]=lettre;     //insertion du caract?e
          compteur_saisie++;
          erreur=0;                   //pas d'erreur pour l'instant
        }
        else                        //erreur
        {
          erreur=1;
        } 
      }
      else
      {
        if(compteur_saisie<1)
        {
          erreur=1;
        }
      }
    }
    if(compteur_saisie==MAX_GARE)
    {
      printf("Saisie superieure a 100 caracteres. La saisie a ete tronquee.\n");
      compteur_saisie--;
    }
    
    if(erreur)
    {
      compteur_saisie=0;
          
      for(i=0;i<MAX_GARE;i++)
      {
        sortie[i]='\0';
      }
          
      if(lettre!='\n')
      {
        while(dump!='\n')
        {
          scanf("%c", &dump);
        }
      }
      dump='a';
      printf("Veuillez saisir un nom valide : 100 lettres maximum, sans caracteres speciaux autres que '-'.\n");
    }
  }
  sortie[compteur_saisie]='\0';                 //fermeture de la chaine de caract?e
} 

/* -------------------------------- */
/* -- Saisie Numerique Securisee -- */
/* -------------------------------- */
void saisie_int(char invite[],int min,int max, int *mon_int){
  
  char digit[MAX_DIGIT];
  int  test;
  char dump;
  
  int i=0;
  int erreur = 1;
  while(erreur==1)
  {
    i=0;
    erreur=0;
    char lettre=50; // compris entre '0' et '9'
    printf("%s",invite);
    while(!erreur && i<MAX_DIGIT && lettre!='\n' && ( (lettre<58) && (lettre>47)) )
    {
      scanf("%c", &lettre);
      
      if( lettre!='\n')
      {
        if( (lettre<58) && (lettre>47) ) 
        {
          digit[i]=lettre;
          i++;
        }
        else
        {
          erreur=1;
        } 
      }
      else
      {
        test = atoi(digit);
        if(test<min || test > max)
        {
          erreur=1;
        }
      }
    }
    if(i>=MAX_DIGIT)
    {
      erreur=1;
      printf("Saisie superieure a 25 caracteres. ");
    }
    if(erreur)
    {
      test=0;
      i=0;
      for(i=0;i<MAX_DIGIT; i++)
      {
        digit[i]='\0';
      }
      if(lettre!='\n')
      {
        while(dump!='\n')
        {
          scanf("%c", &dump);
        }
      }   
      dump='a';
      printf("Veuillez saisir un nombre valide (%d à %d)\n",min, max);
    }
  }
  *mon_int=test;
  for(i=0;i<MAX_DIGIT; i++)
  {
    digit[i]='\0';
  }
}

// ======================================== //
// === Outils de traitement des chaines === //
// ======================================== //

/* --------------------------------------- */
/* -- Convertir une chaine en majuscule -- */
/* --------------------------------------- */
void convmaj(char chaine[])
{
  int i ;

  for (i=0 ; i < strlen(chaine) ; i++)
  {
    chaine[i] = toupper(chaine[i]) ;
  }
}
/* ----------------------------------------- */
/* -- Remplacer les tiret par des espaces -- */
/* ----------------------------------------- */
void tiret_to_space(char chaine[])
{    
  int i,max;
  max = strlen(chaine);
  
  for(i=0;i<max;i++)
  {
    if(chaine[i]==45)
    {
      chaine[i]=32;
    }
  }
}

/* ----------------------------------------- */
/* -- Couper une chaine en 2 sous chaines -- */
/* ----------------------------------------- */
void coupe_chaine_au_caractere(char chaine[], char sschaine1[], char sschaine2[], char sep)
{
  int i=0,j=0,k=0,max;
  max = strlen(chaine);

  while ((i<max) && (chaine[i]!=sep))
  {
    sschaine1[j++] = chaine[i++] ;
  }
  i++;
  sschaine1[j]='\0';
  while (i<max)
  {
    sschaine2[k++] = chaine[i++] ;
  }
  sschaine2[k]='\0';
}

/* --------------------------------------------------- */
/* -- Enlever une sous-chaine au début d'une chaine -- */
/* --------------------------------------------------- */
void nettoie_debut_chaine(char chaine[], char pattern[])
{
  
  int i,j=0,trouve=0 ;
  char tmpchar[MAX_GARE] ;

  for (i=0;i<strlen(pattern);i++)
  {
    if (chaine[i] = pattern[i])
    {
      trouve++;
    }
  }

  if (trouve == strlen(pattern))
  {
    for(i=strlen(pattern);i<strlen(chaine);i++)
    {
      tmpchar[j++] = chaine[i] ;
    }
    tmpchar[j] = '\0' ;
    strcpy(chaine,tmpchar) ;
  }
}

/* ------------------------------------------------------- */
/* -- Enlever les espaces aux début et fin d'une chaine -- */
/* ------------------------------------------------------- */
void trim(char *ch)
{
  char chaine1[200] ;
  int i = 0, j = 0 , k = 0 , taille = 0 ;
  if (strcmp(ch,"") != 0) /* On teste si la chaine n'est pas vide */
  {
    /* --- suppression des espaces en début de chaine --- */
    while (isspace(ch[i++])); /* on passe les espaces du début */
    taille = strlen(ch);
    /* recopie des caractères APRES les espaces */
    for (j=i-1 ; j<taille ; j++) chaine1[k++] = ch[j];
    chaine1[k++] = '\0' ; /* on finalise la chaine */
    strcpy(ch,chaine1) ; /* on recopie la chaine traitée dans ch */
    /* --- suppression des espaces en fin de chaine --- */
    i = strlen(ch);
    while (isspace(ch[--i])); /* on passe les espaces */
    ch[++i] = '\0' ; /* on finalise la chaine */
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

// ================================== //
/* === Outils d'interface système === */
// ================================== //

/* --------------------- */
/* -- Date du système -- */
/* --------------------- */
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

/* ---------------------- ------------------------------ */
/* -- Récupère les informations d'un objet du système -- */
/* ---------------------- ------------------------------ */
void AfficheObjetInfo(char NomObjetInfo[])
{
  int code_erreur;
  struct stat TableInode ;
  // off_t Taille ;
  short int Type_Objet,Proprietaire,Groupe ;
  /* Variable pour la date */
  // char date_heure[MAX_CHAINE];
  /* Variable pour le nom de l'utilisateur */
  // char Info_Login[MAX_NOM];
  
  /* Lecture des informations sur l'objet */
  code_erreur = stat(NomObjetInfo,&TableInode);
  if (code_erreur==-1)
    fprintf(stderr,"%s:%s\n",NomObjetInfo,strerror(errno));
  else
    { /* On récupère les informations */
    Type_Objet=(TableInode.st_mode & S_IFMT);
    // Proprietaire=TableInode.st_uid ;
    // Groupe=TableInode.st_gid ;
    // Taille=TableInode.st_size ;
    // /* Appel du traitement du Login et du Groupe */
    // TraitementLoginGroup(Proprietaire,Groupe,Info_Login) ;
    // /* Appel du traitement de la date */
    // TraitementDate(TableInode.st_mtime, date_heure) ;
    /* Affichage des informations */
    if (Type_Objet == S_IFDIR)
    { /* Liste du répertoire */
      // ListerRep(NomObjetInfo);
    }
    // else
    // { /* Informations du fichier */
    //   // printf("%-*s ",(int)strlen(date_heure),date_heure);
    //   // printf("%-*s ",(int)strlen(Info_Login),Info_Login);
    //   // printf("%10d ",(int)Taille);
    //   printf("%-*s",(int)strlen(NomObjetInfo),NomObjetInfo);
    //   printf("\n");
    // }
  }
}

/* ---------------------- -------------------------- */
/* -- Liste les répertoires d'un répertoire donné -- */
/* ---------------------- -------------------------- */
int ListerRep(char NomRep[], char ListObjet[4000][100])
{
  int i=0;
  DIR *Rep ; /* Descripteur du répertoire */
  struct dirent *Objet ; /* une entrée dans le répertoire */
  char NomObjet[MAX_NOM],TmpNom[MAX_NOM];
  /* Ouverture du répertoire */
  Rep = opendir(NomRep);
  if (Rep == NULL)
  {
    fprintf(stderr,"Erreur d'accès à %s:%s\n",NomRep,strerror(errno));
  }
  else
  {
    do
    {
      /* Lecture d'une entrée dans le répertoire */
      Objet = readdir(Rep);
      if (Objet!=NULL)
      { /* On récupère le nom de l'objet */
        strcpy(NomObjet,(*Objet).d_name);
        /* On affiche les informations */
        if ((strcmp(NomObjet,".")!=0)&&(strcmp(NomObjet,"..")!=0))
        {
          strcpy(ListObjet[i++],NomObjet);
          // strcpy(TmpNom,NomRep);
          // strcat(TmpNom,"/");
          // strcat(TmpNom,NomObjet);
          // strcpy(NomObjet,TmpNom);
          // AfficheObjetInfo(NomObjet);
        }
      }
    }
    while(Objet!=NULL);
    /* Fermeture du répertoire */
    closedir(Rep);
  }
  return i;
}

// ~~~~~~~~~~~
// ~~~ Suppression d'un répertoire donné en paramètre ~~~
// ~~~~~~~~~~~
void supprime_repertoire(char nomrep[])
{
  // adapté du web et un peu du polycopié

  DIR *d1; /* descripteur du répertoire */
  struct dirent *objet ; /* une entrée dans le répertoire */
  struct stat stat_rep, stat_objet;
  char nomobjet[50],nomcomplet[50];

  // stat for the path
  stat(nomrep, &stat_rep);

  // on scanne les objets du dossier
  d1=opendir(nomrep) ;
  if(d1==NULL)
  {
    fprintf(stderr, "Erreur d'accès à '%s':%s\n",nomrep,strerror(errno));
    exit(EXIT_FAILURE);
  }
  else
  {
    do
    {
      objet=readdir(d1) ;

      if (objet!=NULL)
      {
        if ((strcmp(objet->d_name,".")!=0)&&(strcmp(objet->d_name,"..")!=0))
        {
          // On récupère le nom de l'objet
          strcpy(nomobjet,objet->d_name) ;
          // On construit une chaine du path complet
          strcpy(nomcomplet,nomrep);
          strcat(nomcomplet,"/");            
          strcat(nomcomplet,nomobjet);

          // stat for the entry
          stat(nomcomplet, &stat_objet);

          // recursively remove a nested directory
          if (S_ISDIR(stat_objet.st_mode) != 0)
          {
            supprime_repertoire(nomcomplet);
            continue;
          }

          // remove a file object
          unlink(nomcomplet) ;
          // if (unlink(nomcomplet) == 0)
          //   printf("Removed a file: %s\n", nomcomplet);
          // else
          //   printf("Can`t remove a file: %s\n", nomcomplet);
          // // free(nomcomplet);
        }
        rmdir (nomrep) ;
        // if (rmdir(nomrep) == 0)
        // {        
        //   printf("Répertoire supprimé : %s\n", nomrep);
        // }
        // else
        // {
        //   printf("Impossible de supprimer le répertoire : %s\n", nomrep);
        // }
      }
    }
    while(objet!=NULL);
  }
  closedir(d1);
  // on supprime le répertoire qui est maintenant vide
  remove(nomrep);
}
