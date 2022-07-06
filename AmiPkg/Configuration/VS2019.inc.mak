#**********************************************************************
#*                                                                    *
#*   Copyright (c) 1985-2022, American Megatrends International LLC.  *
#*                                                                    *
#*      All rights reserved. Subject to AMI licensing agreement.      *
#*                                                                    *
#**********************************************************************

ifdef EWDK_DIR
  VS_ROOT:=$(EWDK_DIR)\Program Files
else
  ifeq ($(shell if exist "C:\Program Files (x86)" (echo yes) else (echo no) ),yes)
    VS_ROOT:=C:\Program Files (x86)
  else 
    VS_ROOT:=C:\Program Files
  endif
endif
  
ifndef MSVC_ROOT
  VS2019_ROOT:=$(VS_ROOT)\Microsoft Visual Studio\2019
  ifeq ($(shell if exist "$(VS2019_ROOT)" (echo yes) else (echo no) ),no)
    ifdef EWDK_DIR
      $(error EWDK_DIR variable is set to $(EWDK_DIR), which does not appear to be a supported EWDK_2019 distribution)
    else
      $(error Visual studio 2019 is not found at default path $(VS2019_ROOT))
    endif
  endif

  ifndef VS_EDITION
    ifdef EWDK_DIR
      VS_EDITION=BuildTools
    else
      VS_EDITION:=$(lastword $(shell dir /B /A:D /O:N "$(VS2019_ROOT)"))
      $(info VS_EDITION variable is not set. Set to auto-detected value $(VS_EDITION))
    endif
  endif

  MSVC_ROOT:=$(VS2019_ROOT)\$(VS_EDITION)
else
  ifeq ($(shell if exist "$(MSVC_ROOT)\VS\Tools\MSVC" (echo yes) else (echo no) ),no)
    $(error MSVC_ROOT variable is set to $(MSVC_ROOT), which does not appear to be a supported VS2019 distribution)
  endif
endif

  ifndef EWDK_MSVC_VERSION
    EWDK_MSVC_VERSION:=$(lastword $(shell dir /B /A:D /O:N "$(MSVC_ROOT)\VC\Tools\MSVC"))
    $(info EWDK_MSVC_VERSION variable is not set. Set to auto-detected value $(EWDK_MSVC_VERSION))
  endif

  MSVC_DIR:=$(MSVC_ROOT)\VC\Tools\MSVC\$(EWDK_MSVC_VERSION)
  ifeq ($(shell if exist "$(MSVC_DIR)" (echo yes) else (echo no) ),no)
    ifdef EWDK_DIR
      $(error EWDK_DIR variable is set to $(EWDK_DIR), which does not appear to be a supported EWDK_2019 distribution)
    else
      $(error Visual studio 2019 C compiler is not found at path $(MSVC_DIR))
    endif
  endif

  ifndef EWDK_MSVC_REDIST_VERSION
    EWDK_MSVC_VERSION:=$(lastword $(shell dir /B /A:D /O:N "$(MSVC_ROOT)\VC\Redist\MSVC"))
    $(info EWDK_MSVC_REDIST_VERSION variable is not set. Set to auto-detected value $(EWDK_MSVC_REDIST_VERSION))
  endif

  MSVC_REDIST_DIR:=$(MSVC_ROOT)\VC\Redist\MSVC\$(EWDK_MSVC_REDIST_VERSION)
  ifeq ($(shell if exist "$(MSVC_REDIST_DIR)" (echo yes) else (echo no) ),no)
    $(error Visual studio 2019 redistributable libraries are not found at path $(MSVC_REDIST_DIR))
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
      IA32_TOOLS_DIR:=$(MSVC_DIR)\bin\Hostx64\x86
      X64_TOOLS_DIR:=$(MSVC_DIR)\bin\Hostx64\x64
      HOST_BIN_DIR=$(X64_TOOLS_DIR)
      HOST_REDIST_DIR=$(MSVC_REDIST_DIR)\x64
      HOST_RC_DIR=$(WINKITS_BIN_DIR)\x64
      HOST_SYS_LIBS="/LIBPATH:$(MSVC_DIR)\lib\x64" "/LIBPATH:$(WINKITS_LIB_DIR)\um\x64" "/LIBPATH:$(WINKITS_LIB_DIR)\ucrt\x64"
  else ifeq ($(HOST_ARCHITECTURE),IA32)
      IA32_TOOLS_DIR:=$(MSVC_DIR)\bin\Hostx86\x86
      X64_TOOLS_DIR:=$(MSVC_DIR)\bin\Hostx86\x64
      HOST_BIN_DIR=$(IA32_TOOLS_DIR)
      HOST_REDIST_DIR=$(MSVC_REDIST_DIR)\x86
      HOST_RC_DIR=$(WINKITS_BIN_DIR)\x86
      HOST_SYS_LIBS="/LIBPATH:$(MSVC_DIR)\lib\x86" "/LIBPATH:$(WINKITS_LIB_DIR)\um\x86" "/LIBPATH:$(WINKITS_LIB_DIR)\ucrt\x86"
  else
      $(error HOST_ARCHITECTURE environment variable is not set)
  endif

  HOST_SYS_INCLUDES="-I$(WINKITS_INCLUDE_DIR)\ucrt" "-I$(WINKITS_INCLUDE_DIR)\um" "-I$(WINKITS_INCLUDE_DIR)\shared" "-I$(MSVC_DIR)\include"

  export IA32_TOOLS_DIR
  export X64_TOOLS_DIR

  export HOST_BIN_DIR
  export HOST_SYS_LIBS
  export HOST_SYS_INCLUDES
  export PATH:=$(HOST_BIN_DIR);$(HOST_RC_DIR);$(HOST_REDIST_DIR);$(PATH) 

