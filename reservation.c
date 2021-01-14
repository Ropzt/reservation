#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

/* --- déclaration des constantes --- */
#define MAXstrNOMGARE 100
#define SizeDate 11

/* --- déclaration des types globaux --- */
struct horaire { /*--- Creation du type structure horaire ---*/
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
//    struct horaire *p_prec ; // pour l'instant, pas besoin de pointeur
//    struct horaire *p_suiv ; // pour l'instant, pas besoin de pointeur
  };

struct resultat_nodate { /*--- Creation du type structure resultat sans la précision de la date ---*/
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
//  struct resultat_nodate *p_prec      ;
//  struct resultat_nodate *p_suiv      ;
} ;

struct resultat { /*--- Creation du type structure resultat ---*/
  char dep_gare[MAXstrNOMGARE] ;
  char arr_gare[MAXstrNOMGARE] ;
  int  num_train               ;
  //char date[SizeDate]          ;
  int  heure_dep               ;
  int  heure_arr               ;
  char type [10]               ;
//  struct resultat *p_prec      ;
//  struct resultat *p_suiv      ;

} ;
/* --- déclaration des variables globales --- */
int nbhoraire=0 ; // nb de données horaires
struct horaire *tab_horaires   ; /*--- Declaration de la variable tab_horaires ---*/

/* --- déclarations préliminaires --- */
void chargement_horaires() ;
void chargement_horaires_alternatif() ;
void lance_recherche()     ;
void date_sys(int *jour, int *mois, int *annee) ;
// int jour_semaine(int jour_rech, int mois_rech, int annee_rech);
int jour_semaine(int jour_rech, int mois_rech, int annee_rech, int jour, int mois, int annee) ;
struct horaire * recherche_horaire(char rechgare[], int *nb_res_horaire) ;
struct resultat_nodate * compare_nodate(struct horaire gare_dep_trouve[], int nb_gare_dep_trouve, struct horaire gare_arr_trouve[], int nb_gare_arr_trouve, int *nb_res_nodate ) ;
struct resultat * compare_avecdate(struct resultat_nodate tab_res_nodate[], int *nb_res_nodate, int j_semaine, int *nb_res_date  /*, char date_rech[SizeDate]*/ );

/* =========================== */
/* === Programme principal === */
/* =========================== */

int main()
{
  int choix=-1 ; /* valeur lue au clavier (choix utilisateur) */
  
  printf("Chargement des données en cours... \nVeuillez patienter, le programme va bientôt démarrer\n\n");

  chargement_horaires() ; // chargement des données horaires
  // chargement_horaires_alternatif() ; // chargement des données horaires
  
  if(nbhoraire)
  {
    printf("===========================\n");
    printf("Bienvenue chez SNCF Voyages\n");
    printf("===========================\n");
  }
    
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
      case 1: printf("à faire : réserver (=consulter (horaires + tarifs) + réserver)\n") ;
              break ;
      case 2: printf("à faire : consulter les horaires\n") ;
              lance_recherche() ;
              break ;
      case 3: printf("à faire : mes billets\n");
              break ;
      case 0: printf("\nÀ bientôt sur SNCF Voyages !\n");
              break ;
      default: printf("\nDésolés, nous n'avons pas compris votre choix, recommencez\n") ; break ;
    } /* Fin du switch */
  } /* Fin du while */
} /* Fin du main */

