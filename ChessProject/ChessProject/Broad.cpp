#include "Broad.h"
#include <map>

/*std::map <char, std::string> charToLabel =
{
	'k' = "KING",

}*/

Board::Board() //constructor, initialize the board
{
	for (int i = 2; i < 6; i++) //set the empty space
	{
		for (int j = 0; j < 8; j++) //set the empty space
		{
			board[i][j].setEmpty();
		}
	}
	//X ¤W­±¬O0
	board[0][0].setChess(ROOK, BLACK);
	board[0][1].setChess(KNIGHT, BLACK);
	board[0][2].setChess(BISHOP, BLACK);
	board[0][3].setChess(QUEEN, BLACK);
	board[0][4].setChess(KING, BLACK);
	board[0][5].setChess(BISHOP, BLACK);
	board[0][6].setChess(KNIGHT, BLACK);
	board[0][7].setChess(ROOK, BLACK);

	for (int i = 0; i < 8; i++) //set the black pawn
	{
		board[1][i].setChess(PAWN, BLACK);
	}

	board[7][0].setChess(ROOK, WHITE);
	board[7][1].setChess(KNIGHT, WHITE);
	board[7][2].setChess(BISHOP, WHITE);
	board[7][3].setChess(QUEEN, WHITE);
	board[7][4].setChess(KING, WHITE);
	board[7][5].setChess(BISHOP, WHITE);
	board[7][6].setChess(KNIGHT, WHITE);
	board[7][7].setChess(ROOK, WHITE);

	for (int i = 0; i < 8; i++) //set the white pawn
	{
		board[6][i].setChess(PAWN, WHITE);
	}

	for (int i = 0; i < 8; i++) //set the position
	{
		for (int j = 0; j < 8; j++) //set the position
		{
			board[i][j].setPos(Position(j, i));
		}
	}
}

Chess* Board::getBoard() //let can draw board
{
	return &board[0][0];
}

