/*****************************************************************
* File: Board.cpp
 * Author: ­ð¤_µx¡B§d²»ºö
 * Create Date: 2023/05/05
 * Editor: ­ð¤_µx¡B§d²»ºö
 * Update Date: 2023/05/06
 * Description: set and update board
 *********************************************************************/

#include "Board.h"

//intent:constructor and initialize the board
//pre:none
//post:none
Board::Board() 
{
	//set the empty space
	for (int i = 2; i < 6; i++) 
	{
		for (int j = 0; j < 8; j++) 
		{
			board[i][j].setEmpty();
		}
	}

	//set the black chess one by one
	board[0][0].setChess(ROOK, BLACK);
	board[0][1].setChess(KNIGHT, BLACK);
	board[0][2].setChess(BISHOP, BLACK);
	board[0][3].setChess(QUEEN, BLACK);
	board[0][4].setChess(KING, BLACK);
	board[0][5].setChess(BISHOP, BLACK);
	board[0][6].setChess(KNIGHT, BLACK);
	board[0][7].setChess(ROOK, BLACK);

	//set the black pawn
	for (int i = 0; i < 8; i++) 
	{
		board[1][i].setChess(PAWN, BLACK);
	}

	//set the white chess one by one
	board[7][0].setChess(ROOK, WHITE);
	board[7][1].setChess(KNIGHT, WHITE);
	board[7][2].setChess(BISHOP, WHITE);
	board[7][3].setChess(QUEEN, WHITE);
	board[7][4].setChess(KING, WHITE);
	board[7][5].setChess(BISHOP, WHITE);
	board[7][6].setChess(KNIGHT, WHITE);
	board[7][7].setChess(ROOK, WHITE);

	//set the white pawn
	for (int i = 0; i < 8; i++) 
	{
		board[6][i].setChess(PAWN, WHITE);
	}

	//set the position
	for (int i = 0; i < 8; i++) 
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j].setPos(Position(j, i));
		}
	}

	logs.clear();
}

//intent:constructor and initialize the board
//pre:none
//post:none
std::string Board::getBoard()
{
	std::string output;
	for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) {
		Chess& c = board[i][j];
		if (c.getType() == EMPTY || c.getPlayer() == WHITE) output += char(c.getType());
		else output += char(c.getType() + ('a' - 'A'));
	}
	return output;
}

std::string Board::getMaskBoard(Position point) {
	std::string output;
	std::vector<Position> posList = board[point.y][point.x].getValidPos(board);
	bool maskBoard[8][8] = { false };
	for (const auto& pos : posList) {
		maskBoard[pos.y][pos.x] = true;
	}
	for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) {
		if (maskBoard[i][j]) output += '1';
		else output += '0';
	}
	return output;
}

//intent: move the chess
//pre: whose turn
//post: none
bool Board::move(Player& player, Position source, Position target)
{
	Chess& sChess = board[source.y][source.x], & tChess = board[target.y][target.x];

	//if it is not this player's turn
	if (player != sChess.getPlayer()) return false;

	bool canCastle = false;

	//if the chess is king and wants to castling
	if (sChess.getType() == KING && (!sChess.getMoved()) && target.y == source.y && (target.x == 2 || target.x == 6))
	{
		canCastle = castling(source, target); //call the function to check if it can castling
	}
	
	bool EnPassant = false;

	if (abs(target.y - source.y)==1 && abs(target.x - source.x) == 1 &&
		sChess.getType() == PAWN && board[source.y][target.x].getType() == PAWN)
	{
		EnPassant = enPassant(sChess, target, logs[logIndex - 1]);
	}

	//if the target position is valid
	if (!moveAvalible(sChess, tChess) && !canCastle && !EnPassant) return false;

	while (logIndex < logs.size())
	{
		logs.pop_back();
	}
	Log record(sChess, tChess, canCastle, EnPassant);
	logs.push_back(record);
	logIndex++;
		
	if (board[target.y][target.x].getType() == KING)
	{
		if (player == WHITE)
		{
			Chess::setWhiteKingPos(target);
		}
		else
		{
			Chess::setBlackKingPos(target);
		}
	}

	sChess.setMoved();
	tChess.setSpace(sChess);
	sChess.setEmpty();

	//if is pawn check if it can promotion
	
	if (tChess.checkPromotion()) tChess.doPromotion();

	return true;
}

