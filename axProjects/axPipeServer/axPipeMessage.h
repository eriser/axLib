#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

using namespace std;

struct WindowMessage
{
	WindowMessage(){}
	WindowMessage(const int& i1, const int& i2):
					t1(i1), t2(i2)
	{
	}

    int t1, t2; 
};

struct MouseMessage
{
	MouseMessage(){}
	MouseMessage(const double& i1, const double& i2):
					v1(i1), v2(i2)
	{
	}
	double v1, v2;
};

enum axInterProcessStructType
{
	axINTER_PROCESS_WINDOW_MESSAGE,
	axINTER_PROCESS_MOUSE_MESSAGE
};

struct axInterProcess
{
	pid_t pid;
	axInterProcessStructType type;
	MouseMessage mouse_message;
	WindowMessage window_message;
};
