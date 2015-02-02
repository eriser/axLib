#include "ReWireSDK/ReWirePanelAPI.h"

#include "SampleDevice/SampleDeviceProtocol.h"
#include "Sampleutils/SampleUtils.h"

#include <iostream>
#include <string>
#include <cassert>
#if WINDOWS
#include <Windows.h>
#include <conio.h>
#endif // WINDOWS

#if MAC
#include <poll.h>
#endif // MAC

// Util functions for waiting and key input checking
static void Sleep() {
#if MAC
	::usleep(100 * 1000);
#endif // MAC
#if WINDOWS
	::Sleep(100);
#endif // WINDOWS
}

static bool KeyIsPressed() {
#if WINDOWS
	if (_kbhit()) {
		return true;
	}
#endif // WINDOWS
#if MAC
	struct pollfd pls[ 1 ];
	pls[ 0 ].fd = STDIN_FILENO;
	pls[ 0 ].events = POLLIN | POLLPRI;
	if (poll(pls, 1, 0) > 0) {
		return true;
	}
#endif // MAC
	return false;
}


#if WINDOWS
#if TARGET_DEBUGGING
const char kSampleDeviceFileName[]="SampleDeviceDebugging.dll";
#endif // TARGET_DEBUGGING
#if TARGET_TESTING
const char kSampleDeviceFileName[]="SampleDeviceTesting.dll";
#endif // TARGET_TESTING
#if TARGET_DEPLOYMENT
const char kSampleDeviceFileName[]="SampleDevice.dll";
#endif // TARGET_DEPLOYMENT
#endif // WINDOWS

#if MAC
const char kSampleDeviceFileName[]="SampleDevice.bundle";
#endif // MAC



static ReWire::TRWPPortHandle gPanelPortHandle=NULL;
static bool gReWireOpen=false;


static void Open() {
	ReWire::ReWireError result = ReWire::RWPOpen();
	if (result != ReWire::kReWireError_NoError) {
		std::cout << "RWPOpen() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
		throw std::exception();
	}
	gReWireOpen = true;
}

static void Close() {
	if (gReWireOpen) {
		if (gPanelPortHandle != 0) {
			ReWire::RWPComDisconnect(gPanelPortHandle);
			gPanelPortHandle = 0;
			ReWire::ReWireError result = ReWire::RWPUnloadDevice(kSampleDeviceName);
			if (result != ReWire::kReWireError_NoError) {
				std::cout << "RWPUnloadDevice() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
				throw std::exception();
			}
		}
		ReWire::RWPClose();
		gReWireOpen = false;
	}
}

static void RegisterDevice(const std::string& iAppPath, bool iRegister) {
	std::string devicePath;
#if WINDOWS
	std::string::size_type pos = iAppPath.rfind('\\');
	assert(pos != std::string::npos);
	devicePath=iAppPath.substr(0,pos) + "\\" + kSampleDeviceFileName;
#endif // WINDOWS
#if MAC
	std::string::size_type pos = iAppPath.rfind('/');
	assert(pos != std::string::npos);
	devicePath=iAppPath.substr(0,pos) + "/" + kSampleDeviceFileName;
#endif // MAC
	assert(!devicePath.empty());
	ReWire::TReWireFileSpec fileSpec = devicePath.c_str();

	if (iRegister) {
		ReWire::ReWireError result = ReWire::RWPRegisterReWireDevice(fileSpec);
		if (result != ReWire::kReWireError_NoError) {
			std::cout << "RWPRegisterReWireDevice() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
			throw std::exception();
		}
	}
	else {
		ReWire::ReWireError result = ReWire::RWPUnregisterReWireDevice(fileSpec);
		if (result != ReWire::kReWireError_NoError) {
			std::cout << "RWPUnregisterReWireDevice() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
			throw std::exception();
		}
	}
}

static void ConnectToDevice() {
	assert(gReWireOpen);
	ReWire::ReWire_char_t mixerAppRunningFlag = 0;
	ReWire::ReWireError result = ReWire::RWPIsReWireMixerAppRunning(&mixerAppRunningFlag);
	if (result != ReWire::kReWireError_NoError) {
		std::cout << "RWPIsReWireMixerAppRunning() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
		throw std::exception();
	}
	if (mixerAppRunningFlag == 0) {
		std::cout << "No Mixer app is running!" << std::endl;
		throw std::exception();
	}
	result = ReWire::RWPLoadDevice(kSampleDeviceName);
	if (result == ReWire::kReWireImplError_InvalidParameter) {
		std::cout << "RWPLoadDevice() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
		std::cout << "The device was not registered?" << std::endl;
		throw std::exception();
	}
	else if (result != ReWire::kReWireError_NoError) {
		std::cout << "RWPLoadDevice() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
		throw std::exception();
	}

	result = ReWire::RWPComConnect(kSampleDeviceSignature, &gPanelPortHandle);
	if (result != ReWire::kReWireError_NoError) {
		std::cout << "RWPComConnect() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
		result = ReWire::RWPUnloadDevice(kSampleDeviceName);
		throw std::exception();
	}
}

