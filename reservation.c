#include <stdio.h>
#include <stdlib.h>

/* --- déclaration des variables globales --- */

/* --- déclarations préliminaires --- */
void chargement_horaires() ;

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
      case 1: printf("à faire : réserver (=consulter (horaires + tarifs) + réserver)\n") ;
              chargement_horaires() ; // chargement des données horaires
              break ;
      case 2: printf("à faire : consulter les horaires\n") ;
              chargement_horaires() ; // chargement des données horaires
              break ;
      case 3: printf("à faire : mes billets\n");
              break ;
    } /* Fin du switch */
  } /* Fin du while */
} /* Fin du main */

/* ======================== */
/* === Sous-programmes  === */
/* ======================== */

/* -------------------------------------------- */
/* --- Procédures de chargement des données --- */
/* -------------------------------------------- */

/* --- Chargement des données horaires --- */
void chargement_horaires()
{
  FILE *f1;
  char lettre;
  int  i, nbtrajet=0, j, retour;
  
  /*--- Creation du type structure trajet ---*/
  struct trajet {
    int  id               ;
    int  arrive           ;
    int  depart           ;
    int  stop_seq         ;
    int  num_train        ;
    int  lundi            ;
    int  mardi            ;
    int  mercredi         ;
    int  jeudi            ;
    int  vendredi         ;
    int  samedi           ;
    int  dimanche         ;
    int  capacite         ;
    char type[10]         ;
    char nom_gare[100]    ;
//    struct trajet *p_prec ; // pour l'instant, pas besoin de pointeur
//    struct trajet *p_suiv ; // pour l'instant, pas besoin de pointeur
  };
  
  /*--- Declaration de la variable tab_trajet ---*/
  struct trajet *tab_trajet ;
  tab_trajet = (struct trajet *) malloc(sizeof(struct trajet));
    
  /*--- Ouverture fichier test --- */
  //f1=fopen("./data_sncf/sncf_full_test1.txt", "r") ;
  f1=fopen("./data_sncf/sncf_full.csv","r") ;
  
  i=nbtrajet ;
  while ((! feof(f1)))
  {
    /*--- fscanf des 13 premieres colonnes, toutes des int ---*/
    fscanf(f1, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;", 
      &tab_trajet[i].id,
      &tab_trajet[i].arrive,
      &tab_trajet[i].depart,
      &tab_trajet[i].stop_seq,
      &tab_trajet[i].num_train,
      &tab_trajet[i].lundi,
      &tab_trajet[i].mardi,
      &tab_trajet[i].mercredi,
      &tab_trajet[i].jeudi,
      &tab_trajet[i].vendredi,
      &tab_trajet[i].samedi,
      &tab_trajet[i].dimanche,
      &tab_trajet[i].capacite);
    
    /*--- fscanf de la colonne 14 caractere par caractere ---*/
    j=0;
    retour=0;
    while((retour!=59) &&(retour!=10))      // tant que different de LF et de ";"
    {
      fscanf(f1,"%c", &lettre) ;            // fscanf du caractere
      retour=lettre;
      if((retour!=59) &&(retour!=10))       // si different de LF et de ";"
      {
        tab_trajet[i].type[j++] = lettre  ; // insertion
      }
    }
    tab_trajet[i].type[j] = '\0';           // cloture de la chaine
    
    /*--- fscanf de la colonne 15 caractere par caractere ---*/
    j=0;                  // reinitialisation des compteurs
    retour=0;             
    while((retour!=59) &&(retour!=10)&& (!feof(f1)) ) // meme procedure que la colonne 14
    {
      fscanf(f1,"%c", &lettre) ;
      retour=lettre;
      if((retour!=59) &&(retour!=10) && (!feof(f1))) 
      {
        tab_trajet[i].nom_gare[j++] = lettre  ;
      } 
    }
    tab_trajet[i].nom_gare[j] = '\0';

    i++;
    nbtrajet=i ;
    
    /*--- realloc de la variable tab_trajet ---*/
    tab_trajet = (struct trajet *) realloc(tab_trajet,sizeof(struct trajet) * (nbtrajet+1)) ;
  }
  
  /*--- Fermeture fichier ---*/
  fclose(f1);
    
  /*--- Test d'affichage ---*/
  for(i=0;i<100;i++)
  {
    printf("%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%s;%s\n", tab_trajet[i].id , tab_trajet[i].arrive , tab_trajet[i].depart , tab_trajet[i].stop_seq , tab_trajet[i].num_train , tab_trajet[i].lundi , tab_trajet[i].mardi , tab_trajet[i].mercredi , tab_trajet[i].jeudi , tab_trajet[i].vendredi , tab_trajet[i].samedi , tab_trajet[i].dimanche , tab_trajet[i].capacite , tab_trajet[i].type , tab_trajet[i].nom_gare  );
  } 
}
