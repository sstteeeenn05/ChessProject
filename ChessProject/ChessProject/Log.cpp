#include "Log.h"

Log::Log(Chess source, Chess target, bool castling, bool enPassant)
{
	this->source = source;
	this->target = target;
	this->castling = castling;
	this->enPassant = enPassant;
}


Log& Log::getLog()
{
	return *this;
}
