#include "axDebugWin32.h"

ostream& operator << (ostream& stream, const PRINT_COLOR_LIST& color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	return stream;
}

ostream& operator << (const PRINT_COLOR_LIST& color, ostream& stream)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	return stream;
}