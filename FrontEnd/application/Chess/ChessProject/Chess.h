#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "DataType.h"

class Chess {
	static std::vector<std::vector<std::pair<ChessData,ChessData>>> logList;
	static int logIndex;
	static Position wKing, bKing;
	static Chess board[8][8];

	ChessData data;
public:
	static void init();
	static std::string getBoard();
	static std::string getMaskBoard(Position);
	static Chess& getChess(Position);

	static bool canUndo();
	static bool canRedo();
	static bool undo();
	static bool redo();

	static std::vector<Position> getValidPos(Position);
	bool checkValid(Position, std::vector<Position>&);
	void checkStraight(std::vector<Position>&);
	void checkCross(std::vector<Position>&);
	void checkSquare(std::vector<Position>&);
	void checkL(std::vector<Position>&);
	void checkPawn(std::vector<Position>&);
	Position generatePosByPlayer(Position);
	bool onRiver();
	std::pair<Position, Position> getSidePos();
	std::pair<bool, bool> getEnPassant();

	static bool canMove(Position, Position);
	static bool move(Player, Position, Position);

	bool checkPromotion();
	Type doPromotion();

	/*static bool checkMovement(Player);
	static bool isCheckmate(Player);
	static bool checkWin(Player);*/

	static Player getNowPlayer();
	static Player getNowEnemy();

	Chess() = default;
	Chess(Position);
	Chess(Player, Position, Type);
};