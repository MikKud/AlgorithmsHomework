#pragma once
#include "Definitions.h"
#include "MadeMoves.h"
#include "FigureNode.h"
#include "TMoves.h"
#include <vector>
#include <string>
/*
main class that solves the ridle
contains all needed data and wll provide the answer
*/
class Solver
{
public:
	Solver();
	bool addFigure(uint32_t, uint32_t, unsigned char);
	void setDepth(uint32_t);
	int32_t AlphaBeta(uint32_t color, int32_t Depth, int32_t alpha, int32_t beta, std::shared_ptr<MadeMoves>&);
	void printResult(std::ostream& );
	std::shared_ptr<MadeMoves> head;
	uint32_t depth = 0;
	~Solver();
private:
	
	unsigned char desk[8][8];
	FigureNode* whiteList = new FigureNode;
	FigureNode* blackList = new FigureNode;
	void MakeMove(std::shared_ptr<TMoves>);
	void UnMakeMove(std::shared_ptr<TMoves>);
	std::shared_ptr<TMoves> GenerateAllMoves(unsigned char, uint32_t);
	std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> GenerateMove(FigureNode*);
	std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> GenerateMoveKing(FigureNode*);
	std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> GenerateMoveQueen(FigureNode* fPtr);
	std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> GenerateMoveRook(FigureNode* fPtr);
	std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> GenerateMoveBishop(FigureNode* fPtr);
	std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> GenerateMoveKnight(FigureNode* fPtr);
	std::tuple<std::shared_ptr<TMoves>, std::shared_ptr<TMoves>> GenerateMovePawn(FigureNode* fPtr);
	int32_t Evaluate(uint32_t, int32_t);
	void recursivePrint(std::shared_ptr<MadeMoves>, std::vector<std::shared_ptr<MadeMoves>>&, std::ostream&);
	void Apply(std::shared_ptr<TMoves> move, std::shared_ptr<MadeMoves>& tree);
	void updateMadeMoves(std::shared_ptr<MadeMoves>& tree);
	void cleanList(FigureNode*);
};

