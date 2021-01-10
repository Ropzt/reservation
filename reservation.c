#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --- déclaration des constantes --- */
#define MAXstrNOMGARE 100

/* --- déclaration des variables globales --- */
int nbhoraire=0 ; // nb de données horaires

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

struct resultat { /*--- Creation du type structure resultat ---*/
  char dep_gare[MAXstrNOMGARE] ;
  char arr_gare[MAXstrNOMGARE] ;
  int  date_voyage             ;
  int  heure_dep               ;
  int  heure_arr               ;
  char type [10]               ;
//  struct resultat *p_prec      ;
//  struct resultat *p_suiv      ;
} ;

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

struct horaire *tab_horaires   ; /*--- Declaration de la variable tab_horaires ---*/
//struct resultat *tab_resultats ; /*--- Declaration de la variable tab_resultats ---*/
struct resultat_nodate *tab_resultats_nodate ; /*--- Declaration de la variable tab_resultats_nodate ---*/

/* --- déclarations préliminaires --- */
void chargement_horaires() ;
void lance_recherche()     ;

struct horaire * recherche_horaire(char rechgare[], int *nb_res_horaire) ;
int compare(struct horaire gare_dep_trouve[], int *nb_gare_dep_trouve,struct horaire gare_arr_trouve[], int *nb_gare_arr_trouve );

/* =========================== */
/* === Programme principal === */
/* =========================== */

int main()
{

  int choix=-1 ; /* valeur lue au clavier (choix utilisateur) */
  
  printf("Chargement des données en cours... \nVeuillez patienter, le programme va bientôt démarrer\n\n");

  chargement_horaires() ; // chargement des données horaires
  
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

/* --- Chargement des données horaires --- */
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

/* Procédure lance_recherche() appelée par le menu */
void lance_recherche()
{
  int  i, nb_res_depart=0, nb_res_arrive=0, trouve=0;
  char garedep[MAXstrNOMGARE] ; // saisie utilisateur Gare de départ
  char garearr[MAXstrNOMGARE] ; // saisie utilisateur Gare d'arrivée
  int  datev                  ; // saisie utilisateur Date de voyage
  struct horaire *res_depart=NULL ; // pointeur de struct horaire pour les résultats au départ d'une gare
  struct horaire *res_arrive=NULL ; // pointeur de struct horaire pour les résultats à l'arrivée d'une gare

  /* Départ */
  printf("\nGare de départ : "); // invite de saisie
  scanf("%s",garedep)          ; // récupération saisie utilisateur
  convmaj(garedep)             ; // conversion en majuscule
  res_depart = recherche_horaire(garedep,&nb_res_depart) ; // recherche_horaire reçoit la chaine saisie, le nombre de résultats et retourne un tableau de résultats

  if(nb_res_depart==0) // Cas : pas de résultat au départ de la gare saisie
  {
    printf ("\nIl n'y a pas de train au départ de %s\n",garedep) ;
  }
  else // Cas : des résultats au départ de la gare saisie
  {    
    // for (i=0; i < 10 ; i++) // à revoir après (on n'aura pas besoin de la boucle ici)
    // {
    //   printf("%d %s\n",res_depart[i].id ,res_depart[i].nom_gare ); 
    // }
    
    /* Arrivée */
    printf("Gare d'arrivée : "); // invite de saisie
    scanf("%s",garearr)        ; // récupération saisie utilisateur
    convmaj(garearr)           ; // conversion en majuscule
    res_arrive = recherche_horaire(garearr,&nb_res_arrive) ; // recherche_horaire reçoit la chaine saisie, le nombre de résultats et retourne un tableau de résultats
    
    // for (i=0; i < 20 ; i++) // à revoir après (on n'aura pas besoin de la boucle ici)
    // {
    //   printf("%d; %s\n",res_arrive[i].id ,res_arrive[i].nom_gare); 
    // }
    
    printf("comparaison en cours...\n");
    
    trouve = compare(res_depart,&nb_res_depart,res_arrive,&nb_res_arrive);
    
    if(trouve==0)
    {
      printf ("\nIl n'y a pas de liaison entre %s et %s\n",garedep, garearr) ;
    }
    else
    {
      printf("Date du voyage :");
      scanf("%",datev) ;
      if("il n'y a pas de train entre depart et arrivée à cette date")
      {
        printf("\nAucun train ne circule entre %s et %s le %d\n",garedep, garearr, datev) ;
      }
      else
      {
        printf("C'est là que c'est velu...") ;
      }
    }
  }
}

/* Fonction de recherche d'horaires selon la gare saisie
 (retourne un tableau d'horaires qui matchent la gare saisie) */
struct horaire * recherche_horaire(char rechgare[], int *nb_res_horaire)
{
  struct horaire unhoraire ;    // variable locale de type struct horaire
  struct horaire *res_horaire ; // pointeur du tableau de résultats à retourner
  int i        ; // compteur horaire dans tab_horaires
  int j        ; // compteur caractère chaine
  int k        ; // compteur caractère sous-chaine
  int position ; // compteur de la position où le 1er caractère commun est trouvé
  int l=0      ; // compteur xième gare correspondante trouvée
  
  /* allocation de mémoire au tableau de résultats res_horaire */
  res_horaire = (struct horaire *) malloc(sizeof(struct horaire));

  // boucle de comparaison de la saisie et du nom de gare du tableau d'horaires
  for (i=0;i<nbhoraire;i++)                           // pour chaque ligne (de tab_horaires) de 0 à nbhoraires
  {
    unhoraire = tab_horaires[i] ;                     // copie de la ligne dans un struct horaire local
    for (j=0;unhoraire.nom_gare[j]!='\0';j++)         // pour le caractère du nom de la gare, tant qu'il est différent de \O
    {
      k=0;                                            // et du premier caractère de la saisie (sous-chaine)
      if(unhoraire.nom_gare[j] == rechgare[k])        // si le caractère du nom de la gare est égal au caractère de la saisie
      {
        position = j+1 ;                              // on mémorise la position de recherche.
        while (unhoraire.nom_gare[j] == rechgare[k])  // tant que les deux caractères sont égaux...  // Jai modifie ton for(i=0; unhoraire.nom_gare[j] == rechgare[k];i++)
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
          j=position ;                                // on passe au caractère suivant la chaine (retour à la position retenue)
          position=0 ;                                // la position est réinitialisée
        }
      } /* fin du if caractère chaine = caractère sous-chaine (on passe au caractère de la chaine suivant en repartant du 1er caractère de la sous-chaine) */
    } /* fin du for chaque caractère de la chaine */
  } /* fin du for chaque ligne de tab_horaire */
  return res_horaire ;
}

