#include "stdafx.h"
#include "PreStartTuner.h"


PreStartTuner::PreStartTuner()
{
}
void PreStartTuner::start(int argv, char* argc[])
{
	if (argv != 3) 
	{
		std::cout << "Wrong Arguments" << std::endl;
		return;
	}
	fin.open(argc[1]);
	if (!fin.is_open())
	{
		std::cout << "Cant't open file for reading" << std::endl;
		return;
	}
	fout.open(argc[2], std::ios_base::trunc | std::ios_base::out);
	if (!fout.is_open())
	{
		std::cout << "Can't open or create file for writing" << std::endl;
		return;
	}
	if (!scan.scan(solv, fout, fin))
		return;
	int64_t result = solv.AlphaBeta(WHITE, solv.depth, MATE - solv.depth - 1, MATE - solv.depth, solv.head);
	if (result == MATE - solv.depth - 1)
	{
		fout << "There is no solution" << std::endl;
		return;
	}
	solv.printResult(fout);
	fout.close();
	fin.close();
}

PreStartTuner::~PreStartTuner()
{
}
