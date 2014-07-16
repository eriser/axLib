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

