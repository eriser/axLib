//
//  axUrlApi.h
//  axFinanceAPI
//
//  Created by Alexandre Arsenault on 2014-10-21.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef axFinanceAPI_axUrlApi_h
#define axFinanceAPI_axUrlApi_h

#include <iostream>
#include <string>
#include <curl/curl.h>

class axUrlApi
{
public:
    inline static axUrlApi* GetInstance()
    {
        return _url_api == nullptr ? _url_api = new axUrlApi() : _url_api;
    }
    
//    inline static axUrlApi* Create()
//    {
//        return _url_api == nullptr ? _url_api = new axUrlApi() : _url_api;
//    }
    
    std::string GetUrlData(const std::string& url);
    
    static size_t writefunc(void* ptr, size_t size,
                            size_t nmemb, std::string& s);
    
private:
    axUrlApi();
    ~axUrlApi();
    
    static axUrlApi* _url_api;
    CURL* _curl;
    CURLcode res;
};

#endif
