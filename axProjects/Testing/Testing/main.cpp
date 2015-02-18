//
//  main.cpp
//  Testing
//
//  Created by Alexandre Arsenault on 2015-02-16.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include <iostream>

// Since variadic templates are recursive, must have a base case.
void output() { std::cout << '\n'; }

template <typename T, typename ...P>
void output(T t, P ...p)
{
    std::cout << t << ' ';
    {
        output(p...);
    }
}


int main(int argc, const char * argv[])
{
    auto aff = [](auto x){ std::cout << x << std::endl;};
    

    output("Error", 21, 23232, 2121, std::string("aa"));
    
    return 0;
}
