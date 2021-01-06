#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --- Déclaration des constantes --- */
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

struct horaire *tab_horaires   ; /*--- Declaration de la variable tab_horaires ---*/
struct resultat *tab_resultats ; /*--- Declaration de la variable tab_resultats ---*/
  
/* --- déclarations préliminaires --- */
void chargement_horaires() ;
void lance_recherche()     ;

/* =========================== */
/* === Programme principal === */
/* =========================== */

int main()
{

  int choix=-1 ; /* valeur lue au clavier (choix utilisateur) */

  chargement_horaires() ; // chargement des données horaires
  
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
  
  /* --- Allocation de mémoire aux structures tab_horaires --- */
  tab_horaires = (struct horaire *) malloc(sizeof(struct horaire));

  /*--- Ouverture fichier test --- */
  //f1=fopen("./data_sncf/sncf_full_test1.txt", "r") ;
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
    
    /*--- realloc de la variable tab_horaires ---*/
    tab_horaires = (struct horaire *) realloc(tab_horaires,sizeof(struct horaire) * (nbhoraire+1)) ;
  }
  
  /*--- Fermeture fichier ---*/
  fclose(f1);
    
  /*--- Test d'affichage ---*/
  for(i=0;i<100;i++)
  {
    printf("%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%s;%s\n", tab_horaires[i].id , tab_horaires[i].arrive , tab_horaires[i].depart , tab_horaires[i].stop_seq , tab_horaires[i].num_train , tab_horaires[i].lundi , tab_horaires[i].mardi , tab_horaires[i].mercredi , tab_horaires[i].jeudi , tab_horaires[i].vendredi , tab_horaires[i].samedi , tab_horaires[i].dimanche , tab_horaires[i].capacite , tab_horaires[i].type , tab_horaires[i].nom_gare  );
  } 
}


/* ---------------------------------------- */
/* --- Fonctions de recherche de voyage --- */
/* ---------------------------------------- */

// à replacer là-haut :
void recherche_gare_depart(char rechgare[], int gare_dep_trouve[], int *nb_gare_dep_trouve);
void recherche_gare_arrivee() ;

void lance_recherche()
{
  int  i, nbresdep=0;
  char garedep[MAXstrNOMGARE] ; // saisie utilisateur Gare de départ
  char garearr[MAXstrNOMGARE] ; // saisie utilisateur Gare d'arrivée
  int  datev                  ; // saisie utilisateur Date de voyage
  int  garedepres[20] ; // tableau de chaines quand il y a plusieurs gares dans la même ville

  /* Départ */
  printf("\nGare de départ : ");
  scanf("%s",garedep) ;
  convmaj(garedep) ;
  recherche_gare_depart(garedep,garedepres,&nbresdep) ;

  if(nbresdep==0)
  {
    printf ("\nIl n'y a pas de train au départ de %s\n",garedep) ;
  }
  else
  {
    for (i=0; i < nbresdep ; i++) // à revoir après (on n'aura pas besoin de la boucle ici)
    {
      printf("%d\n",garedepres[i] ); 
    }
    /* Arrivée */
    printf("Gare d'arrivée : ");
    scanf("%s",garearr) ;
    convmaj(garearr) ;
    printf("%s",garedep) ;
    //recherche_gare_arrivee() ;
    if("il n'y a pas de correspondances entre gare dep et gare arr")
    {
      printf ("\nIl n'y a pas de liaison entre %s et %s\n",garedep, garearr) ;
    }
    else
    {
      printf("Date du voyage :");
      scanf("%",datev) ;
      if("il n'y a pas de train entre depart et arrivée à cette date")
      {
        printf("\nAucun train ne circule entre %s et %s le 'date'\n",garedep, garearr, datev) ;
      }
    }
  }
}

void recherche_gare_depart(char rechgare[], int gare_dep_trouve[], int *nb_gare_dep_trouve)
{
  struct horaire unhoraire ;
  int i ; // compteur horaire dans tab_horaires
  int j ; // compteur caractère chaine
  int k ; // compteur caractère sous-chaine
  int position ;
  int l=0 ; // compteur xième gare correspondante trouvée

  struct horaire *horaires_dep_trouve ;

  /* allocation de mémoire à la structure tab_resultats */
  tab_resultats = (struct resultat *) malloc(sizeof(struct resultat));
  /* allocation de mémoire à la structure horaires_dep_trouve */
  horaires_dep_trouve = (struct horaire *) malloc(sizeof(struct horaire));


  for (i=0;i<nbhoraire;i++)
  {
    unhoraire = tab_horaires[i] ;
    for (j=0;unhoraire.nom_gare[j]!='\0';j++)
    {
      k=0;
      if(unhoraire.nom_gare[j] == rechgare[k])
      {
        position = j+1 ;
        while (unhoraire.nom_gare[j] == rechgare[k])
        {
          j++ ; // on va se faire engueuler de modifier le compteur dans la boucle
          k++ ;
        }

        if(rechgare[k]=='\0')
        {
          //strcpy(gare_dep_trouve[l++],untrajet.nom_gare) ; // parce que je renvoyais un id en char
          //gare_dep_trouve[l++] = unhoraire.id ;
          horaires_dep_trouve[l++] = unhoraire ;
          *nb_gare_dep_trouve=l ;

          //printf("test 4 : %s\n",rechgare) ;
          //tab_resultats = (struct resultat *) realloc(tab_resultats,sizeof(struct resultat) * (*nb_gare_dep_trouve+1));
          horaires_dep_trouve = (struct horaire *) realloc(horaires_dep_trouve,sizeof(struct horaire) * (*nb_gare_dep_trouve+1));
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

void recherche_gare_arrivee(char rechgare[], int garearrtrouve[], int *nb_gare_arr_trouve)
{

}