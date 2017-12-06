#pragma once
#include "Scanner.h"
#include "Solver.h"
#include <fstream>
#include <istream>
#include <iomanip>

class PreStartTuner
{
public:
	PreStartTuner();
	void start(int argv, char* argc[]);
	~PreStartTuner();
private:
	Scanner scan;
	Solver solv;
	std::ofstream fout;
	std::ifstream fin;
};

