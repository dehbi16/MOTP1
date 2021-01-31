

#include <iostream>
#include "Tools.h"
using namespace std;


int main(int argc, char* argv[])
{
	clock_t	Start, End;	//Déclaration de variable afin de calculer le temps écoulé
	double Elapsed = 0;	//Variable servant à calculer le temps écoulé (Différence entre End et Start
	double	dTheBestFitness = 100000;	//Fitness de la meilleure solution
	SMSSDTProblem* LeProb;	//Déclaration d'un problème	
	LeProb = new SMSSDTProblem(argv[2]);	//Lecture du deuxi;eme paramètre à partir de la console
	//LeProb->printOn(cout);	// Imprimer le Problème
	SMSSDTSolution* pSolution = NULL;	//Solution intermédiaire
	int mode = 4; // 0 : methode prof. 1 : descente. 2 : VNS. 3: RS. 4 : recherche Tabou  

	// argv[1] exécutions de la génération aléatoire
	for (int j = 0; j < atoi(argv[1]); j++)
	{
		Start = clock();	//Démarrer l'horloge	
		SMSSDTSolution	Smeilleur = NULL;	//Sauvegarde de la meilleure solution
		SMSSDTSolution	Svoisin = NULL;
		SMSSDTSolution	Svoisin1 = NULL;

		switch (mode) {
		case 0:
			Smeilleur = SMSSDTSolution(LeProb->getN());
			for (int i = 0; i < 1000; i++)
			{
				pSolution = new SMSSDTSolution(LeProb->getN(), true);	//Une solution aléatoire
				Tools::Evaluer(LeProb, *pSolution);	//Évaluer la solution
				if (pSolution->getObj() < dTheBestFitness) // Si améliore meilleure solution, la garder
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
				// vérifier si Svoisin dans la liste Tabou
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
						// vérifier si psolution dans la liste Tabou
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
		
		
				
			
			
	
		End = clock(); // Arrêter le clock
		Elapsed = (double(End - Start)) / CLOCKS_PER_SEC;	//Calculer le temps écoulé
		Tools::WriteReportLog(Elapsed, Smeilleur, LeProb->getNomFichier());	//Logguer le temps et la meilleure solution
		dTheBestFitness = 100000;

	}


	return 0;

}


