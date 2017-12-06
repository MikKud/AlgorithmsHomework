// ChessV12.cpp: определяет точку входа для консольного приложения.
//
//#include "stdafx.h"
#include "PreStartTuner.h"
#include <iostream>


int main(int argc, char* argv[])
{
	PreStartTuner tuner;
	tuner.start(argc, argv);
	/*Scanner scan;
	Solver solv;
	if (!scan.scan(solv, std::cout, std::cin))
		return 0;
	int64_t result = solv.AlphaBeta(WHITE, solv.depth, MATE - solv.depth - 1, MATE - solv.depth, solv.head);
	if (result == MATE - solv.depth - 1)
	{
		std::cout << "There is no solution" << std::endl;
		return 0;
	}
	solv.printResult(std::cout);*/

	return 0;
}

