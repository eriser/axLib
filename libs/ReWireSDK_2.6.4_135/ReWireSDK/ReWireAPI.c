/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

#include "ReWireAPI.h"

/*
 *
 * Local assertion macro.
 *
 */
#include <assert.h>
#define REWIRE_ASSERT(e) assert(e)

#if WINDOWS
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NOMCX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <shlobj.h>
#endif /* WINDOWS */
#if MAC
#include <Carbon/Carbon.h>
#endif /* MAC */

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif /* ndef TRUE */

#ifdef __cplusplus

namespace ReWire {

#endif /* __cplusplus  */

static const short kReWireCompatibleMajor = 1;
static const short kReWireCompatibleMinor = 8;

static signed long gReWireLoadCount = 0;

#if WINDOWS
static HINSTANCE gReWireDLLInstance = 0;
static const char kWinReWireDebugDLLPathName[] = "ReWireDebug.dll";
static const char kWinReWireBetaDLLPathName[] = "ReWireBeta.dll";
static const char kWinReWireReleaseDLLPathName[] = "ReWire.dll";
#endif /* WINDOWS */

#if MAC
static CFBundleRef gReWireDLLInstance = NULL;
static const char kMacReWireDebuggingDLLFileName[] = "ReWire Debugging.bundle";
static const char kMacReWireTestingDLLFileName[] = "ReWire Testing.bundle";
static const char kMacReWireDeploymentDLLFileName[] = "ReWire.bundle";
#endif /* MAC */

static const char kPropellerheadFolderName [] = "Propellerhead Software";
static const char kReWireFolderName [] = "ReWire";













/*
	Win32 platform code
*/

#if WINDOWS

static char WinIsThisWindowsVersionSupported() {
	char result=0;
	OSVERSIONINFO osVersionInfo;
	int bIsWindows98orLater = 0;
	int bIsNT4orLater = 0;

	memset(&osVersionInfo, 0, sizeof(OSVERSIONINFO));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	result = (char)GetVersionEx(&osVersionInfo);
	if (result==0) {
		return FALSE;
	}
	bIsWindows98orLater = (osVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
						   ((osVersionInfo.dwMajorVersion > 4) || ((osVersionInfo.dwMajorVersion == 4) && (osVersionInfo.dwMinorVersion > 0)));
	bIsNT4orLater =	(osVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (osVersionInfo.dwMajorVersion >= 4);
	if (bIsWindows98orLater || bIsNT4orLater) {
		return TRUE;
	} else {
		return FALSE;
	}
}

static char WinGetFileVersion(const char* pathName,short* outRevision, short* outVersion, short* outBuildHi, short* outBuildLo){
	DWORD lpdwHandle=0;
	DWORD lSize;
	UINT lOldErrorMode;
	char result=FALSE;

	lOldErrorMode=SetErrorMode(SEM_NOOPENFILEERRORBOX);

	lSize=GetFileVersionInfoSizeA((char*)pathName,&lpdwHandle);
	if(0!=lSize){
		BOOL resultFlag;
		char* buffer=NULL;

		buffer=(char*)malloc(lSize);
		if(NULL!=buffer){
			resultFlag=GetFileVersionInfoA((char*)pathName,0,lSize,(LPVOID)buffer);
			if(0!=resultFlag){
				VS_FIXEDFILEINFO* lFInfo=NULL;
				UINT lFLength;

				resultFlag=VerQueryValueA((LPVOID)buffer,"\\",(VOID FAR* FAR*)&lFInfo,&lFLength);
				if(0!=resultFlag){
					*outRevision=HIWORD(lFInfo->dwFileVersionMS);
					*outVersion=LOWORD(lFInfo->dwFileVersionMS);
					*outBuildHi=HIWORD(lFInfo->dwFileVersionLS);
					*outBuildLo=LOWORD(lFInfo->dwFileVersionLS);
					result=TRUE;
				}
			}
			free(buffer);
			buffer=NULL;
		}
	}
	SetErrorMode(lOldErrorMode);
	return result;
}

static char VerifyVersionOfOpenReWireDLL(){
	char completeFileName[256];
	DWORD result=GetModuleFileNameA(gReWireDLLInstance,completeFileName,256);
	if(result==0){
		return FALSE;
	}
	{
		short revision;
		short version;
		short buildHi;
		short buildLo;
		WinGetFileVersion(completeFileName,&revision, &version, &buildHi, &buildLo);
		if(revision!=kReWireCompatibleMajor || version < kReWireCompatibleMinor){
			return FALSE;
		}
	}
	return TRUE;
}


static ReWireError LoadReWireDLLFromPath(const char path[]){
	ReWireError rErrorResult = kReWireError_UnableToLoadDLL;

	UINT oldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

	gReWireDLLInstance=LoadLibraryA(path);
	if(NULL==gReWireDLLInstance){
		DWORD error=GetLastError();
		switch(error){
			case ERROR_NOT_ENOUGH_MEMORY:
			case ERROR_OUTOFMEMORY:
			case ERROR_TOO_MANY_OPEN_FILES:
				rErrorResult = kReWireError_OutOfMemory;
				break;
			case ERROR_DLL_NOT_FOUND:
				rErrorResult = kReWireError_DLLNotFound;
				break;
			case ERROR_GEN_FAILURE:
			case ERROR_BAD_FORMAT:
			case ERROR_READ_FAULT:
				rErrorResult = kReWireError_ReadError;
				break;
			default:
				rErrorResult = kReWireError_UnableToLoadDLL;
				break;
		}
	}
	else{
		rErrorResult = kReWireError_NoError;
	}
	SetErrorMode(oldErrorMode);
	return rErrorResult;
}

static ReWireError AppendToPathSafely(char ioCurrentPath[], const char iPathToAppend[]){
	size_t currentLength = strlen(ioCurrentPath);
	size_t appendLength = strlen(iPathToAppend);
	if(currentLength + appendLength + 1 < MAX_PATH){
		strcpy_s(&ioCurrentPath[currentLength], appendLength + 1, iPathToAppend);
		return kReWireError_NoError;
	}
	else{
		return kReWireError_OutOfMemory;
	}
}

static ReWireError GetCommonProgramFilesFolder(char oCommonProgramFilesFolder[MAX_PATH+1]){
	char path[MAX_PATH + 1];
	BOOL okFlag = SHGetSpecialFolderPathA(      
		NULL,
		path,
		CSIDL_PROGRAM_FILES_COMMON,
		FALSE);
	if(okFlag){
		if(AppendToPathSafely(path, "\\") == kReWireError_NoError){
			if(AppendToPathSafely(path, kPropellerheadFolderName) == kReWireError_NoError){
				if(AppendToPathSafely(path, "\\") == kReWireError_NoError){
					if(AppendToPathSafely(path, kReWireFolderName) == kReWireError_NoError){
						if(AppendToPathSafely(path, "\\") == kReWireError_NoError){
							strcpy_s(oCommonProgramFilesFolder, MAX_PATH + 1, path);
							return kReWireError_NoError;
						}
					}
				}
			}
		}
	}
	return kReWireError_DLLNotFound;
}

static ReWireError LoadReWireDLLByName(const char dllName[]){
	char commonProgramFolder[MAX_PATH+1];

	if(GetCommonProgramFilesFolder(commonProgramFolder) == kReWireError_NoError){
		char dllPath[MAX_PATH + 1];
		strcpy_s(dllPath, MAX_PATH + 1, commonProgramFolder);
		if(AppendToPathSafely(dllPath, dllName) == kReWireError_NoError){
			if(LoadReWireDLLFromPath(dllPath) == kReWireError_NoError){
				return kReWireError_NoError;
			}
		}
	}
	return kReWireError_DLLNotFound;
}

static ReWireError LoadReWireDLL(){
	if (FALSE==WinIsThisWindowsVersionSupported()) {
		return kReWireError_OSVersionNotSupported;
	}
	else{
		ReWireError result=LoadReWireDLLByName(kWinReWireDebugDLLPathName);
		if(result!=kReWireError_NoError){
			result=LoadReWireDLLByName(kWinReWireBetaDLLPathName);
		}
		if(result!=kReWireError_NoError){
			result=LoadReWireDLLByName(kWinReWireReleaseDLLPathName);
		}
		return result;
	}
}

static REWIREPROC FindReWireDLLFunction(const char functionName[]){
	REWIREPROC result=(REWIREPROC)GetProcAddress(gReWireDLLInstance,functionName);
	return result;
}

static void UnloadReWireDLL(){
	if(NULL!=gReWireDLLInstance){
		FreeLibrary(gReWireDLLInstance);
		gReWireDLLInstance=NULL;
	}
}

static char IsReWireLoadedByThisInstance(){
	if(NULL==gReWireDLLInstance){
		return FALSE;
	}else{
		return TRUE;
	}
}

#endif /* WINDOWS */











/*
	MacOS platform code
*/

#if MAC

static char MacResolve(FSRef* inOutMacFileRef) {
	Boolean targetIsFolderFlag;
	Boolean wasAliasedFlag;
	OSErr err;

	REWIRE_ASSERT(inOutMacFileRef != NULL);

	err = FSResolveAliasFile(inOutMacFileRef, TRUE, &targetIsFolderFlag, &wasAliasedFlag);
	if (err == noErr) {
		return TRUE;
	}
	else {
		REWIRE_ASSERT(FALSE);
		return FALSE;
	}
}

static char MacGetNamedTargetInFolder(FSRef* outFileRef, const FSRef* folderRef, const char name[]) {
	CFStringRef nameStringRef;
	UniChar stringBuffer[255];
	CFRange range;
	OSErr err;

	REWIRE_ASSERT(outFileRef != NULL);
	REWIRE_ASSERT(folderRef != NULL);
	REWIRE_ASSERT(name != NULL);

	/* Convert the name from UTF8 to UTF16 and put it into stringBuffer */
	nameStringRef = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingUTF8);
	if (nameStringRef == NULL) {
		return FALSE;
	}
	range.location = 0;
	range.length = CFStringGetLength(nameStringRef);
	REWIRE_ASSERT(range.length <= 255);
	CFStringGetCharacters(nameStringRef, range, &stringBuffer[0]);
	CFRelease(nameStringRef);
	nameStringRef = NULL;

	err = FSMakeFSRefUnicode(folderRef, range.length, stringBuffer, kTextEncodingUnknown, outFileRef);
	if (err == noErr) {
		if (MacResolve(outFileRef)) {
			return TRUE;
		}
	}
		
	return FALSE;
}

static char MacGetSpecialFolder(FSRef* outFoundFolderRef, FSVolumeRefNum domain, OSType dirType) {
	OSErr err;

	REWIRE_ASSERT(outFoundFolderRef != NULL);

	err = FSFindFolder(domain, dirType, kDontCreateFolder, outFoundFolderRef);
	if (err == noErr) {
		return TRUE;
	}
	return FALSE;
}

static char VerifyVersionOfOpenReWireDLL() {
	UInt32 packedVersion;
	UInt8 majorDigit;
	UInt8 minorDigit;
	char result = FALSE;

	if (gReWireDLLInstance != NULL) {
		packedVersion = CFBundleGetVersionNumber(gReWireDLLInstance);
		majorDigit = packedVersion >> 24;
		minorDigit = (packedVersion >> 20) & 0xf;
		if (majorDigit == kReWireCompatibleMajor && minorDigit >= kReWireCompatibleMinor) {
			result = TRUE;
		}
	}
	else {
		REWIRE_ASSERT(FALSE);
	}
	return result;
}

static char MacGetMacURLRefForReWireDLLByName(CFURLRef* outFoundURL, const char dllName[]) {
	unsigned int i;
	short domains[] = { kLocalDomain, kNetworkDomain };
	FSRef folderRef;
	FSRef macFileRef;

	REWIRE_ASSERT(outFoundURL != NULL);
	REWIRE_ASSERT(dllName != NULL);

	/* Mac OS X search: application support folder in all domains */
	for (i = 0; i < (sizeof(domains) / sizeof(short)); i ++) {
		if (MacGetSpecialFolder(&folderRef, domains[i], kApplicationSupportFolderType)) {
			if (MacGetNamedTargetInFolder(&folderRef, &folderRef, kPropellerheadFolderName)) {
				if (MacGetNamedTargetInFolder(&folderRef, &folderRef, kReWireFolderName)) {
					if (MacGetNamedTargetInFolder(&macFileRef, &folderRef, dllName)) {
						*outFoundURL = CFURLCreateFromFSRef(kCFAllocatorDefault, &macFileRef);
						REWIRE_ASSERT(*outFoundURL != NULL);
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

static char MacGetURLRefForReWireDLL(CFURLRef* outFoundURL) {
	REWIRE_ASSERT(outFoundURL != NULL);

	if (MacGetMacURLRefForReWireDLLByName(outFoundURL, kMacReWireDebuggingDLLFileName)) {
		REWIRE_ASSERT(*outFoundURL != NULL);
		return TRUE;
	}
	if (MacGetMacURLRefForReWireDLLByName(outFoundURL, kMacReWireTestingDLLFileName)) {
		REWIRE_ASSERT(*outFoundURL != NULL);
		return TRUE;
	}
	if (MacGetMacURLRefForReWireDLLByName(outFoundURL, kMacReWireDeploymentDLLFileName)) {
		REWIRE_ASSERT(*outFoundURL != NULL);
		return TRUE;
	}
	return FALSE;
}

static ReWireError LoadReWireDLL() {
	ReWireError result = kReWireError_UnableToLoadDLL;
	CFURLRef macFileURL = NULL;

	REWIRE_ASSERT(gReWireDLLInstance == NULL);

	if (MacGetURLRefForReWireDLL(&macFileURL)) {
		REWIRE_ASSERT(macFileURL != NULL);
		gReWireDLLInstance = CFBundleCreate(kCFAllocatorDefault, macFileURL);
		CFRelease(macFileURL);
		macFileURL = NULL;
		if (gReWireDLLInstance == NULL) {
			result = kReWireError_UnableToLoadDLL;
		}
		else {
			result = kReWireError_NoError;
		}
	}
	else {
		result = kReWireError_DLLNotFound;
	}
	return result;
}

static REWIREPROC FindReWireDLLFunction(const char functionName[]) {
	CFStringRef functionNameStringRef;
	REWIREPROC result;

	REWIRE_ASSERT(functionName != NULL);

	if (gReWireDLLInstance != NULL) {
		functionNameStringRef = CFStringCreateWithCString(kCFAllocatorDefault, functionName, kCFStringEncodingUTF8);
		if (functionNameStringRef == NULL) {
			REWIRE_ASSERT(FALSE);
			return NULL;
		}
		REWIRE_ASSERT(sizeof(unsigned long) >= sizeof(void*));
		result = (REWIREPROC)((unsigned long)CFBundleGetFunctionPointerForName(gReWireDLLInstance, functionNameStringRef));
		CFRelease(functionNameStringRef);
		functionNameStringRef = NULL;
		REWIRE_ASSERT(result != NULL);
		return result;
	}
	return NULL;
}

static void UnloadReWireDLL(){
	if (gReWireDLLInstance != NULL) {
		CFRelease(gReWireDLLInstance);
		gReWireDLLInstance = NULL;
	}
	else {
		REWIRE_ASSERT(FALSE);
	}
}

static char IsReWireLoadedByThisInstance() {
	if (gReWireDLLInstance != NULL) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

#endif /* MAC */









/* 

	To support a new platform these functions need to be implemented:

		ReWireError LoadReWireDLL();
		char IsReWireLoadedByThisInstance();
		char VerifyVersionOfOpenReWireDLL();
		REWIREPROC FindReWireDLLFunction(const char functionName[]){
		void UnloadReWireDLL();
*/


/*
	Portable code
*/

ReWireError ReWireLoadReWireSharedLibrary(void) {
	if(IsReWireLoadedByThisInstance()){
		REWIRE_ASSERT(gReWireLoadCount>0);
		gReWireLoadCount++;
		return kReWireError_NoError;
	}
	else {
		REWIRE_ASSERT(gReWireLoadCount==0);
		{
			ReWireError result=LoadReWireDLL();
			if(result==kReWireError_NoError){
				if(VerifyVersionOfOpenReWireDLL()){
					gReWireLoadCount=1;
					return kReWireError_NoError;
				}
				else{
					return kReWireError_DLLTooOld;
				}
			}
			else{
				return result;
			}
		}
	}
}

REWIREPROC ReWireFindReWireSharedLibraryFunction(const char functionName[]) {
	return (REWIREPROC)FindReWireDLLFunction(functionName);
}

ReWireError ReWireUnloadReWireSharedLibrary(void) {
	REWIRE_ASSERT(gReWireLoadCount>0);
	gReWireLoadCount--;
	if(0==gReWireLoadCount){
		UnloadReWireDLL();
		return kReWireError_NoError;
	}
	else{
		return kReWireError_NoError;
	}
}

#ifdef __cplusplus

}

#endif

