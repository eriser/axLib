#ifndef REWIRE_H_
#define REWIRE_H_
/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

/* Structures/types and constants for ReWire SDK */

	/* Is this windows platform? */
#ifndef WINDOWS
	#ifdef _WINDOWS
		#define WINDOWS 1
	#endif /* _WINDOWS */
#endif /* WINDOWS */
#ifndef WINDOWS
	#ifdef WIN32
		#define WINDOWS 1
	#endif /* WIN32 */
#endif /* WINDOWS */
#ifndef WINDOWS
	#ifdef PC
		#define WINDOWS 1
	#endif /* _WINDOWS */
#endif /* WINDOWS */
#ifndef WINDOWS
	#ifdef _WIN32
		#define WINDOWS 1
	#endif /* _WIN32 */
#endif /* WINDOWS */

#ifndef WINDOWS
	/* Is this macintosh platform? */
	#ifndef MAC
		#ifdef _MAC
			#define MAC 1
		#endif /* _MAC */
	#endif /* MAC */
	#ifndef MAC
		#ifdef TARGET_OS_MAC
			#define MAC 1
		#endif /* TARGET_OS_MAC */
	#endif /* MAC */
	#ifndef MAC
		#ifdef OSX
			#define MAC 1
		#endif  /* OSX */
	#endif /* MAC */
#else
	#ifdef MAC
		#error "Can not determine platform, please choose only one platform; WINDOWS or MAC"
	#endif /* MAC */
#endif /* WINDOWS */

#ifndef WINDOWS
	#ifndef MAC
		#error "Can not determine platform, please set preprocessor define WINDOWS or MAC to 1"
	#endif /* MAC */
#endif /* WINDOWS */

#ifndef DEBUG
	#ifdef _DEBUG
		#define DEBUG 1
	#endif /* _DEBUG */
#endif /* DEBUG */
#if DEBUG!=0
	#ifdef NDEBUG
		#error "Can not determine debug/release mode, please set preprocessor define DEBUG 1 or 0"
	#endif /* NDEBUG */
#endif /* DEBUG */
#if DEBUG==0
	#ifndef NDEBUG
		#define NDEBUG
	#endif
#endif


#if defined(_MSC_VER) 
	#pragma pack(push,8)
#elif defined(__GNUC__) && defined(MAC)
	#pragma pack(push,4)
#else
#error "REWIRE: Can not set up struct alignement."
#endif /* defined(_MSC_VER) */

