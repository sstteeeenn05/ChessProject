/*****************************************************************
* File: Board.h
 * Author: §d²»ºö¡B´¿±R®¦¡BÃ¹¬ý²M¡B­ð¤_µx
 * Create Date: 2023/05/05
 * Editor: ´¿±R®¦¡BÃ¹¬ý²M
 * Update Date: 2023/05/24
 * Description: the declaration of Broad
 *********************************************************************/

#pragma once

#include "DataType.h"
#include "Chess.h"
#include <cctype>
#include <map>
#include <cmath>

class Chess;

class Board {
    std::vector<Log> logList;
    int logIndex;
    int beginLogIndex;
    static Chess board[8][8];
    // 0-white, 1-black
    Position kingsPos[2];
    bool isChecking;
    Status status;

public:
    void init(); //Initialize the board

    void init(std::string boardCode, std::string turn, std::string castling); //Initialize the board with a given board code, turn, and castling information

    static Chess &getChess(Position); //Retrieve the chess piece at the given position on the board

    static std::string getBoard(); //Get the current state of the chessboard as a string

    std::string getMaskBoard(Position); //Get the mask board indicating the valid movement positions for a chess piece at the target position

    bool canUndo(); //Check if there are moves to undo

    bool canRedo(); //Check if there are moves to redo

    bool undo(); //Undo the last move

    bool redo(); //Redo the last undone move
     
    std::vector<Position> getValidPos(Position); //Get a list of valid positions for a given chess piece

    bool canMove(Position, Position); //Check if a move from the source position to the target position is valid

    bool move(Player, Position, Position); //Move a chess piece from the source position to the target position

    void checkCastling(Chess&, Chess&, Log&); //Check and handle castling move if applicable

    void checkEnPassant(Chess&, Chess&, Log&); //Check and handle en passant move if applicable

    void clearEnPassanting(Log&); //Clear the en passant status of all pawns on the board

    void checkNextEnPassant(Chess&, Log&); //Check if the next pawn move can be captured en passant

    void checkMovement(const Position &target, std::vector<Position> &validPos); //Check the movement of a chess piece and update valid positions accordingly

    bool isCheck() const; //Check if the current board state is in a check position

    Status isWinOrTie(); //Get the current game status 

    Player getNowPlayer() const; //Get the player who is currently making a move

    Player getNowEnemy() const; //Get the enemy player of the player who is currently making a move

    [[nodiscard]] bool calculateCheck(Player); //Calculate if the specified player is in a check position

    [[nodiscard]] Status calculateWinOrTie(Player); //Check the movement of a chess piece and update valid positions accordingly

    friend class Chess;
};