std::vector<Position> Board::getValidPos(Chess chess) //get all the valid position
{
	Color color = chess.getColor(), enemy;
	Label label = chess.getLabel();
	Position pos = chess.getPos();
	std::vector<Position> validPos;
	validPos.clear();
	Position targetPos;
	int side, pawnOrigin;
	if (color == BLACK) //if the color is black
	{
		enemy = WHITE;
		side = 1;
		pawnOrigin = 1;
	}
	else if (color == WHITE) //if the color is white
	{
		enemy = BLACK;
		side = -1;
		pawnOrigin = 6;
	}

	switch (label) //judge the label
	{
	case KING: //if is king
		if (pos.x + 1 < 8 && pos.y + 1 < 8 && board[pos.y + 1][pos.x + 1].getColor() != color) //if can move
		{

			targetPos = Position(pos.x + 1, pos.y + 1);
			validPos.push_back(targetPos);
		}
		if (pos.y + 1 < 8 && board[pos.y + 1][pos.x].getColor() != color)//if can move
		{

			targetPos = Position(pos.x, pos.y + 1);
			validPos.push_back(targetPos);
		}
		if (pos.x - 1 >= 0 && pos.y + 1 < 8 && board[pos.y + 1][pos.x - 1].getColor() != color) //if can move
		{

			targetPos = Position(pos.x - 1, pos.y + 1);
			validPos.push_back(targetPos);
		}
		if (pos.x + 1 < 8 && board[pos.y][pos.x + 1].getColor() != color) //if can move
		{

			targetPos = Position(pos.x + 1, pos.y);
			validPos.push_back(targetPos);
		}
		if (pos.x - 1 >= 0 && board[pos.y][pos.x - 1].getColor() != color) //if can move
		{

			targetPos = Position(pos.x - 1, pos.y);
			validPos.push_back(targetPos);
		}
		if (pos.x + 1 < 8 && pos.y - 1 >= 0 && board[pos.y - 1][pos.x + 1].getColor() != color) //if can move
		{

			targetPos = Position(pos.x + 1, pos.y - 1);
			validPos.push_back(targetPos);
		}
		if (pos.y - 1 >= 0 && board[pos.y - 1][pos.x].getColor() != color) //if can move
		{

			targetPos = Position(pos.x, pos.y - 1);
			validPos.push_back(targetPos);
		}
		if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && board[pos.y - 1][pos.x - 1].getColor() != color) //if can move
		{

			targetPos = Position(pos.x - 1, pos.y - 1);
			validPos.push_back(targetPos);
		}
		break;

	case QUEEN: //if is queen
		for (int i = pos.x + 1; i < 8; i++) //check go right
		{
			targetPos = Position(i, pos.y);
			if (board[pos.y][i].getColor() == NONE) //if there is nothing in the space
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y][i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}
		for (int i = pos.x - 1; i >= 0; i--) //check go left
		{
			targetPos = Position(i, pos.y);
			if (board[pos.y][i].getColor() == NONE) //if there is nothing in the space
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y][i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}
		for (int i = pos.y + 1; i < 8; i++) //check go down
		{
			targetPos = Position(pos.x, i);
			if (board[i][pos.x].getColor() == NONE) //if there is nothing in the space
			{
				validPos.push_back(targetPos);
			}
			else if (board[i][pos.x].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}
		for (int i = pos.y - 1; i >= 0; i--) //check go up
		{
			targetPos = Position(pos.x, i);
			if (board[i][pos.x].getColor() == NONE) //if there is nothing in the space
			{
				validPos.push_back(targetPos);
			}
			else if (board[i][pos.x].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}
	case BISHOP: //if is bishop
		for (int i = 1; i < 8 && pos.x + i < 8 && pos.y + i < 8; i++) //check down right
		{
			targetPos = Position(pos.x + i, pos.y + i);
			if (board[pos.y + i][pos.x + i].getColor() == NONE) //if there is nothing in the sapce
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y + i][pos.x + i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //can not move
			{
				break;
			}
		}
		for (int i = 1; i < 8 && pos.x + i < 8 && pos.y - i >= 0; i++) //check up right
		{
			targetPos = Position(pos.x + i, pos.y - i);
			if (board[pos.y - i][pos.x + i].getColor() == NONE) //if there is nothing in the sapce
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y - i][pos.x + i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //can not move
			{
				break;
			}
		}
		for (int i = 1; i < 8 && pos.x - i >= 0 && pos.y + i < 8; i++) //check down left
		{
			targetPos = Position(pos.x - i, pos.y + i);
			if (board[pos.y + i][pos.x - i].getColor() == NONE) //if there is nothing in the sapce
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y + i][pos.x - i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //can not move
			{
				break;
			}
		}
		for (int i = 1; i < 8 && pos.x - i >= 0 && pos.y - i >= 0; i++) //check up left
		{
			targetPos = Position(pos.x - i, pos.y - i);
			if (board[pos.y - i][pos.x - i].getColor() == NONE) //if there is nothing in the sapce
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y - i][pos.x - i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //can not move
			{
				break;
			}
		}
		break;
	case KNIGHT: //if is knight
		if (pos.x + 1 < 8 && pos.y + 2 < 8 && board[pos.y + 2][pos.x + 1].getColor() != color) //if can move
		{
			targetPos = Position(pos.x + 1, pos.y + 2);
			validPos.push_back(targetPos);
		}
		if (pos.x + 1 < 8 && pos.y - 2 >= 0 && board[pos.y - 2][pos.x + 1].getColor() != color) //if can move
		{
			targetPos = Position(pos.x + 1, pos.y - 2);
			validPos.push_back(targetPos);
		}
		if (pos.x + 2 < 8 && pos.y + 1 < 8 && board[pos.y + 1][pos.x + 2].getColor() != color) //if can move
		{
			targetPos = Position(pos.x + 2, pos.y + 1);
			validPos.push_back(targetPos);
		}
		if (pos.x + 2 < 8 && pos.y - 1 >= 0 && board[pos.y - 1][pos.x + 2].getColor() != color) //if can move
		{
			targetPos = Position(pos.x + 2, pos.y - 1);
			validPos.push_back(targetPos);
		}
		if (pos.x - 1 < 8 && pos.y + 2 < 8 && board[pos.y + 2][pos.x - 1].getColor() != color) //if can move
		{
			targetPos = Position(pos.x - 1, pos.y + 2);
			validPos.push_back(targetPos);
		}
		if (pos.x - 1 < 8 && pos.y - 2 >= 0 && board[pos.y - 2][pos.x - 1].getColor() != color) //if can move
		{
			targetPos = Position(pos.x - 1, pos.y - 2);
			validPos.push_back(targetPos);
		}
		if (pos.x - 2 < 8 && pos.y + 1 < 8 && board[pos.y + 1][pos.x - 2].getColor() != color) //if can move
		{
			targetPos = Position(pos.x - 2, pos.y + 1);
			validPos.push_back(targetPos);
		}
		if (pos.x - 2 < 8 && pos.y - 1 >= 0 && board[pos.y - 1][pos.x - 2].getColor() != color) //if can move
		{
			targetPos = Position(pos.x - 2, pos.y - 1);
			validPos.push_back(targetPos);
		}
		break;
	case ROOK: //if is rook
		for (int i = pos.x + 1; i < 8; i++) //check go right
		{
			targetPos = Position(i, pos.y);
			if (board[pos.y][i].getColor() == NONE) //if there is nothing in the space
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y][i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}
		for (int i = pos.x - 1; i >= 0; i--) //check go left
		{
			targetPos = Position(i, pos.y);
			if (board[pos.y][i].getColor() == NONE) //if there is nothing in the space
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y][i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}
		for (int i = pos.y + 1; i < 8; i++) //check go down
		{
			targetPos = Position(pos.x, i);
			if (board[i][pos.x].getColor() == NONE) //if there is nothing in the space
			{
				validPos.push_back(targetPos);
			}
			else if (board[i][pos.x].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}
		for (int i = pos.y - 1; i >= 0; i--) //check go up
		{
			targetPos = Position(pos.x, i);
			if (board[i][pos.x].getColor() == NONE) //if there is nothing in the space
			{
				validPos.push_back(targetPos);
			}
			else if (board[i][pos.x].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}
		break;
	case PAWN: //if is pawn
		if (pos.y == pawnOrigin) //the first step, can move one or tow
		{
			for (int i = 1; i <= 2; i++) //can move one or two step, check if can move
			{
				targetPos = Position(pos.x, (pos.y + (i * side)));
				if (board[pos.y + (i * side)][pos.x].getColor() == NONE) //if there is nothing in the space
				{
					validPos.push_back(targetPos);
				}
				else //if there is something in the space
				{
					break;
				}
			}
		}
		if (pos.y + 1 < 8 && pos.y - 1 >= 0) //not the first step, can move one step
		{
			targetPos = Position(pos.x, (pos.y + (1 * side)));
			if (board[pos.y + (1 * side)][pos.x].getColor() == NONE) //if there is nothing in the sapce
			{
				validPos.push_back(targetPos);
			}
			if (pos.x + 1 < 8 && board[pos.y + (1 * side)][pos.x + 1].getColor() == enemy) //if there is something can eat
			{
				targetPos = Position(pos.x + 1, pos.y + (1 * side));
				validPos.push_back(targetPos);
			}
			if (pos.x - 1 >= 0 && board[pos.y + (1 * side)][pos.x - 1].getColor() == enemy) //if there is something can eat
			{
				targetPos = Position(pos.x - 1, pos.y + (1 * side));
				validPos.push_back(targetPos);
			}
		}
		break;
		}
		return validPos;
}

void Board::move(Player& player) //move
{
	Position source, target;
	std::string a, b;
	while (true) //do until move success
	{
		std::cin >> a >> b;
		source.x = a[0] - 'a';
		source.y = 8 - (a[1] - '0');
		target.x = b[0] - 'a';
		target.y = 8 - (b[1] - '0');
		if (player.getColor() != board[source.y][source.x].getColor()) //wrong color
		{
			std::cout << "Fail" << std::endl;
			continue;
		}
		if (moveAvalible(board[source.y][source.x], target)) //if can move
		{
			board[target.y][target.x].setSpace(board[source.y][source.x]);
			board[source.y][source.x].setEmpty();
			std::cout << "Success" << std::endl;
			if (board[target.y][target.x].getLabel() == PAWN) //if is pawn
			{
				board[target.y][target.x].checkPawn();
			}
			break;
		}
		else //if cannot move
		{
			std::cout << "Fail" << std::endl;
			continue;
		}
	}
}

bool Board::moveAvalible(Chess chess, Position target) //check if can move
{
	std::vector<Position> validPos;
	validPos = getValidPos(chess);
	for (auto& i : validPos)
	{
		std::cout << i.x << i.y << std::endl;
		if (i == target)
		{
			return true;
		}
	}
	return false;
}



/*Board::Board(std::string fen)
{
	int i, j = 0;
	for (int i = 0; i < 8; i++)
	{
		for (; j < fen.length(); j++)
		{
			if (fen[j] == '/')
			{
				break;
			}
			else if (fen[j] >= 'a' && fen[j] <= 'z')
			{
				board[j/8][j%8]=
			}
		}
	}

	for (i = 0; i < fen.length(); i++)
	{
		if (fen[i] != '/')
		{
			if (fen[i] >= 'a' && fen[i] <= 'a') //black chess
			{

			}
		}
	}
}*/