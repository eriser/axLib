#include "SampleUtils.h"

#include <cassert>
#if MAC
#include <libkern/OSAtomic.h>
#endif // MAC

namespace {

#if WINDOWS
	bool PortableCompareAndSwap(volatile LONG* iVar, LONG iRequiredOld, LONG iNew) {
		LONG oldValue = InterlockedCompareExchange(iVar, iNew, iRequiredOld);
		bool changed = (oldValue == iRequiredOld);
		return changed;
	}

	void SleepMinimal() {
		Sleep(1);
	}
#endif // WINDOWS

#if MAC
	bool PortableCompareAndSwap(volatile int32_t* iVar, int32_t iRequiredOld, int32_t iNew) {
		bool changed = OSAtomicCompareAndSwap32Barrier(iRequiredOld, iNew, iVar);
		return changed;
	}

	void SleepMinimal() {
		usleep(1);
	}
#endif // MAC

} // anonymous namespace



std::string NSSampleUtils::GetErrorString(ReWire::ReWireError result) {
	switch (result) {
		case ReWire::kReWireError_ReWireOpenByOtherApplication:
			return "kReWireError_ReWireOpenByOtherApplication";
		case ReWire::kReWireError_DLLNotFound:
			return "kReWireError_DLLNotFound";
		case ReWire::kReWireError_DLLTooOld:
			return "kReWireError_DLLTooOld";
		case ReWire::kReWireError_UnableToLoadDLL:
			return "kReWireError_UnableToLoadDLL";
		case ReWire::kReWireError_NotEnoughMemoryForDLL:
			return "kReWireError_NotEnoughMemoryForDLL";
		case ReWire::kReWireError_OutOfMemory:
			return "kReWireError_OutOfMemory";
		case ReWire::kReWireError_UnableToOpenDevice:
			return "kReWireError_UnableToOpenDevice";
		case ReWire::kReWireError_AlreadyExists:
			return "kReWireError_AlreadyExists";
		case ReWire::kReWireError_NotFound:
			return "kReWireError_NotFound";
		case ReWire::kReWireError_Busy:
			return "kReWireError_Busy";
		case ReWire::kReWireError_BufferFull:
			return "kReWireError_BufferFull";
		case ReWire::kReWireError_OSVersionNotSupported:
			return "kReWireError_OSVersionNotSupported";
		case ReWire::kReWireError_PortNotConnected:
			return "kReWireError_PortNotConnected";
		case ReWire::kReWireError_PortConnected:
			return "kReWireError_PortConnected";
		case ReWire::kReWireError_PortStale:
			return "kReWireError_PortStale";
		case ReWire::kReWireError_ReadError:
			return "kReWireError_ReadError";
		case ReWire::kReWireError_NoMoreMessages:
			return "kReWireError_NoMoreMessages";
		case ReWire::kReWireError_AccessDenied:
			return "kReWireError_AccessDenied";
		case ReWire::kReWireImplError_ReWireNotOpen:
			return "kReWireImplError_ReWireNotOpen";
		case ReWire::kReWireImplError_ReWireAlreadyOpen:
			return "kReWireImplError_ReWireAlreadyOpen";
		case ReWire::kReWireImplError_DeviceNotOpen:
			return "kReWireImplError_DeviceNotOpen";
		case ReWire::kReWireImplError_DeviceAlreadyOpen:
			return "kReWireImplError_DeviceAlreadyOpen";
		case ReWire::kReWireImplError_AudioInfoInvalid:
			return "kReWireImplError_AudioInfoInvalid";
		case ReWire::kReWireImplError_InvalidParameter:
			return "kReWireImplError_InvalidParameter";
		case ReWire::kReWireImplError_InvalidSignature:
			return "kReWireImplError_InvalidSignature";
		default:
			assert(false);
	}
	return "";
}


NSSampleUtils::CSpinLock::CSpinLock()
: fLockTaken(0)
{
}

NSSampleUtils::CSpinLock::~CSpinLock()
{
	assert(fLockTaken == 0);
}

bool NSSampleUtils::CSpinLock::TryLock() 
{
	return PortableCompareAndSwap(&fLockTaken, 0, 1);
}

void NSSampleUtils::CSpinLock::Lock()
{
	bool succeeded = false;
	do {
		succeeded = TryLock();
		if (!succeeded) {
			SleepMinimal();
		}
	} while (!succeeded);
	assert(fLockTaken == 1);
}

void NSSampleUtils::CSpinLock::Unlock()
{
	bool changed = PortableCompareAndSwap(&fLockTaken, 1, 0);
	assert(changed);
}
