/**

Copyright (c) 2012, Intel Corporation
All rights reserved. This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Base.h>

//
// Below data structure is from vcruntime.h and gs_report.c (Microsoft Visual Studio)
//

UINTN __security_cookie = 0x1F2E3D4C;

static void __cdecl __report_gsfailure(UINTN StackCookie)
{
}

__declspec(noreturn) void __cdecl __report_rangecheckfailure()
{
}

void __fastcall __security_check_cookie(UINTN cookie)
{
}

void __GSHandlerCheck(void)
{
}

