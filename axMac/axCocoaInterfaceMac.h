//
//  axFileDialog.h
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef MidiSequencer_axFileDialog_h
#define MidiSequencer_axFileDialog_h

#include "axC++.h"

std::string axOpenFileDialog();

void TestFunctionInterface();

std::string CocoaGetAppDirectory();

void AddEventToDispatchQueue();

void axCocoaResizeFrame(const axSize& size);

#endif
