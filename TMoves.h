#pragma once
#include "FigureNode.h"
#include <memory>
/*--------------------
This class represents a move node, it points to the figure which moved, from where, its destination and so on
by this data we can fully restore the state of the desk before the move
*/
class TMoves
{
public:
	TMoves();
	TMoves(FigureNode*,int32_t, int32_t, int32_t, int32_t, char, char = 0);
	~TMoves();
	std::shared_ptr<TMoves> next;
public:
	FigureNode* figureInTheList = nullptr;
	int32_t xFrom = 0, yFrom = 0;
	int32_t xTo = 0, yTo = 0;
	char eatenFigure = 0;
	char change = 0;
};

