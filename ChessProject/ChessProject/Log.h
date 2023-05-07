#pragma once

#include "Chess.h"

class Log
{
private:
	Chess source;
	Chess target;
public:
	Log(Chess source, Chess target);
	Log& getLog();
};