#ifdef __cplusplus
namespace ReWire {
extern "C" {
#endif /* def __cplusplus */



#if !REWIRE_TYPES_DEFINED
#if defined(_MSC_VER)
/* Safe guess on MSC */
typedef char ReWire_char_t;
typedef unsigned char ReWire_uchar_t;
typedef __int8 ReWire_int8_t;
typedef unsigned __int8 ReWire_uint8_t;
typedef __int16 ReWire_int16_t;
typedef unsigned __int16 ReWire_uint16_t;
typedef __int32 ReWire_int32_t;
typedef unsigned __int32 ReWire_uint32_t;
#elif defined(__GNUC__) && defined(MAC)
/* Should be safe, but guessing stops here */
typedef char ReWire_char_t;
typedef unsigned char ReWire_uchar_t;
typedef char ReWire_int8_t;
typedef unsigned char ReWire_uint8_t;
typedef short ReWire_int16_t;
typedef unsigned short ReWire_uint16_t;
typedef int ReWire_int32_t;
typedef unsigned int ReWire_uint32_t;
#else
#error "REWIRE: ReWire types not defined."
#endif /* defined(_MSC_VER) */
#endif /* !REX_TYPES_DEFINED */



enum {
	/* Not really errors */
	kReWireError_ReWireOpenByOtherApplication	= 7,

	/* Run-time errors */
	kReWireError_DLLNotFound					= 100,
	kReWireError_DLLTooOld						= 101,
	kReWireError_UnableToLoadDLL				= 9,
	kReWireError_NotEnoughMemoryForDLL			= 2,
	kReWireError_OutOfMemory					= 8,
	kReWireError_UnableToOpenDevice				= 5,
	kReWireError_AlreadyExists					= 105,
	kReWireError_NotFound						= 106,
	kReWireError_Busy							= 107,
	kReWireError_BufferFull						= 108,
	kReWireError_OSVersionNotSupported			= 109,
	kReWireError_PortNotConnected				= 110,
	kReWireError_PortConnected					= 111,
	kReWireError_PortStale						= 112,
	kReWireError_ReadError						= 113,
	kReWireError_NoMoreMessages					= 114,
	kReWireError_AccessDenied					= 115,

	/* Implementation errors */					
	kReWireImplError_ReWireNotOpen				= 200,
	kReWireImplError_ReWireAlreadyOpen			= 6,
	kReWireImplError_DeviceNotOpen				= 202,
	kReWireImplError_DeviceAlreadyOpen			= 4,
	kReWireImplError_AudioInfoInvalid			= 204,
	kReWireImplError_InvalidParameter			= 3,
	kReWireImplError_InvalidSignature			= 206,

	/* DLL error - call the cops! */			
	kReWireError_Undefined						= 666,

	kReWireError_NoError						= 1
};
typedef ReWire_int32_t ReWireError;

#define kReWirePPQ						15360
#define kReWireDeviceNameSize			31
#define kReWireAudioChannelCount		256
#define kReWireAudioChannelNameSize		31
#define kReWireEventControllerNameSize	31
#define kReWireEventNoteNameSize		31
#define kReWireEventChannelNameSize		31
#define kReWireEventBusNameSize			31
#define kReWireEventControllerCount		128
#define kReWireEventNoteCount			128
#define kReWireEventChannelCount		16
#define kReWireEventBusCount			256
#define kReWireReservedEventBusIndex	255

#define REWIRE_BITFIELD_SIZE(count) \
	((count+31)/32)

typedef struct {
	ReWire_uint32_t fStructSize;
	ReWire_char_t fName[kReWireDeviceNameSize + 1];
	ReWire_int32_t fChannelCount;
	ReWire_char_t fChannelNames[kReWireAudioChannelCount][kReWireAudioChannelNameSize + 1];
	ReWire_uint32_t fSuggestedDefaultOnChannelsBitField[REWIRE_BITFIELD_SIZE(kReWireAudioChannelCount)];
	ReWire_uint32_t fStereoPairsBitField[REWIRE_BITFIELD_SIZE(kReWireAudioChannelCount / 2)];
	ReWire_uint32_t fMaxEventOutputBufferSize;
	ReWire_int32_t fCodedForReWireVersion;
} ReWireDeviceInfo;



typedef struct {
	ReWire_uint32_t fStructSize;
	ReWire_int32_t fSampleRate;
	ReWire_int32_t fMaxBufferSize;
} ReWireAudioInfo;


typedef struct {
	ReWire_uint32_t fStructSize;
	ReWireAudioInfo	fAudioInfo;
} ReWireOpenInfo;


typedef struct {
	ReWire_uint16_t fMIDIBusIndex;
	ReWire_uint16_t fChannel;
} ReWireEventTarget;



enum {
	kReWireMIDIEvent	=	10,
	kReWireRequestRepositionEvent	=	11,
	kReWireRequestSignatureEvent	=	12,
	kReWireRequestTempoEvent		=	13,
	kReWireRequestLoopEvent			=	14,
	kReWireRequestPlayEvent			=	15,
	kReWireRequestStopEvent			=	16,
	kReWireRevealEventTargetEvent	=	100,
	kReWireEventTargetChangedEvent	=	101
};
typedef ReWire_int32_t ReWireEventType;



typedef struct {
	ReWire_uint16_t fEventType;
	ReWire_uint8_t fReserved1;
	ReWire_uint8_t fReserved2;

	ReWire_uint32_t fUsedByExplitEventStructs1;
	ReWire_uint32_t fUsedByExplitEventStructs2;
	ReWire_uint32_t fUsedByExplitEventStructs3;
	ReWire_uint32_t fUsedByExplitEventStructs4;
	ReWire_uint32_t fUsedByExplitEventStructs5;
} ReWireEvent;



enum {
	kReWireMIDINoteOff		=	0x80,
	kReWireMIDINoteOn		=	0x90,
	kReWireMIDIPolyPress	=	0xa0,
	kReWireMIDIControl		=	0xb0,
	kReWireMIDIProgram		=	0xc0,
	kReWireMIDIMonopress	=	0xd0,
	kReWireMIDIPitchbend	=	0xe0
};
typedef ReWire_int32_t ReWireMIDIEventType;

typedef struct {
	ReWire_uint16_t fEventType;
	ReWire_uint8_t fReserved1;
	ReWire_uint8_t fReserved2;

	ReWire_int32_t fRelativeSamplePos;
	ReWire_uint32_t fReserved3;
	ReWireEventTarget fEventTarget;
	ReWire_uint16_t fMIDIEventType;
	ReWire_uint16_t fData1;
	ReWire_uint16_t fData2;
	ReWire_uint16_t fReserved4;
} ReWireMIDIEvent;



typedef struct {
	ReWire_uint16_t fEventType;
	ReWire_uint8_t fReserved1;
	ReWire_uint8_t fReserved2;

	ReWireEventTarget fEventTarget;
	ReWire_uint32_t fReserved3;
	ReWire_uint32_t fReserved4;
	ReWire_uint32_t fReserved5;
	ReWire_uint32_t fReserved6;
} ReWireRevealEventTargetEvent;



typedef struct {
	ReWire_uint16_t fEventType;
	ReWire_uint8_t fReserved1;
	ReWire_uint8_t fReserved2;

	ReWire_uint8_t fSignatureNumerator;
	ReWire_uint8_t fSignatureDenominator;
	ReWire_uint8_t fReserved3;
	ReWire_uint8_t fReserved4;
	ReWire_uint32_t fReserved5;
	ReWire_uint32_t fReserved6;
	ReWire_uint32_t fReserved7;
	ReWire_uint32_t fReserved8;
} ReWireRequestSignatureEvent;



typedef struct {
	ReWire_uint16_t fEventType;
	ReWire_uint8_t fReserved1;
	ReWire_uint8_t fReserved2;

	ReWire_uint32_t fTempo;
	ReWire_uint32_t fReserved3;
	ReWire_uint32_t fReserved4;
	ReWire_uint32_t fReserved5;
	ReWire_uint32_t fReserved6;
} ReWireRequestTempoEvent;



typedef struct {
	ReWire_uint16_t fEventType;
	ReWire_uint8_t fReserved1;
	ReWire_uint8_t fReserved2;

	ReWire_int32_t fLoopStartPPQ15360Pos;
	ReWire_int32_t fLoopEndPPQ15360Pos;
	ReWire_uint8_t fLoopOn;
	ReWire_uint8_t fReserved3;
	ReWire_uint8_t fReserved4;
	ReWire_uint8_t fReserved5;
	ReWire_uint32_t fReserved6;
	ReWire_uint32_t fReserved7;
} ReWireRequestLoopEvent;



typedef struct {
	ReWire_uint16_t fEventType;
	ReWire_uint8_t fReserved1;
	ReWire_uint8_t fReserved2;

	ReWire_int32_t fPPQ15360Pos;
	ReWire_uint32_t fReserved3;
	ReWire_uint32_t fReserved4;
	ReWire_uint32_t fReserved5;
	ReWire_uint32_t fReserved6;
} ReWireRequestRepositionEvent;



typedef struct {
	ReWire_uint16_t fEventType;
	ReWire_uint8_t fReserved1;
	ReWire_uint8_t fReserved2;

	ReWire_uint32_t fReserved3;
	ReWire_uint32_t fReserved4;
	ReWire_uint32_t fReserved5;
	ReWire_uint32_t fReserved6;
	ReWire_uint32_t fReserved7;
} ReWireRequestPlayEvent;



typedef struct {
	ReWire_uint16_t fEventType;
	ReWire_uint8_t fReserved1;
	ReWire_uint8_t fReserved2;

	ReWire_uint32_t fReserved3;
	ReWire_uint32_t fReserved4;
	ReWire_uint32_t fReserved5;
	ReWire_uint32_t fReserved6;
	ReWire_uint32_t fReserved7;
} ReWireRequestStopEvent;


typedef struct {
	ReWire_uint16_t fEventType;
	ReWire_uint8_t fReserved1;
	ReWire_uint8_t fReserved2;

	ReWire_uint32_t fReserved3;
	ReWire_uint32_t fReserved4;
	ReWire_uint32_t fReserved5;
	ReWire_uint32_t fReserved6;
	ReWire_uint32_t fReserved7;
} ReWireEventTargetChangedEvent;



typedef struct {
	ReWire_uint32_t fBufferStructSize;
	ReWire_uint32_t fEventStructSize;
	ReWire_uint32_t fCount;
	ReWire_uint32_t fMaxEventCount;
	ReWireEvent* fEventBuffer;
} ReWireEventBuffer;






enum {
	kReWireEventControllerTypeUnused	= 0,
	kReWireEventControllerTypeContinous	= 1,
	kReWireEventControllerTypeBipolar	= 2,
	kReWireEventControllerTypeStep		= 3
};
typedef ReWire_int32_t ReWireEventControllerType;
	
/*	44 bytes	*/
typedef struct {
	ReWire_uint32_t fStructSize;
	ReWire_uint8_t fType;
	ReWire_uint8_t fReserved;
	ReWire_uint16_t fMinValue;
	ReWire_uint16_t fMaxValue;
	ReWire_char_t fControllerName[kReWireEventControllerNameSize+1];
} ReWireEventControllerInfo;



enum {
	kReWireEventNoteTypeUnused	= 0,
	kReWireEventNoteTypeNote	= 1,
	kReWireEventNoteTypeTrigg	= 2
};
typedef ReWire_int32_t ReWireEventNoteType;

typedef struct {
	ReWire_uint32_t fStructSize;
	ReWire_uint8_t fType;
	ReWire_uint8_t fReserved;
	ReWire_char_t fKeyName[kReWireEventNoteNameSize+1];
} ReWireEventNoteInfo;



typedef struct {
	ReWire_uint32_t fStructSize;
	ReWire_uint32_t fUsedControllerBitField[REWIRE_BITFIELD_SIZE(kReWireEventControllerCount)];
	ReWire_uint32_t fUsedNoteBitField[REWIRE_BITFIELD_SIZE(kReWireEventNoteCount)];
	ReWire_char_t fChannelName[kReWireEventChannelNameSize+1];
} ReWireEventChannelInfo;



typedef struct {
	ReWire_uint32_t fStructSize;
	ReWire_uint32_t fUsedChannelBitField[REWIRE_BITFIELD_SIZE(kReWireEventChannelCount)];
	ReWire_char_t fBusName[kReWireEventBusNameSize+1];
} ReWireEventBusInfo;



typedef struct {
	ReWire_uint32_t fStructSize;
	ReWire_uint32_t fUsedBusBitField[REWIRE_BITFIELD_SIZE(kReWireEventBusCount)];
} ReWireEventInfo;






enum {
	kReWirePlayModeStop			= 0,
	kReWirePlayModeKeepPlaying	= 1,
	kReWirePlayModeChaseAndPlay	= 2
};
typedef ReWire_int32_t ReWirePlayMode;

	
typedef struct {
	ReWire_uint32_t fStructSize;
	ReWireEventBuffer fEventInBuffer;
	ReWire_uint32_t fRequestedChannelsBitField[REWIRE_BITFIELD_SIZE(kReWireAudioChannelCount)];
	float* fAudioBuffers[kReWireAudioChannelCount];
	ReWire_int32_t fPPQ15360TickOfBatchStart;
	ReWire_uint32_t fFramesToRender;
	ReWire_uint32_t fPlayMode;
	ReWire_uint32_t fTempo;
	ReWire_uint32_t fSignatureNumerator;
	ReWire_uint32_t fSignatureDenominator;
	ReWire_int32_t fLoopStartPPQ15360Pos;
	ReWire_int32_t fLoopEndPPQ15360Pos;
	ReWire_uint32_t fLoopOn;
} ReWireDriveAudioInputParams;



typedef struct {
	ReWire_uint32_t fStructSize;
	ReWireEventBuffer fEventOutBuffer;
	ReWire_uint32_t fServedChannelsBitField[REWIRE_BITFIELD_SIZE(kReWireAudioChannelCount)];
} ReWireDriveAudioOutputParams;



typedef struct {
	ReWire_uint32_t fStructSize;
	ReWire_uint32_t fDeviceToPanelFIFOSize;
	ReWire_uint32_t fPanelToDeviceFIFOSize;
} ReWirePipeInfo;












extern void ReWireClearBitField(ReWire_uint32_t bitField[],ReWire_uint32_t bitCount);
extern void ReWireSetBitInBitField(ReWire_uint32_t bitField[],ReWire_uint16_t bitIndex);
extern void ReWireClearBitInBitField(ReWire_uint32_t bitField[],ReWire_uint16_t bitIndex);
extern ReWire_int8_t ReWireIsBitInBitFieldSet(const ReWire_uint32_t bitField[],ReWire_uint16_t bitIndex);
extern void ReWireSetBitRangeInBitField(ReWire_uint32_t bitField[],ReWire_uint16_t bitIndexStart,ReWire_uint16_t bitCount);

extern const ReWireMIDIEvent* ReWireCastToMIDIEvent(const ReWireEvent* event);
extern const ReWireRequestRepositionEvent* ReWireCastToRequestRepositionEvent(const ReWireEvent* event);
extern const ReWireRequestSignatureEvent* ReWireCastToRequestSignatureEvent(const ReWireEvent* event);
extern const ReWireRequestTempoEvent* ReWireCastToRequestTempoEvent(const ReWireEvent* event);
extern const ReWireRequestLoopEvent* ReWireCastToRequestLoopEvent(const ReWireEvent* event);
extern const ReWireRequestPlayEvent* ReWireCastToRequestPlayEvent(const ReWireEvent* event);
extern const ReWireRequestStopEvent* ReWireCastToRequestStopEvent(const ReWireEvent* event);
extern const ReWireEventTargetChangedEvent* ReWireCastToEventTargetChangedEvent(const ReWireEvent* event);
extern const ReWireRevealEventTargetEvent* ReWireCastToRevealEventTargetEvent(const ReWireEvent* event);

extern ReWireMIDIEvent* ReWireConvertToMIDIEvent(ReWireEvent* event,const ReWireEventTarget* eventTarget);
extern ReWireRequestRepositionEvent* ReWireConvertToRequestRepositionEvent(ReWireEvent* event);
extern ReWireRequestSignatureEvent* ReWireConvertToRequestSignatureEvent(ReWireEvent* event);
extern ReWireRequestTempoEvent* ReWireConvertToRequestTempoEvent(ReWireEvent* event);
extern ReWireRequestLoopEvent* ReWireConvertToRequestLoopEvent(ReWireEvent* event);
extern ReWireRequestPlayEvent* ReWireConvertToRequestPlayEvent(ReWireEvent* event);
extern ReWireRequestStopEvent* ReWireConvertToRequestStopEvent(ReWireEvent* event);
extern ReWireEventTargetChangedEvent* ReWireConvertToEventTargetChangedEvent(ReWireEvent* event);
extern ReWireRevealEventTargetEvent* ReWireConvertToRevealEventTargetEvent(ReWireEvent* event);

extern void ReWirePrepareDeviceInfo(ReWireDeviceInfo* structure);
extern void ReWirePrepareOpenInfo(ReWireOpenInfo* structure,ReWire_int32_t sampleRate,ReWire_int32_t maxFrameCount);
extern void ReWirePrepareAudioInfo(ReWireAudioInfo* structure,ReWire_int32_t sampleRate,ReWire_int32_t maxFrameCount);
extern void ReWirePrepareEventTarget(ReWireEventTarget* structure,ReWire_uint16_t bus,ReWire_uint16_t channel);
extern void ReWirePrepareEventControllerInfo(ReWireEventControllerInfo* structure);
extern void ReWirePrepareEventNoteInfo(ReWireEventNoteInfo* structure);
extern void ReWirePrepareEventChannelInfo(ReWireEventChannelInfo* structure);
extern void ReWirePrepareEventBusInfo(ReWireEventBusInfo* structure);
extern void ReWirePrepareEventInfo(ReWireEventInfo* structure);
extern void ReWirePrepareEventBuffer(ReWireEventBuffer* structure,ReWire_uint32_t eventBufferCount,ReWireEvent* eventBuffer);
extern void ReWirePrepareDriveAudioInputParams(ReWireDriveAudioInputParams* structure,ReWire_uint32_t eventBufferCount,ReWireEvent* eventBuffer);
extern void ReWirePrepareDriveAudioOutputParams(ReWireDriveAudioOutputParams* params,ReWire_uint32_t eventBufferCount,ReWireEvent* eventBuffer);
extern void ReWirePreparePipeInfo(ReWirePipeInfo* structure,ReWire_uint32_t deviceToPanelFIFOSize,ReWire_uint32_t panelToDeviceFIFOSize);





/* On Windows a standard path. On Mac a POSIX path, utf-8 coded */
typedef const ReWire_char_t* TReWireFileSpec;

	
	
	
/* Internal stuff */

/*	Format is aaabbbccc in decimal. aaa=major,bbb=minor,ccc=revision. That
	is: versionLong=major * 1000000L + minor * 1000 + revision.
	These numbers are automatically set up by RWMOpen,RWPOpen and RWDOpen.
	You only need REWIRE_DEVICE_DLL_API_VERSION (in the implementation of the
	functions ReWireGetDeviceNameAndVersion and ReWireGetDeviceInfo in a device.*/

#define REWIRE_BUILD_VERSION(major,minor,revision)	((major) * 1000000L + (minor) * 1000 + (revision))
#define REWIRE1_MIXER_APP_API_VERSION		REWIRE_BUILD_VERSION(2,0,0)
#define REWIRE1_DEVICE_DLL_API_VERSION		REWIRE_BUILD_VERSION(0,0,8)
#define REWIRE2_DEVICE_DLL_API_VERSION		REWIRE_BUILD_VERSION(1,0,0)
#define REWIRE2_MIXER_APP_API_VERSION		REWIRE_BUILD_VERSION(3,0,0)
#define REWIRE_MIXER_APP_API_VERSION		REWIRE2_MIXER_APP_API_VERSION
#define REWIRE_DEVICE_DLL_API_VERSION		REWIRE2_DEVICE_DLL_API_VERSION
#define REWIRE_PANEL_APP_API_VERSION		REWIRE_BUILD_VERSION(1,0,0)

#ifdef __cplusplus
} /* extern "C" */
} /* namespace ReWire */
#endif /* def __cplusplus */

#if defined(_MSC_VER) 
	#pragma pack(pop)
#elif defined(__GNUC__) && defined(MAC)
	#pragma pack(pop)
#else
#error "REWIRE: Can not set up struct alignement."
#endif /* defined(_MSC_VER) */

#endif /* REWIRE_H_ */

