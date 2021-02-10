#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

/* --- déclaration des constantes --- */
#define MAXstrNOMGARE 100
#define SizeDate 11

/* === déclaration des types globaux === */
struct horaire { /*--- Structure des horaires de train ---*/
  int  id                      ;
  int  arrive                  ;
  int  depart                  ;
  int  stop_seq                ;
  int  num_train               ;
  int  lundi                   ;
  int  mardi                   ;
  int  mercredi                ;
  int  jeudi                   ;
  int  vendredi                ;
  int  samedi                  ;
  int  dimanche                ;
  int  capacite                ;
  char type[10]                ;
  char nom_gare[MAXstrNOMGARE] ;
  // struct horaire *p_prec ; // pour l'instant, pas besoin de pointeur
  // struct horaire *p_suiv ; // pour l'instant, pas besoin de pointeur
  };
  
  struct date {
  	int jour;
  	int mois;
  	int annee;
  	char char_date[9];
  };

struct resultat_nodate { /*--- Structure de résultats sans date ---*/
  char dep_gare[MAXstrNOMGARE] ;
  char arr_gare[MAXstrNOMGARE] ;
  int  num_train               ;
  int  lundi                   ;
  int  mardi                   ;
  int  mercredi                ;
  int  jeudi                   ;
  int  vendredi                ;
  int  samedi                  ;
  int  dimanche                ;
  int  heure_dep               ;
  int  heure_arr               ;
  char type [10]               ;
  // struct resultat_nodate *p_prec      ;
  // struct resultat_nodate *p_suiv      ;
} ;

struct resultat { /*--- Structure de résultats avec date et comparés (départ/arrivée) ---*/
  char dep_gare[MAXstrNOMGARE] ;
  char arr_gare[MAXstrNOMGARE] ;
  int  num_train               ;
  // char date[SizeDate]          ;
  int  heure_dep               ;
  int  heure_arr               ;
  char type [10]               ;
  // struct resultat *p_prec      ;
  // struct resultat *p_suiv      ;

} ;

/* --- déclaration des variables globales --- */
int  jhebdo_num_sys, jour_sys, mois_sys, annee_sys ; // éléments de la date du système
char jhebdo_alpha_sys[9] ;

struct horaire *tab_horaires   ; /*--- horaires de train (type horaire) ---*/
int  nbhoraire=0 ; // nb de données horaires de train

struct date *tab_date; /*--- tableau des dates prises en charge par le programme ---*/

/* === déclarations préliminaires === */
void convmaj(char chaine[]) ;
void dump_buffer() ;
void date_sys(int *jour, int *mois, int *annee, int *jhebdo_num) ;
void interprete_jour_semaine(int jhebdo_num, char jhebdo_alpha[]) ;
// int  calcul_jour_semaine(int jour_rech, int mois_rech, int annee_rech, int jour, int mois, int annee) ;
int calcul_jour_semaine(int jour_rech, int mois_rech, int annee_rech, int jour, int mois, int annee, int jour_sem) ;
// void date_suivante_precedente(int jhebdo, int jour, int mois, int annee, int *jhebdo_rech, int *jour_rech, int *mois_rech, int *annee_rech, int increment) ;
void date_suivante_precedente(int *jhebdo_rech, int *jour_rech, int *mois_rech, int *annee_rech, int increment) ;
int valide_date(int * jour, int * mois, int * annee) ;
int date_anterieure(int jour, int mois, int annee, int jour_ref, int mois_ref, int annee_ref) ;
int lecture_choix(int deb, int fin, char lettre, int * erreur) ;


void chargement_horaires() ;
void chargement_horaires_alternatif() ;
void crea_date(int jour, int mois, int annee);

void lance_recherche()     ;
struct horaire * recherche_horaire(char rechgare[], int *nb_res_horaire) ;
struct resultat_nodate * compare_nodate(struct horaire gare_dep_trouve[], int nb_gare_dep_trouve, struct horaire gare_arr_trouve[], int nb_gare_arr_trouve, int *nb_res_nodate ) ;
struct resultat * compare_avecdate(struct resultat_nodate tab_res_nodate[], int *nb_res_nodate, int jhebdo, int *nb_res_date  /*, char date_rech[SizeDate]*/ );
void tri(struct resultat tab_res[], int * nb_res_date) ;

