#include <stdio.h>
#include <stdlib.h>

//structure représentant un automate fini non déterministe
typedef struct AFND
{
	//nombre d'états de notre automate
	int nbEtats;

	//nombre de transitions entre chaque couple départ+arrivée
	int** nbTransitions;

	//pour chaque couple départ+arrivée contient un tableau des transitions entre départ et arrivée
	//chaque case contient le caractère déclenchant la transition
	int*** transition;

	//nombre d'états initiaux de notre automate
	int nbEtatsInitiaux;

	//tableau contenant les états initiaux de notre automate
	int* initial;

	//nombre d'états finaux ou accepteurs de notre automate
	int nbEtatsFinaux;

	//tableau contenant les états finaux ou accepteurs de notre automate
	int* final;
} AFND;

//structure représentant un automate fini déterministe
typedef struct AFD
{
	//nombre d'états de notre automate
	int nbEtats;

	//pour chaque couple caractère+départ contient l'état d'arrivée de la transition pour le caractère donné pour l'état de départ donné
	//contient -1 si il n'existe pas de transition par le caractère donné pour l'état de départ donné
	int* transition[256];

	//état initial de notre automate
	int initial;

	//nombre d'états finaux ou accepteurs de notre automate
	int nbEtatsFinaux;

	//tableau contenant les états finaux ou accepteurs de notre automate
	int* final;	
} AFD;

//valorise les nombres d'états, états initiaux et états finaux puis alloue les tableaux correspondants.
void construireAFNDVierge(AFND* automate, int nbEtats, int nbEtatsInitiaux, int nbEtatsFinaux);

//libère la mémoire allouée à un automate fini non déterministe
void desallouerAFND(AFND* automate);

//construit un automate non déterministe reconnaissant le langage qui contient le seul mot vide
void construireAFNDMotVide(AFND* automate);

//construit un automate non déterministe reconnaissant le langage vide
void construireAFNDLangageVide(AFND* automate);

//construit un automate non déterministe reconnaissant le langage qui contient un seul mot composé du caractère c sans répétition
void construireAFNDLangageUnCar(AFND* automate, char c);

//construit un automate non déterministe reconnaissant l'union des langages des deux automates non déterministes en entrée
void unionAFND(AFND* automate1, AFND* automate2, AFND* automate_union);

//construit un automate non déterministe reconnaissant la concaténation des langages des deux automates non déterministes en entrée
void concatenationAFND(AFND* automate1, AFND* automate2, AFND* concatenation);

//construit un automate non déterministe reconnaissant la fermeture itérative de Kleene du langage de l'automate non déterminisate en entrée
void fermetureIterativeDeKleene(AFND* automate, AFND* fermeIterativement);

//valorise les nombres d'états, états initiaux et états finaux puis alloue le tableau d'états finaux
void construireAFDVierge(AFD* automate, int nbEtats, int nbEtatsFinaux);

//libère la mémoire allouée à un automate fini déterministe
void desallouerAFD(AFD* automate);

//déterminise un automate fini non déterministe
void determiniser(AFND* nonDeter, AFD* deter);

//retourne vrai si les deux états ont le même nombre de composantes et si chaque composante de l'état 1 est présente dans l'état 2
//utilisé dans la fonction déterminiser
int est_meme_etat(int compEtat1, int compEtat2, int* tableEtat1, int* tableEtat2);

//retourne vrai si le mot fourni est reconnu par l'automate fourni, retourne faux autrement
int est_reconnu(char* mot, int longueurMot, AFD* automate);

//minimise un automate fini déterministe
void minimiser(AFD* automate, AFD* minimal);

int main(int argc, char const *argv[])
{

	AFND mot3;
	AFND motc;
	AFND motd;
	AFND mot3c;
	AFND mot3c_d;
	AFND etoile3c_d;
	AFD deter_etoile3c_d;
	AFD mini_etoile3c_d;

	construireAFNDLangageUnCar(&mot3,51);
	construireAFNDLangageUnCar(&motc,99);
	construireAFNDLangageUnCar(&motd,100);

	concatenationAFND(&mot3,&motc,&mot3c);

	unionAFND(&mot3c,&motd,&mot3c_d);

	fermetureIterativeDeKleene(&mot3c_d,&etoile3c_d);
	determiniser(&etoile3c_d,&deter_etoile3c_d);
	minimiser(&deter_etoile3c_d,&mini_etoile3c_d);

	if(est_reconnu("d3cdd3c3c",9,&mini_etoile3c_d))
	{
		printf("premier mot reconnu\n");
	}
	else
	{
		printf(" premier mot non reconnu\n");
	}

	if(est_reconnu("d3cdd3ch3c",10,&mini_etoile3c_d))
	{
		printf("second mot reconnu\n");
	}
	else
	{
		printf("second mot non reconnu\n");
	}

	desallouerAFND(&mot3);
	desallouerAFND(&motc);
	desallouerAFND(&motd);
	desallouerAFND(&mot3c);
	desallouerAFND(&mot3c_d);
	desallouerAFND(&etoile3c_d);
	desallouerAFD(&deter_etoile3c_d);
	desallouerAFD(&mini_etoile3c_d);

	return 0;
}

