

#include <iostream>
#include "Tools.h"
using namespace std;

void voi(SMSSDTSolution& Sol, SMSSDTProblem* LeProb) { // fonction qui parcours le voisinage 
	double	dTheBestFitness = 100000;
	SMSSDTSolution	Smeilleur(LeProb->getN());

	for (int i = 0; i < LeProb->getN(); i++) {
		for (int j = i + 1; j < LeProb->getN(); j++) {

			Sol.opt(LeProb->getN(), i, j);
			Tools::Evaluer(LeProb, Sol);
			//Évaluer la solution
			if (Sol.getObj() < dTheBestFitness) // Si améliore meilleure solution, la garder
			{
				Smeilleur = Sol;
				dTheBestFitness = Smeilleur.getObj();

			}
			Sol.opt(LeProb->getN(), i, j);

			//S = Sol;
		}
	}
	Sol = Smeilleur;
}
void desente(SMSSDTProblem* LeProb, SMSSDTSolution* pSolution, SMSSDTSolution& Smeilleur) {


	double	dTheBestFitness = 100000;	//Fitness de la meilleure solution



	int e = 0;
	Tools::Evaluer(LeProb, *pSolution);	//Évaluer la solution
	dTheBestFitness = pSolution->getObj();
	while (e == 0) {
		SMSSDTSolution	sol(LeProb->getN());
		sol = *pSolution;
		voi(sol, LeProb);



		if (sol.getObj() < dTheBestFitness) // Si la meilleur sol du voisinage  améliore meilleure solution, la garder
		{
			*pSolution = sol;
			dTheBestFitness = pSolution->getObj();
		}
		else {
			e = 1;

		}

		//Logguer le temps et la meilleure solution
	}
	Smeilleur = *pSolution;
	//End = clock(); // Arrêter le clock
	//Elapsed = (double(End - Start)) / CLOCKS_PER_SEC;	//Calculer le temps écoulé
	//Tools::WriteReportLog(Elapsed, *pSolution, LeProb->getNomFichier());
}

void shaking(SMSSDTProblem* LeProb, vector<int> const& solution, vector<int>& voisins, int mode) {
	switch (mode) {
	case 0:
		Tools::swapMove(solution, voisins, LeProb->getN());
		break;
	case 1:
		Tools::insertionMove(solution, voisins, LeProb->getN());
		break;
	case 2:

		Tools::EDDMove(solution, voisins, LeProb->getD(), LeProb->getN(), rand() % (LeProb->getN()));
		break;
	case 4:

		Tools::scrambleMove(solution, voisins, LeProb->getN(), rand() % (LeProb->getN()));
		break;
	case 5:
		Tools::inversionMove(solution, voisins, LeProb->getN(), rand() % (LeProb->getN()));

		break;
	}
}

int main(int argc, char* argv[])
{
	clock_t	Start, End;	//D�claration de variable afin de calculer le temps �coul�
	double Elapsed = 0;	//Variable servant � calculer le temps �coul� (Diff�rence entre End et Start
	double	dTheBestFitness = 100000;	//Fitness de la meilleure solution
	SMSSDTProblem* LeProb;	//D�claration d'un probl�me	
	LeProb = new SMSSDTProblem(argv[2]);	//Lecture du deuxi;eme param�tre � partir de la console
	//LeProb->printOn(cout);	// Imprimer le Probl�me
	SMSSDTSolution* pSolution = NULL;	//Solution interm�diaire
	int mode = 2; // 0 : methode prof. 1 : descente. 2 : VNS. 3: RS. 4 : recherche Tabou  
	int amelioration = 0;
	int m = 0;
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
			pSolution = new SMSSDTSolution(LeProb->getN(), true);
			desente(LeProb, pSolution, Smeilleur);
			break;
		case 2:
			//desente(LeProb, Smeilleur);

			//Une solution aléatoire
			pSolution = new SMSSDTSolution(LeProb->getN(), true);

			//pSolution = new SMSSDTSolution(LeProb); // solution par heuristique EDD
			Tools::Evaluer(LeProb, *pSolution);

			Svoisin = *pSolution;
			Svoisin1 = *pSolution;
			amelioration = 0;

			while (amelioration < 50) {
				shaking(LeProb, pSolution->Solution, Svoisin.Solution, m);
				desente(LeProb, &Svoisin, Svoisin1);

				if (Svoisin1.getObj() < pSolution->getObj()) {
					*pSolution = Svoisin1;
					m = -1;
					amelioration = 0;
				}
				else {
					amelioration++;
				}
				if (m < 5) {
					m++;
				}
				else { m = 0; }
				if (pSolution->getObj() == 0) {
					break;
				}
			}
			amelioration = 0;
			Smeilleur = *pSolution;
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
			//cout << "i : " << i << endl;
			//cout << "nombreStagnation : " << nombreStagnation << endl;
			//cout << "dTheBestFitness : " << dTheBestFitness << endl;
		}
		break;



		case 4:
			// INITIALISATION DE LISTE TABOU
			vector<vector <int>> Ta;
			int N = 10;
			int index = 0;
			Ta.resize(N);
			for (int i = 0; i < N; i++) Ta[i].resize(LeProb->getN());

			// INITIALISATION DE LA SOLUTION
			Svoisin = SMSSDTSolution(LeProb->getN(), true);
			Tools::Evaluer(LeProb, Svoisin);
			Smeilleur = Svoisin;
			int nbiter = 0;
			while (nbiter < 100) {
				nbiter++;
				// TROUVER LA SOLUTION MINIMISE LA FONCTION DANS Nt(X)
				// v�rifier si Svoisin dans la liste Tabou
				do {
					Svoisin1 = SMSSDTSolution(LeProb, Svoisin);
				} while (Tools::contains(Ta, Svoisin1.Solution));

				Tools::Evaluer(LeProb, Svoisin1);
				for (int j = 0; j < 30; j++) {
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
					nbiter = 0;
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


