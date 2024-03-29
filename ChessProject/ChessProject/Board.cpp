/*****************************************************************
* File: Board.cpp
 * Author: ��_�x�B�d����
 * Create Date: 2023/05/05
 * Editor: ��_�x�B�d����
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

//intent:get all the chess on the board 
//pre:none
//post:string
std::string Board::getBoard()
{
	std::string output;

	//loop in the board
	for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) {
		Chess& c = board[i][j];
		
		if (c.getType() == EMPTY || c.getPlayer() == WHITE) output += char(c.getType()); // it is not the black chess
		else output += char(c.getType() + ('a' - 'A'));
	}
	return output;
}


std::string Board::getMaskBoard(Position point) 
{
	std::string output;
	std::vector<Position> posList = board[point.y][point.x].getValidPos(board);
	bool maskBoard[8][8] = { false };

	for (const auto& pos : posList) 
	{
		maskBoard[pos.y][pos.x] = true;
	}
	for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) 
	{
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

	//if the chess is pawn and wants to enPassant check if the horizontal distance is 1
	if (abs(target.y - source.y) == 1 && abs(target.x - source.x) == 1 &&
		sChess.getType() == PAWN && board[source.y][target.x].getType() == PAWN)
	{
		EnPassant = enPassant(sChess, target, logs[logIndex - 1]);
	}

	//if the target position is valid
	if (!moveAvalible(sChess, tChess) && !canCastle && !EnPassant) return false;


	//delete all the steps that had undo
	while (logIndex < logs.size())
	{
		logs.pop_back();
	}

	Log record(sChess, tChess, canCastle, EnPassant);
	logs.push_back(record);
	logIndex++;
	
	// if the king is at the target
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

	//if is pawn and check if it can promotion
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

	//check if it is this player's turn
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

//intent:check if there is any steps before
//pre:none
//post:true if it has steps in front of it
bool Board::canUndo() 
{
	return logIndex;
}

//intent:check if there is any steps after
//pre:none
//post:true if it has steps back of it
bool Board::canRedo() 
{
	return logIndex < logs.size();
}

//intent:undo the last step
//pre:none
//post:true if it undo successfully
bool Board::undo() 
{
	//if there is no steps in front of it
	if (!canUndo()) return false;

	Log record(logs[logIndex - 1].source, logs[logIndex - 1].target, logs[logIndex - 1].castling, logs[logIndex - 1].enPassant);

	//if the last step king had castling
	if (record.castling)
	{
		//if it is the short castling then return the rook
		if (record.target.getPos().x == 6)
		{
			board[record.source.getPos().y][7].setSpace(board[record.source.getPos().y][5]);
			board[record.source.getPos().y][5].setEmpty();
		}
		else //if it is the long castling then return the rook
		{
			board[record.source.getPos().y][0].setSpace(board[record.source.getPos().y][3]);
			board[record.source.getPos().y][3].setEmpty();
		}
	}

	//if the last step pawn had enPassant
	if (record.enPassant)
	{
		//if the black pawn ate the white chess then recover it
		if (record.source.getPos().y == 4)
		{
			board[4][record.target.getPos().x].setChess(PAWN, WHITE);
		}
		else //if the white pawn ate the black chess then recover it
		{
			board[3][record.target.getPos().x].setChess(PAWN, BLACK);
		}
	}

	board[record.source.getPos().y][record.source.getPos().x] = record.source; //return the source back
	board[record.target.getPos().y][record.target.getPos().x] = record.target; //return the target back
	logIndex--;
	return true;
}

//intent:redo the next step
//pre:none
//post:true if it redo successfully
bool Board::redo() 
{
	//if there is no steps after it
	if (!canRedo()) return false;

	Log record(logs[logIndex].source, logs[logIndex].target, logs[logIndex].castling, logs[logIndex].enPassant);

	//if the next step king had castling
	if (record.castling)
	{
		//if it is the short castling then set the rook to the castling pos
		if (record.target.getPos().x == 6)
		{
			board[record.source.getPos().y][5].setSpace(board[record.source.getPos().y][7]);
			board[record.source.getPos().y][7].setEmpty();
		}
		else //if it is the long castling then set the rook to the castling pos
		{
			board[record.source.getPos().y][3].setSpace(board[record.source.getPos().y][0]);
			board[record.source.getPos().y][0].setEmpty();
		}
	}

	//if the last step pawn had enPassant
	if (record.enPassant)
	{
		//let the black chess to be eaten
		if (record.source.getPos().y == 4) 
		{
			board[4][record.target.getPos().x].setEmpty();
		}
		else //let the white chess to be eaten
		{
			board[3][record.target.getPos().x].setEmpty();
		}
	}

	board[record.target.getPos().y][record.target.getPos().x].setSpace(board[record.source.getPos().y][record.source.getPos().x]); //set the source back
	board[record.source.getPos().y][record.source.getPos().x].setEmpty(); //set the target back
	logIndex++;
	return true;
}

//intent:get the index of log
//pre:none
//post:the int represent the number of log
int Board::getLogIndex()
{
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
		//if it is the long castling and the rook hasn't moved
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
			else //return the rook back
			{
				board[source.y][0].setSpace(board[source.y][3]);
				board[source.y][3].setEmpty();
				return false;
			}
		}
		else if ((!board[source.y][7].getMoved()) && target.x == 6) //if it is the short castling and the rook hasn't moved
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
			else  //return the rook back
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

//intent:check if pawn can enPassant and eat the enermy pawn
//pre:position of source and target and the record
//post:true if it can enPassant
bool Board::enPassant(Chess& chess, Position target, Log record)
{
	//check if it is not pawn
	if (record.source.getType() != PAWN)
	{
		return false;
	}

	//check if the vertical distance is 2 
	if (abs(record.source.getPos().y - record.target.getPos().y) != 2)
	{
		return false;
	}

	Player color = chess.getPlayer();
	Chess king;

	//if it is the black chess
	if (color == BLACK)
	{
		
		//check if y is 4
		if (chess.getPos().y != 4)
		{
			return false;
		}

		king.setPos(Chess::getBlackKingPos());
		king.setChess(KING, BLACK); //get the black king
		bool check = true;
		Chess tmp1, tmp2;
		tmp1.setSpace(board[target.y][target.x]); //save the chess on the target pos
		board[target.y][target.x].setSpace(board[4][chess.getPos().x]); //move the black chess
		board[4][chess.getPos().x].setEmpty(); //set the origin pos of black chess to blank
		tmp2.setSpace(board[4][target.x]); //save the white chess
		board[4][target.x].setEmpty(); //eat the white chess
		check = board[4][target.x].checkCheck(color, king.getPos(), board); //if the move won't let the king to be check by any enermy chess
		board[4][chess.getPos().x].setSpace(board[target.y][target.x]); //return the black chess back
		board[target.y][target.x].setSpace(tmp1); //return the target back

		//if the king won't be check
		if (!check)
		{			
			return true;
		}
		else
		{
			board[4][target.x].setSpace(tmp2); //return the white pawn back
			return false;
		}
	}
	else //if it is the white chess
	{
		//check if y is 3
		if (chess.getPos().y != 3)
		{
			return false;
		}

		king.setPos(Chess::getWhiteKingPos());
		king.setChess(KING, WHITE); //gst the white king
		bool check = true;
		Chess tmp1, tmp2;
		tmp1.setSpace(board[target.y][target.x]); //save the chess on the target pos
		board[target.y][target.x].setSpace(board[3][chess.getPos().x]); //move the white chess
		board[3][chess.getPos().x].setEmpty();  //set the origin pos of white chess to blank
		tmp2.setSpace(board[3][target.x]); //save the black chess
		board[3][target.x].setEmpty(); //eat the black chess
		check = board[3][target.x].checkCheck(color, king.getPos(), board); //if the move won't let the king to be check by any enermy chess
		board[3][chess.getPos().x].setSpace(board[target.y][target.x]); //return the white chess back
		board[target.y][target.x].setSpace(tmp1); //return the target back

		//if the king won't be check
		if (!check)
		{
			return true;
		}
		else
		{
			board[3][target.x].setSpace(tmp2); //return the black pawn back
			return false;
		}
	}
}

//intent:check if the king is being check by checking all the same color chess on the board
//pre:the player
//post:bool
bool Board::checkMovement(Player player)
{
	//loop the whole board
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//if it is not this player's chess
			if (board[i][j].getPlayer() != player)
			{
				continue;
			}
			else
			{
				std::vector<Position> validPos;
				validPos.clear();
				validPos = board[i][j].getValidPos(board); //get all the valid move pos of this chess

				//if there is any chess still can move
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

//intent:check if the player win
//pre:the player
//post:bool
bool Board::checkWin(Player player)
{
	Player enemy;
	Chess enemyKing;

	//if the color is white
	if (player== WHITE)
	{
		enemy = BLACK;
		enemyKing.setPos(Chess::getBlackKingPos());
		enemyKing.setChess(KING, BLACK);
	}
	else //if the color is black
	{
		enemy = WHITE;
		enemyKing.setPos(Chess::getWhiteKingPos());
		enemyKing.setChess(KING, WHITE);
	}
	
	if (checkMovement(enemy)) return false; //if there is still some chess can move

	return enemyKing.checkCheck(enemyKing.getPlayer(), enemyKing.getPos(), board); //check if the enermyKing will be check
}