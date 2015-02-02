//
//  axUrlApi.cpp
//  axFinanceAPI
//
//  Created by Alexandre Arsenault on 2014-10-21.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//
#include "axUrlApi.h"

axUrlApi* axUrlApi::_url_api = nullptr;

axUrlApi::axUrlApi()
{
    _curl = curl_easy_init();
}

axUrlApi::~axUrlApi()
{
    // Always cleanup.
    if(_curl)
    {
        curl_easy_cleanup(_curl);
    }
}

std::string axUrlApi::GetUrlData(const std::string& url)
{
    std::string str;
    if(_curl)
    {
        curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, axUrlApi::writefunc);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &str);
        
        // Perform the request, res will get the return code.
        res = curl_easy_perform(_curl);
        
        // Check for errors.
        if(res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " <<
            curl_easy_strerror(res) << std::endl;
        }
    }
    
//    std::cout << "Content : " << std::endl << str << std::endl;
    
    return str;
}

size_t axUrlApi::writefunc(void* ptr, size_t size,
                           size_t nmemb, std::string& s)
{
    std::string tmp;
    tmp.assign((const char*)ptr);
    
    s += tmp;
//    std::cout << "write." << std::endl;
//    std::cout << "SIZE : " << size << std::endl;
    return size * nmemb;
}



