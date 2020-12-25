#include <stdio.h>
#include <stdlib.h>

//structure représentant un automate fini non déterministe
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

//structure représentant un automate fini déterministe
typedef struct AFD
{
	int nbEtats;
	int* transition[256];
	int initial;
	int nbEtatsFinaux;
	int* final;	
} AFD;

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

//construit un automate non déterministe reconnaissant l'union des langages des deux automates non déterministes en entrée
void unionAFND(AFND* automate1, AFND* automate2, AFND* automate_union);

//valorise les nombres d'états, états initiaux et états finaux puis alloue le tableau d'états finaux
void construireAFDVierge(AFD* automate, int nbEtats, int nbEtatsFinaux);

//libère la mémoire allouée à un automate fini déterministe
void desallouerAFD(AFD* automate);

//déterminise un automate fini non déterministe
void determiniser(AFND* nonDeter, AFD* deter);

//retourne vrai si les deux états ont le même nombre de composantes et si chaque composante de l'état 1 est présente dans l'état 2
int est_meme_etat(int compEtat1, int compEtat2, int* tableEtat1, int* tableEtat2);

//retourne vrai si le mot fourni est reconnu par l'automate fourni, retourne faux autrement
int est_reconnu(char* mot, int longueurMot, AFD* automate);

//minimise un automate fini déterministe
void minimiser(AFD* automate, AFD* minimal);

int main(int argc, char const *argv[])
{

	AFND automate3;
	AFND automateMotVide;
	AFND union_3MotVide;

	char* mot;

	AFD union_3MotVideDeter;
	AFD union_minimale;

	construireAFNDLangageUnCar(&automate3,51);
	construireAFNDMotVide(&automateMotVide);
	unionAFND(&automate3,&automateMotVide,&union_3MotVide);

	determiniser(&union_3MotVide, &union_3MotVideDeter);

	printf("Nombre d'états avant minimisation : %d\n",union_3MotVideDeter.nbEtats);

	minimiser(&union_3MotVideDeter,&union_minimale);

	printf("Nombre d'états après minimisation : %d\n",union_minimale.nbEtats);

	if(est_reconnu("",0,&union_minimale))
	{
		printf("reconnu\n");
	}
	else
	{
		printf("non reconnu\n");
	}
 
	desallouerAFND(&automate3);
	desallouerAFND(&automateMotVide);
	desallouerAFND(&union_3MotVide);
	desallouerAFD(&union_3MotVideDeter);
	desallouerAFD(&union_minimale);
	
	return 0;
}