/* Fonction de comparaison des résultats départ/arrivée (recherche de match) */
int compare(struct horaire gare_dep_trouve[], int *nb_gare_dep_trouve, struct horaire gare_arr_trouve[], int *nb_gare_arr_trouve ) //
{
  int trouve=0;
  int i=0, j=0, l=0;
  tab_resultats_nodate = (struct resultat_nodate *) malloc(sizeof(struct resultat_nodate));           // une structure avec un peu plus d'infos que resultat
  
  printf("%d ; %s : %d\n", gare_dep_trouve[i].id, gare_dep_trouve[i].nom_gare, gare_dep_trouve[i].stop_seq);  // simple test d'affichage, c'est ici que j'ai vu le probleme de transfert
  printf("%d ; %s : %d\n", gare_arr_trouve[i].id, gare_arr_trouve[i].nom_gare, gare_arr_trouve[i].stop_seq);
  
  while(i<*nb_gare_arr_trouve){                                                     // pour chaque arrivee, on test tous les departs
    while(j<*nb_gare_dep_trouve){
      if( gare_dep_trouve[j].id == gare_arr_trouve[i].id)                       // condition id = id
      {
        if(gare_dep_trouve[j].stop_seq < gare_arr_trouve[i].stop_seq)          // condition depart avant arrivee
        {

          strcpy(tab_resultats_nodate[l].dep_gare, gare_dep_trouve[j].nom_gare) ;   // copie de toutes les infos selon la structure resultat_nodate
          strcpy(tab_resultats_nodate[l].arr_gare, gare_arr_trouve[i].nom_gare) ;
            tab_resultats_nodate[l].num_train = gare_dep_trouve[j].num_train ;
          tab_resultats_nodate[l].lundi     = gare_dep_trouve[j].lundi     ;
          tab_resultats_nodate[l].mardi     = gare_dep_trouve[j].mardi     ;
          tab_resultats_nodate[l].mercredi  = gare_dep_trouve[j].mercredi  ;
          tab_resultats_nodate[l].jeudi     = gare_dep_trouve[j].jeudi     ;
          tab_resultats_nodate[l].vendredi  = gare_dep_trouve[j].vendredi  ;
          tab_resultats_nodate[l].samedi    = gare_dep_trouve[j].samedi    ;
          tab_resultats_nodate[l].dimanche  = gare_dep_trouve[j].dimanche  ;
          tab_resultats_nodate[l].heure_dep = gare_dep_trouve[j].depart    ;
          tab_resultats_nodate[l].heure_arr = gare_arr_trouve[i].arrive    ;
          strcpy(tab_resultats_nodate[l].type, gare_dep_trouve[j].type)    ;
          l++;
          trouve++;
          printf("%d\n", trouve);                                               // evidemment ce printf n'a pas encore marche pour moi
          tab_resultats_nodate = (struct resultat_nodate *) realloc(tab_resultats_nodate,sizeof(struct resultat_nodate) * (trouve));
          
        }// fin du if2
      }// fin du if1
      j++;                 // increment du while pour les depart
    }// fin du while2
    i++;                     // increment du while pour les arrivees
  }// fin du while1 
  
  for(i=0;i<trouve;i++){    // affichage de test, n'a pas encore marche evidement
    printf("%s ; %s ; %d ; %d ; %d\n", tab_resultats_nodate[i].dep_gare, tab_resultats_nodate[i].arr_gare, tab_resultats_nodate[i].num_train, tab_resultats_nodate[i].heure_dep, tab_resultats_nodate[i].heure_arr );
  }
     
  return(trouve);          // on retourne trouve qui nous servira dans le if de lance_recherche pour passer � la date, compare() est donc une fonction en int
  
}
