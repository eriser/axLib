#ifndef REWIREAPI_H_
#define REWIREAPI_H_

/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

/*	Structures/types and constants for basic (non-portable) services.
	This is internal to ReWire - not for mixer or device writer. */

#include "ReWire.h"

#ifdef __cplusplus
namespace ReWire {
extern "C" {
#endif /* __cplusplus */

#if WINDOWS && !_WIN64
#define REWIRECALL _cdecl
#else
#define REWIRECALL
#endif /* WINDOWS && !_WIN64 */

typedef ReWire_int32_t (REWIRECALL *REWIREPROC)();

	/* Internal stuff */

extern ReWireError ReWireLoadReWireSharedLibrary(void);
extern REWIREPROC ReWireFindReWireSharedLibraryFunction(const ReWire_char_t functionName[]);
extern ReWireError ReWireUnloadReWireSharedLibrary(void);

#ifdef __cplusplus
} /* extern "C" */
} /* namespace ReWire */
#endif /* __cplusplus */

#endif /* REWIREAPI_H_ */