void minimiser(AFD* automate, AFD* minimal)
{
	int i,j,k,l,m;
	int final;
	int* classe1;
	int* classe2;
	int nbClasses1;
	int nbClasses2;
	int* transitionClasse[256];
	int dest;
	int arret;
	int nextClasse;
	int identique;

	nextClasse = 2;

	arret = 0;

	for (i = 0; i < 256; i++)
	{
		transitionClasse[i] = malloc(sizeof(int)*automate->nbEtats);
	}
	
	classe1 = malloc(sizeof(int)*automate->nbEtats);
	classe2 = malloc(sizeof(int)*automate->nbEtats);

	nbClasses1 = 2;
	nbClasses2 = 0;

	for (i = 0; i < automate->nbEtats; i++)
	{
		final = 0;
		for (j = 0; j < automate->nbEtatsFinaux; j++)
		{
			if (i == automate->final[j])
			{
				final = 1;
			}
			if (final == 1)
			{
				classe1[i] = 0;
			}
			else
			{
				classe1[i] = 1;
			}
		}
	}

	while(arret == 0)
	{
		if(nextClasse == 1)
		{
			for (i = 0; i < 256; i++)
			{
				for (j = 0; j < automate->nbEtats; j++)
				{
					if(automate->transition[i][j] == -1)
					{
						transitionClasse[i][j] = -1;
					}
					else
					{
						transitionClasse[i][j] = classe2[automate->transition[i][j]];
					}
				}
			}

			nbClasses1 = 0;
			for (i = 0; i < automate->nbEtats; i++)
			{
				classe1[i] = nbClasses1;
				for (j = 0; j < i; j++)
				{
					identique = 1;
					for (k = 0; k < 256; k++)
					{
						if(transitionClasse[k][i] != transitionClasse[k][j])
						{
							identique = 0;
						}
					}
					if(identique == 1)
					{
						classe1[i] = classe1[j];
					}
				}
				if(classe1[i] = nbClasses1){
					nbClasses1++;
				}
			}
			nextClasse = 2;
		}
		else
		{

			for (i = 0; i < 256; i++)
			{
				for (j = 0; j < automate->nbEtats; j++)
				{
					if(automate->transition[i][j] == -1)
					{
						transitionClasse[i][j] = -1;
					}
					else
					{
						transitionClasse[i][j] = classe1[automate->transition[i][j]];
					}
				}
			}

			nbClasses2 = 0;
			for (i = 0; i < automate->nbEtats; i++)
			{
				classe2[i] = nbClasses2;
				for (j = 0; j < i; j++)
				{
					identique = 1;
					for (k = 0; k < 256; k++)
					{
						if(transitionClasse[k][i] != transitionClasse[k][j])
						{
							identique = 0;
						}
					}
					if(identique == 1)
					{
						classe2[i] = classe2[j];
					}
				}
				if(classe2[i] == nbClasses2){
					nbClasses2++;
				}
			}
			nextClasse = 1;
		}

		if(nbClasses1 != nbClasses2)
		{
			arret = 1;
		}
		else
		{
			identique = 1;
			for (i = 0; i < automate->nbEtats; i++)
			{
				if(classe1[i] != classe2[i])
				{
					identique = 0;
				}
			}
			if (identique == 1)
			{
				arret = 1;
			}
		}
	}


	if(nextClasse == 2)
	{
		construireAFDVierge(minimal,nbClasses1,0);
		minimal->initial = classe1[automate->initial];
		for(i = 0; i < automate->nbEtats; i++)
		{
			for(j = 0; j < automate->nbEtatsFinaux; j++)
			{
				if(automate->final[j] == i)
				{
					minimal->nbEtatsFinaux++;
					minimal->final = realloc(minimal->final, sizeof(int)*minimal->nbEtatsFinaux);
					minimal->final[nbClasses1-1] = classe1[i];
				}
			}
			for(j = 0; j < 256; j++)
			{
				minimal->transition[j][classe1[i]] = transitionClasse[j][i];
			}
		}
	}
	else
	{
		construireAFDVierge(minimal,nbClasses2,0);
		minimal->initial = classe2[automate->initial];
		for(i = 0; i < automate->nbEtats; i++)
		{
			for(j = 0; j < automate->nbEtatsFinaux; j++)
			{
				if(automate->final[j] == i)
				{
					minimal->nbEtatsFinaux++;
					minimal->final = realloc(minimal->final, sizeof(int)*minimal->nbEtatsFinaux);
					minimal->final[nbClasses2-1] = classe2[i];
				}
			}
			for(j = 0; j < 256; j++)
			{
				minimal->transition[j][classe2[i]] = transitionClasse[j][i];
			}
		}
	}
	for (i = 0; i < 256; i++)
	{
		free(transitionClasse[i]);
	}
	free(classe1);
	free(classe2);

}

int est_reconnu(char* mot, int longueurMot, AFD* automate)
{
	int etatCourant;
	int i;
	int reconnu;
	reconnu = 0;
	etatCourant = automate->initial;

	for (i = 0; i < longueurMot; i++)
	{
		printf("lecture de %c\n",mot[i] );
		if(automate->transition[mot[i]][etatCourant] != -1)
		{
			etatCourant = automate->transition[mot[i]][etatCourant];
			printf("transition vers : %d\n", etatCourant);
		}
	}

	for(i = 0; i < automate->nbEtatsFinaux; i++)
	{
		if(automate->final[i] == etatCourant)
		{
			reconnu = 1;
			printf("%d est accepteur\n",etatCourant );
		}
	}
	return reconnu;
}

