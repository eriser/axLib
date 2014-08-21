#include <iostream>
#include <string>
#include <vector>

#include "MixerApp.h"

const char* kDeviceName = "ReWire sample device";
const char* kMetronomeAudioName = "Metronome";
const char* kSawAudioName = "Saw 1";
const char* kSawEventBusName = "Main bus";
const char* kSawEventChannelName = "Saw 1 synth";


namespace {
	
void SetupMixer(CMixerApp& iMixerApp)
{
	std::cout << "Setup mixer" << std::endl;
	
	
	const std::vector<CReWireDevice*>* deviceList = iMixerApp.GetDeviceList();
	
	CReWireDevice* device = NULL;
	for (std::vector<CReWireDevice*>::const_iterator it = deviceList->begin(); it != deviceList->end(); it++) {
		CReWireDevice* curDevice = *it;
		std::string deviceName = NSDeviceFunctions::GetDeviceName(*curDevice);
		std::cout << "Device: " << deviceName << std::endl;
		if (deviceName == kDeviceName) {
			device = curDevice;
			break;
		}
	}
	
	
	if (device != NULL) {
		CMixerParams mixerParams;
		
		int metronomeChannel = -1;
		int sawChannel = -1;
		const ReWire::ReWireDeviceInfo* info = device->GetDeviceInfo();
		if (info->fChannelCount > 0) {
			for (unsigned int deviceChannel = 0; deviceChannel < static_cast<unsigned int>(info->fChannelCount); deviceChannel++) {
				if (std::string(info->fChannelNames[deviceChannel]) == kMetronomeAudioName) {
					metronomeChannel = deviceChannel;
				}
				else if (std::string(info->fChannelNames[deviceChannel]) == kSawAudioName) {
					sawChannel = deviceChannel;
				}
			}
		}
		
		if (metronomeChannel >= 0) {
			iMixerApp.AssignDeviceAndChannelToReWireChannel(device, metronomeChannel, 0);
			mixerParams.fMixerLevel[0] = 120;
		}
		if (sawChannel >= 0) {
			iMixerApp.AssignDeviceAndChannelToReWireChannel(device, sawChannel, 1);
			mixerParams.fMixerLevel[1] = 64;
		}
		
		unsigned short eventBus = 0;
		unsigned short eventChannel = 0;
		std::vector<std::string> busNames;
		std::vector<int> busIndexes;
		NSDeviceFunctions::GetBusNames(*device, busNames, busIndexes);
		for (unsigned int busIndex = 0; busIndex < busNames.size() ; busIndex++) {
			if (busNames[busIndex] == kSawEventBusName) {
				eventBus = busIndexes[busIndex];
				break;
			}
		}
		
		std::vector<std::string> channelNames;
		std::vector<int> channelIndexes;
		NSDeviceFunctions::GetChannelNames(*device, eventBus, channelNames, channelIndexes);
		for (unsigned int channelIndex = 0; channelIndex < channelNames.size(); channelIndex++) {
			if (channelNames[channelIndex] == kSawEventChannelName) {
				eventChannel = channelIndexes[channelIndex];
				break;
			}
		}
		
		iMixerApp.SetParams(mixerParams);
		
		iMixerApp.SetEventDestinationDevice(device);
		iMixerApp.SetEventBusDestination(eventBus);
		iMixerApp.SetEventChannelDestination(eventChannel);
		iMixerApp.SetEventDestinationSelected(true);
	}
}

void DisableMixer(CMixerApp& iMixerApp)
{
	for (unsigned int mixerChannel=0; mixerChannel < kMixerChannelCount; mixerChannel++) {
		iMixerApp.AssignDeviceAndChannelToReWireChannel(NULL, 0, mixerChannel);
	}
	
	iMixerApp.SetEventDestinationSelected(false);

}

void PrintChannels(CMixerApp& iApp)
{

	std::cout << "******** Mixer channels ********" << std::endl;
	for (unsigned int mixerChannel=0; mixerChannel < kMixerChannelCount; mixerChannel++) {
		std::cout << "Mixer#" << mixerChannel << " ";
		
		CReWireDevice* device = NULL;
		long deviceChannelIndex = -1;
		iApp.GetAssignedDeviceAndChannelToReWireChannel(mixerChannel, device, deviceChannelIndex);
		
		if (device) {
			const ReWire::ReWireDeviceInfo* info = device->GetDeviceInfo();
			std::cout << "'" << info->fName << "'";
			
			std::cout << "/'" << info->fChannelNames[deviceChannelIndex] << "'";
			
			
			const CMixerParams& params = iApp.GetParams();
			int mixerLevel = params.fMixerLevel[mixerChannel];
			std::cout << " @ " << mixerLevel;

		}
		else {
			std::cout << "-/- -";
		}
		std::cout << std::endl;
	}
	std::cout << "*****************************" << std::endl;
}

} // anonymous namespace


int main(int /*argc*/, char* /*argv[]*/)
{
	CMixerApp mixerApp;
	bool rewireClosed = true;
	try {
		mixerApp.OpenReWire();
		rewireClosed = false;
	}
	catch (...) {
		std::cout << "Couldn't open ReWire - quitting." << std::endl;
		return 1;
	}
	
	SetupMixer(mixerApp);
	
	PrintChannels(mixerApp);

	bool requestedQuit = false;
	do {
		std::string command;
		
		requestedQuit = false;
		std::cout << "Command (play, stop, quit): " << std::flush;
		std::cin >> command;
		if (command == "play") {
			mixerApp.Play();
		}
		else if (command == "stop") {
			mixerApp.Stop();
		}
		else if (command == "quit") {
			requestedQuit = true;
			DisableMixer(mixerApp);
			rewireClosed = mixerApp.TryCloseReWire();
			if (!rewireClosed) {
				std::cout << "Couldn't close ReWire. Plase quit all panel apps" << std::endl;
			}
		}
		else {
			std::cout << "Unknown command: " << command << std::endl;
		}
		
	} while (!requestedQuit || !rewireClosed);
		
	return 0;	
}

