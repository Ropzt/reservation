#include <stdio.h>
#include <string.h>

#define MAX_GARE 100

struct UnTarif {
  char  garedep[MAX_GARE] ;
  char  garearr[MAX_GARE] ;
  float prix0             ;
  float prix2             ;
  float prix1             ;
};

struct UnTarif *tarifs           ;
int    nbtarif      ;

void chargement_tarifs();
void convmaj(char chaine[]);
void coupe_chaine_au_caractere(char chaine[], char sschaine1[], char sschaine2[], char sep);

main(){
	
	chargement_tarifs();
	
}



void chargement_tarifs()
{
  FILE *f1;
  int  i, retour;
  char line[500] ; // pour lire ligne par ligne avec sscanf
  char od[200]   ; // =origine-destination
  char dumpchar[MAX_GARE];
  char sep='-';
  
  

  /* --- Allocation de mémoire au tableau trajet --- */
  tarifs = (struct UnTarif *) malloc(sizeof(struct UnTarif));

  /*--- Ouverture fichier horaires --- */
  //f1=fopen("./data_sncf/sncf_full_test.txt", "r") ;
  f1=fopen("./data/tarif/tarifs-tgv-par-od.csv","r") ;
  if (f1 == NULL)
  {
    printf("Erreur de chargement des tarifs\n") ;
  }
  else
  {
    i=0 ;
    fgets(line, sizeof(line), f1); // lire la 1re ligne et ne rien faire (je l'enlève car il n'y a plus les entêtes dans le csv)
    while (fgets(line,sizeof(line),f1) != NULL)
    {
      retour=sscanf(line,"%202[^;];%f;%f;%f;%101[^\n]",
        od,
        &tarifs[i].prix0,
        &tarifs[i].prix2,
        &tarifs[i].prix1,
        dumpchar);
      coupe_chaine_au_caractere(od,tarifs[i].garedep,tarifs[i].garearr,sep);
      convmaj(tarifs[i].garedep);
      convmaj(tarifs[i].garedep);
      // supprime_accent(tarifs[i].garedep);
      // supprime_accent(tarifs[i].garearr);
  	  //printf("%s-%s prems=%f seconde=%f premiere=%f\n",tarifs[i].garedep,tarifs[i].garearr,tarifs[i].prix0,tarifs[i].prix2,tarifs[i].prix1);
      i++ ;
      nbtarif = i ;
      tarifs = (struct UnTarif *) realloc(tarifs,sizeof(struct UnTarif) * (nbtarif+1)) ;
    }
    fclose(f1) ;
  }
  printf("nombre de tarifs %d\n",nbtarif);
  printf("tarif 1140 : %f ; %f ; %f\n",tarifs[1140].prix0, tarifs[1140].prix2 , tarifs[1140].prix1  );
}

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

void convmaj(char chaine[])
{
  int i ;

  for (i=0 ; i < strlen(chaine) ; i++)
  {
    chaine[i] = toupper(chaine[i]) ;
  }
}