void determiniser(AFND* nonDeter, AFD* deter)
{
	int i,j,k,l,m;
	int* transitionDeter[256];
	int nbEtatsDeter;
	int* finalDeter;
	int nbEtatsFinauxDeter;
	int initialDeter;
	int curseurEtatDeter;
	int* tableNouvEtat;
	int compNouvEtat;
	int commun;
	int** tableEtat;
	int* compEtat;
	int courant;
	int duplicat;
	int etatFinalAjoute;
	

	courant = 0;
	nbEtatsDeter = 1;
	compEtat = malloc(sizeof(int));
	compEtat[courant] = nonDeter->nbEtatsInitiaux;
	tableEtat = malloc(sizeof(int*));
	tableEtat[courant] = malloc(sizeof(int)*compEtat[courant]);

	for (i = 0; i < 256; i++){
		transitionDeter[i] = malloc(sizeof(int));
		transitionDeter[i][0] = -1;
	}

	for (i = 0; i < compEtat[courant]; i++)
	{
		tableEtat[courant][i] = nonDeter->initial[i];
	}
	while(courant < nbEtatsDeter)
	{
		for (i = 0; i < 256; i++)
		{
			compNouvEtat = 0;
			for (j = 0; j < compEtat[courant]; j++)
			{
					for (l = 0; l < nonDeter->nbEtats; l++)
					{
						for (m = 0; m < nonDeter->nbTransitions[tableEtat[courant][j]][l]; m++)
						{
							if(nonDeter->transition[tableEtat[courant][j]][l][m] == i)
							{
								compNouvEtat++;
								if(compNouvEtat == 1)
								{
									tableNouvEtat = malloc(sizeof(int));
								}
								else
								{
									tableNouvEtat = realloc(tableNouvEtat, sizeof(int)*compNouvEtat);
								}

								tableNouvEtat[compNouvEtat-1] = l;

							}
						}
					}
			}
			if(compNouvEtat > 0)
			{
				duplicat = 0;
				for (j = 0; j < nbEtatsDeter; j++)
				{
					if(est_meme_etat(compNouvEtat,compEtat[j],tableNouvEtat,tableEtat[j]))
					{
						duplicat = 1;
						transitionDeter[i][courant] = j;
					}
				}
				if(duplicat==0)
				{
					nbEtatsDeter++;
					
					compEtat = realloc(compEtat, sizeof(int)*nbEtatsDeter);
					tableEtat = realloc(tableEtat, sizeof(int*)*nbEtatsDeter);

					compEtat[nbEtatsDeter-1] = compNouvEtat;

					tableEtat[nbEtatsDeter-1] = malloc(sizeof(int)*compNouvEtat);
					
					for (j = 0; j < compNouvEtat; j++)
					{
						tableEtat[nbEtatsDeter-1][j] = tableNouvEtat[j];
					}

					for (j = 0; j < 256; j++)
					{
						transitionDeter[j] = realloc(transitionDeter[j], sizeof(int)*nbEtatsDeter);
						transitionDeter[j][nbEtatsDeter-1] = -1;
					}
					transitionDeter[i][courant] = nbEtatsDeter-1;
				}
				
			}
			if(compNouvEtat > 0)
			{
				free(tableNouvEtat);
			}
		}
		courant++;
		
	}
	nbEtatsFinauxDeter = 0;
	for (i = 0; i < nbEtatsDeter; i++)
	{
		etatFinalAjoute = 0;
		for (j = 0; j < compEtat[i]; j++)
		{
			for (k = 0; k < nonDeter->nbEtatsFinaux; k++)
			{
				if(tableEtat[i][j] == nonDeter->final[k] && etatFinalAjoute == 0)
				{
					nbEtatsFinauxDeter++;
					if(nbEtatsFinauxDeter == 1)
					{
						finalDeter = malloc(sizeof(int));
					}
					else
					{
						finalDeter = realloc(finalDeter, sizeof(int)*nbEtatsFinauxDeter);
					}
					finalDeter[nbEtatsFinauxDeter-1] = i;
					etatFinalAjoute = 1;
				}
			}
		}
	}
	construireAFDVierge(deter, nbEtatsDeter, nbEtatsFinauxDeter);
	
	deter->initial = 0;


	for (i = 0; i < nbEtatsFinauxDeter; i++)
	{
		deter->final[i] = finalDeter[i];
	}
	free(finalDeter);

	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < deter->nbEtats; j++)
		{
			deter->transition[i][j] = transitionDeter[i][j];
		}
		free(transitionDeter[i]);
	}

	for(i = 0; i < nbEtatsDeter; i++)
	{
		free(tableEtat[i]);
	}
	free(tableEtat);
	free(compEtat);
}

