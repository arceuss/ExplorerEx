//+-------------------------------------------------------------------------
//
//	ExplorerEx - Windows XP Explorer
//	Copyright (C) Microsoft
// 
//	File:       criticalsection.h
// 
//	Description:	Reimplements some of the critical section functions
//					used throughout explorer.
// 
//	History:    Jan-22-25   kfh83  Created
//
//+-------------------------------------------------------------------------

#pragma once

#include "synchapi.h"
#include "debug.h"

#define ENTERCRITICAL   Shell_EnterCriticalSection();
#define LEAVECRITICAL   Shell_LeaveCriticalSection();

//
//  Structs
//

typedef struct _nonreentrantcriticalsection
{
    CRITICAL_SECTION critsec;

#ifdef DEBUG
    DWORD dwOwnerThread;
#endif   /* DEBUG */

    BOOL bEntered;
}
NONREENTRANTCRITICALSECTION;

//
//  Function Definitions
//

/* critical section used for access serialization */

NONREENTRANTCRITICALSECTION s_nrcs =
{
   { 0 },

#ifdef DEBUG
   INVALID_THREAD_ID,
#endif   /* DEBUG */

   FALSE
};


BOOL EnterNonReentrantCriticalSection(
    NONREENTRANTCRITICALSECTION pnrcs)
{
    BOOL bEntered;

#ifdef DEBUG

    BOOL bBlocked;

    ASSERT(IS_VALID_STRUCT_PTR(pnrcs, CNONREENTRANTCRITICALSECTION));

    /* Is the critical section already owned by another thread? */

    /* Use pnrcs->bEntered and pnrcs->dwOwnerThread unprotected here. */

    bBlocked = (pnrcs->bEntered &&
        GetCurrentThreadId() != pnrcs->dwOwnerThread);

    if (bBlocked)
        TRACE_OUT(("EnterNonReentrantCriticalSection(): Blocking thread %lx.  Critical section is already owned by thread %#lx.",
            GetCurrentThreadId(),
            pnrcs->dwOwnerThread));

#endif

    EnterCriticalSection(&(pnrcs.critsec));

    bEntered = (!pnrcs.bEntered);

    if (bEntered)
    {
        pnrcs.bEntered = TRUE;

#ifdef DEBUG

        pnrcs->dwOwnerThread = GetCurrentThreadId();

        if (bBlocked)
            TRACE_OUT(("EnterNonReentrantCriticalSection(): Unblocking thread %lx.  Critical section is now owned by this thread.",
                pnrcs->dwOwnerThread));
#endif

    }
    else
    {
        LeaveCriticalSection(&(pnrcs.critsec));

        //ERROR_OUT(("EnterNonReentrantCriticalSection(): Thread %#lx attempted to reenter non-reentrant code.",
            //GetCurrentThreadId()));
    }

    return(bEntered);
}


void LeaveNonReentrantCriticalSection(
    NONREENTRANTCRITICALSECTION pnrcs)
{
    ASSERT(IS_VALID_STRUCT_PTR(pnrcs, CNONREENTRANTCRITICALSECTION));

    if (EVAL(pnrcs.bEntered))
    {
        pnrcs.bEntered = FALSE;
#ifdef DEBUG
        pnrcs->dwOwnerThread = INVALID_THREAD_ID;
#endif

        LeaveCriticalSection(&(pnrcs.critsec));
    }

    return;
}

BOOL BeginExclusiveAccess(void)
{
    return(EnterNonReentrantCriticalSection(s_nrcs));
}


void EndExclusiveAccess(void)
{
    LeaveNonReentrantCriticalSection(s_nrcs);

    return;
}

void Shell_EnterCriticalSection(void)
{
    BeginExclusiveAccess();

    return;
}

void Shell_LeaveCriticalSection(void)
{
    EndExclusiveAccess();

    return;
}