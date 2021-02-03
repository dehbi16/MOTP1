

#include <iostream>
#include "Tools.h"
using namespace std;

#include <math.h>
#include <iomanip>


int main(int argc, char* argv[])
{
	clock_t	Start, End;	//D�claration de variable afin de calculer le temps �coul�
	double Elapsed = 0;	//Variable servant � calculer le temps �coul� (Diff�rence entre End et Start
	double	dTheBestFitness = 100000;	//Fitness de la meilleure solution
	SMSSDTProblem* LeProb;	//D�claration d'un probl�me	
	LeProb = new SMSSDTProblem(argv[2]);	//Lecture du deuxi;eme param�tre � partir de la console
	//LeProb->printOn(cout);	// Imprimer le Probl�me
	SMSSDTSolution* pSolution = NULL;	//Solution interm�diaire
	int mode = 3; // 0 : methode prof. 1 : descente. 2 : VNS. 3: RS. 4 : recherche Tabou  

	// argv[1] ex�cutions de la g�n�ration al�atoire
	for (int j = 0; j < atoi(argv[1]); j++)
	{
		Start = clock();	//D�marrer l'horloge	
		SMSSDTSolution	Smeilleur = NULL;	//Sauvegarde de la meilleure solution
		SMSSDTSolution	Svoisin = NULL;
		SMSSDTSolution	Svoisin1 = NULL;

		switch (mode) {
		case 0:
			Smeilleur = SMSSDTSolution(LeProb->getN());
			for (int i = 0; i < 1000; i++)
			{
				pSolution = new SMSSDTSolution(LeProb->getN(), true);	//Une solution al�atoire
				Tools::Evaluer(LeProb, *pSolution);	//�valuer la solution
				if (pSolution->getObj() < dTheBestFitness) // Si am�liore meilleure solution, la garder
				{
					Smeilleur = *pSolution;
					dTheBestFitness = Smeilleur.getObj();
				}
				delete pSolution;
			}
			break;
		case 1:
			break;


		case 3:
		{
			int nombreStagnation = 0;
			int nombreStagnationMax = 150;
			float temperature = 1;	//On definit une valeur initiale pour la temp�rature
			float delta = 1;	//On d�finie le facteur de d�croisance de la temp�rature

			pSolution = new SMSSDTSolution(LeProb->getN(), true);	//Une g�n�re une solution al�atoire pour initilaiser l'agorithme
			Tools::Evaluer(LeProb, *pSolution);	//On �value la solution cr�e
			if (pSolution->getObj() < dTheBestFitness) // Si am�liore meilleure solution, la garder
			{
				Smeilleur = *pSolution;
				dTheBestFitness = Smeilleur.getObj();
			}

			int i;
			for (i = 0; i < 1000 /*&& nombreStagnation < nombreStagnationMax*/; i++)
			{
				SMSSDTSolution* nouvelleSolution = new SMSSDTSolution(LeProb, *pSolution);	//On cr�e al�atoirement une solution voisine de la solution actuelle
				Tools::Evaluer(LeProb, *nouvelleSolution);
				float nombreAleatoire = (float)(((float)rand() / ((float)RAND_MAX + 1.0)) * (1 - 0));	//On choisie al�atoirement un nombre entre 0 et 1
				//out << "La diferance est de  : " << pSolution->getObj() - nouvelleSolution->getObj() << endl;
				//cout << "La proba est de  : " << setprecision(5) << exp((float)(pSolution->getObj() - nouvelleSolution->getObj()) / temperature) << endl;
				if (nombreAleatoire < exp((float)(pSolution->getObj() - nouvelleSolution->getObj())/ temperature))
				{
					pSolution = nouvelleSolution;
					nombreStagnation = 0;
					if (pSolution->getObj() < dTheBestFitness) // Si am�liore meilleure solution, la garder
					{
						Smeilleur = *pSolution;
						dTheBestFitness = Smeilleur.getObj();
					}
				}
				else
				{
					nombreStagnation = nombreStagnation + 1;
				}
				temperature = temperature * delta;
			}
			cout << "i : " << i << endl;
			cout << "nombreStagnation : " << nombreStagnation << endl;
			cout << "dTheBestFitness : " << dTheBestFitness << endl;
		}
		break;



		case 4:
			// INITIALISATION DE LISTE TABOU
			vector<vector <int>> Ta;
			int N = 6;
			int index = 0;
			Ta.resize(N);
			for (int i = 0; i < N; i++) Ta[i].resize(LeProb->getN());

			// INITIALISATION DE LA SOLUTION
			Svoisin = SMSSDTSolution(LeProb->getN(), true);
			Tools::Evaluer(LeProb, Svoisin);
			Smeilleur = Svoisin;

			for (int i = 0; i < 1000; i++) {
				// TROUVER LA SOLUTION MINIMISE LA FONCTION DANS Nt(X)
				// v�rifier si Svoisin dans la liste Tabou
				do {
					Svoisin1 = SMSSDTSolution(LeProb, Svoisin);
				} while (Tools::contains(Ta, Svoisin1.Solution));

				Tools::Evaluer(LeProb, Svoisin1);
				for (int j = 0; j < 7; j++) {
					do {
						pSolution = new SMSSDTSolution(LeProb, Svoisin);
					} while (Tools::contains(Ta, pSolution->Solution));

					Tools::Evaluer(LeProb, *pSolution);
					if (pSolution->getObj() < Svoisin1.getObj()) {
						// v�rifier si psolution dans la liste Tabou
						Svoisin1 = *pSolution;
					}
				}

				if (Svoisin1.getObj() < Smeilleur.getObj()) {
					Smeilleur = Svoisin1;
				}
				Svoisin = Svoisin1;

				Ta[index % N] = Svoisin.Solution;

				index++;
				/*
				for (int k = 0; k < N; k++) {
					for (int l = 0; l < LeProb->getN(); l++) {
						cout << Ta[k][l] << " ";
					}
					cout << endl;
				}
				cout<< endl;
				*/

			}
			break;
		}
		
		
				
			
			
	
		End = clock(); // Arr�ter le clock
		Elapsed = (double(End - Start)) / CLOCKS_PER_SEC;	//Calculer le temps �coul�
		Tools::WriteReportLog(Elapsed, Smeilleur, LeProb->getNomFichier());	//Logguer le temps et la meilleure solution
		dTheBestFitness = 100000;

	}


	return 0;

}