void construireAFNDVierge(AFND* automate, int nbEtats, int nbEtatsInitiaux, int nbEtatsFinaux)
{
	//on initialise le nombre d'états
	automate->nbEtats = nbEtats;
	//on alloue la première dimension du tableau de transitions
	automate->transition = malloc(sizeof(int**)*automate->nbEtats);

	//on alloue la seconde dimension du tableau de transitions
	for (int i = 0; i < automate->nbEtats; i++)
	{
		automate->transition[i] = malloc(sizeof(int*)*automate->nbEtats);
	}

	//on alloue la première dimension du tableau contenant le nombre de transition pour chaque paire
	automate->nbTransitions = malloc(sizeof(int*)*automate->nbEtats);
	
	//on alloue la seconde dimension et on initialise le nombre de transitions entre chaque paire à 0
	for (int i = 0; i < automate->nbEtats; i++)
	{
		automate->nbTransitions[i] = malloc(sizeof(int*)*automate->nbEtats);
		for (int j = 0; j < automate->nbEtats; j++)
		{
			automate->nbTransitions[i][j] = 0;
		}
	}

	//on initialise le nombre d'états initiaux et on alloue le tableau à la taille donnée
	automate->nbEtatsInitiaux = nbEtatsInitiaux;
	automate->initial = malloc(sizeof(int)*automate->nbEtatsInitiaux);

	//on initialise le nombre d'états finaux et on alloue le tableau à la taille donnée
	automate->nbEtatsFinaux = nbEtatsFinaux;
	automate->final = malloc(sizeof(int)*automate->nbEtatsFinaux);
}

void desallouerAFND(AFND* automate)
{
	int i,j,k;

	//pour chaque paire d'états i et j
	for(i = 0; i < automate->nbEtats; i++){
		for(j = 0; j < automate->nbEtats; j++){
			//si il y a des transitions de i à j 
			if(automate->nbTransitions[i][j] > 0){
				//on libère la mémoire dédiée à ces transitions
				free(automate->transition[i][j]);
			}
		}
		//on libère la mémoire dédié aux secondes dimensions des tableaux de transitions et nombres de transitions
		free(automate->nbTransitions[i]);
		free(automate->transition[i]);
	}

	//ainsi que la première dimension
	free(automate->nbTransitions);
	free(automate->transition);
	//on libère également les tableaux d'états initiaux et finaux
	free(automate->initial);
	free(automate->final);
}

void construireAFNDMotVide(AFND* automate)
{
	//on constuit notre AFND en connaissant déjà le nombre d'états nécessaires
	construireAFNDVierge(automate, 2, 1, 1);

	//0 est initial et final
	automate->initial[0] = 0;
	automate->final[0] = 0;

	//256 transitions de 0 à 1 car on transitionne vers 1 pour tout l'alphabet
	automate->nbTransitions[0][1] = 256;

	//on alloue le tableau des transitions
	automate->transition[0][1] = malloc(sizeof(int)*automate->nbTransitions[0][1]);

	//on ajoute une transition de 0 à 1 pour chaque caractère de l'alphabet
	for (int i = 0; i < automate->nbTransitions[0][1] ; i++)
	{
		automate->transition[0][1][i] = i;
	}
}

