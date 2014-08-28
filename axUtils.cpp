#include "axUtils.h"

string axFloatToString( const double& value, int num_char )
{
    double v = value;
    string s;

    ostringstream ss;
    ss << fixed << setprecision(8) << v;
    s = ss.str();
    s.resize( num_char );

    return s;
}

string axIntToString( const int& value )
{
    ostringstream ss;
    ss << value;
    return ss.str();
}

string axGetExtension(const string& path)
{
    return string(path.substr(path.find_last_of(".") + 1));
}

string OpenFileDialog(const string& app_name, string folder_path)
{
#ifdef __linux__
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

        if(folder_path != "")
        {
            execl(app_name.c_str(), folder_path.c_str(), 0);
        }
        else
        {
            execl(app_name.c_str(), 0);
        }
        

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
#endif //__linux__

#ifdef _MSC_VER
	return "";
#endif //_MSC_VER
}


double rand_val(int seed)
{
	const long a = 16807; // Multiplier
	const long m = 2147483647; // Modulus
	const long q = 127773; // m / a
	const long r = 2836; // m % a
	static long random_seed_variable; // Random int value.

	// Set the seed if argument is non-zero and then return zero
	if (seed > 0)
	{
		random_seed_variable = seed;
		return(0.0);
	}

	// RNG using integer arithmetic
	long x_div_q = random_seed_variable / q;
	long x_mod_q = random_seed_variable % q;
	long x_new = (a * x_mod_q) - (r * x_div_q);

	if (x_new > 0)
	{
		random_seed_variable = x_new;
	}
	else
	{
		random_seed_variable = x_new + m;
	}

	// Return a random value between 0.0 and 1.0.
	return((double)random_seed_variable / m);
}

double axNormalDistributionRandomGenerator(const double& mean, 
										   const double& std_dev)
{
	double u = 0.0, theta = 0.0; // Variables for Box-Muller method.
	//double x; // Normal(0, 1) rv.
	double norm_rv; // The adjusted normal rv.

	// Generate u
	while (u == 0.0)
	{
		u = rand_val(0);
	}

	// Compute r
	double r = sqrt(-2.0 * log(u));

	// Generate theta
	while (theta == 0.0)
	{
		theta = 2.0 * M_PI * rand_val(0);
	}

	// Generate x value.
	// Normal(0, 1) rv.
	double x = r * cos(theta);

	// Adjust x value for specified mean and variance.
	norm_rv = (x * std_dev) + mean;

	// Return the normally distributed RV value
	return(norm_rv);
}

