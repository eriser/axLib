//
//  axDatabase.h
//  DBTest
//
//  Created by Alexandre Arsenault on 2015-01-26.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __AX_DATABASE__
#define __AX_DATABASE__

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <vector>

typedef std::vector<std::pair<std::string, std::string>> axVectorPairString;
typedef std::pair<std::string, std::string> axPairString;
typedef std::vector<axVectorPairString> axVectorPairStringVector;

class axDatabase
{
public:
    axDatabase(const std::string& path);
    
    axVectorPairStringVector Request(const std::string& request);
    
private:
    sqlite3* _dataBase;
    bool _isValid;
    
    static int DatabaseCallback(void *data,
                                int argc,
                                char **argv,
                                char **azColName);
};

#endif // __AX_DATABASE__