// =========================== //
/* === Programme principal === */
// =========================== //

int main()
{
  char lettre   ;  // char lu au clavier
  int  choix=-1 ;  // choix utilisateur reconstitué
  int  erreur   ;  // code erreur pour expressions conditionnelles
  
  printf("Chargement des données en cours... \nVeuillez patienter, le programme va bientôt démarrer\n\n");

  chargement_horaires() ; // chargement des données horaires à partir d'un fichier
  // chargement_horaires_alternatif() ; // chargement des données horaires à partir d'un fichier

  // Date du système
  date_sys(&jour_sys, &mois_sys, &annee_sys, &jhebdo_num_sys) ; // récupère la date du système
  interprete_jour_semaine(jhebdo_num_sys, jhebdo_alpha_sys)   ; // interprète le jour de semaine
  printf("Nous sommes le %s %d/%d/%d\n", jhebdo_alpha_sys, jour_sys, mois_sys, annee_sys) ;
  crea_date(jour_sys, mois_sys, annee_sys);

  if(nbhoraire) // si le nombre d'horaire chargés est différent de 0
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
              break ;
      case 2: printf("à faire : consulter les horaires\n") ;
              lance_recherche() ;
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
void chargement_horaires_alternatif()
{
  FILE *f1;
  char lettre;
  int  i, j, retour;
  char line[500] ; // pour lire ligne par ligne avec sscanf
  
  /* --- Allocation de mémoire au tableau tab_horaires --- */
  tab_horaires = (struct horaire *) malloc(sizeof(struct horaire));

  /*--- Ouverture fichier horaires --- */
  //f1=fopen("./data_sncf/sncf_full_test.txt", "r") ;
  f1=fopen("./data_sncf/sncf_full.csv","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement") ;
  }
  else
  {
    //fgets(line, sizeof(line), f1); // lire la 1re ligne et ne rien faire (je l'enlève car il n'y a plus les entêtes dans le csv)
    i=0 ;
    while (fgets(line,sizeof(line),f1) != NULL)
    {
      retour=sscanf(line,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%11[^;];%101[^\n]", 
        &tab_horaires[i].id,
        &tab_horaires[i].arrive,
        &tab_horaires[i].depart,
        &tab_horaires[i].stop_seq,
        &tab_horaires[i].num_train,
        &tab_horaires[i].lundi,
        &tab_horaires[i].mardi,
        &tab_horaires[i].mercredi,
        &tab_horaires[i].jeudi,
        &tab_horaires[i].vendredi,
        &tab_horaires[i].samedi,
        &tab_horaires[i].dimanche,
        &tab_horaires[i].capacite,
        tab_horaires[i].type,
        tab_horaires[i].nom_gare) ;
      convmaj(tab_horaires[i].nom_gare) ;

      // strlen de la chaine nom_gare renvoie une case de trop
      // à améliorer, parce que ce n'est pas bien normal. 
      // ou c'est le LF ?
      tab_horaires[i].nom_gare[strlen(tab_horaires[i].nom_gare)-1] = '\0' ;

      // printf("Fichier trips : %d lignes chargées\n",nbhoraire+1) ;
      // printf("strlen type : %d\n",strlen(tab_horaires[i].type) );      
      // printf("strlen nom_gare: %d\n",strlen(tab_horaires[i].nom_gare) );
      // printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %s %s\n",
      //   tab_horaires[i].id, tab_horaires[i].arrive,
      //   tab_horaires[i].depart,
      //   tab_horaires[i].stop_seq,
      //   tab_horaires[i].num_train,
      //   tab_horaires[i].lundi,
      //   tab_horaires[i].mardi,
      //   tab_horaires[i].mercredi,
      //   tab_horaires[i].jeudi,
      //   tab_horaires[i].vendredi,
      //   tab_horaires[i].samedi,
      //   tab_horaires[i].dimanche,
      //   tab_horaires[i].capacite,
      //   tab_horaires[i].type,
      //   tab_horaires[i].nom_gare) ;

      i++ ;
      nbhoraire = i ;

      /*--- realloc de mémoire au tableau tab_horaires ---*/
      tab_horaires = (struct horaire *) realloc(tab_horaires,sizeof(struct horaire) * (nbhoraire+1)) ;
    }
  }

  printf("Fichier trips : %d lignes chargées\n\n",nbhoraire) ; // pour vérifier
  // printf("Je ne sais pas pourquoi retour vaut %d\n",retour );
  // printf("1re ligne (indice 0) : %d | %s\n", voyages[0].service_id, voyages[0].trip_id);
  // printf("2e ligne (indice 1) : %d | %s\n", voyages[1].service_id, voyages[1].trip_id);
  // printf("Ligne du milieu (indice %d) : %d | %s\n", nbvoyages/2,voyages[nbvoyages/2].service_id, voyages[nbvoyages/2].trip_id);
  // printf("Avant-dernière ligne (indice %d) : %d | %s\n", nbvoyages-2,voyages[nbvoyages-2].service_id, voyages[nbvoyages-2].trip_id);
  // printf("Dernière ligne (indice %d) : %d | %s\n",nbvoyages-1,voyages[nbvoyages-1].service_id, voyages[nbvoyages-1].trip_id);

  fclose(f1) ;
}

