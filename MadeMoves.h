#pragma once
#include <cstdint>
#include <memory>
#include "TMoves.h"
#include <string>
#include "Definitions.h"
/*------------------------------
this class represents made moves, main purpose - strore solution
next - points to a move that replaces current one
child - points to a list of answers on current move from the opponent and so on

last is used to update solution if we found a better one
*/
class MadeMoves
{
public:
	MadeMoves() {
		x1 = 0, y1 = 0;//From where the figure moved;
		x2 = 0, y2 = 0;//Destination of figure movement;
		transfiguration = 0;
	}
	MadeMoves(int32_t _x1, int32_t _y1, int32_t _x2, int32_t _y2, char _trans = 0) :x1(_x1), y1(_y1), x2(_x2), y2(_y2), transfiguration(_trans) {}
	std::shared_ptr<MadeMoves> next;
	std::shared_ptr<MadeMoves> last;
	std::shared_ptr<MadeMoves> child;
	std::string stringify();
	~MadeMoves();
private:
	int32_t x1 = 0, y1 = 0;//From where the figure moved;
	int32_t x2 = 0, y2 = 0;//Destination of figure movement;
	unsigned char transfiguration = 0;
};

