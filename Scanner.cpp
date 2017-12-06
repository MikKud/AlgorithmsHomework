//#include "stdafx.h"
#include "Scanner.h"


Scanner::Scanner(){}
/*----------------------------
first we get depth - how many steps we have on white chess in common sence to solve the problem
then we begin to scan figures
e2Q - meens QUEEN On e2
counter - represents position of the letter in a word
position zero - letters from e to h are possible
position one - letters representing numbers from 1 to 8
position second - letters representing first capital letters of the figures
to make KING and KNIGHT different - to enter KNIGHT you have to go for first to letters like "e2KN"
if everything went ok - return true
otherwise fallse
*/
bool Scanner::scan(Solver& solv, std::ostream& os/*cout*/, std::istream& is/*file*/){
	uint32_t depth = 0;
	is >> depth;
	if (depth == 0 || depth > 6){
		switch (depth){
		case(0):
			os << "The mate must be already on the table" << std::endl;
			break;
		default:
			os << "Sorry the task is too diffcult, it will take me a while to do it, try another task" << std::endl;
			break;
		}
		return false;
	}
	solv.setDepth(depth);
	char c;
	is.read(&c, 1);
	uint16_t counter = 0;
	char COLOR = WHITE;
	uint32_t x = 0, y = 0;//coords
	char figure;
	while (true) {//ASCII encoding for letters
		if (!is.good())
		{
			os << "You entered something wrong, error occured on entering, probably forgot new line in the end of the file " << std::endl;
			return false;
		}
		is.read(&c, 1);
		if (c == ' '){
			if (counter == 3) {
				if (!solv.addFigure(x, y, figure)) {
					os << "Error on adding figure to the desk " << std::endl;
					return false;
				}
				counter = 0;
			}
			else {
				os << "Error in input with spaces" << std::endl;
				return false;
			}
		}
		else if (c == '\n'){
			if (counter == 3) {
				if (!solv.addFigure(x, y, figure)) {
					os << "Error on adding figure to the desk " << std::endl;
					return false;
				}
				if (COLOR == BLACK)
					return true;
				COLOR = BLACK;
				counter = 0;
			}
			else {
				os << "Error in input with begining a new line" << std::endl;
				return false;
			}
		}
		else {
			if (counter == 0) {
				if (c <= 104 && c >= 97) { //between 'a' and 'h'
					x = c - 97;
					counter++;
					continue;
				}
				else{
					os << "You entered something wrong, error occured on entering " << c << std::endl;
					return false;
				}
			}
			else if (counter == 1) {
				if (c <= 56 && c >= 49) {  //between '1' and '8'
					y = c - 49;
					counter++;
					continue;
				}
				else {
					os << "You entered something wrong, error occured on entering " << c << std::endl;
					return false;
				}
			}
			else if (counter == 2) {
				switch (c){
				case('Q'):
					figure = QUEEN | COLOR;
					counter++;
					break;
				case('R'):
					figure = ROOK | COLOR;
					counter++;
					break;
				case('B'):
					figure = BISHOP | COLOR;
					counter++;
					break;
				case('P'):
					figure = PAWN | COLOR;
					counter++;
					break;
				case('K'): {
					is.read(&c, 1);
					if (c == ' ') {
						figure = KING | COLOR;
						if (!solv.addFigure(x, y, figure)) {
							os << "Error on adding figure to the desk " << std::endl;
							return false;
						}
						counter = 0;
					}
					 else if (c == '\n') {
						figure = KING | COLOR;
						if (!solv.addFigure(x, y, figure)) {
							os << "Error on adding figure to the desk " << std::endl;
							return false;
						}
						if (COLOR == BLACK)
							return true;
						COLOR = BLACK;
						counter = 0;
					}
					 else if (c == 'N') {
						 figure = KNIGHT | COLOR;
						 counter++;
					}
					 break;
				}
				default:
					os << "You entered something wrong, error occured on entering " << c << std::endl;
					return false;
				}
			}
		}
	}
}

Scanner::~Scanner() {}
