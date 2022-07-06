#**********************************************************************
#*                                                                    *
#*   Copyright (c) 1985-2022, American Megatrends International LLC.  *
#*                                                                    *
#*      All rights reserved. Subject to AMI licensing agreement.      *
#*                                                                    *
#**********************************************************************

ifdef EWDK_DIR
  VS_ROOT:=$(EWDK_DIR)\Program Files
else  ifdef VSINSTALLDIR
  VS_ROOT:=$(subst \Microsoft Visual Studio 14.0\,,$(VSINSTALLDIR))
else ifdef VS140COMNTOOLS
  VS_ROOT:=$(subst \Microsoft Visual Studio 14.0\Common7\Tools\,,$(VS140COMNTOOLS))
else
  ifeq ($(shell if exist "C:\Program Files (x86)" (echo yes) else (echo no) ),yes)
    VS_ROOT:=C:\Program Files (x86)
  else 
    VS_ROOT:=C:\Program Files
  endif
endif
  
  MSVC_DIR:=$(VS_ROOT)\Microsoft Visual Studio 14.0\VC
  ifeq ($(shell if exist "$(MSVC_DIR)" (echo yes) else (echo no) ),no)
    ifdef EWDK_DIR
      $(error EWDK_DIR variable is set to $(EWDK_DIR), which does not appear to be a supported EWDK 1703 distribution)
    else ifdef VSINSTALLDIR
      $(error default Visual Studio 2015 installation path VSINSTALLDIR ($(VSINSTALLDIR)) is incorrect or absent)
    else ifdef VS140COMNTOOLS
      $(error default Visual Studio 2015 common tools path VS140COMNTOOLS ($(VS140COMNTOOLS)) is incorrect or absent)
    else
      $(error Visual studio 2015 C compiler is not found at default path $(MSVC_DIR))
    endif
  endif

  #Export HOST variables

  ifndef EWDK_WIN_KITS_VERSION
    WINKITS_ROOT:=$(VS_ROOT)\Windows Kits\10
    EWDK_WIN_KITS_VERSION:=$(lastword $(shell dir /B /A:D /O:N "$(WINKITS_ROOT)\bin\10*"))
    $(info EWDK_WIN_KITS_VERSION variable is not set. Set to auto-detected value $(EWDK_WIN_KITS_VERSION))
  endif

  WINKITS_BIN_DIR=$(VS_ROOT)\Windows Kits\10\bin\$(EWDK_WIN_KITS_VERSION)
  WINKITS_LIB_DIR=$(VS_ROOT)\Windows Kits\10\Lib\$(EWDK_WIN_KITS_VERSION)
  WINKITS_INCLUDE_DIR=$(VS_ROOT)\Windows Kits\10\Include\$(EWDK_WIN_KITS_VERSION)

  ifeq ($(shell if exist "$(WINKITS_LIB_DIR)" (echo yes) else (echo no) ),no)
    $(error EWDK_WIN_KITS_VERSION variable is set to $(EWDK_WIN_KITS_VERSION), which does not appear to be a correct version)
  endif

  ifeq ($(HOST_ARCHITECTURE),X64)
      IA32_TOOLS_DIR:=$(MSVC_DIR)\bin\amd64_x86
      X64_TOOLS_DIR:=$(MSVC_DIR)\bin\amd64
      HOST_BIN_DIR=$(X64_TOOLS_DIR)
      HOST_REDIST_DIR=$(MSVC_DIR)\redist\x64\Microsoft.VC140.CRT
      HOST_RC_DIR=$(WINKITS_BIN_DIR)\x64
      HOST_SYS_LIBS="/LIBPATH:$(MSVC_DIR)\lib\amd64" "/LIBPATH:$(WINKITS_LIB_DIR)\um\x64" "/LIBPATH:$(WINKITS_LIB_DIR)\ucrt\x64"
  else ifeq ($(HOST_ARCHITECTURE),IA32)
      IA32_TOOLS_DIR:=$(MSVC_DIR)\bin
      X64_TOOLS_DIR:=$(MSVC_DIR)\bin\x86_amd64
      HOST_BIN_DIR=$(IA32_TOOLS_DIR)
      HOST_REDIST_DIR=$(MSVC_DIR)\redist\x86\Microsoft.VC140.CRT
      HOST_RC_DIR=$(WINKITS_BIN_DIR)\x86
      HOST_SYS_LIBS="/LIBPATH:$(MSVC_DIR)\lib" "/LIBPATH:$(WINKITS_LIB_DIR)\um\x86" "/LIBPATH:$(WINKITS_LIB_DIR)\ucrt\x86"
  else
      $(error HOST_ARCHITECTURE environment variable is not set)
  endif

  HOST_SYS_INCLUDES="-I$(WINKITS_INCLUDE_DIR)\ucrt" "-I$(MSVC_DIR)\include"

  export IA32_TOOLS_DIR
  export X64_TOOLS_DIR

  export HOST_BIN_DIR
  export HOST_SYS_LIBS
  export HOST_SYS_INCLUDES
  export PATH:=$(HOST_BIN_DIR);$(HOST_RC_DIR);$(HOST_REDIST_DIR);$(PATH) 

ifdef EWDK_DIR
  USE_EWDK:=1
  export USE_EWDK
endif