/* ======================== */
/* === Sous-programmes  === */
/* ======================== */
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
    while((retour!=59) &&(retour!=10))      // tant que different de LF et de ";"
    {
      fscanf(f1,"%c", &lettre) ;            // fscanf du caractere
      retour=lettre;
      if((retour!=59) &&(retour!=10))       // si different de LF et de ";"
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
/* Procédure lance_recherche() appelée par le menu */
// ~~~~~~~~~~~
void lance_recherche()
{
  int  i ;
  int  nb_res_depart=0 ;
  int  nb_res_arrive=0 ;
  int  nb_res_nodate=0 ;
  int  nb_res_date=0   ;
  int jour_sys, mois_sys, annee_sys, j_sem_sys ; // éléments de la date du système
  int jour, mois, annee, j_semaine             ; // éléments de la date de voyage
  int choix2 ;

  char garedep[MAXstrNOMGARE] ; // saisie utilisateur Gare de départ
  char garearr[MAXstrNOMGARE] ; // saisie utilisateur Gare d'arrivée
  char date_rech[SizeDate]    ; // saisie utilisateur Date de voyage
  struct horaire *res_depart=NULL ; // pointeur de struct horaire pour les résultats au départ d'une gare
  struct horaire *res_arrive=NULL ; // pointeur de struct horaire pour les résultats à l'arrivée d'une gare
  struct resultat_nodate *tab_res_nodate=NULL ; // pointeur de struct resultat_nodate pour les résultats communs
  struct resultat *tab_res=NULL ; // pointeur de struct resultat pour les résultats communs

  /* Départ */
  printf("\nGare de départ                      : "); // invite de saisie
  scanf("%s",garedep)                               ; // récupération saisie utilisateur gare de départ
  convmaj(garedep)                                  ; // conversion en majuscule
  res_depart = recherche_horaire(garedep,&nb_res_depart) ; // recherche_horaire reçoit la chaine saisie, le nombre de résultats et retourne un tableau de résultats

  if(nb_res_depart==0) // Cas : pas de résultat au départ de la gare saisie
  {
    printf ("\nIl n'y a pas de train au départ de %s\n",garedep) ;
  }
  else // Cas : des résultats au départ de la gare saisie
  {    
    /* Arrivée */
    printf("Gare d'arrivée                      : "); // invite de saisie
    scanf("%s",garearr)                             ; // récupération saisie utilisateur gare d'arrivée
    convmaj(garearr)                                ; // conversion en majuscule
    res_arrive = recherche_horaire(garearr,&nb_res_arrive) ; // recherche_horaire reçoit la chaine saisie, le nombre de résultats et retourne un tableau de résultats
    
    tab_res_nodate = compare_nodate(res_depart,nb_res_depart,res_arrive,nb_res_arrive,&nb_res_nodate);
    
    if(nb_res_nodate==0) // Cas : pas de résultat entre la gare de départ et la gare d'arrivée
    {
      printf ("\nIl n'y a pas de liaison entre %s et %s\n",garedep, garearr) ;
    }
    else // Cas : des résultats entre la gare de départ et la gare d'arrivée
    {
      /* -- Date -- */
      date_sys(&jour_sys, &mois_sys, &annee_sys)       ; // récupère la date du système
  // printf("Nous somme le %d/%d/%d\n", jour_sys, mois_sys, annee_sys) ;
      printf("Entrez une date (format JJ/MM/AAAA) : ") ; // invite de saisie
      scanf("%d/%d/%d",&jour, &mois, &annee)           ; // récupération saisie utilisateur date de voyage
      while ((jour_sys > jour) | (mois_sys > mois) | (annee_sys > annee))
      {
        printf("\nNous ne proposons pas de voyage dans le passé\n") ;
        printf("\nGare de départ                      : %s\n",garedep); 
        printf("Gare d'arrivée                      : %s\n",garearr);
        printf("Entrez une date (format JJ/MM/AAAA) : "); // invite de saisie
        scanf("%d/%d/%d",&jour, &mois, &annee)          ; // récupération saisie utilisateur date de voyage
      }
      j_semaine = jour_semaine(jour, mois, annee, jour_sys, mois_sys, annee_sys)     ; // calcul du jour de la semaine de la date de voyage
      /*
      strcpy(date_rech,jour) ;
      strcat(date_rech,'/')  ;
      strcat(date_rech,mois) ;
      strcat(date_rech,'/')  ;
      strcat(date_rech,annee);
      */
      
      //printf("%s\n", date_rech);
      tab_res = compare_avecdate(tab_res_nodate, &nb_res_nodate, j_semaine, &nb_res_date /*,date_rech*/);

      if(nb_res_nodate==0)
      {
        printf("\nAucun train ne circule entre %s et %s le %s\n",garedep, garearr, date_rech) ;
      }
      else
      {
        while (choix2 != 0)
        {
          printf("\n") ;
          printf("--------------------------------------------------------------------------------------\n") ;
          printf(" n° | Gare de départ         | Gare d'arrivée         | numéro | hh:mm | hh:mm | Type\n") ;
          printf("--------------------------------------------------------------------------------------\n") ;
          for(i=0;i<nb_res_date;i++)
          {
            printf("%3d | %-22s | %-22s | %6d | %5d | %5d | %s\n", i+1, tab_res[i].dep_gare, tab_res[i].arr_gare, tab_res[i].num_train, tab_res[i].heure_dep, tab_res[i].heure_arr, tab_res[i].type) ;
          }
          printf("--------------------------------------------------------------------------------------\n") ;
          printf("\n") ;
          printf("-1- Choisir un train circulant le %d %d/%d/%d\n",j_semaine, jour, mois, annee) ; // faire une fonction qui actualise la date (mutualiser avec jour_semaine ?)
          printf("-2- Voir les trains du jour précédent\n") ;
          printf("-3- Voir les trains du jour suivant\n") ;
          printf("-4- Modifier la recherche\n") ;
          printf("-0- Retour à l'accueil\n") ;
          printf("\nChoix : ") ;
          scanf("%d",&choix2) ;

          switch (choix2)
          {
            case 1: printf("choisir un train (n°) : ") ;
            break;
            case 2:
              if (j_semaine == 0)
              {
                j_semaine = 6 ;
              }
              else
              {
                j_semaine-- ;
              }
              tab_res=compare_avecdate(tab_res_nodate, &nb_res_nodate, j_semaine, &nb_res_date) ;
            break;
            case 3: 
              if (j_semaine == 6)
              {
                j_semaine = 0 ;
              }
              else
              {
                j_semaine++ ;
              }
              tab_res=compare_avecdate(tab_res_nodate, &nb_res_nodate, j_semaine, &nb_res_date) ;
              break;
            case 4: printf("c'est peut-être pas la peine de faire cette entrée si c'est pour demander 'voulez vous changer le départ, oui, non, voulez-vous changer l'arrivée, oui, non etc.\n") ;
            break;
            case 0: break;
            default: printf("\nDésolés, nous n'avons pas compris votre choix, recommencez\n") ; break ;
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

void date_sys(int *jour, int *mois, int *annee)
{
  time_t nb_sec_1970, temps ;
  struct tm date ;

  // int jour, mois, annee ; // éléments de la date système
  int j_semaine ;         // jour de la semaine de la date système

  /* -- met la date en francais -- */
  setlocale(LC_ALL,"");

  /*-- Récupère la date système -- */

  /* -- Secondes depuis 01/01/1970 -- */
  nb_sec_1970 = time(&temps);

  /* -- Conversion en date -- */
  date = *localtime(&nb_sec_1970);

  /* Éléments intelligibles de la date du système */
  *jour      = date.tm_mday       ; // jour du système
  *mois      = date.tm_mon  +1    ; // mois du système
  *annee     = date.tm_year +1900 ; // année du système
  j_semaine = date.tm_wday       ; // jour de la semaine du système
}

// ~~~~~~~~~~~
/* Fonction de calcul du jour de la semaine de la date donnée
   (la fonction incrémente les paramètres de la date système 
    (jour de la semaine, jour, mois, année) jusqu'à atteindre
    la date recherchée pour en connaitre le jour de la semaine)*/
// ~~~~~~~~~~~
int jour_semaine(int jour_rech, int mois_rech, int annee_rech, int jour, int mois, int annee)
{
  int j_semaine ;         // jour de la semaine de la date système

  int i, j=0 ;
  int annee_bi[10];   //tableau d'années bissextiles

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
  {                                                                        // on incrémente les 4 paramètres de date
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
    }
    if(mois==13)                                                          // (année si on a changé d'année dans le switch)
    {
      mois=1;
      annee++;
    }
    j_semaine++;                                                          // (jour de la semaine)
    if (j_semaine==7)
      j_semaine=0;                                                        // remise à 0 si jour de la semaine 7 (convention : 0 à 6)          
  } /* fin du while d'incrémentation d'un jour */
    return j_semaine; // renvoie un int
}

// ~~~~~~~~~~~
/* Fonction de selection des resultats en fonction du jour de la semaine desire 
  (retourne un tableau des résultats, construit à partir des match) */
// ~~~~~~~~~~~
struct resultat * compare_avecdate(struct resultat_nodate tab_res_nodate[], int *nb_res_nodate, int j_semaine, int *nb_res_date  /*, char date_rech[SizeDate]*/ )       //la plus belle des fonctions
{
  
  // printf("passage dans date : jour de la semaine %d\n", j_semaine);
  // printf("passage dans date : numéro de train du 2e résultat du tableau de résultats no date %d\n", tab_res_nodate[1].num_train);     // ces 2 printf ne s'allument pas, la fonction n'arrive donc pas a s'executer
  int i=0 ; // compteur de tab_resultat_nodate                    // j'ai mis en commentaire toutes les mentions de la date dans la structure resultat jusqu'a trouver comment convertir proprement
  int j=0 ; // compteur de tab_resultat
  struct resultat *tab_resultats ; // pointeur du tableau de résultats communs à retourner
  
  /* allocation de mémoire au tableau de résultats tab_resultats */
  tab_resultats = (struct resultat *) malloc(sizeof(struct resultat));
  
  switch(j_semaine)        // en fonction de j_semaine, on cherche dans la bonne colonne
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