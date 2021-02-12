#include <stdio.h>
#include <string.h>

#define MAXstrNOMGARE 100

void saisie_text(char invite[], char sortie[]);

main(){
	
	char gare_dep[MAXstrNOMGARE];
	char gare_arr[MAXstrNOMGARE];
	
	saisie_text("Gare de depart : ", gare_dep);
	
	printf("%s\n", gare_dep);
	
	saisie_text("Gare d'arrivee : ", gare_arr);
	
	printf("%s\n", gare_arr);
	
}
	
	
	
	
		
	void saisie_text(char invite[], char sortie[]){
		
	 int i;
	  char dump, lettre;									//securisation des saisies
	  int val_choix, erreur;										//securisation des saisies
	  int compteur_saisie, compteur_saisie_date;					//securisation des saisies
	  int choix_reserver, date_prob;
	  int jour_prec, mois_prec, annee_prec;
  
  
		compteur_saisie=0;
		erreur = 1;
		while(erreur==1)
		{
			erreur=0;
			lettre='a';
			printf("\n%s                      : ", invite); 		// invite de saisie
			while(!erreur && compteur_saisie<MAXstrNOMGARE && lettre!='\n' && ( ((lettre<91) && (lettre>64)) || ((lettre<123) && (lettre>96)) || (lettre==32) || (lettre==45) ) )
			{															// si la saisie n'est pas une lettre maj ou min, un LF, un espace ou un -, ou qu'elle est trop grande, erreur
				scanf("%c", &lettre);
				
				if(compteur_saisie<MAXstrNOMGARE && lettre!='\n')  				// si la saisie est trop grande ou un LF, sortir, si il n'y avait pas d'erreur avant, fin de la saisie avec success
				{
					if( ((lettre<91) && (lettre>64)) || ((lettre<123) && (lettre>96)) || (lettre==32) || (lettre==45) ) 
					{													// si la saisie n'est pas une lettre maj ou min, un espace ou un -
						sortie[compteur_saisie]=lettre;    	//insertion du caractère
						compteur_saisie++;
						erreur=0;										//pas d'erreur pour l'instant
					}
					else												//erreur
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
			if(compteur_saisie==MAXstrNOMGARE)
			{
				printf("Saisie superieure a 100 caracteres. La saisie a ete tronquee.\n");
				compteur_saisie--;
			}
			
			if(erreur)
			{
				compteur_saisie=0;
						
				for(i=0;i<MAXstrNOMGARE;i++)
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
		sortie[compteur_saisie]='\0';									//fermeture de la chaine de caractère
	}	
		
		