void construireAFNDLangageVide(AFND* automate)
{
	//on construit un automate à 1 état initial mais pas accepteur
	construireAFNDVierge(automate, 1, 1, 0);

	//0 est initial
	automate->initial[0] = 0;
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

void concatenationAFND(AFND* automate1, AFND* automate2, AFND* concatenation)
{
	//booléens utilisés pour vérifier si un état est initial ou final
	int initial;
	int final;

	//compteur indiquant le nombre d'états à la fois finaux et initiaux
	int finalInitial;

	//nombre d'états finaux dans le nouvel automate
	int nbEtatsFinaux;

	//compteurs de boucle
	int i,j,k,l,m,n,o;

	//on initialise le compteur à 0
	finalInitial = 0;

	//on compte le nombre d'états à la fois initiaux et finaux dans auto2
	for(i = 0; i < automate2->nbEtats; i++){
		final = 0;
		initial = 0;

		for(j = 0; j < automate2->nbEtatsInitiaux; j++)
		{
			if(automate2->initial[j] == i)
			{
				initial = 1;
			}
		}
		for(j = 0; j < automate2->nbEtatsFinaux; j++)
		{
			if(automate2->final[j] == i)
			{
				final = 1;
			}
		}
		if(final&&initial)
		{
			finalInitial++;
		}
	}

	//le nombre d'états finaux du nouvel est le nombre d'états finaux d'auto2 si il n'y a pas d'état initial et final dans auto2
	//sinon c'est le nombre d'états finaux de auto1 et auto2 moins le nombre d'états initiaux et finaux d'auto2
	if(finalInitial > 0)
	{
		nbEtatsFinaux = automate1->nbEtatsFinaux + automate2->nbEtatsFinaux - finalInitial;
	}
	else
	{
		nbEtatsFinaux = automate2->nbEtatsFinaux;
	}

	construireAFNDVierge(concatenation, automate1->nbEtats + automate2->nbEtats - automate2->nbEtatsInitiaux, automate1->nbEtatsInitiaux, nbEtatsFinaux);
	

	//on ajoute les états initiaux de auto1
	for(i = 0; i < automate1->nbEtatsInitiaux; i++)
	{
		concatenation->initial[i] = automate1->initial[i];
	}


	//on ajoute les états finaux de auto2 qui ne sont pas initiaux
	k = 0;
	for(i = 0; i < automate2->nbEtatsFinaux; i++)
	{
		initial = 0;
		for (j = 0; j < automate2->nbEtatsInitiaux; j++)
		{
			if(automate2->initial[j] == automate2->final[i])
			{
				initial = 1;
			}
		}
		if(!initial)
		{
			concatenation->final[k] = automate2->final[i] + (automate1->nbEtats - automate2->nbEtatsInitiaux) ;
			k++;
			
		}
	}
	//si il existe au moins un état d'auto2 qui est initial et final on ajoute tous les états finaux de auto1 aux états finaux
	if(finalInitial > 0)
	{
		j = 0;
		for(i = k; i < concatenation->nbEtatsFinaux; i++)
		{
			concatenation->final[i] = automate1->final[j];
			j++;
		}
	}

	//on ajoute toutes les transitions de auto1
	for(i = 0; i < automate1->nbEtats; i++)
	{
		for(j = 0; j < automate1->nbEtats; j++)
		{
			concatenation->nbTransitions[i][j] = automate1->nbTransitions[i][j];
			if(concatenation->nbTransitions[i][j] > 0)
			{
				concatenation->transition[i][j] = malloc(sizeof(int)*concatenation->nbTransitions[i][j]);
			}
			for(k = 0; k < automate1->nbTransitions[i][j]; k++)
			{
				concatenation->transition[i][j][k] = automate1->transition[i][j][k];
			}
		}
	}

	//pour chaque état 1 de auto2
	for (i = 0; i < automate2->nbEtats; i++)
	{
		initial = 0;
		for(j = 0; j < automate2->nbEtatsInitiaux; j++){
			if(i == automate2->initial[i])
			{
				initial = 1;
			}
		}
		//si i est initial
		if(initial)
		{
			//pour chaque état j de auto 2
			for(j = 0; j < automate2->nbEtats; j++)
			{
				initial = 0;
				for(k = 0; k < automate2->nbEtatsInitiaux; k++)
				{
					if(j == automate2->initial[k])
					{
						initial = 1;
					}
				}
				//si j n'est pas initial
				if(!initial)
				{
					//pour chaque transition k de i vers j
					for(k = 0; k < automate2->nbTransitions[i][j]; k++)
					{
						//pour chaque l état de auto1
						for(l = 0; l < automate1->nbEtats; l++)
						{
							final = 0;
							for(m = 0; m < automate1->nbEtatsFinaux; m++)
							{
								if(l == automate1->final[m])
								{
									final = 1;
								}
							}
							//si l est un état final
							if(final)
							{
								//on ajoute la transition k mais de l vers j en décalant du nombre d'états de auto1 et du nombre d'états initiaux de auto2 qu'on a retirés
								concatenation->nbTransitions[l][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)]++;
								if(concatenation->nbTransitions[l][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)] == 1)
								{
									concatenation->transition[l][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)] = malloc(sizeof(int)*concatenation->nbTransitions[l][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)]);
								}
								else
								{
									concatenation->transition[l][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)] = realloc(concatenation->transition[l][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)], sizeof(int)*concatenation->nbTransitions[l][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)]);
								}
								concatenation->transition[l][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)][concatenation->nbTransitions[l][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)]-1] = automate2->transition[i][j][k];
							}
						}
					}
				}
				else //si j est initial
				{
					//pour chaque transition k de i vers j
					for(k = 0; k < automate2->nbTransitions[i][j]; k++)
					{
						//pour chaque état l de auto 1
						for(l = 0; l < automate1->nbEtats; l++)
						{
							final = 0;
							for(m = 0; m < automate1->nbEtatsFinaux; m++)
							{
								if(l == automate1->final[m])
								{
									final = 1;
								}
							}
							//si l est final
							if(final)
							{
								//pour chaque état n de auto1
								for(n = 0; n < automate1->nbEtats; m++)
								{
									final = 0;
									for(o = 0; o < automate1->nbEtatsFinaux; o++)
									{
										if(n == automate1->final[o])
										{
											final = 1;
										}
									}
									//si n est final
									if(final)
									{
										//on ajoute la transition k mais de l vers n sans décalage car l et n proviennent de auto1
										if(concatenation->nbTransitions[l][n] == 0){
											concatenation->nbTransitions[l][n]++;
											concatenation->transition[l][n] = malloc(sizeof(int)*concatenation->nbTransitions[l][n]);
										}
										else
										{
											concatenation->nbTransitions[l][n]++;
											concatenation->transition[l][n] = realloc(concatenation->transition[l][n],sizeof(int)*concatenation->nbTransitions[l][n]);
										}
										concatenation->transition[l][n][concatenation->nbTransitions[l][n]-1] = automate2->transition[i][j][k];
									}
								}
							}
						}
					}
				}
			}
		}
		else //si i n'est pas initial 
		{
			//pour chaque état j de auto2
			for (j = 0; j < automate2->nbEtats; j++)
			{
				//si il existe au moins une transition de i à j dans auto2
				if(automate2->nbTransitions[i][j] > 0)
				{
					//on ajoute toutes les transitions k de i vers j en décalant du nombre d'états d'auto1 et du nombre d'états initiaux d'auto2 qu'on a retirés
					if(concatenation->nbTransitions[i + (automate1->nbEtats - automate2->nbEtatsInitiaux)][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)] == 0)
					{
						concatenation->nbTransitions[i + (automate1->nbEtats - automate2->nbEtatsInitiaux)][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)] += automate2->nbTransitions[i][j];
						concatenation->transition[i + (automate1->nbEtats - automate2->nbEtatsInitiaux)][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)] = malloc(sizeof(int)*concatenation->nbTransitions[i + (automate1->nbEtats - automate2->nbEtatsInitiaux)][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)]);
					}
					else
					{
						concatenation->nbTransitions[i + (automate1->nbEtats - automate2->nbEtatsInitiaux)][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)] += automate2->nbTransitions[i][j];
						concatenation->transition[i + (automate1->nbEtats - automate2->nbEtatsInitiaux)][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)] = realloc(concatenation->transition[i + (automate1->nbEtats - automate2->nbEtatsInitiaux)][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)], sizeof(int)*concatenation->nbTransitions[i + (automate1->nbEtats - automate2->nbEtatsInitiaux)][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)]);
					}
					for(k = 0; k < automate2->nbTransitions[i][j]; k++)
					{
						concatenation->transition[i + (automate1->nbEtats - automate2->nbEtatsInitiaux)][j + (automate1->nbEtats - automate2->nbEtatsInitiaux)][k] = automate2->transition[i][j][k];
					}
				}
				
			}
		}
	}
}

