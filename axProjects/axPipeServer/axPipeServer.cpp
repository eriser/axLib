#include "axPipeServer.h"

axPipeServer::axPipeServer(const string& pipe_name)
{
	OpenPipe(pipe_name);
}

axPipeServer::~axPipeServer()
{
	// Need to remove named pipe.
}

void axPipeServer::OpenPipe(const string& name)
{
	umask(0);
	int err = mknod(name.c_str(), S_IFIFO|0666, 0);

	switch(errno)
	{
		case EACCES:
		cerr << "One of the directories in pathname did not allow search \
				(execute) permission." << endl; break;
		
		case EDQUOT:
		cerr << "The user's quota of disk blocks or inodes on the file \
				system has been exhausted." << endl; break;
		
		case EEXIST:
		cerr << "pathname already exists. This includes the case where \
				 pathname is a symbolic link, dangling or not." << endl;
		break;

		case ENAMETOOLONG:
		cerr << "Either the total length of pathname is greater than \
				 PATH_MAX, or an individual filename component has a \
				 length greater than NAME_MAX. In the GNU system, there \
				 is no imposed limit on overall filename length, but \
				 some file systems may place limits on the length of a \
				 component." << endl; break;
		
		case ENOENT: 
		cerr << "A directory component in pathname does not exist or is \
				 a dangling symbolic link." << endl; break;
		
		case ENOSPC: 
		cerr << "The directory or file system has no room for the \
				 new file." << endl; break;
		
		case ENOTDIR:
		cerr << "A component used as a directory in pathname is not, \
				 in fact, a directory." << endl; break;
		
		case EROFS:
		cerr << "pathname refers to a read-only file system." << endl;
		break;
	}
}

void axPipeServer::Process()
{
	axInterProcess msg;

	FILE *fp = fopen(FIFO_FILE, "r");
	
	// size_t fread(void* ptr, size_t size, size_t count, FILE* stream);
	//------------------------------------------------------------------
	// ptr    : Pointer to a block of memory with a size of at least 
	//          (size*count) bytes, converted to a void*.
	// size   : Size, in bytes, of each element to be read.
	// size_t : is an unsigned integral type.
	// count  : Number of elements, each one with a size of size bytes.
	// stream : Pointer to a FILE object that specifies an input stream.
	while(fread((void*)&msg, sizeof(axInterProcess), 1, fp) == 1)
	{
		switch(msg.type)
		{
			case axINTER_PROCESS_MOUSE_MESSAGE:
			cout << msg.pid << " : " << msg.mouse_message.v1 << " " << msg.mouse_message.v2 << endl;
			break;

			case axINTER_PROCESS_WINDOW_MESSAGE:
			cout << msg.pid << " : " << msg.window_message.t1 << " " << msg.window_message.t2 << endl;
			break;

			default:
			cout << "Type unknown." << endl;
		}
	}

	fclose(fp);
}

void axPipeServer::MainLoop()
{
	while(1)
	{
		Process();
	}
	
}

int main()
{
	axPipeServer server(FIFO_FILE);
	server.MainLoop();

	return 0;
}