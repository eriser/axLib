#include "axDatabase.h"

axDatabase::axDatabase(const std::string& path):
_isValid(false)
{
    int rc = sqlite3_open(path.c_str(), &_dataBase);
    
    if(rc)
    {
        std::cerr << "Can't open database: " <<
        sqlite3_errmsg(_dataBase) << std::endl;
    }
    else
    {
        _isValid = true;
    }
}

axVectorPairStringVector axDatabase::Request(const std::string& request)
{
    axVectorPairStringVector attributes;
    
    if(_isValid)
    {
        char* errMsg = 0;
        int rc = sqlite3_exec(_dataBase,
                              request.c_str(),
                              DatabaseCallback,
                              (void*)&attributes,
                              &errMsg);
        
        if( rc != SQLITE_OK )
        {
            std::cerr << "SQL request error : " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }
    
    return attributes;
}

int axDatabase::DatabaseCallback(void *data,
                                 int argc,
                                 char **argv,
                                 char **azColName)
{
    axVectorPairStringVector* vec =
    reinterpret_cast<axVectorPairStringVector*>(data);
    
    axVectorPairString attributes;
    for(int i = 0; i < argc; i++)
    {
        std::string param_value(argv[i] ? argv[i] : "NULL");
        attributes.push_back(axPairString(azColName[i], param_value));
    }
    vec->push_back(attributes);
    
    return 0;
}