#include "Broad.h"
#include <map>

/*std::map <char, std::string> charToLabel =
{
	'k' = "king",

}*/

Board::Board()
{
	for (int i = 2; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j].setChess(EMPTY, NONE);
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

	for (int i = 0; i < 8; i++)
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

	for (int i = 0; i < 8; i++)
	{
		board[6][i].setChess(PAWN, WHITE);
	}

}

Chess* Board::drawBoard()
{
	return &board[0][0];
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