int est_meme_etat(int compEtat1, int compEtat2, int* tableEtat1, int* tableEtat2)
{
	int identique;
	int commun;
	int i,j;
	identique = 1;

	if(compEtat1 != compEtat2)
	{
		identique = 0;
	}
	else
	{
		for (i = 0; i < compEtat1; i++)
		{
			commun = 0;
			for (j = 0; j < compEtat2; j++)
			{
				if(tableEtat1[i] == tableEtat2[j])
				{
					commun = 1;
				}
			}
			if(commun == 0)
			{
				identique = 0;
			}
		}
	}
	return identique;
}

void desallouerAFD(AFD* automate)
{
	for (int i = 0; i < 256; i++)
	{
		free(automate->transition[i]);
	}
	free(automate->final);
}

void construireAFDVierge(AFD* automate, int nbEtats, int nbEtatsFinaux)
{
	automate->nbEtats = nbEtats;
	automate->nbEtatsFinaux = nbEtatsFinaux;

	for (int i = 0; i < 256; i++)
	{
		automate->transition[i] = malloc(sizeof(int)*automate->nbEtats);
		for (int j = 0; j < automate->nbEtats; j++)
		{
			automate->transition[i][j] = -1;
		}
	}

	automate->final = malloc(sizeof(int)*automate->nbEtatsFinaux);

}

