#include "Log.h"

Log::Log(Chess source, Chess target)
{
	this->source = source;
	this->target = target;
}

Log& Log::getLog()
{
	return *this;
}
