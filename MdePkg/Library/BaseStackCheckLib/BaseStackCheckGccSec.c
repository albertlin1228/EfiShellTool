/** @file
 Base Stack Check library for GCC/clang.

 Use -fstack-protector-all compiler flag to make the compiler insert the
 __stack_chk_guard "canary" value into the stack and check the value prior
 to exiting the function. If the "canary" is overwritten __stack_chk_fail()
 is called. This is GCC specific code.

 Copyright (c) 2012, Apple Inc. All rights reserved.<BR>
 SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>

/// "canary" value that is inserted by the compiler into the stack frame.
VOID *__stack_chk_guard = (VOID*)0x0AFF;

/**
 Error path for compiler generated stack "canary" value check code. If the
 stack canary has been overwritten this function gets called on exit of the
 function.
**/
VOID
__stack_chk_fail (
 VOID
 )
{
}
