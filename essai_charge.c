#include <stdio.h>
#include <ctype.h>
#include <string.h>


void chargement();

void main()
{
	

	chargement();

}

void chargement()
{
	FILE *f1;
	char lettre;
	int  i=0, j, retour;
	
	
	/*----Creation du type structure trajet-----*/
	struct trajet {
		int id              ;
		int arrive		    ;
		int depart			;
		int stop_seq		;
		int num_train		;
		int monday			;
		int tuesday			;
		int wednesday		;
		int thursday		;
		int friday			;
		int saturday		;
		int sunday			;
		int capacite		;
		char type[10]		;
		char nom_gare[100]	;
	};
	
	/*--- Declaration de la variable tab_trajet ---*/
	struct trajet tab_trajet[30];
	
	

	/*--- Ouverture fichier test --- */
	f1=fopen("sncf_full_test.txt", "r");
	
	
	while ((! feof(f1))){

    	/*--- fscanf des 13 premieres colonnes, toutes des int ---*/
    	
    	fscanf(f1, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;", &tab_trajet[i].id , &tab_trajet[i].arrive , &tab_trajet[i].depart , &tab_trajet[i].stop_seq , &tab_trajet[i].num_train , &tab_trajet[i].monday , &tab_trajet[i].tuesday , &tab_trajet[i].wednesday , &tab_trajet[i].thursday , &tab_trajet[i].friday , &tab_trajet[i].saturday , &tab_trajet[i].sunday , &tab_trajet[i].capacite);
    	
    	
    	
    	/*--- fscanf de la colonne 14 caractere par caractere ---*/
    	j=0;
    	retour=0;
		while((retour!=59) &&(retour!=10))      // tant que different de LF et de ";"
    	{
    		fscanf(f1,"%c", &lettre) ;			// fscanf du caractere
    		retour=lettre;
      		if((retour!=59) &&(retour!=10)) 	// si different de LF et de ";"
     		{
       			 tab_trajet[i].type[j++] = lettre  ;   // insertion
     		}
		}
		tab_trajet[i].type[j] = '\0';           // cloture de la chaine
		
		
		/*--- fscanf de la colonne 15 caractere par caractere ---*/
		j=0;									// reinitialisation des compteurs
    	retour=0;							
		while((retour!=59) &&(retour!=10)&& (!feof(f1))	)	// meme procedure que la colonne 14
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
	}
	
	/*--- Fermeture fichier ---*/
    fclose(f1);
    
    /*--- Test d'affichage ---*/
	for(i=0;i<30;i++){
		printf("%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%s;%s\n", tab_trajet[i].id , tab_trajet[i].arrive , tab_trajet[i].depart , tab_trajet[i].stop_seq , tab_trajet[i].num_train , tab_trajet[i].monday , tab_trajet[i].tuesday , tab_trajet[i].wednesday , tab_trajet[i].thursday , tab_trajet[i].friday , tab_trajet[i].saturday , tab_trajet[i].sunday , tab_trajet[i].capacite , tab_trajet[i].type , tab_trajet[i].nom_gare  );
	}
	
	
    
};
