#pragma once

#include "Chess.h"

class Log
{
public:
	Chess source;
	Chess target;
	bool castling;
	bool enPassant;
	Log(Chess source, Chess target, bool castling, bool enPassant);
	Log& getLog();
	Chess getSource();
	Chess getTarget();
};