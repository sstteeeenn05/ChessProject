#pragma once

#include "Chess.h"

class Log
{
public:
	Chess source;
	Chess target;
	Log(Chess source, Chess target);
	Log& getLog();
	Chess getSource();
	Chess getTarget();
};