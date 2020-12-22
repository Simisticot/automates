#include <stdio.h>
#include <stdlib.h>

typedef struct AFND
{
	int nbEtats;
	int** nbTransitions;
	int*** transition;
	int nbEtatsInitiaux;
	int* initial;
	int nbEtatsFinaux;
	int* final;
} AFND;

//construit un automate non déterministe reconnaissant le langage qui contient le seul mot vide
void construireAFNDMotVide(AFND* automate);

//construit un automate non déterministe reconnaissant le langage vide
void construireAFNDLangageVide(AFND* automate);

//construit un automate non déterministe reconnaissant le langage qui contient un seul mot composé du caractère c sans répétition
void construireAFNDLangageUnCar(AFND* automate, char c);

//valorise les nombres d'états, états initiaux et états finaux puis alloue les tableaux correspondants.
void construireAFNDVierge(AFND* automate, int nbEtats, int nbEtatsInitiaux, int nbEtatsFinaux);

//libère la mémoire allouée à un automate fini non déterministe
void desallouerAFND(AFND* automate);

int main(int argc, char const *argv[])
{
	//cas de test où on construit un automate qui reconnaît le mot "3" (3 -> caractère 51 en ASCII)
	AFND automate3;
	construireAFNDLangageUnCar(&automate3,51);

	printf("première transition de 0 vers 1 : %d\n",automate3.transition[0][1][0]);

	desallouerAFND(&automate3);
	
	return 0;
}

void construireAFNDVierge(AFND* automate, int nbEtats, int nbEtatsInitiaux, int nbEtatsFinaux){
	automate->nbEtats = nbEtats;
	automate->transition = malloc(sizeof(int**)*automate->nbEtats);

	for (int i = 0; i < automate->nbEtats; i++)
	{
		automate->transition[i] = malloc(sizeof(int*)*automate->nbEtats);
	}

	automate->nbTransitions = malloc(sizeof(int**)*automate->nbEtats);
	
	for (int i = 0; i < automate->nbEtats; i++)
	{
		automate->nbTransitions[i] = malloc(sizeof(int*)*automate->nbEtats);
	}

	automate->nbEtatsInitiaux = nbEtatsInitiaux;
	automate->initial = malloc(sizeof(int)*automate->nbEtatsInitiaux);


	automate->nbEtatsFinaux = nbEtatsFinaux;
	automate->final = malloc(sizeof(int)*automate->nbEtatsFinaux);
}


void construireAFNDMotVide(AFND* automate){
	construireAFNDVierge(automate, 2, 1, 1);
	automate->initial[0] = 0;
	automate->final[0] = 0;

	automate->nbTransitions[0][0] = 0;
	automate->nbTransitions[0][1] = 256;

	automate->transition[0][1] = malloc(sizeof(int)*automate->nbTransitions[0][1]);

	for (int i = 0; i < automate->nbTransitions[0][1] ; i++)
	{
		automate->transition[0][1][i] = i;
	}
}

void construireAFNDLangageVide(AFND* automate){
	construireAFNDVierge(automate, 1, 1, 0);
	automate->initial[0] = 0;
	automate->nbTransitions[0][0] = 0;
}

void construireAFNDLangageUnCar(AFND* automate, char c){
	
	//remplissage des valeurs triviales
	construireAFNDVierge(automate, 3, 1, 1);

	//état initial
	automate->initial[0] = 0;
	
	//nombre de transitions par couple d'états
	//on a une transition de 0 à 1 (par c) et 256 transitions de 1 à 2 (par tous les caractères ASCII)
	automate->nbTransitions[0][0] = 0;
	automate->nbTransitions[0][1] = 1;
	automate->nbTransitions[0][2] = 0;

	automate->nbTransitions[1][0] = 0;
	automate->nbTransitions[1][1] = 0;
	automate->nbTransitions[1][2] = 256;

	automate->nbTransitions[2][0] = 0;
	automate->nbTransitions[2][1] = 0;
	automate->nbTransitions[2][2] = 0;

	//allocation des tableaux de transitions là où il en existe
	automate->transition[0][1] = malloc(sizeof(int*)*automate->nbTransitions[0][1]);
	automate->transition[1][2] = malloc(sizeof(int*)*automate->nbTransitions[1][2]);

	//la transition de 0 à 1 se fait par c, on met donc c dans la seule case de transition[0][1][0]
	automate->transition[0][1][0] = c;

	//on met un caractère ASCII différent dans chaque case de transition[1][2] jusqu'à ce que tous les caractères ASCII y soient
	for (int i = 0; i < automate->nbTransitions[1][2]; ++i)
	{
		//le fait que la case i contiennet i n'est pas significatif
		//il importe seulement que le tableau de 256 cases contienne les 256 caractères peu importe leur ordre
		automate->transition[1][2][i] = i;
	}

}

void desallouerAFND(AFND* automate){
	int i,j,k;

	for(i = 0; i < automate->nbEtats; i++){
		for(j = 0; j < automate->nbEtats; j++){
			if(automate->nbTransitions[i][j] > 0){
				free(automate->transition[i][j]);
			}
		}
		free(automate->nbTransitions[i]);
		free(automate->transition[i]);
	}

	free(automate->nbTransitions);
	free(automate->transition);
	free(automate->initial);
	free(automate->final);
}