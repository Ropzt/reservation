#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

#define SizeDate 11

struct date {
  	int jour;
  	int mois;
  	int annee;
  	char char_date[9];
  };
  
int  jhebdo_num_sys, jour_sys, mois_sys, annee_sys ; // éléments de la date du système

struct date *tab_date; /*--- tableau des dates prises en charge par le programme ---*/
int nbdate;

void date_sys(int *jour, int *mois, int *annee, int *jhebdo_num) ;
int calcul_jour_semaine(int jour_rech, int mois_rech, int annee_rech, int jour, int mois, int annee, int jour_sem) ;
void crea_date(int jour, int mois, int annee);
void date_to_char(int j, int m, int a, char chdate[]); // conversion d'une date faite de 3 int en une string au format AAAAMMJJ

main(){
	int i;
	
	date_sys(&jour_sys, &mois_sys, &annee_sys, &jhebdo_num_sys) ; // récupère la date du système
	crea_date(jour_sys, mois_sys, annee_sys);
	
	for(i=0;i<nbdate;i++)
	{
		printf("%s\n", tab_date[i].char_date);
	}
	
	
}

void crea_date(int jour, int mois, int annee)
{
	int jour_end, mois_end, annee_end;
	int j, m, a;
	int i, nbdate;
	char chdate[SizeDate];
	
	
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
  	
  	date_to_char(j,m,a, chdate);
  	
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

void date_to_char(int j, int m, int a, char chdate[]) // conversion d'une date faite de 3 int en une string au format AAAAMMJJ
{
	
	
	chdate[0]=(a/1000)+48;
  	chdate[1]=((a%1000)/100)+48;
  	chdate[2]=((a%100)/10)+48;
  	chdate[3]=(a%10)+48;
  	chdate[4]=(m/10)+48;
  	chdate[5]=(m%10)+48;
  	chdate[6]=(j/10)+48;
  	chdate[7]=(j%10)+48;
  	chdate[8]='\0';
  	
  	
}


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
