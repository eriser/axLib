#include "axPipeClient.h"

// #define FIFO_FILE "mypipe"

axPipeClient::axPipeClient(const string& pipe_name)
{
    OpenFile(pipe_name);
}

axPipeClient::~axPipeClient()
{
   fclose(_fp);
}

void axPipeClient::SendMsg(const axInterProcess& msg)
{
    fwrite((const void *)&msg, sizeof(axInterProcess), 1, _fp);
    fflush(_fp);
}

void axPipeClient::OpenFile(const string& name)
{
    if((_fp = fopen(name.c_str(), "w")) == NULL) 
    {
        perror("fopen");
        exit(1);
    }
}


int main(int argc, char *argv[])
{
    axPipeClient client(FIFO_FILE);

    axInterProcess msg;
    msg.pid = getpid();

    msg.type = axINTER_PROCESS_MOUSE_MESSAGE;
    msg.mouse_message = MouseMessage(32.53, 32.92);
    client.SendMsg(msg);

    msg.type = axINTER_PROCESS_WINDOW_MESSAGE;
    msg.window_message = WindowMessage(31, 39);
    client.SendMsg(msg);
    
    return(0);
}
