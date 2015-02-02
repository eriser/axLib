#ifndef SAMPLEUTILS_H
#define SAMPLEUTILS_H

#include "ReWireSDK/ReWire.h"
#include <string>
#ifdef WINDOWS
#include <Windows.h>
#endif // WINDOWS 


namespace NSSampleUtils
{
	std::string GetErrorString(ReWire::ReWireError result);
	
	class CSpinLock
	{
		public: CSpinLock();
		public: ~CSpinLock();
			
		public: bool TryLock();
		public: void Lock();
		public: void Unlock();
			
#if WINDOWS
		typedef LONG TAtomicValue;
#endif // WINDOWS
#if MAC
		typedef int32_t TAtomicValue;
#endif // MAC
		private: volatile TAtomicValue fLockTaken;
	};
	
	
	class CScopedLocker
	{
		public: CScopedLocker(CSpinLock& iSpinLock) : fLock(iSpinLock) { fLock.Lock(); }
		public: ~CScopedLocker() { fLock.Unlock(); }
			
		private: CSpinLock& fLock;
	};

}

#endif // SAMPLEUTILS_H