void construireAFNDVierge(AFND* automate, int nbEtats, int nbEtatsInitiaux, int nbEtatsFinaux)
{
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


void construireAFNDMotVide(AFND* automate)
{
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

void construireAFNDLangageVide(AFND* automate)
{
	construireAFNDVierge(automate, 1, 1, 0);
	automate->initial[0] = 0;
	automate->nbTransitions[0][0] = 0;
}

void construireAFNDLangageUnCar(AFND* automate, char c)
{
	int i,j;

	//remplissage des valeurs triviales
	construireAFNDVierge(automate, 3, 1, 1);

	//état initial
	automate->initial[0] = 0;

	//état final
	automate->final[0] = 1;
	
	//nombre de transitions par couple d'états
	//on a une transition de 0 à 1 (par c), 255 de 0 à 2 (par tous les caractères ASCII sauf c) et 256 transitions de 1 à 2 (par tous les caractères ASCII)
	automate->nbTransitions[0][0] = 0;
	automate->nbTransitions[0][1] = 1;
	automate->nbTransitions[0][2] = 255;

	automate->nbTransitions[1][0] = 0;
	automate->nbTransitions[1][1] = 0;
	automate->nbTransitions[1][2] = 256;

	automate->nbTransitions[2][0] = 0;
	automate->nbTransitions[2][1] = 0;
	automate->nbTransitions[2][2] = 0;

	//allocation des tableaux de transitions là où il en existe
	automate->transition[0][1] = malloc(sizeof(int*)*automate->nbTransitions[0][1]);
	automate->transition[0][2] = malloc(sizeof(int*)*automate->nbTransitions[0][2]);
	automate->transition[1][2] = malloc(sizeof(int*)*automate->nbTransitions[1][2]);
	

	//la transition de 0 à 1 se fait par c, on met donc c dans la seule case de transition[0][1][0]
	automate->transition[0][1][0] = c;

	//on met tous les caractères ASCII sauf dans les transitions de 0 vers 2 
	j = 0;
	for (i = 0; i < automate->nbTransitions[0][2]; i++)
	{
		if (i != c)
		{
			automate->transition[0][2][j] = i;
			j++;
		}
	}

	//on met un caractère ASCII différent dans chaque case de transition[1][2] jusqu'à ce que tous les caractères ASCII y soient
	for (i = 0; i < automate->nbTransitions[1][2]; i++)
	{
		//le fait que la case i contiennet i n'est pas significatif
		//il importe seulement que le tableau de 256 cases contienne les 256 caractères peu importe leur ordre
		automate->transition[1][2][i] = i;
	}

}

void desallouerAFND(AFND* automate)
{
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

void unionAFND(AFND* automate1, AFND* automate2, AFND* automate_union)
{
	int i,j,k;
	construireAFNDVierge(automate_union, automate1->nbEtats + automate2->nbEtats, automate1->nbEtatsInitiaux + automate2->nbEtatsInitiaux, automate1->nbEtatsFinaux + automate2->nbEtatsFinaux);

	//ajout des états initiaux des automates 1 et 2, les uns à la suite des autres
	for (i = 0; i < automate_union->nbEtatsInitiaux; i++)
	{
		automate_union->initial[i] = automate1->initial[i];
	}

	for(i = automate1->nbEtatsInitiaux; i <automate1->nbEtatsInitiaux + automate2->nbEtatsInitiaux; i++)
	{
		automate_union->initial[i] = automate2->initial[i-automate1->nbEtatsInitiaux] + automate1->nbEtats;
	}


	//ajout des états finaux des automates 1 et 2, les uns à la suite des autres
	for (i = 0; i < automate_union->nbEtatsFinaux; i++)
	{
		automate_union->final[i] = automate1->final[i];
	}

	for(i = automate1->nbEtatsFinaux; i <automate1->nbEtatsFinaux + automate2->nbEtatsFinaux; i++)
	{
		automate_union->final[i] = automate2->final[i-automate1->nbEtatsFinaux] + automate1->nbEtats;
	}

	//ajout des transitions de l'automate 1
	for (i = 0; i < automate1->nbEtats; i++)
	{
		for (j = 0; j < automate1->nbEtats; j++)
		{
			automate_union->nbTransitions[i][j] = automate1->nbTransitions[i][j];
			if(automate_union->nbTransitions[i][j] > 0)
			{
				automate_union->transition[i][j] = malloc(sizeof(int)*automate_union->nbTransitions[i][j]);
			}

			for (k = 0; k < automate_union->nbTransitions[i][j]; k++)
			{
				automate_union->transition[i][j][k] = automate1->transition[i][j][k];
			}
		}
	}

	//ajout des transitions de l'automate 2
	for (i = automate1->nbEtats; i < automate1->nbEtats + automate2->nbEtats; i++)
	{
		for (j = automate1->nbEtats; j < automate1->nbEtats + automate2->nbEtats; j++)
		{
			automate_union->nbTransitions[i][j] = automate2->nbTransitions[i - automate1->nbEtats][j - automate1->nbEtats];
			if(automate_union->nbTransitions[i][j] > 0)
			{
				automate_union->transition[i][j] = malloc(sizeof(int)*automate_union->nbTransitions[i][j]);
			}

			for (k = 0; k < automate_union->nbTransitions[i][j]; k++)
			{
				automate_union->transition[i][j][k] = automate2->transition[i - automate1->nbEtats][j - automate1->nbEtats][k];
			}
		}
	}
}