void fermetureIterativeDeKleene(AFND* automate, AFND* fermeIterativement)
{
	//compteurs de boucle
	int i,j,k,l,m;

	//compteur d'états à la fois initiaux et finaux
	int initialFinal;

	//booléens utilisés pour vérifier si un état est initial ou final
	int initial;
	int final;

	initialFinal = 0;

	//on compte le nombre d'états initiaux et finaux
	for (i = 0; i < automate->nbEtatsInitiaux; i++)
	{
		for (j = 0; j < automate->nbEtatsFinaux; j++)
		{
			if(automate->initial[i] == automate->final[j])
			{
				initialFinal++;
			}
		}
	}

	//on construit un AFND du même nombre d'états et d'états initiaux, le nombre d'états finaux et le nombre de l'automate initial plus les états initiaux qui ne sont pas finaux dans l'automate initial
	construireAFNDVierge(fermeIterativement, automate->nbEtats, automate->nbEtatsInitiaux, automate->nbEtatsFinaux + (automate->nbEtatsInitiaux - initialFinal));
	
	//k sert de curseur pour le tableau des finaux
	k = 0;
	//on ajoute tous les états finaux d'auto
	for (i = 0; i < automate->nbEtatsFinaux; i++)
	{
		fermeIterativement->final[k] = automate->final[i];
		k++;
	}

	//on ajoute tous les états initiaux de auto qui ne sont pas finaux dans auto
	//car ils ont été ajoutés dans le for précédent
	for (i = 0; i < automate->nbEtatsInitiaux; i++)
	{
		fermeIterativement->initial[i] = automate->initial[i];
		final = 0;
		for (j = 0; j < automate->nbEtatsFinaux; j++)
		{
			if(automate->final[j] == automate->initial[i])
			{
				final = 1;
			}
		}
		if(!final)
		{
			fermeIterativement->final[k] = automate->initial[i];
			k++;
		}
	}

	//on ajoute toutes les transitions de auto 
	for(i = 0; i < automate->nbEtats; i++)
	{
		for(j = 0; j < automate->nbEtats; j++)
		{
			if(automate->nbTransitions[i][j] > 0)
			{
				fermeIterativement->nbTransitions[i][j] = automate->nbTransitions[i][j];
				fermeIterativement->transition[i][j] = malloc(sizeof(int)*fermeIterativement->nbTransitions[i][j]);
				for(k = 0; k < automate->nbTransitions[i][j]; k++)
				{
					fermeIterativement->transition[i][j][k] = automate->transition[i][j][k];
				}
			}
		}
	}

	//pour chaque état initial initial[i] de auto
	for(i = 0; i < automate->nbEtatsInitiaux; i++)
	{
		//pour chaque état j de auto
		for(j = 0; j < automate->nbEtats; j++)
		{
			//si il y a une transition de initial[i] à j
			if(automate->nbTransitions[automate->initial[i]][j] > 0)
			{
				//pour chaque état final final[k] de auto
				for(k = 0; k < automate->nbEtatsFinaux; k++)
				{
					//m = nombre de transition actuel de final[k] à j
					m = fermeIterativement->nbTransitions[automate->final[k]][j];
					//on ajoute le nombre de transitions de initial[i] à j dans auto au nombre de transitions de final[k] à j dans fermé
					fermeIterativement->nbTransitions[automate->final[k]][j] += automate->nbTransitions[automate->initial[i]][j];
					//si il n'y avait pas de transitions on alloue la mémoire, si il y en avait on la réalloue
					if(m == 0)
					{
						fermeIterativement->transition[automate->final[k]][j] = malloc(sizeof(int)*fermeIterativement->nbTransitions[automate->final[k]][j]);
					}
					else
					{
						fermeIterativement->transition[automate->final[k]][j] = realloc(fermeIterativement->transition[automate->final[k]][j], sizeof(int)*fermeIterativement->nbTransitions[automate->final[k]][j]);
					}
					//pour chaque transition l de initial[i] à j dans auto
					for (l = 0; l < automate->nbTransitions[automate->initial[i]][j]; l++)
					{
						//on ajoute une transition identique au départ de final[k] dans fermé
						fermeIterativement->transition[automate->final[k]][j][m] = automate->transition[automate->initial[i]][j][l];
						m++;
					}
				}
			}
		}
	}
}

