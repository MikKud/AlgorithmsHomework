//#include "stdafx.h"
#include "Solver.h"
#include <tuple>
#include <iostream>

Solver::Solver()
{
	for (uint32_t i = 0; i < 8; i++)
		for (uint32_t j = 0; j < 8; j++)
			desk[i][j] = 0;

	whiteList = new FigureNode;
	blackList = new FigureNode;
}

void Solver::setDepth(uint32_t _depth) {//we include not only white moves, but also black moves + king eating;
	depth = 2 * _depth + 1;
}

bool Solver::addFigure(uint32_t x, uint32_t y, unsigned char figure) {
	if (desk[x][y]) // if there is already something on the desk on the needed place - something went wrong
		return false;
	desk[x][y] = figure;
	if (COLOR(figure) == WHITE) {
		return whiteList->add(x, y, figure);
	}
	else{
		return blackList->add(x, y, figure);
	}

}

std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> Solver::GenerateAllMoves(unsigned char color, uint32_t Depth) //second data n return value zero => only one move - eat king
{
	std::shared_ptr<TMoves> move;
	std::shared_ptr<TMoves> last;
	if (COLOR(color) == BLACK) {
		FigureNode* ptr = blackList;
		std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> thisAndLast = GenerateMove(ptr);
		move = std::get<0>(thisAndLast);
		if (move && FIGURE(move->eatenFigure) == KING)
			return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(move, std::shared_ptr<TMoves>());
		last = std::get<1>(thisAndLast);
		ptr = ptr->next;
		while (ptr != blackList->last)
		{
			std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> thisAndLastLocal = GenerateMove(ptr);
			if (std::get<0>(thisAndLastLocal)) //check for not empty moves
			{
				if (FIGURE(std::get<0>(thisAndLastLocal)->eatenFigure) == KING)
				{
					move = std::get<0>(thisAndLastLocal);
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(move, std::shared_ptr<TMoves>());
				}
				last->next = std::make_shared<TMoves>(TMoves());
				*last->next = *std::get<0>(thisAndLastLocal);
				last = std::get<1>(thisAndLastLocal);
			}
			ptr = ptr->next;
		}
		//if (Depth == 2)
			//last->next = std::make_shared<TMoves>(TMoves(blackList, blackList->x, blackList->y, blackList->x, blackList->y, 0));
	}
	else {
		FigureNode* ptr = whiteList;
		std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> thisAndLast = GenerateMove(ptr);
		move = std::get<0>(thisAndLast);
		if (move && FIGURE(move->eatenFigure) == KING)
			return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(move, std::shared_ptr<TMoves>());
		last = std::get<1>(thisAndLast);
		ptr = ptr->next;
		while (ptr != whiteList->last)
		{
			std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> thisAndLastLocal = GenerateMove(ptr);
			if (std::get<0>(thisAndLastLocal))//check for not empty moves
			{
				if (FIGURE(std::get<0>(thisAndLastLocal)->eatenFigure) == KING)
				{
					move = std::get<0>(thisAndLastLocal);
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(move, std::shared_ptr<TMoves>());
				}
				last->next = std::make_shared<TMoves>(TMoves());
				*last->next = *std::get<0>(thisAndLastLocal);
				last = std::get<1>(thisAndLastLocal);
			}
			ptr = ptr->next;
		}
		//if (Depth == 2)
			//last->next = std::make_shared<TMoves>(TMoves(whiteList, whiteList->x, whiteList->y, whiteList->x, whiteList->y, 0));
	}
	return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(move, last);
}

std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> Solver::GenerateMove(FigureNode* fPtr)
{
	std::shared_ptr<TMoves> first = std::make_shared<TMoves>(TMoves());
	std::shared_ptr<TMoves> last = std::make_shared<TMoves>(TMoves());
	last->next = first;
	if(fPtr->getFigure() != desk[fPtr->x][fPtr->y])
		return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);

	switch (fPtr->figureType())
	{
	case KING:
		return GenerateMoveKing(fPtr);
	case QUEEN:
		return GenerateMoveQueen(fPtr);
	case ROOK:
		return GenerateMoveRook(fPtr);
	case BISHOP:
		return GenerateMoveBishop(fPtr);
	case KNIGHT:
		return GenerateMoveKnight(fPtr);
	case PAWN:
		return GenerateMovePawn(fPtr);
	default:
		break;
	}
}

