#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <algorithm>

using namespace std;

string OpenFileDialog(const string& app_name)
{
	int parent_to_child[2]; /* Parent to child pipe */
	int child_to_parent[2]; /* Child to parent pipe */

	/* Make pipes */
	if( pipe(parent_to_child) < 0)
	{
		cerr << "Can't make pipe" << endl;;
		_exit(1);
	}

	if( pipe(child_to_parent) < 0)
	{
		cerr << "Can't make pipe" << endl;;
		_exit(1);
	}


	pid_t p_id = fork();

	// Child.
	if (p_id == 0)                
	{
		
		close(1); // Close current stdout.
		// close(0); // Close current stdin.s

		// Make stdout go to write end of pipe. 
		int err = dup(child_to_parent[1]); 
		// int err = dup2(1, child_to_parent[0]);

		if(err == -1)
		{
			cerr << "Error copying file descriptor." << endl;
		}
		// close(parent_to_child[0]);
		// close(parent_to_child[1]);
		// close(child_to_parent[0]);

		execl(app_name.c_str(), 0);

		_exit(0);
	}

	// Failed to fork.
	else if (p_id < 0)            
	{
	    cerr << "Failed to open " << app_name << endl;
	    _exit(1);
	    // Throw exception
	}

	// Parent.
	else
	{
		char buf[1024];

		int n_byte = read(child_to_parent[0], buf, 1024-1);
		string file(buf);

		// file.erase( std::remove(file.begin(), file.end(), '\r'), file.end());
		file.erase( std::remove(file.begin(), file.end(), '\n'), file.end());
		file.erase( std::remove(file.begin(), file.end(), '\0'), file.end());
		return file;
	}


}

void ExecApplication(const string& app_name)
{
	pid_t p_id = fork();

	// Child.
	if (p_id == 0)                
	{
		execl(app_name.c_str(), 0);
		cerr << "File didn't open." << endl;
		_exit(0);
	}

	// Failed to fork.
	else if (p_id < 0)            
	{
	    cerr << "Failed to open " << app_name << endl;
	    _exit(1);
	    // Throw exception
	}
}

int main()
{
	string f = OpenFileDialog("/home/alexarse/Desktop/axLib/axProjects/FileDialog/main");
	cout << f << endl;

	return 0;
}