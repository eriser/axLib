#include "axVstInterface.h"

extern "C"
{
	VstIntPtr VSTCALLBACK hostCallback(AEffect *effect, VstInt32 opcode, VstInt32 index,
		VstInt32 value, void *ptr, float opt) {
		switch (opcode) 
		{
		case audioMasterVersion:
			return 2400;
		case audioMasterIdle:
			effect->dispatcher(effect, effEditIdle, 0, 0, 0, 0);
			// Handle other opcodes here... there will be lots of them
		case audioMasterGetTime:
			return NULL;

		case audioMasterGetCurrentProcessLevel:
			// No so sure if kVstProcessLevelUser or kVstProcessLevelRealtime.
			return kVstProcessLevelRealtime;

		
		default:
			printf("Plugin requested value of opcode %d\n", opcode);
			break;
		}
	}
}

axVstInterface::axVstInterface(const unsigned int sample_rate,
							   const unsigned int buffer_size):
							   _sampleRate(sample_rate),
							   _buffreSize(buffer_size)
{
	int nChannel = 2;
	_inBuffer = new axFloat*[nChannel];
	_outBuffer = new axFloat*[nChannel];

	for (int i = 0; i < nChannel; i++)
	{
		_inBuffer[i] = new axFloat[buffer_size];
		_outBuffer[i] = new axFloat[buffer_size];
	}
}

void axVstInterface::LoadVstPlugin(const std::string& vst_path)
{
	_plugin = NULL;
	char *vstPath = "C:\\VST\\TAL-Chorus-LX.dll";

	HMODULE  modulePtr = LoadLibrary(vstPath);
	if (modulePtr == NULL)
	{
		printf("Failed trying to load VST from '%s', error %d\n",
			vstPath, GetLastError());
		return ;
	}

	vstPluginFuncPtr mainEntryPoint = (vstPluginFuncPtr)GetProcAddress(modulePtr, "VSTPluginMain");
	// Instantiate the plugin
	_plugin = mainEntryPoint(hostCallback);


	//-------------------------------------------------------------------------
	configurePluginCallbacks();

	cout << "Configure done." << endl;
	//IPluginFactory* fac = GetProcAddress(modulePtr, "GetPluginFactory");

	/*axFlag_exist*/
	VstInt32 nParam = _plugin->numParams;
	cout << "Number of params :" << nParam << endl;

	dispatcherFuncPtr dispatcher = (dispatcherFuncPtr)(_plugin->dispatcher);
	//char* param_name = nullptr;
	char* pn = new char[8];
	AudioEffect* aef = (AudioEffect*)_plugin->object;

	for (int i = 0; i < nParam; i++)
	{
		aef->getParameterName(i, pn);
		//dispatcher(_plugin, effGetParamLabel, i, 0, (void*)param_name, 0.0f);
		cout << pn << endl;
		//cout << _plugin->
	}


	//cout << _plugin->getParameter(_plugin, 0);
	//if (plugin->flags == effFlagsHasEditor)
	//if (axFlag_exist(effFlagsHasEditor, _plugin->flags))
	//{
	//	cout << "VST plugin has editor." << endl;
	//	//AudioEffect* aef = (AudioEffect*)_plugin->object;
	//	//AEffEditor* aefEditor = aef->getEditor();
	//	//HWND test_win;
	//	//aefEditor->open(static_cast<axCoreWin32*>(axCORE)->GetMainWindowHandle());
	//}
	//else
	//{
	//	cout << "VST plugin has no editor." << endl;
	//}
	//return plugin;
}

int axVstInterface::configurePluginCallbacks() 
{
	// Check plugin's magic number
	// If incorrect, then the file either was not loaded properly, is not a
	// real VST plugin, or is otherwise corrupt.
	if (_plugin->magic != kEffectMagic) 
	{
		printf("Plugin's magic number is bad\n");
		return -1;
	}

	// Create dispatcher handle
	dispatcherFuncPtr dispatcher = (dispatcherFuncPtr)(_plugin->dispatcher);
	//dispatcher = (dispatcherFuncPtr)(plugin->dispatcher);

	// Set up plugin callback functions
	_plugin->getParameter = (getParameterFuncPtr)_plugin->getParameter;
	_plugin->processReplacing = (processFuncPtr)_plugin->processReplacing;
	_plugin->setParameter = (setParameterFuncPtr)_plugin->setParameter;

	//AudioEffect* audioEffect = plugin->object;
	//AEffEditor* plugin->getEditor();

	return 0;//plugin;
}


void axVstInterface::startPlugin()
{
	dispatcherFuncPtr dispatcher = (dispatcherFuncPtr)(_plugin->dispatcher);

	dispatcher(_plugin, effOpen, 0, 0, NULL, 0.0f);

	// Set some default properties
	float sampleRate = 44100.0f;
	dispatcher(_plugin, effSetSampleRate, 0, 0, NULL, sampleRate);
	int blocksize = 1024;
	dispatcher(_plugin, effSetBlockSize, 0, blocksize, NULL, 0.0f);

	//plugin->

	//resume();
}

void axVstInterface::resumePlugin()
{
	dispatcherFuncPtr dispatcher = (dispatcherFuncPtr)(_plugin->dispatcher);
	dispatcher(_plugin, effMainsChanged, 0, 1, NULL, 0.0f);
}

void axVstInterface::suspendPlugin()
{
	dispatcherFuncPtr dispatcher = (dispatcherFuncPtr)(_plugin->dispatcher);
	dispatcher(_plugin, effMainsChanged, 0, 0, NULL, 0.0f);
}

void axVstInterface::axOpenVstWindow()
{
	//cout << "VST plugin has editor." << endl;
	AudioEffect* aef = (AudioEffect*)_plugin->object;
	//AEffEditor* aefEditor = aef->getEditor();
	//HWND test_win;
	//aefEditor->open(static_cast<axCoreWin32*>(axCORE)->GetMainWindowHandle());
	//dispatcherFuncPtr dispatcher = (dispatcherFuncPtr)(plugin->dispatcher);
	//dispatcher(plugin, audioMasterOpenWindow, 0, 0, NULL, 0.0f);
}

axFloat** axVstInterface::GetInputBuffers()
{
	return _inBuffer;
}

axFloat** axVstInterface::GetOutputBuffers()
{
	return _outBuffer;
}

void axVstInterface::processAudio(float **inputs, float **outputs)
{
	// Always reset the output array before processing.
	// silenceChannel(outputs, numChannels, numFrames);

	// Note: If you are processing an instrument, you should probably zero
	// out the input channels first to avoid any accidental noise. If you
	// are processing an effect, you should probably zero the values in the
	// output channels. See the silenceChannel() function below.
	// However, if you are reading input data from file (or elsewhere), this
	// step is not necessary.
	//silenceChannel(inputs, numChannels, numFrames);

	_plugin->processReplacing(_plugin, inputs, outputs, _buffreSize);
}