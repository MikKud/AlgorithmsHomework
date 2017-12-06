#pragma once
#include <iostream> 
#include "Solver.h"
class Scanner
{
public:
	Scanner();
	bool scan(Solver&, std::ostream&, std::istream&);
	~Scanner();
private:
	
};