void construireAFDVierge(AFD* automate, int nbEtats, int nbEtatsFinaux)
{
	//on initialise nombre d'états et ombre d'états finaux
	automate->nbEtats = nbEtats;
	automate->nbEtatsFinaux = nbEtatsFinaux;

	//on initialise le tableau de transitions à -1 pour tout couple départ+caractère et on alloue nbEtats cases pour chaque caractère
	for (int i = 0; i < 256; i++)
	{
		automate->transition[i] = malloc(sizeof(int)*automate->nbEtats);
		for (int j = 0; j < automate->nbEtats; j++)
		{
			automate->transition[i][j] = -1;
		}
	}

	//on alloue une case pour chaque état final
	automate->final = malloc(sizeof(int)*automate->nbEtatsFinaux);

}

void desallouerAFD(AFD* automate)
{
	//on libère la deuxièmle dimension du tableau de transitions
	for (int i = 0; i < 256; i++)
	{
		free(automate->transition[i]);
	}
	//on libère le tableau d'états finaux
	free(automate->final);
}

void determiniser(AFND* nonDeter, AFD* deter)
{
	//compteurs de boucle
	int i,j,k,l,m,n;

	//tableau des transitions pour la version déterministe
	int* transitionDeter[256];

	//nombre d'états de la version déterministe
	int nbEtatsDeter;

	//tableaux des états finaux de la version déterministe
	int* finalDeter;

	//nombre d'états finaux de la version déterministe
	int nbEtatsFinauxDeter;

	//état initial de la version déterministe
	int initialDeter;

	//table contenant les états composant le nouvel état
	int* tableNouvEtat;

	//nombre d'états composant le nouvel état
	int compNouvEtat;

	//table des états de la version déterministe
	int** tableEtat;

	//nombre d'états composant un état de la version déterministe
	int* compEtat;

	//état courant
	int courant;

	//booléen permettant de vérifier si un élément est déjà dans un ensemble auquel on veut l'ajouter
	int duplicat;

	//booléen indiquant si un état final a déjà été ajouté à la version déterministe
	int etatFinalAjoute;
	
	//on initialise l'état courant à 0
	courant = 0;

	//on initialise le nombre d'états de la version déterministe à 1
	nbEtatsDeter = 1;

	//on alloue une case pour le tableau nombre de composants
	compEtat = malloc(sizeof(int));

	//le premier état est l'état initial composé de tous les états initiaux de la version non déterministe
	compEtat[courant] = nonDeter->nbEtatsInitiaux;
	tableEtat = malloc(sizeof(int*));
	tableEtat[courant] = malloc(sizeof(int)*compEtat[courant]);

	//on initialise toutes les transitions vers l'état 0 à -1
	for (i = 0; i < 256; i++){
		transitionDeter[i] = malloc(sizeof(int));
		transitionDeter[i][0] = -1;
	}

	//on ajoute tous les états initiaux aux composants de l'états courant
	for (i = 0; i < compEtat[courant]; i++)
	{
		tableEtat[courant][i] = nonDeter->initial[i];
	}

	//tant qu'il reste des états de la version déterministe à traiter
	while(courant < nbEtatsDeter)
	{
		//pour chaque caractère i
		for (i = 0; i < 256; i++)
		{
			//on initialise le nombre de composant d'un potentiel nouvel état à 0
			compNouvEtat = 0;

			//pour chaque composant j de l'état courant
			for (j = 0; j < compEtat[courant]; j++)
			{
					//pour chaque état l de la version non déterministe
					for (l = 0; l < nonDeter->nbEtats; l++)
					{
						//pour chaque transition m entre le composant j et l'état l
						for (m = 0; m < nonDeter->nbTransitions[tableEtat[courant][j]][l]; m++)
						{
							//si cette transition se fait pour le caractère i
							if(nonDeter->transition[tableEtat[courant][j]][l][m] == i)
							{
								//on initialise le booléen à 0
								duplicat = 0;

								//pour chaque composant n de notre nouvel état
								for(n = 0; n < compNouvEtat; n++)
								{
									//si l = n on passe le booléen à vrai
									if(l==tableNouvEtat[n])
									{
										duplicat=1;
									}
								}
								//si notre composant n'est pas encore dans le nouvel état on l'y ajoute
								if(!duplicat)
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
			}
			//si notre potentiel nouvel état a au moins un composant
			if(compNouvEtat > 0)
			{
				//on vérifie si notre potentiel nouvel état fait déjà partie des états découverts
				duplicat = 0;
				for (j = 0; j < nbEtatsDeter; j++)
				{
					if(est_meme_etat(compNouvEtat,compEtat[j],tableNouvEtat,tableEtat[j]))
					{
						duplicat = 1;
						transitionDeter[i][courant] = j;
					}
				}
				//sinon on l'ajoute aux états de la version déterministe à traiter
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
			//si de la mémoire a été allouée au potentiel nouvel état on la libère pour l'itération suivante
			if(compNouvEtat > 0)
			{
				free(tableNouvEtat);
			}
		}
		//on incrémente l'état courant
		courant++;
		
	}
	//pour chaque état de la version déterministe on l'ajoute aux états finaux si un de ces composants est un état final de la version non déterministe
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
	//on initialise la version déterministe
	construireAFDVierge(deter, nbEtatsDeter, nbEtatsFinauxDeter);
	
	//l'état initial est toujours 0, il est traité manuellement avant la boucle 
	deter->initial = 0;

	//on ajoute les états finaux découverts à la version déterministe
	for (i = 0; i < nbEtatsFinauxDeter; i++)
	{
		deter->final[i] = finalDeter[i];
	}
	free(finalDeter);

	//on ajoute les transitions découvertes à la version déterministe
	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < deter->nbEtats; j++)
		{
			deter->transition[i][j] = transitionDeter[i][j];
		}
		//on libère le tableau de transition par la même occasion
		free(transitionDeter[i]);
	}

	//on libère le reste des tableaux intermédiaires
	for(i = 0; i < nbEtatsDeter; i++)
	{
		free(tableEtat[i]);
	}
	free(tableEtat);
	free(compEtat);
}

int est_meme_etat(int compEtat1, int compEtat2, int* tableEtat1, int* tableEtat2)
{
	//booléen valeur de retour
	int identique;

	//indique si deux composants sont en commun entre deux états
	int commun;

	//compteurs de boucle
	int i,j;

	//on initialise identique à vrai
	identique = 1;

	//si les deux états n'ont pas le même nombre de composants ils ne sont pas identiques
	if(compEtat1 != compEtat2)
	{
		identique = 0;
	}
	else //on vérifie que chaque composant de l'état 1 est dans l'état 2
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
			//si un composant de l'état 1 n'est pas dans l'état 2 alors on passe le booléen à faux
			if(commun == 0)
			{
				identique = 0;
			}
		}
	}
	//on retourne notre booléen
	return identique;
}

