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
#ifndef MidiSequencer_axFileDialog_h
#define MidiSequencer_axFileDialog_h

#include "axC++.h"

std::string axOpenFileDialog();

void TestFunctionInterface();

std::string CocoaGetAppDirectory();

void AddEventToDispatchQueue();

void axCocoaResizeFrame(const axSize& size);

void axCocoaHideMouse();
void axCocoaShowMouse();

// For vst interface, 'parent' is given by host.
void* CreateNSWindow(void* parent, void*& child);

#endif
