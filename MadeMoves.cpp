//#include "stdafx.h"
#include "MadeMoves.h"


std::string MadeMoves::stringify()
{
	std::string result;
	//x = c - 97;
	//y = c - 49;
	result.push_back(x1 + 97);
	result.push_back(y1 + 49);
	result.push_back(' ');
	result.push_back(x2 + 97);
	result.push_back(y2 + 49); 
	result.push_back(' ');
	switch (FIGURE(transfiguration))
	{
	case(KING):
		result.push_back('k');
		break;
	case(QUEEN):
		result.push_back('q');
		break;
	case(ROOK):
		result.push_back('r');
		break;
	case(BISHOP):
		result.push_back('b');
		break;
	case(KNIGHT):
		result.push_back('kn');
		break;
	case(PAWN):
		result.push_back('p');
		break;
	default:
		break;
	}
	return result;
}
MadeMoves::~MadeMoves()
{
}
