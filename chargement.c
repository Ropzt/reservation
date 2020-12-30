#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXRECH 100

void saisie();
void chargement();

void main()
{
	
/*	
	
	while(choix)
	{
		printf("1-Rechercher des trains\n");
		printf("0-Quitter");
		scanf("%d", &choix);
		
		
		switch(choix)
		{
			case 1 :
				break;
			case 0 :
				printf("Ciao\n");
				break;
		}
		
			while(choix1)
			{
				printf("1-Rechercher des trains par villes\n");
				printf("2-Rechercher des trains par gares\n");
				printf("0-Retour au Menu Principale");
				scanf("%d", &choix1);
		
				switch(choix)
				{
					case 1 :
						break;
					case 2 :
						break;
					case 0 :
						printf("Ciao\n");
						break;
				}
		
		
		
			}
		
		
		
	}
	*/
	chargement();
	//saisie()
}

void chargement()
{
	FILE *f1;
	char trip_id[10][100],depart[10][100], gare[10][100], seq[10][10], service[10][100],lundi[10][100],mardi[10][100],mercredi[10][100],jeudi[10][100],vendredi[10][100],samedi[10][100],dimanche[10][100],start[10][100],end[10][100];
	char lettre;
	int  i, j, retour;
	
	i=0;
	f1=fopen("full_sncf2.csv", "r");
	while ((! feof(f1)))
    {
    	j=0;
    	retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		{
       			 trip_id[i][j] = lettre  ;
       			 j++ ;
     		}	
     		
		}
		trip_id[i][j] = "\0";
		printf("%s\n",trip_id[i]);
		
		j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 depart[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
		depart[i][j] = "\0";
    	printf("%s\n",depart[i]);
		
		j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 gare[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
		gare[i][j] = "\0";
    	printf("%s\n",gare[i]);
		
		j=0;
    	retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 seq[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
		seq[i][j] = "\0";
		printf("%s\n",seq[i]);
		
		j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 service[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
		service[i][j] = "\0";
    	printf("%s\n",service[i]);
    	
    	j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 lundi[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
    	printf("%s\n",lundi[i]);
    	
    	j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 mardi[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
    	printf("%s\n",mardi[i]);
    	
    	j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 mercredi[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
    	printf("%s\n",mercredi[i]);
    	
    	j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 jeudi[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
    	printf("%s\n",jeudi[i]);
    	
    	j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 vendredi[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
    	printf("%s\n",vendredi[i]);
    	
    	j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 samedi[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
    	printf("%s\n",samedi[i]);
    	
    	j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 dimanche[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
    	printf("%s\n", dimanche[i]);
    	
    	j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 start[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
    	printf("%s\n",start[i]);
    	
    	j=0;
		retour=0;
    	while((retour!=59) &&(retour!=10)&&(retour!=34))
    	{
    		fscanf(f1,"%c", &lettre) ;
    		retour=lettre;
      		if ((retour!=59) &&(retour!=10)&&(retour!=34)) 
     		 {
       			 end[i][j] = lettre  ;
       			 j++ ;
     		 }	
		}
    	printf("%s\n",end[i]);
    	
        i++ ;
      
    }
    fclose(f1);
    
    i=0;
    for(i=0;i<10;i++)	
	{
		printf("%s / %d / %s",trip_id[i], seq[i], gare[i]);
	}
    
}


void saisie(){
	
	char lettre, recherche_depart[MAXRECH];
	int i, retour, in ;
	
	
	printf("Veuillez renseigner une gare de départ:");
	while (( lettre!=10 ) && (i<MAXRECH))
    {
      retour=scanf("%c", &lettre) ;
      if (retour != 10) 
      {
        recherche_depart[i] = lettre  ;
        i++ ;
      }
    }
    
    /*--Gestion des caracteres speciaux--*/
    for(i=0;i<strlen(recherche_depart);i++)
	{
		in=recherche_depart[i];
		if( ((in>=33)&&(in<=47)) || ((in>=58)&&(in<=64)) || ((in>=91)&&(in<=96)) || ((in>=123)&&(in<=126)) )
		{
			in=32;
			recherche_depart[i]=in;
		}
	}
	
	/*--MAJ FOR ALL--*/
	for(i=0;i<strlen(recherche_depart);i++)
	{	
		recherche_depart[i]=toupper(recherche_depart[i]);
	}	
	
}

