//
//  axTrempoline.h
//  ax
//
//  Created by Alexandre Arsenault on 2014-10-14.
//  Copyright (c) 2014 alexarse. All rights reserved.
//

#ifndef ax_axTrempoline_h
#define ax_axTrempoline_h

// This is the C "trampoline" function that will be used
// to invoke a specific Objective-C method FROM C++.
int MyObjectDoSomethingWith (void *myObjectInstance, void *parameter);

#endif