//intent: if this position is valid
//pre: which chess and the target position
//post:bool
bool Board::moveAvalible(Chess& source, Chess& target)
{
	std::vector<Position> validPos;
	Position targetPos = target.getPos();
	validPos = source.getValidPos(board);

	if (target.getPlayer() != source.getPlayer())
	{
		//run in the vector saved valid positions
		for (auto& i : validPos)
		{
			//if the position if found
			if (i == targetPos)
			{
				return true;
			}
		}
	}
	return false;
}

bool Board::canUndo() {
	return logIndex;
}

bool Board::canRedo() {
	return logIndex < logs.size();
}

bool Board::undo() //undo
{
	if (!canUndo()) return false;
	Log record(logs[logIndex - 1].source, logs[logIndex - 1].target, logs[logIndex - 1].castling, logs[logIndex - 1].enPassant);
	if (record.castling)
	{
		if (record.target.getPos().x == 6)
		{
			board[record.source.getPos().y][7].setSpace(board[record.source.getPos().y][5]);
			board[record.source.getPos().y][5].setEmpty();
		}
		else
		{
			board[record.source.getPos().y][0].setSpace(board[record.source.getPos().y][3]);
			board[record.source.getPos().y][3].setEmpty();
		}
	}
	if (record.enPassant)
	{
		if (record.source.getPos().y == 4)
		{
			board[4][record.target.getPos().x].setChess(PAWN, WHITE);
		}
		else
		{
			board[3][record.target.getPos().x].setChess(PAWN, BLACK);
		}
	}
	board[record.source.getPos().y][record.source.getPos().x] = record.source;
	board[record.target.getPos().y][record.target.getPos().x] = record.target;
	logIndex--;
	return true;
}

bool Board::redo() //redo
{
	if (!canRedo()) return false;
	Log record(logs[logIndex].source, logs[logIndex].target, logs[logIndex].castling, logs[logIndex].enPassant);
	if (record.castling)
	{
		if (record.target.getPos().x == 6)
		{
			board[record.source.getPos().y][5].setSpace(board[record.source.getPos().y][7]);
			board[record.source.getPos().y][7].setEmpty();
		}
		else
		{
			board[record.source.getPos().y][3].setSpace(board[record.source.getPos().y][0]);
			board[record.source.getPos().y][0].setEmpty();
		}
	}
	if (record.enPassant)
	{
		if (record.source.getPos().y == 4)
		{
			board[4][record.target.getPos().x].setEmpty();
		}
		else
		{
			board[3][record.target.getPos().x].setEmpty();
		}
	}
	board[record.target.getPos().y][record.target.getPos().x].setSpace(board[record.source.getPos().y][record.source.getPos().x]);
	board[record.source.getPos().y][record.source.getPos().x].setEmpty();
	logIndex++;
	return true;
}

int Board::getLogIndex() {
	return logIndex;
}

