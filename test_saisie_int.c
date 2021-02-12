#include <stdio.h>
#define MAX_DIGIT 25

void saisie_int(char invite[],int max, int *mon_int);

main(){
	int nb, max;
	
	max=50;
	saisie_int("Saisir un age :", max, &nb);
	
	printf("%d\n", nb);
}

void saisie_int(char invite[],int max, int *mon_int){
	
	char digit[MAX_DIGIT];
	int test;
	char dump;
	
	int i=0;
	int erreur = 1;
	while(erreur==1)
	{
		i=0;
		erreur=0;
		char lettre=50;
		printf("%s",invite);
		while(!erreur && i<MAX_DIGIT && lettre!='\n' && ( (lettre<58) && (lettre>47)) )
		{
			scanf("%c", &lettre);
			
				
			if( lettre!='\n')
			{
				if( (lettre<58) && (lettre>47) ) 
				{
					digit[i]=lettre;
					i++;
				}
				else
				{
					erreur=1;
				}	
			}
			else
			{
				test = atoi(digit);
				if(test<1 || test > max)
				{
					erreur=1;
				}
			}
		}
		if(i>=MAX_DIGIT)
		{
			erreur=1;
			printf("Saisie superieure a 25 caracteres. ");
		}
		if(erreur)
		{
			test=0;
			i=0;
			for(i=0;i<MAX_DIGIT; i++)
			{
				digit[i]='\0';
			}
			if(lettre!='\n')
			{
				while(dump!='\n')
				{
					scanf("%c", &dump);
				}
			}		
			dump='a';
			printf("Veuillez saisir un nombre valide.\n");
		}
	}
	*mon_int=test;
	for(i=0;i<MAX_DIGIT; i++)
	{
		digit[i]='\0';
	}
}
