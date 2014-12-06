//
//  axMsg.h
//  axManager
//
//  Created by Alexandre Arsenault on 2014-12-05.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __axManager__axMsg__
#define __axManager__axMsg__

class axMsg
{
public:
    axMsg();
    
    virtual ~axMsg();
    
    virtual axMsg* GetCopy() = 0;
    
};

#endif /* defined(__axManager__axMsg__) */