// ~~~~~~~~~~~
/* --- Chargement des données horaires --- */
// ~~~~~~~~~~~
void chargement_horaires()
{
  FILE *f1;
  char lettre;
  int  i, j, retour;
  
  /* --- Allocation de mémoire au tableau tab_horaires --- */
  tab_horaires = (struct horaire *) malloc(sizeof(struct horaire));

  /*--- Ouverture fichier horaires --- */
  //f1=fopen("./data_sncf/sncf_full_test.txt", "r") ;
  f1=fopen("./data_sncf/sncf_full.csv","r") ;
  
  i=nbhoraire ;
  while ((! feof(f1)))
  {
    /*--- fscanf des 13 premieres colonnes, toutes des int ---*/
    fscanf(f1, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;", 
      &tab_horaires[i].id,
      &tab_horaires[i].arrive,
      &tab_horaires[i].depart,
      &tab_horaires[i].stop_seq,
      &tab_horaires[i].num_train,
      &tab_horaires[i].lundi,
      &tab_horaires[i].mardi,
      &tab_horaires[i].mercredi,
      &tab_horaires[i].jeudi,
      &tab_horaires[i].vendredi,
      &tab_horaires[i].samedi,
      &tab_horaires[i].dimanche,
      &tab_horaires[i].capacite);
    
    /*--- fscanf de la colonne 14 caractere par caractere ---*/
    j=0;
    retour=0;
    while((retour!=59) &&(retour!=10))        // tant que different de LF et de ";"
    {
      fscanf(f1,"%c", &lettre) ;              // fscanf du caractere
      retour=lettre;
      if((retour!=59) &&(retour!=10))         // si different de LF et de ";"
      {
        tab_horaires[i].type[j++] = lettre  ; // insertion
      }
    }
    tab_horaires[i].type[j] = '\0';           // cloture de la chaine
    convmaj(tab_horaires[i].type) ;           // conversion en majuscule
    
    /*--- fscanf de la colonne 15 caractere par caractere ---*/
    j=0;                  // reinitialisation des compteurs
    retour=0;             
    while((retour!=59) &&(retour!=10)&& (!feof(f1)) ) // meme procedure que la colonne 14
    {
      fscanf(f1,"%c", &lettre) ;
      retour=lettre;
      if((retour!=59) &&(retour!=10) && (!feof(f1))) 
      {
        tab_horaires[i].nom_gare[j++] = lettre  ;
      } 
    }
    tab_horaires[i].nom_gare[j] = '\0';       // cloture de la chaine
    // strlen de la chaine nom_gare renvoie une case de trop
    // à améliorer, parce que ce n'est pas bien normal. 
    // ou c'est le LF ?
    tab_horaires[i].nom_gare[strlen(tab_horaires[i].nom_gare)-1] = '\0' ;
    convmaj(tab_horaires[i].nom_gare) ;           // conversion en majuscule

    i++;
    nbhoraire=i ;
    
    /*--- realloc de mémoire au tableau tab_horaires ---*/
    tab_horaires = (struct horaire *) realloc(tab_horaires,sizeof(struct horaire) * (nbhoraire+1)) ;
  }
  
  /*--- Fermeture fichier ---*/
  fclose(f1);
    
  /*--- Test d'affichage ---
  for(i=0;i<100;i++)
  {
    printf("%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%s;%s\n", tab_horaires[i].id , tab_horaires[i].arrive , tab_horaires[i].depart , tab_horaires[i].stop_seq , tab_horaires[i].num_train , tab_horaires[i].lundi , tab_horaires[i].mardi , tab_horaires[i].mercredi , tab_horaires[i].jeudi , tab_horaires[i].vendredi , tab_horaires[i].samedi , tab_horaires[i].dimanche , tab_horaires[i].capacite , tab_horaires[i].type , tab_horaires[i].nom_gare  );
  } */
}

