#pragma once
#include <iostream>
#include <string>
#include <vector>


enum Label
{
	KING,
	QUEEN,
	BISHOP,
	KNIGHT,
	ROOK,
	PAWN,
	EMPTY
};

enum Color
{
	WHITE,
	BLACK,
	NONE
};

enum Result
{
	WIN,
	LOSE,
	CONTINUE
};