#pragma once
#include <cstdint> //needed for uint32_t uint16_t etc.
/*-----------------------------------
this is a class represeting a node in the list of figures, that will be sorted by the value of the figure
contains coords
its main purpose is to find figures faster on the desk and to sort generated movements in better ways 
*/


class FigureNode
{
	
public:
	FigureNode() { figure = '7'; }
	bool add(uint32_t, uint32_t, unsigned char);
	FigureNode* next = nullptr;
	FigureNode* last = this; //only valid in the head node;
	unsigned char figureType();
	uint32_t x = 0, y = 0;//coords on the desk
	unsigned char color();
	unsigned char getFigure();
	void setFigure(unsigned char f) { figure = f; }
	~FigureNode();
private:
	unsigned char figure = '7';//figure;
};