/* ---------------------------------------- */
/* --- Fonctions de recherche de voyage --- */
/* ---------------------------------------- */

// ~~~~~~~~~~~
// Procédure lance_recherche() appelée par le menu
// ~~~~~~~~~~~
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

  char garedep[MAXstrNOMGARE] ; // saisie utilisateur Gare de départ
  char garearr[MAXstrNOMGARE] ; // saisie utilisateur Gare d'arrivée
  char date_rech[SizeDate]    ; // saisie utilisateur Date de voyage
  
  struct horaire *res_depart=NULL ; // pointeur de struct horaire pour les résultats au départ d'une gare
  struct horaire *res_arrive=NULL ; // pointeur de struct horaire pour les résultats à l'arrivée d'une gare
  struct resultat_nodate *tab_res_nodate=NULL ; // pointeur de struct resultat_nodate pour les résultats communs
  struct resultat *tab_res=NULL ; // pointeur de struct resultat pour les résultats communs

  /* === Départ === */
  printf("\nGare de départ                      : ")     ; // invite de saisie
  scanf("%s",garedep)                                    ; // récupération saisie utilisateur Gare de départ
  convmaj(garedep)                                       ; // conversion en majuscule
  res_depart = recherche_horaire(garedep,&nb_res_depart) ; // recherche_horaire reçoit la chaine saisie, le nombre de résultats et retourne un tableau de résultats

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
    res_arrive = recherche_horaire(garearr,&nb_res_arrive) ; // recherche_horaire reçoit la chaine saisie, le nombre de résultats et retourne un tableau de résultats
    
    tab_res_nodate = compare_nodate(res_depart,nb_res_depart,res_arrive,nb_res_arrive,&nb_res_nodate);
    
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
          printf("---------------------------------------------------------------------------------------\n") ;
          printf(" n° | Gare de départ         | Gare d'arrivée         | numéro | hh:mm | hh:mm | Type\n") ;
          printf("---------------------------------------------------------------------------------------\n") ;
          for(i=0;i<nb_res_date;i++)
          {
            printf("%3d | %-22s | %-22s | %6d | %2d:%02d | %2d:%02d | %s\n", i+1, 
              tab_res[i].dep_gare, tab_res[i].arr_gare, tab_res[i].num_train, 
              // tab_res[i].heure_dep/100, (tab_res[i].heure_dep-tab_res[i].heure_dep/100*100), 
              // tab_res[i].heure_arr/100, (tab_res[i].heure_arr-tab_res[i].heure_arr/100*100), tab_res[i].type) ;
              tab_res[i].heure_dep/100, tab_res[i].heure_dep%100,tab_res[i].heure_arr/100, 
              tab_res[i].heure_arr%100, tab_res[i].type) ;
          }
          printf("---------------------------------------------------------------------------------------\n") ;
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
struct horaire * recherche_horaire(char rechgare[], int *nb_res_horaire)
{
  int i        ; // compteur horaire dans tab_horaires
  int j        ; // compteur caractère chaine
  int k        ; // compteur caractère sous-chaine
  int position ; // compteur de la position où le 1er caractère commun est trouvé
  int l=0      ; // compteur xième gare correspondante trouvée
  struct horaire unhoraire ;    // variable locale de type struct horaire
  struct horaire *res_horaire ; // pointeur du tableau de résultats à retourner
  
  /* allocation de mémoire au tableau de résultats res_horaire */
  res_horaire = (struct horaire *) malloc(sizeof(struct horaire));

