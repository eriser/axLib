#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "axPipeMessage.h"

#define FIFO_FILE "mypipe"

class axPipeClient
{
public:
    axPipeClient(const string& pipe_name);

    ~axPipeClient();

    void SendMsg(const axInterProcess& msg);

    void OpenFile(const string& name);

private:
    FILE* _fp;
};