static void Play() {
	assert(gPanelPortHandle != 0);
	ReWire::ReWire_uint8_t messageBody[] = {kPlay};
	ReWire::ReWireError result = ReWire::RWPComSend(gPanelPortHandle, 0, sizeof(messageBody), messageBody);
	if (result != ReWire::kReWireError_NoError) {
		std::cout << "RWPComSend() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
	}
}

static void Stop() {
	assert(gPanelPortHandle != 0);
	ReWire::ReWire_uint8_t messageBody[] = {kStop};
	ReWire::ReWireError result = ReWire::RWPComSend(gPanelPortHandle, 0, sizeof(messageBody), messageBody);
	if (result != ReWire::kReWireError_NoError) {
		std::cout << "RWPComSend() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
	}
}

static void Pack32BitUnsignedBig(ReWire::ReWire_uint8_t binary[], ReWire::ReWire_uint32_t v){
	assert(binary !=0);
	binary[3]=static_cast<ReWire::ReWire_uint8_t>(v);
	binary[2]=static_cast<ReWire::ReWire_uint8_t>(v >> 8);
	binary[1]=static_cast<ReWire::ReWire_uint8_t>(v >> 16);
	binary[0]=static_cast<ReWire::ReWire_uint8_t>(v >> 24);
}

static void SetTempo() {
	std::cout << "Tempo (BPM): ";
	ReWire::ReWire_uint32_t tempo=0;
	std::cin >> tempo;
	if (tempo>0 && tempo<400) {
		tempo*=1000;
		assert(gPanelPortHandle != 0);
		ReWire::ReWire_uint8_t messageBody[] = {kSetTempo,  0,  0,  0,  0};
		Pack32BitUnsignedBig(&messageBody[1], tempo);
		ReWire::ReWireError result = ReWire::RWPComSend(gPanelPortHandle, 0, sizeof(messageBody), messageBody);
		if (result != ReWire::kReWireError_NoError) {
			std::cout << "RWPComSend() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
		}
	}
}

static bool PollForQuitRequest(){
	assert(gPanelPortHandle != 0);
	ReWire::ReWire_uint16_t bytesAvailable = 0;
	ReWire::ReWireError result = ReWire::RWPComBytesAvailable(gPanelPortHandle, 0, &bytesAvailable);
	while ((result == ReWire::kReWireError_NoError) && (bytesAvailable > 0)) {
		ReWire::ReWire_uint16_t messageSize = 1;
		ReWire::ReWire_uint8_t messageBody[100];
		result = ReWire::RWPComRead(gPanelPortHandle, 0, &messageSize, messageBody);
		if (result == ReWire::kReWireError_NoMoreMessages) {
			return false;
		}
		else if (result == ReWire::kReWireError_NoError) {
			if (messageBody[0] == kRequestQuit) {
				return true;
			}
		}
		else {
			std::cout << "RWPComRead() failed, error code: " << NSSampleUtils::GetErrorString(result) << std::endl;
		}
		result = ReWire::RWPComBytesAvailable(gPanelPortHandle, 0, &bytesAvailable);
	}
	return false;
}


static void MainMenu() {
	bool stop=false;
	bool printMenu=true;
	while (!stop) {
		if (printMenu) {
			std::cout << "Sample Panel Menu" << std::endl;
			std::cout << "P - Play" << std::endl;
			std::cout << "S - Stop" << std::endl;
			std::cout << "T - Set Tempo" << std::endl;
			std::cout << "Q - Quit" << std::endl;
			printMenu=false;
		}
		if (KeyIsPressed()) {
			printMenu=true;
			char nextChar;
			std::cin >> nextChar;
			switch(nextChar) {
				case 'P':
				case 'p':
					Play();
					break;
				case 'S':
				case 's':
					Stop();
					break;
				case 'T':
				case 't':
					SetTempo();
					break;
				case 'Q':
				case 'q':
					stop=true;
					break;
			}
		}
		if (PollForQuitRequest()) {
			stop=true;
		}
		Sleep();
	}
}

int main(int argc, const char* argv[]) {
	try {
#if MAC
		assert(argc>0);
		const std::string appPath=argv[0];
#endif // MAC
#if WINDOWS
		char appPath[1000];
		DWORD result=GetModuleFileNameA(NULL,appPath,1000);
		if (result==0) {
			std::cout << "Failed to get app path" << std::endl;
			throw std::exception();
		}
#endif // WINDOWS
		const std::string command=argc>1?argv[1]:"";
		Open();
		if (command=="registerdevice") {
			RegisterDevice(appPath,true);
			std::cout << "Device registered" << std::endl;
		}
		else if (command=="unregisterdevice") {
			RegisterDevice(appPath,false);
			std::cout << "Device unregistered" << std::endl;
		}
		else {
			ConnectToDevice();
			MainMenu();
		}
		Close();
	}
	catch(...) {
		try {
			Close();
		}
		catch(...) {
		}

		system("pause");
		return EXIT_FAILURE;
	}
	system("pause");
	return EXIT_SUCCESS;
}
