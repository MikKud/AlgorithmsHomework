//#include "stdafx.h"
#include "FigureNode.h"
#include "Definitions.h"


bool FigureNode::add(int32_t _x, int32_t _y, unsigned char _figure) {
	if (FIGURE(last->figure) < FIGURE(_figure))//figures must go down on their value
		return false;
	last->x = _x;
	last->y = _y;
	last->figure = _figure;
	last->next = new FigureNode();
	last = last->next;
	return true;
}
unsigned char FigureNode::figureType()
{
	return FIGURE(figure);
}

unsigned char FigureNode::color()
{
	return COLOR(figure);
}


unsigned char FigureNode::getFigure()
{
	return figure;
}

FigureNode::~FigureNode()
{
}
