//#include "stdafx.h"
#include "TMoves.h"


TMoves::TMoves()
{
	figureInTheList = nullptr;
	xFrom = 0, yFrom = 0;
	xTo = 0, yTo = 0;
	eatenFigure = 0;
	change = 0;
}

TMoves::TMoves(FigureNode* ptr, int32_t xF, int32_t yF, int32_t xT, int32_t yT, char eat, char _change) :figureInTheList(ptr), xFrom(xF), yFrom(yF), xTo(xT), yTo(yT), eatenFigure(eat), change(_change)
{}

TMoves::~TMoves()
{
}
