#pragma once

#define STRSAFE_NO_CB_FUNCTIONS
#define STRSAFE_NO_DEPRECATE

#include <shlwapi.h>
#include <stdio.h>
#include <DocObj.h>
#include <winuserp.h>
#include <shobjidl_core.h>
#include <Windows.h>
#include <shlobj.h>
#include <strsafe.h>
#include <RegStr.h>
#include <shlguid.h>
#include <patternhelper.h>
#include <docobj.h>
#include <synchapi.h>
#include <pshpack4.h>
#include <poppack.h>
#include <userenv.h>        // GetProfileType
#include <dpa_dsa.h>
#include <vssym32.h>
#include <commctrl.h>
#include <shfusion.h>
#include <windowsx.h>
#include <ole2.h>
#include <wininet.h>
#include <shellapi.h>
#include <cpl.h>
#include <ddeml.h>
#include <uxtheme.h>
#include <cfgmgr32.h>
#include <exdisp.h>
#include <mshtml.h>
#include <shtypes.h>
#include <wmistr.h>
#include <ntwmi.h>
#include <evntrace.h>
#include <vector>
#include <wtypes.h>
#include <initguid.h>

// wtl so ass
#undef IID_IPrintDialogCallback
#undef IID_IPrintDialogServices

#include <cguid.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstuff.h>
#include <winuserp.h>

#include <lmcons.h> // for UNLEN
#include "rcids.h"
#include <regstr.h>
#include <combaseapi.h>
#include "ieguidp.h"
#include <iostream>
#include <Dbghelp.h>
#include "startids.h"           // for IDM_PROGRAMS et al
#include "shdguid.h"
#include <dsrole.h>  // DsRoleGetPrimaryDomainInformation, DsRoleFreeMemory
#include <sddl.h>
#include <dbt.h>
#include <vsstyle.h>
#include <fileapi.h>

#include <raserror.h>
#include <netcon.h>
#include <ras.h>
#include <dwmapi.h>
#include <wtsapi32.h>   // for NOTIFY_FOR_THIS_SESSION
#include <shellapi.h>
#include <psapi.h>
#include <ddraw.h>      // DirectDraw stuff..
#include <malloc.h>

#include <winbase.h>