  // boucle de comparaison de la saisie et du nom de gare du tableau d'horaires
  for (i=0;i<nbhoraire;i++)                           // pour chaque ligne (de tab_horaires) de 0 à nbhoraire
  {
    unhoraire = tab_horaires[i] ;                     // copie de la ligne dans un struct horaire local
    for (j=0;unhoraire.nom_gare[j]!='\0';j++)         // pour le caractère du nom de la gare, tant qu'il est différent de \O
    {
      k=0;                                            // et du premier caractère de la saisie (sous-chaine)
      if(unhoraire.nom_gare[j] == rechgare[k])        // si le caractère du nom de la gare est égal au caractère de la saisie
      {
        position = j+1 ;                              // on mémorise la position de recherche.
        while (unhoraire.nom_gare[j] == rechgare[k])  // tant que les deux caractères sont égaux...
        {
          j++ ;                                       // ... on passe au caractère suivant pour le nom de la gare
          k++ ;                                       // ... et pour la saisie
        }
        if(rechgare[k]=='\0')                         // si la saisie (sous-chaine) arrive à la fin (= toute la sous-chaine a été trouvée dans la chaine)
        {
          res_horaire = (struct horaire *) realloc(res_horaire,sizeof(struct horaire) * (*nb_res_horaire+1));
                                                      // realloc de mémoire au tableau res_horaire
          res_horaire[l] = unhoraire ;                // les infos horaires sont copiées dans le tableau res_horaire
          l++ ;
          *nb_res_horaire=l ;                         // le nombre d'horaires trouvés est actualisé
        }
        else                                          // sinon (les caractères comparés de la chaine et de la sous-chaine sont différents)
        {
          j=position ;                                // on passe au caractère suivant dans la chaine (retour à la position retenue)
          position=0 ;                                // la position est réinitialisée
        }
      } /* fin du if caractère chaine = caractère sous-chaine (on passe au caractère de la chaine suivant en repartant du 1er caractère de la sous-chaine) */
    } /* fin du for chaque caractère de la chaine */
  } /* fin du for chaque ligne de tab_horaire */
  return res_horaire ;
}

// ~~~~~~~~~~~
/* Fonction de comparaison des résultats départ/arrivée 
  (retourne un tableau des résultats, construit à partir des match) */
// ~~~~~~~~~~~
struct resultat_nodate * compare_nodate(struct horaire gare_dep_trouve[], int nb_gare_dep_trouve, struct horaire gare_arr_trouve[], int nb_gare_arr_trouve, int *nb_res_nodate) //
{
  int i=0 ; // compteur résultats à l'arrivée
  int j=0 ; // compteur résultats au départ
  int k=0 ; // compteur match arrivée/départ
  struct resultat_nodate *tab_resultats_nodate ; // pointeur du tableau de résultats communs à retourner
  
  /* allocation de mémoire au tableau de résultats tab_resultats_nodate */
  tab_resultats_nodate = (struct resultat_nodate *) malloc(sizeof(struct resultat_nodate));

