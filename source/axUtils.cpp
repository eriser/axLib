/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/
#include "axUtils.h"

axStringVector GetVectorFromStringDelimiter(const std::string& str,
                                            const std::string& delimiter)
{
    axStringVector vec;
    
    std::string r = str;
    size_t pos = 0;
    std::string token;
    while ((pos = r.find(delimiter)) != std::string::npos)
    {
        token = r.substr(0, pos);
        vec.push_back(token);
        r.erase(0, pos + delimiter.length());
    }
    
    vec.push_back(r);
    
    return vec;
}

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

//string OpenFileDialog(const string& app_name, string folder_path)
//{
//#ifdef __linux__
//    int parent_to_child[2]; /* Parent to child pipe */
//    int child_to_parent[2]; /* Child to parent pipe */
//
//    /* Make pipes */
//    if( pipe(parent_to_child) < 0)
//    {
//        cerr << "Can't make pipe" << endl;;
//        _exit(1);
//    }
//
//    if( pipe(child_to_parent) < 0)
//    {
//        cerr << "Can't make pipe" << endl;;
//        _exit(1);
//    }
//
//
//    pid_t p_id = fork();
//
//    // Child.
//    if (p_id == 0)                
//    {
//        
//        close(1); // Close current stdout.
//        // close(0); // Close current stdin.s
//
//        // Make stdout go to write end of pipe. 
//        int err = dup(child_to_parent[1]); 
//        // int err = dup2(1, child_to_parent[0]);
//
//        if(err == -1)
//        {
//            cerr << "Error copying file descriptor." << endl;
//        }
//        // close(parent_to_child[0]);
//        // close(parent_to_child[1]);
//        // close(child_to_parent[0]);
//
//        if(folder_path != "")
//        {
//            execl(app_name.c_str(), folder_path.c_str(), 0);
//        }
//        else
//        {
//            execl(app_name.c_str(), 0);
//        }
//        
//
//        _exit(0);
//    }
//
//    // Failed to fork.
//    else if (p_id < 0)            
//    {
//        cerr << "Failed to open " << app_name << endl;
//        _exit(1);
//        // Throw exception
//    }
//
//    // Parent.
//    else
//    {
//        char buf[1024];
//
//        int n_byte = read(child_to_parent[0], buf, 1024-1);
//        string file(buf);
//
//        // file.erase( std::remove(file.begin(), file.end(), '\r'), file.end());
//        file.erase( std::remove(file.begin(), file.end(), '\n'), file.end());
//        file.erase( std::remove(file.begin(), file.end(), '\0'), file.end());
//        return file;
//    }
//#endif //__linux__
//
//#ifdef _MSC_VER
//	return "";
//#endif //_MSC_VER
//    
//    return "";
//}

