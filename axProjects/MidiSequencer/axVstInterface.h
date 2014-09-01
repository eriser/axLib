#ifndef __AX_VST_INTERFACE__
#define __AX_VST_INTERFACE__

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <vector>
#include <string>

#include "axUtils.h"
#include "axC++.h"

#include "aeffectx.h"
#include "audioeffect.h"
#include "audioeffectx.h"
#include "aeffeditor.h"

#include "axCoreWin32.h"

extern "C" 
{
	VstIntPtr VSTCALLBACK hostCallback(AEffect *effect, VstInt32 opcode, VstInt32 index,
		VstInt32 value, void *ptr, float opt);
}

// Plugin's entry point
typedef AEffect *(*vstPluginFuncPtr)(audioMasterCallback host);

// Plugin's dispatcher function
typedef VstIntPtr(*dispatcherFuncPtr)(AEffect *effect, VstInt32 opCode,
	VstInt32 index, VstInt32 value, void *ptr, float opt);

// Plugin's getParameter() method
typedef float(*getParameterFuncPtr)(AEffect *effect, VstInt32 index);

// Plugin's setParameter() method
typedef void(*setParameterFuncPtr)(AEffect *effect, VstInt32 index, float value);

// Plugin's processEvents() method
typedef VstInt32(*processEventsFuncPtr)(VstEvents *events);

// Plugin's process() method
typedef void(*processFuncPtr)(AEffect *effect, float **inputs,
	float **outputs, VstInt32 sampleFrames);


class axVstInterface
{
public:
	axVstInterface(const unsigned int sample_rate, 
				   const unsigned int buffer_size);

	void LoadVstPlugin(const std::string& vstPath);

	int configurePluginCallbacks();

	void startPlugin();

	void resumePlugin();

	void suspendPlugin();

	void processAudio( float **inputs, float **outputs);

	void axOpenVstWindow();

	axFloat** GetInputBuffers();

	axFloat** GetOutputBuffers();

private:
	axFloat** _inBuffer;
	axFloat** _outBuffer;
	unsigned int _sampleRate;
	unsigned int _buffreSize;
	std::string _vstPath;
	std::vector<std::string> _vstParamsName;

	AEffect* _plugin;
	dispatcherFuncPtr _vstDispatcher;
};

#endif // __AX_VST_INTERFACE__