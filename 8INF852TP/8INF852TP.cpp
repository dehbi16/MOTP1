

#include <iostream>
#include "Tools.h"
using namespace std;


int main(int argc, char* argv[])
{
	clock_t	Start, End;	//D�claration de variable afin de calculer le temps �coul�
	double Elapsed = 0;	//Variable servant � calculer le temps �coul� (Diff�rence entre End et Start
	double	dTheBestFitness = 100000;	//Fitness de la meilleure solution
	SMSSDTProblem* LeProb;	//D�claration d'un probl�me	
	LeProb = new SMSSDTProblem(argv[2]);	//Lecture du deuxi;eme param�tre � partir de la console
	//LeProb->printOn(cout);	// Imprimer le Probl�me
	SMSSDTSolution* pSolution = NULL;	//Solution interm�diaire


	// argv[1] ex�cutions de la g�n�ration al�atoire
	for (int j = 0; j < atoi(argv[1]); j++)
	{
		Start = clock();	//D�marrer l'horloge	
		SMSSDTSolution	Smeilleur(LeProb->getN());	//Sauvegarde de la meilleure solution
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
		End = clock(); // Arr�ter le clock
		Elapsed = (double(End - Start)) / CLOCKS_PER_SEC;	//Calculer le temps �coul�
		Tools::WriteReportLog(Elapsed, Smeilleur, LeProb->getNomFichier());	//Logguer le temps et la meilleure solution
		dTheBestFitness = 100000;

	}


	return 0;

}


