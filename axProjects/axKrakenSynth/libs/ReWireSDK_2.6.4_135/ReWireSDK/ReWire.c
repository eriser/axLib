/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

#include "ReWire.h"
#include <string.h>

/*
 *
 * Local assertion macro.
 *
 */
#include <assert.h>
#define REWIRE_ASSERT(e) assert(e)

#ifdef __cplusplus

namespace ReWire {

#endif

static void ReWireZeroMemory(void* data,ReWire_uint32_t size){
	REWIRE_ASSERT(0!=data);
	REWIRE_ASSERT(size>0);
	memset(data,0,size);
}

void ReWireClearBitField(ReWire_uint32_t bitField[], ReWire_uint32_t bitCount) {
	ReWire_uint32_t bitFieldSize = REWIRE_BITFIELD_SIZE(bitCount);
	ReWireZeroMemory(bitField, (ReWire_uint32_t)(bitFieldSize * sizeof(ReWire_uint32_t)));
}

void ReWireSetBitInBitField(ReWire_uint32_t bitField[], ReWire_uint16_t bitIndex) {
	ReWire_uint32_t arrayIndex = bitIndex >> 5;
	ReWire_uint32_t localBitIndex = bitIndex & 0x1f;
	ReWire_uint32_t bitMask=1 << localBitIndex;
	bitField[arrayIndex] = bitField[arrayIndex] | bitMask;
}

void ReWireClearBitInBitField(ReWire_uint32_t bitField[], ReWire_uint16_t bitIndex) {
	ReWire_uint32_t arrayIndex = bitIndex >> 5;
	ReWire_uint32_t localBitIndex = bitIndex & 0x1f;
	ReWire_uint32_t bitMask = 1 << localBitIndex;
	bitField[arrayIndex] = bitField[arrayIndex] & (~bitMask);
}

char ReWireIsBitInBitFieldSet(const ReWire_uint32_t bitField[], ReWire_uint16_t bitIndex) {
	ReWire_uint32_t arrayIndex = bitIndex >> 5;
	ReWire_uint32_t localBitIndex = bitIndex & 0x1f;
	ReWire_uint32_t bitMask = 1 << localBitIndex;
	if (bitMask & bitField[arrayIndex]) {
		return 1;
	}
	else {
		return 0;
	}
}

void ReWireSetBitRangeInBitField(ReWire_uint32_t bitField[], ReWire_uint16_t start, ReWire_uint16_t count) {
	ReWire_uint16_t bitIndex;
	for (bitIndex = start; bitIndex < (start + count); bitIndex++) {
		ReWireSetBitInBitField(bitField, bitIndex);
	}
}

const ReWireMIDIEvent* ReWireCastToMIDIEvent(const ReWireEvent* event){
	REWIRE_ASSERT(0!=event);
	REWIRE_ASSERT(event->fEventType==kReWireMIDIEvent);
	return (const ReWireMIDIEvent*)event;
}

const ReWireRequestRepositionEvent* ReWireCastToRequestRepositionEvent(const ReWireEvent* event){
	REWIRE_ASSERT(0!=event);
	REWIRE_ASSERT(event->fEventType==kReWireRequestRepositionEvent);
	return (const ReWireRequestRepositionEvent*)event;
}

const ReWireRequestSignatureEvent* ReWireCastToRequestSignatureEvent(const ReWireEvent* event){
	REWIRE_ASSERT(0!=event);
	REWIRE_ASSERT(event->fEventType==kReWireRequestSignatureEvent);
	return (const ReWireRequestSignatureEvent*)event;
}

const ReWireRequestTempoEvent* ReWireCastToRequestTempoEvent(const ReWireEvent* event){
	REWIRE_ASSERT(0!=event);
	REWIRE_ASSERT(event->fEventType==kReWireRequestTempoEvent);
	return (const ReWireRequestTempoEvent*)event;
}

const ReWireRequestLoopEvent* ReWireCastToRequestLoopEvent(const ReWireEvent* event){
	REWIRE_ASSERT(0!=event);
	REWIRE_ASSERT(event->fEventType==kReWireRequestLoopEvent);
	return (const ReWireRequestLoopEvent*)event;
}

const ReWireRequestPlayEvent* ReWireCastToRequestPlayEvent(const ReWireEvent* event){
	REWIRE_ASSERT(0!=event);
	REWIRE_ASSERT(event->fEventType==kReWireRequestPlayEvent);
	return (const ReWireRequestPlayEvent*)event;
}

const ReWireRequestStopEvent* ReWireCastToRequestStopEvent(const ReWireEvent* event){
	REWIRE_ASSERT(0!=event);
	REWIRE_ASSERT(event->fEventType==kReWireRequestStopEvent);
	return (const ReWireRequestStopEvent*)event;
}

const ReWireEventTargetChangedEvent* ReWireCastToEventTargetChangedEvent(const ReWireEvent* event){
	REWIRE_ASSERT(0!=event);
	REWIRE_ASSERT(event->fEventType==kReWireEventTargetChangedEvent);
	return (const ReWireEventTargetChangedEvent*)event;
}

const ReWireRevealEventTargetEvent* ReWireCastToRevealEventTargetEvent(const ReWireEvent* event){
	REWIRE_ASSERT(0!=event);
	REWIRE_ASSERT(event->fEventType==kReWireRevealEventTargetEvent);
	return (const ReWireRevealEventTargetEvent*)event;
}



ReWireMIDIEvent* ReWireConvertToMIDIEvent(ReWireEvent* event,const ReWireEventTarget* eventTarget){
	ReWireMIDIEvent* theEvent=(ReWireMIDIEvent*)event;
	ReWireZeroMemory(event,sizeof(ReWireEvent));
	theEvent->fEventType=kReWireMIDIEvent;
	theEvent->fEventTarget=*eventTarget;
	return theEvent;
}

ReWireRequestRepositionEvent* ReWireConvertToRequestRepositionEvent(ReWireEvent* event){
	ReWireZeroMemory(event,sizeof(ReWireEvent));
	event->fEventType=kReWireRequestRepositionEvent;
	return (ReWireRequestRepositionEvent*)event;
}

ReWireRequestSignatureEvent* ReWireConvertToRequestSignatureEvent(ReWireEvent* event){
	ReWireZeroMemory(event,sizeof(ReWireEvent));
	event->fEventType=kReWireRequestSignatureEvent;
	return (ReWireRequestSignatureEvent*)event;
}

ReWireRequestTempoEvent* ReWireConvertToRequestTempoEvent(ReWireEvent* event){
	ReWireZeroMemory(event,sizeof(ReWireEvent));
	event->fEventType=kReWireRequestTempoEvent;
	return (ReWireRequestTempoEvent*)event;
}

ReWireRequestLoopEvent* ReWireConvertToRequestLoopEvent(ReWireEvent* event){
	ReWireZeroMemory(event,sizeof(ReWireEvent));
	event->fEventType=kReWireRequestLoopEvent;
	return (ReWireRequestLoopEvent*)event;
}

ReWireRequestPlayEvent* ReWireConvertToRequestPlayEvent(ReWireEvent* event){
	ReWireZeroMemory(event,sizeof(ReWireEvent));
	event->fEventType=kReWireRequestPlayEvent;
	return (ReWireRequestPlayEvent*)event;
}

ReWireRequestStopEvent* ReWireConvertToRequestStopEvent(ReWireEvent* event){
	ReWireZeroMemory(event,sizeof(ReWireEvent));
	event->fEventType=kReWireRequestStopEvent;
	return (ReWireRequestStopEvent*)event;
}

ReWireEventTargetChangedEvent* ReWireConvertToEventTargetChangedEvent(ReWireEvent* event){
	ReWireZeroMemory(event,sizeof(ReWireEvent));
	event->fEventType=kReWireEventTargetChangedEvent;
	return (ReWireEventTargetChangedEvent*)event;
}

ReWireRevealEventTargetEvent* ReWireConvertToRevealEventTargetEvent(ReWireEvent* event){
	ReWireZeroMemory(event,sizeof(ReWireEvent));
	event->fEventType=kReWireRevealEventTargetEvent;
	return (ReWireRevealEventTargetEvent*)event;
}


void ReWirePrepareDeviceInfo(ReWireDeviceInfo* structure){
	ReWireZeroMemory(structure,sizeof(ReWireDeviceInfo));
	structure->fStructSize=sizeof(ReWireDeviceInfo);
}

void ReWirePrepareOpenInfo(ReWireOpenInfo* structure,ReWire_int32_t sampleRate,ReWire_int32_t maxFrameCount){
	ReWireZeroMemory(structure,sizeof(ReWireOpenInfo));
	structure->fStructSize=sizeof(ReWireOpenInfo);
	ReWirePrepareAudioInfo(&structure->fAudioInfo,sampleRate,maxFrameCount);
}

void ReWirePrepareAudioInfo(ReWireAudioInfo* structure,ReWire_int32_t sampleRate,ReWire_int32_t maxFrameCount){
	ReWireZeroMemory(structure,sizeof(ReWireAudioInfo));
	structure->fStructSize=sizeof(ReWireAudioInfo);
	structure->fSampleRate=sampleRate;
	structure->fMaxBufferSize=maxFrameCount;
}

void ReWirePrepareEventTarget(ReWireEventTarget* structure,ReWire_uint16_t bus,ReWire_uint16_t channel){
	ReWireZeroMemory(structure,sizeof(ReWireEventTarget));
	REWIRE_ASSERT(bus<kReWireReservedEventBusIndex);
	REWIRE_ASSERT(channel<kReWireEventChannelCount);
	structure->fMIDIBusIndex=bus;
	structure->fChannel=channel;
}

void ReWirePrepareEventControllerInfo(ReWireEventControllerInfo* structure){
	ReWireZeroMemory(structure,sizeof(ReWireEventControllerInfo));
	structure->fStructSize=sizeof(ReWireEventControllerInfo);
}

void ReWirePrepareEventNoteInfo(ReWireEventNoteInfo* structure){
	ReWireZeroMemory(structure,sizeof(ReWireEventNoteInfo));
	structure->fStructSize=sizeof(ReWireEventNoteInfo);
}

void ReWirePrepareEventChannelInfo(ReWireEventChannelInfo* structure){
	ReWireZeroMemory(structure,sizeof(ReWireEventChannelInfo));
	structure->fStructSize=sizeof(ReWireEventChannelInfo);
}

void ReWirePrepareEventBusInfo(ReWireEventBusInfo* structure){
	ReWireZeroMemory(structure,sizeof(ReWireEventBusInfo));
	structure->fStructSize=sizeof(ReWireEventBusInfo);
}

void ReWirePrepareEventInfo(ReWireEventInfo* structure){
	ReWireZeroMemory(structure,sizeof(ReWireEventInfo));
	structure->fStructSize=sizeof(ReWireEventInfo);
}

void ReWirePrepareEventBuffer(ReWireEventBuffer* structure,ReWire_uint32_t eventBufferCount,ReWireEvent* eventBuffer){
	ReWireZeroMemory(eventBuffer,(ReWire_uint32_t)(sizeof(ReWireEvent)*eventBufferCount));
	structure->fBufferStructSize=sizeof(ReWireEventBuffer);
	structure->fEventStructSize=sizeof(ReWireEvent);
	structure->fMaxEventCount=eventBufferCount;
	structure->fEventBuffer=eventBuffer;
}

void ReWirePrepareDriveAudioInputParams(ReWireDriveAudioInputParams* structure,ReWire_uint32_t eventBufferCount,ReWireEvent* eventBuffer){
	ReWireZeroMemory(structure,sizeof(ReWireDriveAudioInputParams));
	structure->fStructSize=sizeof(ReWireDriveAudioInputParams);

	ReWirePrepareEventBuffer(&structure->fEventInBuffer,eventBufferCount,eventBuffer);
}

void ReWirePrepareDriveAudioOutputParams(ReWireDriveAudioOutputParams* structure,ReWire_uint32_t eventBufferCount,ReWireEvent* eventBuffer){
	ReWireZeroMemory(structure,sizeof(ReWireDriveAudioOutputParams));
	structure->fStructSize=sizeof(ReWireDriveAudioOutputParams);

	ReWirePrepareEventBuffer(&structure->fEventOutBuffer,eventBufferCount,eventBuffer);
}

void ReWirePreparePipeInfo(ReWirePipeInfo* structure,ReWire_uint32_t deviceToPanelFIFOSize,ReWire_uint32_t panelToDeviceFIFOSize){
	ReWireZeroMemory(structure,sizeof(ReWirePipeInfo));
	structure->fStructSize=sizeof(ReWirePipeInfo);
	structure->fDeviceToPanelFIFOSize=deviceToPanelFIFOSize;
	structure->fPanelToDeviceFIFOSize=panelToDeviceFIFOSize;
}

#ifdef __cplusplus

}

#endif
