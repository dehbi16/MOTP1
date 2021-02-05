#ifndef __SMSSDTPROBLEM_CPP
#define __SMSSDTPROBLEM_CPP


#include "SMSSDTSolution.h"
#include "Tools.h"

#include <cmath>
#include <conio.h>
using namespace std;

/**
 * SMSSDTSolution()
 * Constructeur de la classe
 * Assigne les valeurs aléatoires initiales à la solution
**/

SMSSDTSolution::SMSSDTSolution(int N, bool test) {
	// scheduling vector
	Solution.resize(N);
	TT.resize(N);
	CT.resize(N);
	ST.resize(N);
	// initialisation of possible values
	vector < int > possibles(N);
	for (int i = 0; i < N; i++)
		possibles[i] = i;
	// random initialization
	int rInd;              // random index
	for (int i = 0; i < N; i++)
	{
		rInd = (int)(((double)rand() / ((double)RAND_MAX + 1.0)) * (N - i));//rng.uniform (N - i);

		//	1/(int)(((double)rand() / ((double)RAND_MAX + 1.0)) * (1000 - 0))
		// exp(obj_meilleur - min)/temp
		Solution[i] = possibles[rInd];
		possibles[rInd] = possibles[N - i - 1];
	}
}

SMSSDTSolution::SMSSDTSolution(SMSSDTProblem* LeProb, SMSSDTSolution& Sol) {
	this->setObj(-1);
	int N = LeProb->getN();
	Solution.resize(N);
	TT.resize(N);
	CT.resize(N);
	ST.resize(N);
	vector<int> s = Sol.Solution;
	Tools::swapMove(Sol.Solution, Solution, N);

}

// CONSTRUCTEUR REGLE EDD
SMSSDTSolution::SMSSDTSolution(SMSSDTProblem* LeProb) {
	this->setObj(-1);
	int N = LeProb->getN();
	std::vector < int > d(N), p(N);
	d = LeProb->getD();
	Solution.resize(N);
	TT.resize(N);
	CT.resize(N);
	ST.resize(N);
	for (int k = 0; k < N; k++) Solution[k] = k;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			if (d[Solution[i]] > d[Solution[j]]) {
				int x = Solution[i];
				Solution[i] = Solution[j];
				Solution[j] = x;
			}
		}
	}
}


/**
 * Save
 * Fonction permettant d'écrire la solution dans un flux
 * @param Stream : Flux dans lequel on doit écrire
**/
ostream& SMSSDTSolution::Save(ostream& Stream) {

	for (int i = 0; i < (int)Solution.size(); i++) {
		Stream << Solution[i] << " ";

	}

	return Stream;
}


SMSSDTSolution::~SMSSDTSolution() {
	Solution.clear();
}

void SMSSDTSolution::inverse(int N, int a, int b) {
	if (a != b) {
		if (a > b) {
			int x = a;
			a = b;
			b = x;
		}
		int d = b - a;
		for (int i = 1; i < d / 2; i++) {
			int x = Solution[i + a];
			Solution[i + a] = Solution[-i + b];
			Solution[-i + b] = x;

		}
	}
}

void SMSSDTSolution::opt(int N, int a, int b) {



	int x = Solution[a];
	Solution[a] = Solution[b];
	Solution[b] = x;
	//inverse(N, a, b);

}




#endif  //__SMSSDTPROBLEM_CPP