//intent:check if king can castling and move the pawn
//pre:position of source and target
//post:true if it can castling
bool Board::castling(Position source, Position target)
{
	bool check;

	//if king hasn't moved before
	if (!board[source.y][source.x].getMoved())
	{
		//if it is the long castling and the castle hasn't moved
		if (!(board[source.y][0].getMoved()) && target.x == 2)
		{
			//loop all the space between them
			for (int i = 1; i <= 3; i++)
			{
				//make sure all are empty
				if (board[source.y][i].getType() != EMPTY)
				{
					return false;
				}
			}

			//simulate the change
			board[source.y][2].setSpace(board[source.y][4]);
			board[source.y][3].setSpace(board[source.y][0]);
			board[source.y][4].setEmpty();
			board[source.y][0].setEmpty();

			//call the checkCheck function to mke sure the king won't be check
			check = board[source.y][2].checkCheck(board[source.y][2].getPlayer(), board[source.y][2].getPos(), board);

			board[source.y][4].setSpace(board[source.y][2]); //return the king back to the origin
			board[source.y][2].setEmpty();

			//if the king won't be check
			if (!check)
			{
				return true;
			}
			else //return the castle back
			{
				board[source.y][0].setSpace(board[source.y][3]);
				board[source.y][3].setEmpty();
				return false;
			}
		}
		else if ((!board[source.y][7].getMoved()) && target.x == 6) //if it is the short castling and the castle hasn't moved
		{
			//loop all the space between them
			for (int i = 5; i <= 6; i++)
			{
				//make sure all are empty
				if (board[source.y][i].getType() != EMPTY)
				{
					return false;
				}
			}

			//simulate the change
			board[source.y][6].setSpace(board[source.y][4]);
			board[source.y][5].setSpace(board[source.y][7]);
			board[source.y][4].setEmpty();
			board[source.y][7].setEmpty();

			//call the checkCheck function to mke sure the king won't be check
			check = board[source.y][6].checkCheck(board[source.y][6].getPlayer(), board[source.y][6].getPos(), board);

			board[source.y][4].setSpace(board[source.y][6]);//return the king back to the origin
			board[source.y][6].setEmpty();

			//if the king won't be check
			if (!check)
			{
				return true;
			}
			else  //return the castle back
			{
				board[source.y][7].setSpace(board[source.y][5]);
				board[source.y][5].setEmpty();
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

bool Board::enPassant(Chess& chess, Position target, Log record)
{
	if (record.source.getType() != PAWN)
	{
		return false;
	}
	if (abs(record.source.getPos().y - record.target.getPos().y) != 2)
	{
		return false;
	}
	Player color = chess.getPlayer();
	Chess king;
	if (color == BLACK)
	{
		
		if (chess.getPos().y != 4)
		{
			return false;
		}
		king.setPos(Chess::getBlackKingPos());
		king.setChess(KING, BLACK);
		bool check = true;
		Chess tmp1, tmp2;
		tmp1.setSpace(board[target.y][target.x]);
		board[target.y][target.x].setSpace(board[4][chess.getPos().x]);
		board[4][chess.getPos().x].setEmpty();
		tmp2.setSpace(board[4][target.x]);
		board[4][target.x].setEmpty();
		check = board[4][target.x].checkCheck(color, king.getPos(), board);
		board[4][chess.getPos().x].setSpace(board[target.y][target.x]);
		board[target.y][target.x].setSpace(tmp1);
		if (!check)
		{			
			return true;
		}
		else
		{
			board[4][target.x].setSpace(tmp2);
			return false;
		}
	}
	else
	{
		if (chess.getPos().y != 3)
		{
			return false;
		}
		king.setPos(Chess::getWhiteKingPos());
		king.setChess(KING, WHITE);
		bool check = true;
		Chess tmp1, tmp2;
		tmp1.setSpace(board[target.y][target.x]);
		board[target.y][target.x].setSpace(board[3][chess.getPos().x]);
		board[3][chess.getPos().x].setEmpty();
		tmp2.setSpace(board[3][target.x]);
		board[3][target.x].setEmpty();
		check = board[3][target.x].checkCheck(color, king.getPos(), board);
		board[3][chess.getPos().x].setSpace(board[target.y][target.x]);
		board[target.y][target.x].setSpace(tmp1);
		if (!check)
		{
			return true;
		}
		else
		{
			board[3][target.x].setSpace(tmp2);
			return false;
		}
	}
}

bool Board::checkMovement(Player player)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].getPlayer() != player)
			{
				continue;
			}
			else
			{
				std::vector<Position> validPos;
				validPos.clear();
				validPos = board[i][j].getValidPos(board);
				if (validPos.size() != 0)
				{
					validPos.clear();
					return true;
				}
				else
				{
					continue;
				}
			}
		}
	}
	return false;
}

bool Board::checkWin(Player player)
{
	Player enemy;
	Chess enemyKing;
	if (player== WHITE)
	{
		enemy=BLACK;
		enemyKing.setPos(Chess::getBlackKingPos());
		enemyKing.setChess(KING, BLACK);
	}
	else
	{
		enemy=WHITE;
		enemyKing.setPos(Chess::getWhiteKingPos());
		enemyKing.setChess(KING, WHITE);
	}
	if (checkMovement(enemy)) return false;

	return enemyKing.checkCheck(enemyKing.getPlayer(), enemyKing.getPos(), board);
}