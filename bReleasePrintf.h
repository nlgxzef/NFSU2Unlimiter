#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

bool EnableReleasePrintf = 0;

#ifdef DEBUG
	EnableReleasePrintf = 1;
#endif // DEBUG

int bReleasePrintf(const char* fmt, ...)
{
	int result = -1;

	va_list args;
	va_start(args, fmt);
	if (EnableReleasePrintf) result = vprintf(fmt, args);
	va_end(args);
	return result;
}