std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> Solver::GenerateMoveKing(FigureNode* fPtr)
{
	std::shared_ptr<TMoves> first = std::make_shared<TMoves>(TMoves());
	std::shared_ptr<TMoves> temp = first;
	std::shared_ptr<TMoves> last = std::make_shared<TMoves>(TMoves());
	last->next = first;

	for (int32_t i = -1; i <= 1; i++)
	{
		if (int32_t(fPtr->x) + i < 0 || int32_t(fPtr->x) + i>7) //out of desk by x - next x
			continue;
		for (int32_t j = -1; j <= 1; j++)
		{
			if (int32_t(fPtr->y) + j < 0 || int32_t(fPtr->y) + j>7) //out of desk by y
				continue;
			if (desk[fPtr->x + i][fPtr->y + j])
			{
				if (COLOR(desk[fPtr->x + i][fPtr->y + j]) == fPtr->color()) // if on desk figure of the same color - cant move there
					continue;
				else { // otherwise we can eat it
					temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y + j, desk[fPtr->x + i][fPtr->y + j]));
					last = temp->next;
					temp = temp->next;
					if (FIGURE(desk[fPtr->x + i][fPtr->y + j]) == KING)
					{
						first->next = last;
						return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
					}
				}
			}
			else {
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y + j, 0));
				last = temp->next;
				temp = temp->next;
			}
		}
	}
	return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
}