  for(i=0 ; i<nb_gare_arr_trouve ; i++)                                          // pour chaque arrivee
  {                                                   
    for (j=0 ; j<nb_gare_dep_trouve ; j++)
    {                                                 // pour chaque départ
      if( gare_dep_trouve[j].id == gare_arr_trouve[i].id)                         // condition id départ = id arrivée
      {
        if(gare_dep_trouve[j].stop_seq < gare_arr_trouve[i].stop_seq)             // condition depart avant arrivee
        {
          strcpy(tab_resultats_nodate[k].dep_gare, gare_dep_trouve[j].nom_gare) ; // copie des infos dans la structure resultat_nodate
          strcpy(tab_resultats_nodate[k].arr_gare, gare_arr_trouve[i].nom_gare) ;
          tab_resultats_nodate[k].num_train = gare_dep_trouve[j].num_train ;
          tab_resultats_nodate[k].lundi     = gare_dep_trouve[j].lundi     ;
          tab_resultats_nodate[k].mardi     = gare_dep_trouve[j].mardi     ;
          tab_resultats_nodate[k].mercredi  = gare_dep_trouve[j].mercredi  ;
          tab_resultats_nodate[k].jeudi     = gare_dep_trouve[j].jeudi     ;
          tab_resultats_nodate[k].vendredi  = gare_dep_trouve[j].vendredi  ;
          tab_resultats_nodate[k].samedi    = gare_dep_trouve[j].samedi    ;
          tab_resultats_nodate[k].dimanche  = gare_dep_trouve[j].dimanche  ;
          tab_resultats_nodate[k].heure_dep = gare_dep_trouve[j].depart    ;
          tab_resultats_nodate[k].heure_arr = gare_arr_trouve[i].arrive    ;
          strcpy(tab_resultats_nodate[k].type, gare_dep_trouve[j].type)    ;
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
          tab_resultats_nodate = (struct resultat_nodate *) realloc(tab_resultats_nodate,sizeof(struct resultat_nodate) * (k+1));          
        } // fin du if sur l'id du voyage
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
struct resultat * compare_avecdate(struct resultat_nodate tab_res_nodate[], int *nb_res_nodate, int jhebdo, int *nb_res_date  /*, char date_rech[SizeDate]*/ )       //la plus belle des fonctions
{
  
  // printf("passage dans date : jour de la semaine %d\n", jhebdo);
  // printf("passage dans date : numéro de train du 2e résultat du tableau de résultats no date %d\n", tab_res_nodate[1].num_train);     // ces 2 printf ne s'allument pas, la fonction n'arrive donc pas a s'executer
  int i=0 ; // compteur de tab_resultat_nodate                    // j'ai mis en commentaire toutes les mentions de la date dans la structure resultat jusqu'a trouver comment convertir proprement
  int j=0 ; // compteur de tab_resultat
  struct resultat *tab_resultats ; // pointeur du tableau de résultats communs à retourner
  
  /* allocation de mémoire au tableau de résultats tab_resultats */
  tab_resultats = (struct resultat *) malloc(sizeof(struct resultat));
  
  switch(jhebdo)        // en fonction de jhebdo, on cherche dans la bonne colonne
  {
    case 0: 
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].dimanche)
        {
          strcpy(tab_resultats[j].dep_gare, tab_res_nodate[i].dep_gare) ;
          strcpy(tab_resultats[j].arr_gare, tab_res_nodate[i].arr_gare) ;
          // strcpy(tab_resultats[j].date    , date_rech)                   ;
          strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].num_train = tab_res_nodate[i].num_train      ;
          tab_resultats[j].heure_dep = tab_res_nodate[i].heure_dep      ;
          tab_resultats[j].heure_arr = tab_res_nodate[i].heure_arr      ;
                
          j++;
          tab_resultats = (struct resultat *) realloc(tab_resultats,sizeof(struct resultat) * (j+1));
        }
      }
      break;
    case 1:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].lundi)
        {
          strcpy(tab_resultats[j].dep_gare, tab_res_nodate[i].dep_gare) ; 
          strcpy(tab_resultats[j].arr_gare, tab_res_nodate[i].arr_gare) ;
          // strcpy(tab_resultats[j].date    , date_rech)                  ;
          strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].num_train = tab_res_nodate[i].num_train      ;
          tab_resultats[j].heure_dep = tab_res_nodate[i].heure_dep      ;
          tab_resultats[j].heure_arr = tab_res_nodate[i].heure_arr      ;
                
          j++;
          tab_resultats = (struct resultat *) realloc(tab_resultats,sizeof(struct resultat) * (j+1));
        }
      }
      break;
    case 2:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].mardi)
        {
          strcpy(tab_resultats[j].dep_gare, tab_res_nodate[i].dep_gare) ; 
          strcpy(tab_resultats[j].arr_gare, tab_res_nodate[i].arr_gare) ;
          // strcpy(tab_resultats[j].date    , date_rech)                  ;
          strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].num_train = tab_res_nodate[i].num_train      ;
          tab_resultats[j].heure_dep = tab_res_nodate[i].heure_dep      ;
          tab_resultats[j].heure_arr = tab_res_nodate[i].heure_arr      ;
                
          j++;
          tab_resultats = (struct resultat *) realloc(tab_resultats,sizeof(struct resultat) * (j+1));
        }
      }
      break;
    case 3:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].mercredi)
        {
          strcpy(tab_resultats[j].dep_gare, tab_res_nodate[i].dep_gare) ; 
          strcpy(tab_resultats[j].arr_gare, tab_res_nodate[i].arr_gare) ;
          // strcpy(tab_resultats[j].date    , date_rech)                  ;
          strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].num_train = tab_res_nodate[i].num_train      ;
          tab_resultats[j].heure_dep = tab_res_nodate[i].heure_dep      ;
          tab_resultats[j].heure_arr = tab_res_nodate[i].heure_arr      ;
                
          j++;
          tab_resultats = (struct resultat *) realloc(tab_resultats,sizeof(struct resultat) * (j+1));
        }
      }
      break;
    case 4:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].jeudi)
        {
          strcpy(tab_resultats[j].dep_gare, tab_res_nodate[i].dep_gare) ; 
          strcpy(tab_resultats[j].arr_gare, tab_res_nodate[i].arr_gare) ;
          // strcpy(tab_resultats[j].date    , date_rech)                  ;
          strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].num_train = tab_res_nodate[i].num_train      ;
          tab_resultats[j].heure_dep = tab_res_nodate[i].heure_dep      ;
          tab_resultats[j].heure_arr = tab_res_nodate[i].heure_arr      ;
                
          j++;
          tab_resultats = (struct resultat *) realloc(tab_resultats,sizeof(struct resultat) * (j+1));
        }
      }
      break;
    case 5:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].vendredi)
        {
          strcpy(tab_resultats[j].dep_gare, tab_res_nodate[i].dep_gare) ; 
          strcpy(tab_resultats[j].arr_gare, tab_res_nodate[i].arr_gare) ;
          // strcpy(tab_resultats[j].date    , date_rech)                  ;
          strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].num_train = tab_res_nodate[i].num_train      ;
          tab_resultats[j].heure_dep = tab_res_nodate[i].heure_dep      ;
          tab_resultats[j].heure_arr = tab_res_nodate[i].heure_arr      ;
                
          j++;
          tab_resultats = (struct resultat *) realloc(tab_resultats,sizeof(struct resultat) * (j+1));
        }
      }
      break;
    case 6:
      for(i=0;i<*nb_res_nodate;i++)
      {
        if(tab_res_nodate[i].samedi)
        {
          strcpy(tab_resultats[j].dep_gare, tab_res_nodate[i].dep_gare) ; 
          strcpy(tab_resultats[j].arr_gare, tab_res_nodate[i].arr_gare) ;
          // strcpy(tab_resultats[j].date    , date_rech)                  ;
          strcpy(tab_resultats[j].type    , tab_res_nodate[i].type)     ;
          tab_resultats[j].num_train = tab_res_nodate[i].num_train      ;
          tab_resultats[j].heure_dep = tab_res_nodate[i].heure_dep      ;
          tab_resultats[j].heure_arr = tab_res_nodate[i].heure_arr      ;
                
          j++;
          tab_resultats = (struct resultat *) realloc(tab_resultats,sizeof(struct resultat) * (j+1));
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
void tri(struct resultat tab_res[], int * nb_res_date)
{
  int i ; // compteur du tableau d'entrée
  int j ; // compteur pour trouver l'indice à partir duquel décaler
  struct resultat unresultat ;

  for (i=0 ; i < *nb_res_date ; i++)
  {
    unresultat = tab_res[i] ;
    j = i ;
    while ((j>0) && (unresultat.heure_dep < tab_res[j-1].heure_dep))
    {
      tab_res[j]=tab_res[j-1];
      j-- ;
    }
    tab_res[j] = unresultat ;
  }
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
  *jhebdo_num = date.tm_wday       ; // jour de semaine du système (0 � 6)
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

void crea_date(int jour, int mois, int annee)
{
	int jour_end, mois_end, annee_end;
	int j, m, a;
	int i, nbdate;
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
  
  
  
  tab_date = (struct date *) malloc(sizeof(struct date));
  
  i=0;
  while( (j!=jour_end) | (m!=mois_end ) | (a!=annee_end))
  {
  	tab_date[i].jour=j;
  	tab_date[i].mois=m;
  	tab_date[i].annee=a;
  	
  	chdate[0]=(a/1000)+48;
  	chdate[1]=((a%1000)/100)+48;
  	chdate[2]=((a%100)/10)+48;
  	chdate[3]=(a%10)+48;
  	chdate[4]=(m/10)+48;
  	chdate[5]=(m%10)+48;
  	chdate[6]=(j/10)+48;
  	chdate[7]=(j%10)+48;
  	chdate[8]='\0';
  	
  	
  	
  	strcpy(tab_date[i].char_date,chdate);
  	
  	printf("%s\n", tab_date[i].char_date);
  	
  	j++;
  	
  	i++;
  	nbdate=i;
  	tab_date = (struct date *) realloc(tab_date,sizeof(struct date) * (nbdate+1)) ;
  	
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