int est_reconnu(char* mot, int longueurMot, AFD* automate)
{
	//état courant pendant l'exécution
	int etatCourant;

	//compteur de boucle
	int i;

	//booléen valeur de retour
	int reconnu;

	//on initialise la valeur de retour à faux
	reconnu = 0;

	//l'état courant est initilement l'état intial
	etatCourant = automate->initial;

	//pour chaque caractère composant le mot
	for (i = 0; i < longueurMot; i++)
	{
		printf("lecture de %c\n",mot[i] );
		//si il existe une transition depuis l'état courant par ce caractère 
		if(automate->transition[mot[i]][etatCourant] != -1)
		{
			//l'état courant devient la destination de cette transition
			etatCourant = automate->transition[mot[i]][etatCourant];
			printf("transition vers : %d\n", etatCourant);
		}
	}

	//si l'état courant à la fin de l'exécutuion fait partie des états finaux on passe la valeur de retour à vrai
	for(i = 0; i < automate->nbEtatsFinaux; i++)
	{
		if(automate->final[i] == etatCourant)
		{
			reconnu = 1;
			printf("%d est accepteur\n",etatCourant );
		}
	}
	//on retourne notre valeur de retour
	return reconnu;
}

void minimiser(AFD* automate, AFD* minimal)
{
	//compteurs de boucles
	int i,j,k,l,m;

	//indique si le compteur courant correspond à un état final
	int final;

	// deux tableaux de classes class1[i] contient la classe ou l'ensemble d'états dans lequel se trouve i
	// la boucle s'arrête quand le tableau de classes précédent est identique au tableau courant
	// on en utilise donc deux qu'on rempli à tour de rôle pour préserer l'itération précédente
	int* classe1;
	int* classe2;

	//nombre de classes existantes dans chaque tableau, pas utile pour les accès mémoire mais permet de remplir les tableau
	//permet également de valoriser le nombre d'états du nouvel automate car une classe = un nouvel état
	int nbClasses1;
	int nbClasses2;

	//contient pour chaque couple [caractère][état] la classe de l'état vers lequel on transitionne pour le couple donné
	//permet de raffiner les classes à la fin de chaque itération, deux états ne restent dans la même classe que si ils transitionnent vers la même classe pour tout l'alphabet
	int* transitionClasse[256];

	//variable temporaire pour stocker l'état destination d'une transition
	int dest;

	//variable d'arrêt pour la boucle de minimisation
	int arret;

	//variable contenant 1 ou 2 en fonction du tableau de classes à remplir à l'itération suivante
	int nextClasse;

	//booléen indiquant si deux états ou classes, ou tableaux de classes sont identiques
	int identique;

	//booléen indiquant si un état final a déjà été ajouté à la version minimale
	int dejaAjoute;


	//on commence par remplir le tableau de classes 1 donc le suivant doit être le 2
	nextClasse = 2;

	//on initialise la variable d'arrêt à 0 pour la boucle de minimisation
	arret = 0;

	//on alloue une case par état de notre automate pour chaque caractère dans le tableau des transitions
	for (i = 0; i < 256; i++)
	{
		transitionClasse[i] = malloc(sizeof(int)*automate->nbEtats);
	}
	
	//pour chaque tableau de classes on alloue une case par état également (puisqu'on veut stocker une classe par état)
	classe1 = malloc(sizeof(int)*automate->nbEtats);
	classe2 = malloc(sizeof(int)*automate->nbEtats);

	//si il n'y a pas d'état final ou si ils le sont tous on a une seule classe et tous les états vont dedans
	//autrement on a deux classes : une pour les états finaux et une pour les autres
	if(automate->nbEtatsFinaux == 0 || automate->nbEtatsFinaux == automate->nbEtats)
	{
		nbClasses1 = 1;
		for (i = 0; i < automate->nbEtats; i++)
		{
			classe1[i] = 0;
		}
	}
	else
	{
		nbClasses1 = 2;
		for (i = 0; i < automate->nbEtats; i++)
		{
			final = 0;
			for (j = 0; j < automate->nbEtatsFinaux; j++)
			{
				if (i == automate->final[j])
				{
					final = 1;
				}
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
	//les classes 2 sont initialisées plus tard
	nbClasses2 = 0;

	//tant que la condition d'arrêt n'est pas atteinte
	while(arret == 0)
	{
		//si le prochain tableau à remplir est le 1
		if(nextClasse == 1)
		{
			//pour chaque caractère i
			for (i = 0; i < 256; i++)
			{
				//pour chaque état j
				for (j = 0; j < automate->nbEtats; j++)
				{
					//si il n'y a pas de transition par i à partir de j
					if(automate->transition[i][j] == -1)
					{
						//j ne transitionne vers aucune classe par i
						transitionClasse[i][j] = -1;
					}
					else //si il y a une transition par i à partir de j
					{
						//on crée une transition vers la classe de la destination dans transitionClasse
						transitionClasse[i][j] = classe2[automate->transition[i][j]];
					}
				}
			}
			//on initialise le nombre de classes à 0 car on rempli le tableau depuis rien
			nbClasses1 = 0;
			//pour chaque état i
			for (i = 0; i < automate->nbEtats; i++)
			{
				//on initialise l'état de i au nombre d'états du tableau
				//le plus bas possible étant 0
				classe1[i] = nbClasses1;
				//pour chaque état j traité avant i
				for (j = 0; j < i; j++)
				{
					//on initialise le booléen identique à vrai
					identique = 1;
					//pour chaque caractère k
					for (k = 0; k < 256; k++)
					{
						//si on ne transitionne pas vers la même classe depuis les états i et j pour le caractère k
						if(transitionClasse[k][i] != transitionClasse[k][j])
						{
							//le booléen identique devient faux pour indiquer que les deux états ne sont pas équivalents au regard de leurs transitions
							//ils ne peuvent donc pas appartenir à la même classe
							identique = 0;
						}
					}
					//si les états i et j sont identiques au regard de leurs transitions
					if(identique == 1)
					{
						//on donne la classe de j à i
						classe1[i] = classe1[j];
					}
				}
				//si i a toujours la classe égale au nombre de classes qu'on lui a donné au début de l'itération
				//celà implique qu'on n'a trouvé aucun j équivalent à i du point de vue de ses transitions
				if(classe1[i] == nbClasses1){
					//on incrémente juste le nombre de classes ce qui revient à créer une nouvelle classe pour i
					nbClasses1++;
				}
			}
			nextClasse = 2;
		}
		else // si le prochain tableau à remplir est le 1 on fait le même traitement en inversant les tableaux
		{
			//pour chaque caractère i
			for (i = 0; i < 256; i++)
			{
				//pour chaque état j
				for (j = 0; j < automate->nbEtats; j++)
				{
					//si il n'y a pas de transition depuis j par i
					if(automate->transition[i][j] == -1)
					{
						//j ne transtionne vers aucune classe par i
						transitionClasse[i][j] = -1;
					}
					else //si il y a une transtion depuis j par i
					{
						//j transitionne vers la classe de la destination de la transition depuis j par i
						transitionClasse[i][j] = classe1[automate->transition[i][j]];
					}
				}
			}
			//on initialise le nombre de classes à 0 car on remplie le tableau depuis rien
			nbClasses2 = 0;
			//pour chaque état i
			for (i = 0; i < automate->nbEtats; i++)
			{
				//on assigne le nombre de classes comme classe à l'état
				//minimum 0
				classe2[i] = nbClasses2;
				//pour chaque état j traité avant i
				for (j = 0; j < i; j++)
				{
					//on initialise le booléen identique à vrai
					identique = 1;
					//pour chaque caractère k
					for (k = 0; k < 256; k++)
					{
						//si i et j ne transitionnent pas vers la même classe pour k
						if(transitionClasse[k][i] != transitionClasse[k][j])
						{
							//le booléen passe à faux car i et j ne sont pas identiques au regard de leurs transitions
							identique = 0;
						}
					}
					//si i et j sont identiques au regard de leurs transition
					if(identique == 1)
					{
						//on donne à i la classe de j
						classe2[i] = classe2[j];
					}
				}
				//si i a toujours la classe égale au nombre de classes qu'on lui a donné au début de l'itération
				//celà implique qu'on n'a trouvé aucun j équivalent à i du point de vue de ses transitions
				if(classe2[i] == nbClasses2){
					//on incrémente juste le nombre de classes ce qui revient à créer une nouvelle classe pour i
					nbClasses2++;
				}
			}
			//le prochain tableau à traiter est le 1
			nextClasse = 1;
		}
		//ici on vérifie si la condition d'arrêt est remplie
		if(nbClasses1 != nbClasses2)
		{
			arret = 0;
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

	//selon le dernier tableau de classes rempli on initialise et on complète l'automate minimal avec l'un ou l'autre de nos tableaux
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
					dejaAjoute = 0;
					for(k = 0; k < minimal->nbEtatsFinaux; k++)
					{
						if(minimal->final[k] == classe1[i])
						{
							dejaAjoute = 1;
						}
					}
					if(!dejaAjoute)
					{
						minimal->nbEtatsFinaux++;
						minimal->final = realloc(minimal->final, sizeof(int)*minimal->nbEtatsFinaux);
						minimal->final[minimal->nbEtatsFinaux-1] = classe1[i];
					}
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
		k = 0;
		for(i = 0; i < automate->nbEtats; i++)
		{
			for(j = 0; j < automate->nbEtatsFinaux; j++)
			{
				if(automate->final[j] == i)
				{
					dejaAjoute = 0;
					for(k = 0; k < minimal->nbEtatsFinaux; k++)
					{
						if(minimal->final[k] == classe2[i])
						{
							dejaAjoute = 1;
						}
					}
					if(!dejaAjoute)
					{
						minimal->nbEtatsFinaux++;
						minimal->final = realloc(minimal->final, sizeof(int)*minimal->nbEtatsFinaux);
						minimal->final[minimal->nbEtatsFinaux-1] = classe2[i];
					}
				}
			}
			for(j = 0; j < 256; j++)
			{
				minimal->transition[j][classe2[i]] = transitionClasse[j][i];
			}
		}
	}

	//on libère les tableaux de transitions et de classes 
	for (i = 0; i < 256; i++)
	{
		free(transitionClasse[i]);
	}
	free(classe1);
	free(classe2);

}