#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "axPipeMessage.h"

#define FIFO_FILE "mypipe"

using namespace std;

class axPipeServer
{
public:
	axPipeServer(const string& pipe_name);

	~axPipeServer();

	void OpenPipe(const string& name);

	void Process();

	void MainLoop();
};