std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> Solver::GenerateMoveQueen(FigureNode* fPtr)
{
	std::shared_ptr<TMoves> first = std::make_shared<TMoves>(TMoves());
	std::shared_ptr<TMoves> temp = first;
	std::shared_ptr<TMoves> last = std::make_shared<TMoves>(TMoves());
	last->next = first;

	for (int32_t i = fPtr->x - 1; i >= 0; i--) // y - doesnt change, x - decrease
	{
		if (desk[i][fPtr->y])
		{
			if (COLOR(desk[i][fPtr->y]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, fPtr->y, desk[i][fPtr->y]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][fPtr->y]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y,  i, fPtr->y, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t i = fPtr->x + 1; i <= 7; i++) // y - doesnt change, x - increase
	{
		if (desk[i][fPtr->y])
		{
			if (COLOR(desk[i][fPtr->y]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, fPtr->y, desk[i][fPtr->y]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][fPtr->y]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, fPtr->y, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t j = fPtr->y - 1; j >= 0; j--) // x - doesnt change, y - decrease
	{
		if (desk[fPtr->x][j])
		{
			if (COLOR(desk[fPtr->x][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x, j, desk[fPtr->x][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[fPtr->x][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t j = fPtr->y +1 ; j <= 7; j++ ) // x - doesnt change, y - increase
	{
		if (desk[fPtr->x][j])
		{
			if (COLOR(desk[fPtr->x][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x, j, desk[fPtr->x][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[fPtr->x][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t i = fPtr->x - 1, j = fPtr->y - 1; i >= 0 && j >= 0; i--, j--) //x - decreasing, y-decreasing
	{
		if (desk[i][j])
		{
			if (COLOR(desk[i][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, desk[i][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t i = fPtr->x + 1, j = fPtr->y + 1; i <= 7 && j <= 7; i++, j++) //x - increasing, y-increasing
	{
		if (desk[i][j])
		{
			if (COLOR(desk[i][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, desk[i][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t i = fPtr->x - 1, j = fPtr->y + 1; i >= 0 && j <= 7; i--, j++) //x - decreasing, y-increasing
	{
		if (desk[i][j])
		{
			if (COLOR(desk[i][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, desk[i][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t i = fPtr->x + 1, j = fPtr->y - 1; i <= 7 && j >= 0; i++, j--) //x - decreasing, y-decreasing
	{
		if (desk[i][j])
		{
			if (COLOR(desk[i][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, desk[i][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
}

std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> Solver::GenerateMoveRook(FigureNode* fPtr)
{
	std::shared_ptr<TMoves> first = std::make_shared<TMoves>(TMoves());
	std::shared_ptr<TMoves> temp = first;
	std::shared_ptr<TMoves> last = std::make_shared<TMoves>(TMoves());
	last->next = first;

	for (int32_t i = fPtr->x - 1; i >= 0; i--) // y - doesnt change, x - decrease
	{
		if (desk[i][fPtr->y])
		{
			if (COLOR(desk[i][fPtr->y]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, fPtr->y, desk[i][fPtr->y]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][fPtr->y]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, fPtr->y, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t i = fPtr->x + 1; i <= 7; i++) // y - doesnt change, x - increase
	{
		if (desk[i][fPtr->y])
		{
			if (COLOR(desk[i][fPtr->y]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, fPtr->y, desk[i][fPtr->y]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][fPtr->y]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, fPtr->y, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t j = fPtr->y - 1; j >= 0; j--) // x - doesnt change, y - decrease
	{
		if (desk[fPtr->x][j])
		{
			if (COLOR(desk[fPtr->x][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x, j, desk[fPtr->x][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[fPtr->x][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t j = fPtr->y + 1; j <= 7; j++) // x - doesnt change, y - increase
	{
		if (desk[fPtr->x][j])
		{
			if (COLOR(desk[fPtr->x][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x, j, desk[fPtr->x][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[fPtr->x][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
}

std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> Solver::GenerateMoveBishop(FigureNode* fPtr)
{
	std::shared_ptr<TMoves> first = std::make_shared<TMoves>(TMoves());
	std::shared_ptr<TMoves> temp = first;
	std::shared_ptr<TMoves> last = std::make_shared<TMoves>(TMoves());
	last->next = first;

	for (int32_t i = fPtr->x - 1, j = fPtr->y - 1; i >= 0 && j >= 0; i--, j--) //x - decreasing, y-decreasing
	{
		if (desk[i][j])
		{
			if (COLOR(desk[i][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, desk[i][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t i = fPtr->x + 1, j = fPtr->y + 1; i <= 7 && j <= 7; i++, j++) //x - increasing, y-increasing
	{
		if (desk[i][j])
		{
			if (COLOR(desk[i][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, desk[i][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t i = fPtr->x - 1, j = fPtr->y + 1; i >= 0 && j <= 7; i--, j++) //x - decreasing, y-increasing
	{
		if (desk[i][j])
		{
			if (COLOR(desk[i][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, desk[i][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	for (int32_t i = fPtr->x + 1, j = fPtr->y - 1; i <= 7 && j >= 0; i++, j--) //x - decreasing, y-decreasing
	{
		if (desk[i][j])
		{
			if (COLOR(desk[i][j]) == fPtr->color()) // if on desk figure of the same color - cant move futher
				break;
			else
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, desk[i][j]));
				last = temp->next;
				temp = temp->next;
				if (FIGURE(desk[i][j]) == KING)
				{
					first->next = last;
					return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
				}
				break;
			}
		}
		else
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, i, j, 0));
			last = temp->next;
			temp = temp->next;
		}
	}

	return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
}

std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> Solver::GenerateMoveKnight(FigureNode* fPtr)
{
	std::shared_ptr<TMoves> first = std::make_shared<TMoves>(TMoves());
	std::shared_ptr<TMoves> temp = first;
	std::shared_ptr<TMoves> last = std::make_shared<TMoves>(TMoves());
	last->next = first;

	for (int32_t i = -2; i <= 2; i++)
	{
		if (i == 0)
			continue;
		if (!(i % 2))
		{
			if (fPtr->x + i <= 7 && fPtr->x + i >= 0 && fPtr->y + 1 <= 7) {
				if (desk[fPtr->x + i][fPtr->y + 1])
				{
					if (COLOR((desk[fPtr->x + i][fPtr->y + 1]) == fPtr->color())); // if on desk figure of the same color - cant move here
					else
					{
						temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y + 1, desk[fPtr->x + i][fPtr->y + 1]));
						last = temp->next;
						temp = temp->next;
						if (FIGURE(desk[fPtr->x + i][fPtr->y + 1]) == KING)
						{
							first->next = last;
							return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
						}
					}
				}
				else
				{
					temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y + 1, 0));
					last = temp->next;
					temp = temp->next;
				}
			}
			if (fPtr->x + i <= 7 && fPtr->x + i >= 0 && fPtr->y - 1 >= 0) {
				if (desk[fPtr->x + i][fPtr->y - 1])
				{
					if (COLOR((desk[fPtr->x + i][fPtr->y - 1]) == fPtr->color())); // if on desk figure of the same color - cant move here
					else
					{
						temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y - 1, desk[fPtr->x + i][fPtr->y - 1]));
						last = temp->next;
						temp = temp->next;
						if (FIGURE(desk[fPtr->x + i][fPtr->y - 1]) == KING)
						{
							first->next = last;
							return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
						}
					}
				}
				else
				{
					temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y - 1, 0));
					last = temp->next;
					temp = temp->next;
				}
			}
		}//if (!(i%2))
		else
		{
			if (fPtr->x + i <= 7 && fPtr->x + i >= 0 && fPtr->y + 2 <= 7) {
				if (desk[fPtr->x + i][fPtr->y + 2])
				{
					if (COLOR((desk[fPtr->x + i][fPtr->y + 2]) == fPtr->color())); // if on desk figure of the same color - cant move here
					else
					{
						temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y + 2, desk[fPtr->x + i][fPtr->y + 2]));
						last = temp->next;
						temp = temp->next;
						if (FIGURE(desk[fPtr->x + i][fPtr->y + 2]) == KING)
						{
							first->next = last;
							return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
						}
					}
				}
				else
				{
					temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y + 2, 0));
					last = temp->next;
					temp = temp->next;
				}
			}
			if (fPtr->x + i <= 7 && fPtr->x + i >= 0 && fPtr->y - 2 >= 0) {
				if (desk[fPtr->x + i][fPtr->y - 2])
				{
					if (COLOR((desk[fPtr->x + i][fPtr->y - 1]) == fPtr->color())); // if on desk figure of the same color - cant move here
					else
					{
						temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y - 2, desk[fPtr->x + i][fPtr->y - 2]));
						last = temp->next;
						temp = temp->next;
						if (FIGURE(desk[fPtr->x + i][fPtr->y - 2]) == KING)
						{
							first->next = last;
							return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
						}
					}
				}
				else
				{
					temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y - 2, 0));
					last = temp->next;
					temp = temp->next;
				}
			}
		}
	}//for
	return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
}

std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> Solver::GenerateMovePawn(FigureNode* fPtr)
{
	std::shared_ptr<TMoves> first = std::make_shared<TMoves>(TMoves());
	std::shared_ptr<TMoves> temp = first;
	std::shared_ptr<TMoves> last = std::make_shared<TMoves>(TMoves());
	last->next = first;

	if (fPtr->color() == BLACK)
	{
		for (int32_t i = -1; i <= 1; i++)
		{
			if (fPtr->y == 0)
				break;
			if (fPtr->y == 1)
			{
				if (((i % 2 && desk[fPtr->x + i][fPtr->y - 1] && (COLOR(desk[fPtr->x + i][fPtr->y - 1]) == WHITE)) || (!(i % 2) && !desk[fPtr->x][fPtr->y - 1])) && fPtr->x + i <= 7 && fPtr->x + i >= 0)
				{
					for (uint8_t j = 2; j <= 6; j++)
					{
						temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y - 1, desk[fPtr->x + i][fPtr->y - 1],j));
						last = temp->next;
						temp = temp->next;
					}
				}
			}
			else if (((i % 2 && desk[fPtr->x+i][fPtr->y - 1] && (COLOR(desk[fPtr->x + i][fPtr->y - 1])==WHITE)) || ( !(i%2) && !desk[fPtr->x][fPtr->y - 1] )) && fPtr->x+i<=7 && fPtr->x + i >= 0)
			{
				temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x+i , fPtr->y - 1, desk[fPtr->x + i][fPtr->y - 1]));
				last = temp->next;
				temp = temp->next;
			}
		}
		if (fPtr->y == 6 && !desk[fPtr->x][5] && !desk[fPtr->x][4])
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x, fPtr->y -2, 0));
			last = temp->next;
			temp = temp->next;
		}
	}
	else
	{
		for (int32_t i = -1; i <= 1; i++)
		{
			if (fPtr->y == 7)
				break;
			if (fPtr->y == 6)
			{
				if (((i % 2 && desk[fPtr->x + i][fPtr->y + 1] && (COLOR(desk[fPtr->x + i][fPtr->y + 1]) == BLACK)) || (!(i % 2) && !desk[fPtr->x][fPtr->y + 1])) && fPtr->x + i <= 7 && fPtr->x + i >= 0)
				{
					for (uint8_t j = 2; j <= 6; j++)
					{
						temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y + 1, desk[fPtr->x + i][fPtr->y + 1], j));
						last = temp->next;
						temp = temp->next;
					}
				}
			}
			else if (((i % 2 && desk[fPtr->x + i][fPtr->y + 1] && (COLOR(desk[fPtr->x + i][fPtr->y + 1]) == BLACK)) || (!(i % 2) && !desk[fPtr->x][fPtr->y + 1])) && fPtr->x + i <= 7 && fPtr->x + i >= 0)
			{
					temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x + i, fPtr->y + 1, desk[fPtr->x + i][fPtr->y + 1]));
					last = temp->next;
					temp = temp->next;
			}
		}
		if (fPtr->y == 1 && !desk[fPtr->x][2] && !desk[fPtr->x][3])
		{
			temp->next = std::make_shared<TMoves>(TMoves(fPtr, fPtr->x, fPtr->y, fPtr->x, fPtr->y + 2, 0));
			last = temp->next;
			temp = temp->next;
		}
	}
	return std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>>(first->next, last);
}

void Solver::MakeMove(std::shared_ptr<TMoves> movePtr)
{
	desk[movePtr->xTo][movePtr->yTo] = desk[movePtr->xFrom][movePtr->yFrom];
	if(movePtr->xFrom!=movePtr->xTo || movePtr->yFrom != movePtr->yTo) // check for the "pass" move
		desk[movePtr->xFrom][movePtr->yFrom] = 0;
	movePtr->figureInTheList->x = movePtr->xTo;
	movePtr->figureInTheList->y = movePtr->yTo;
	if (movePtr->change)
	{
		desk[movePtr->xTo][movePtr->yTo] = COLOR(desk[movePtr->xTo][movePtr->yTo]) | movePtr->change;
		movePtr->figureInTheList->setFigure(desk[movePtr->xTo][movePtr->yTo]);
	}
}

void Solver::UnMakeMove(std::shared_ptr<TMoves> movePtr)
{
	desk[movePtr->xFrom][movePtr->yFrom] = desk[movePtr->xTo][movePtr->yTo];
	if (movePtr->xFrom != movePtr->xTo || movePtr->yFrom != movePtr->yTo)
		desk[movePtr->xTo][movePtr->yTo] = movePtr->eatenFigure;
	movePtr->figureInTheList->x = movePtr->xFrom;
	movePtr->figureInTheList->y = movePtr->yFrom;
	if (movePtr->change)
	{
		desk[movePtr->xFrom][movePtr->yFrom] = COLOR(desk[movePtr->xFrom][movePtr->yFrom]) | PAWN;
		movePtr->figureInTheList->setFigure(desk[movePtr->xFrom][movePtr->yFrom]);
	}
}

bool Solver::AttackKing(FigureNode* whoAtack, int32_t x, int32_t y)
{
	if (whoAtack->x == x && whoAtack->y == y)
		return false;
	return std::abs(int32_t(whoAtack->x) - x) <= 1 && std::abs(int32_t(whoAtack->y) - y) <= 1;
}

bool Solver::AttackQueen(FigureNode* whoAtack, int32_t x, int32_t y)
{
	if (whoAtack->x == x && whoAtack->y == y)
		return false;
	if (x + y == whoAtack->x + whoAtack->y)
	{
		for (int i = -((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)), j = ((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)); ; i = i - ((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)), j = j + ((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)))
		{
			if (desk[whoAtack->x + i][whoAtack->y + j] && (whoAtack->x + i != x) && (whoAtack->y + j != y))
				return false;
			if ((whoAtack->x + i == x) && (whoAtack->y + j == y))
				return true;
		}
	}

	if (x - y == int32_t(whoAtack->x) - int32_t(whoAtack->y))
	{
		for (int i = -((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)), j = -((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)); ; i = i - ((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)), j = j - ((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)))
		{
			if (desk[whoAtack->x + i][whoAtack->y + j] && (whoAtack->x + i != x) && (whoAtack->y + j != y))
				return false;
			if ((whoAtack->x + i == x) && (whoAtack->y + j == y))
				return true;
		}
	}

	if (whoAtack->x == x)
	{
		for (int j = ((y - whoAtack->y) / std::abs(int32_t(whoAtack->y) - y));; j = j + ((y - whoAtack->y) / std::abs(int32_t(whoAtack->y) - y)))
		{
			if (desk[whoAtack->x][whoAtack->y + j] && (whoAtack->y + j != y))
				return false;
			if ((whoAtack->x == x) && (whoAtack->y + j == y))
				return true;
		}
	}

	if (whoAtack->y == y)
	{
		for (int i = ((x - whoAtack->x) / std::abs(int32_t(whoAtack->x) - x));; i = i + ((x - whoAtack->x) / std::abs(int32_t(whoAtack->x) - x)))
		{
			if (desk[whoAtack->x + i][whoAtack->y] && (whoAtack->x + i != x))
				return false;
			if ((whoAtack->x + i == x) && (whoAtack->y == y))
				return true;
		}
	}
	return false;
}

bool Solver::AttackRook(FigureNode* whoAtack, int32_t x, int32_t y)
{
	if (whoAtack->x == x && whoAtack->y == y)
		return false;
	if (whoAtack->x == x)
	{
		for (int j = ((y - whoAtack->y) / std::abs(int32_t(whoAtack->y) - y));; j = j + ((y - whoAtack->y) / std::abs(int32_t(whoAtack->y) - y)))
		{
			if (desk[whoAtack->x][whoAtack->y + j] && (whoAtack->y + j != y))
				return false;
			if ((whoAtack->x == x) && (whoAtack->y + j == y))
				return true;
		}
	}

	if (whoAtack->y == y)
	{
		for (int i = ((x - whoAtack->x) / std::abs(int32_t(whoAtack->x) - x));; i = i + ((x - whoAtack->x) / std::abs(int32_t(whoAtack->x) - x)))
		{
			if (desk[whoAtack->x + i][whoAtack->y] && (whoAtack->x + i != x))
				return false;
			if ((whoAtack->x + i == x) && (whoAtack->y == y))
				return true;
		}
	}
	return false;
}

bool Solver::AttackBishop(FigureNode* whoAtack, int32_t x, int32_t y)
{
	if (whoAtack->x == x && whoAtack->y == y)
		return false;
	if (x + y == whoAtack->x + whoAtack->y)
	{
		for (int i = -((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)), j = ((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)); ; i = i - ((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)), j = j + ((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)))
		{
				if (desk[whoAtack->x + i][whoAtack->y + j] && (whoAtack->x + i != x) && (whoAtack->y + j != y))
					return false;
				if ((whoAtack->x + i == x) && (whoAtack->y + j == y))
					return true;
		}
	}

	if (x - y == int32_t(whoAtack->x) - int32_t(whoAtack->y))
	{
		for (int i = -((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)),  j = -((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)); ; i = i - ((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)), j = j - ((whoAtack->x - x) / std::abs(int32_t(whoAtack->x) - x)))
		{
				if (desk[whoAtack->x + i][whoAtack->y + j] && (whoAtack->x + i != x) && (whoAtack->y + j != y))
					return false;
				if ((whoAtack->x + i == x) && (whoAtack->y + j == y))
					return true;
		}
	}
	return false;
}

bool Solver::AttackKnight(FigureNode* whoAtack, int32_t x, int32_t y)
{
	if (whoAtack->x == x && whoAtack->y == y)
		return false;
	if ((std::abs(int32_t(whoAtack->x) - x) == 2 && std::abs(int32_t(whoAtack->y) - y) == 1) || (std::abs(int32_t(whoAtack->x) - x) == 1 && std::abs(int32_t(whoAtack->y) - y) == 2))
		return true;
	return false;
}

bool Solver::AttackPawn(FigureNode* whoAtack, int32_t x, int32_t y)
{
	if (whoAtack->x == x && whoAtack->y == y)
		return false;
	if (COLOR(whoAtack->color()) == BLACK)
	{
		return (std::abs(int32_t(whoAtack->x) - x) == 1 && int32_t(whoAtack->y) - y == 1);
	}
	else
	{
		return (std::abs(int32_t(whoAtack->x) - x) == 1 && int32_t(whoAtack->y) - y == -1);
	}
}


bool Solver::underAttack(FigureNode* whoAtack, int32_t x, int32_t y) // possible to optimise
{
	auto last = whoAtack->last;
	bool result = false;
	while (whoAtack!=last && !result) {
		if (whoAtack->getFigure() == desk[whoAtack->x][whoAtack->y])
		{
			switch (whoAtack->figureType())
			{
			case KING:
				result = result || AttackKing(whoAtack, x, y);
				break;
			case QUEEN:
				result = result || AttackQueen(whoAtack, x, y);
				break;
			case ROOK:
				result = result || AttackRook(whoAtack, x, y);
				break;
			case BISHOP:
				result = result || AttackBishop(whoAtack, x, y);
				break;
			case KNIGHT:
				result = result || AttackKnight(whoAtack, x, y);
				break;
			case PAWN:
				result = result || AttackPawn(whoAtack, x, y);
				break;
			default:
				break;
			}
		}
		whoAtack = whoAtack->next;
	}
	return result;
}

bool Solver::Stalemate(uint32_t color, std::shared_ptr<TMoves>& move)
{
	auto temp = move;
	if (COLOR(color) == BLACK)
	{
		if (underAttack(whiteList, move->xFrom, move->yFrom))
			return false;
		bool result = true;
		while (result && temp)
		{
			//MakeMove(temp);
			result = result && underAttack(whiteList, temp->xTo, temp->yTo);
			//UnMakeMove(temp);
			temp = temp->next;

		}
		return result;
	}
	else
	{
		if (underAttack(blackList, move->xFrom, move->yFrom))
			return false;
		bool result = true;
		while (result && temp)
		{
			result = result && underAttack(blackList, temp->xTo, temp->yTo);
			temp = temp->next;
		}
		return result;

	}
}

int32_t Solver::AlphaBeta(uint32_t color, int32_t Depth, int32_t alpha, int32_t beta, std::shared_ptr<MadeMoves>& tree)
{
	if (!Depth)
		return Evaluate(color, Depth);
	auto tuple = GenerateAllMoves(color, Depth);
	std::shared_ptr<TMoves> move = std::get<0>(tuple);
	std::shared_ptr<TMoves> last = std::get<1>(tuple);
	if((move->figureInTheList->figureType()==KING) &&last && (last->figureInTheList->figureType()==KING))
		if(Stalemate(color, move))
			return -MATE + this->depth + 1;
	while (move && alpha < beta)
	{
		int32_t tmp = 0;
		if (FIGURE(move->eatenFigure) == KING)
		{
			Depth--;
			Apply(move, tree);
			tmp = -Evaluate(~color, Depth);
			UnMakeMove(move);
			if (tmp > alpha)
			{
				alpha = tmp;
				updateMadeMoves(tree);
			}
		}
		else
		{
			Apply(move, tree);
			tmp = -AlphaBeta(~color, Depth - 1, -beta, -alpha, tree->last->child);
			UnMakeMove(move);
			if (tmp > alpha)
			{
				alpha = tmp;
				updateMadeMoves(tree);
			}
		}
		move = move->next;
	}
	return alpha;
}

void Solver::Apply(std::shared_ptr<TMoves> move, std::shared_ptr<MadeMoves>& tree)
{
	MakeMove(move);
	if (!tree)
	{
		tree = std::make_shared<MadeMoves>(MadeMoves(move->xFrom, move->yFrom, move->xTo, move->yTo, move->change));
		tree->last = tree;
	}
	else
	{
		tree->last->next = std::make_shared<MadeMoves>(MadeMoves(move->xFrom, move->yFrom, move->xTo, move->yTo, move->change));
		tree->last = tree->last->next;
	}
}

int32_t Solver::Evaluate(uint32_t color, int32_t Depth)
{
	if (COLOR(color) == BLACK)
	{
		if ((desk[blackList->x][blackList->y]) == (KING | BLACK))
			return -MATE + this->depth + 1;
		else
			return -MATE + this->depth - Depth;
	}
	else
	{
		if ((desk[whiteList->x][whiteList->y]) == (KING | WHITE))
			return -MATE + this->depth + 1;
		else
			return -MATE + this->depth - Depth;
	}
}

void Solver::printResult(std::ostream& os)
{
	std::vector<std::shared_ptr<MadeMoves>> result;
	std::vector<std::shared_ptr<MadeMoves>> temp;
	recursivePrint(head, result, temp, os);

}

void Solver::recursivePrint(std::shared_ptr<MadeMoves> head, std::vector<std::shared_ptr<MadeMoves>>& result, std::vector<std::shared_ptr<MadeMoves>>& temp, std::ostream& os)
{
	
	while (head)
	{
		
		result.push_back(head);
		head = head->child;
	}
	if (result.size() == this->depth) {
		bool match = true;
		if (temp.size() == result.size()) {
			for (int32_t i = 0; i < result.size() - 2; i++)
			{
				
				if (result[i] != temp[i])
					match = false;
			}
		}
		else
		{
			match = false;
		}
		if (!match) {
			for (int32_t i = 0; i < result.size() - 2; i++)
			{
				os << result[i]->stringify() << std::endl;
			}
			os << std::endl;
		}
	}
	temp = result;
	while (true&& !result.empty())
	{
		result.pop_back();
		if (result.empty())
			break;
		if (result[result.size() - 1]->next)
		{
			result[result.size() - 1] = result[result.size() - 1]->next;
			recursivePrint(result[result.size() - 1]->child, result,temp, os);
		}
	}
}

void Solver::updateMadeMoves(std::shared_ptr<MadeMoves>& tree)
{
	tree = tree->last;
}

void Solver::cleanList(FigureNode* ptr)
{
	if (ptr->next->next)
		cleanList(ptr->next);
	delete ptr->next;

}

Solver::~Solver()
{
	if (whiteList->next)
		cleanList(whiteList);
	else
		delete whiteList;

	if (blackList->next)
		cleanList(blackList);
	else
		delete blackList;
}
