# Microsoft Developer Studio Project File - Name="drempels" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=drempels - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "drempels.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "drempels.mak" CFG="drempels - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "drempels - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "drempels - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/drempels", VAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "drempels - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /Gz /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib ddraw.lib dxguid.lib winmm.lib LIBCMT.LIB dsound.lib /nologo /subsystem:windows /machine:I386 /out:"release\drempels.exe"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /Gz /Gm /GX /ZI /Od /Op /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 comctl32.lib ddraw.lib dsound.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib winmm.lib LIBCMT.LIB msimg32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"debug\drempels.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "drempels - Win32 Release"
# Name "drempels - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "*.c; *.cpp"
# Begin Source File

SOURCE=.\gpoly.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\texmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\yuv.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\gpoly.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\sysstuff.h
# End Source File
# Begin Source File

SOURCE=.\texmgr.h
# End Source File
# Begin Source File

SOURCE=.\video.h
# End Source File
# Begin Source File

SOURCE=.\yuv.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\let_op.ico
# End Source File
# Begin Source File

SOURCE=.\Resource.rc
# End Source File
# End Group
# Begin Group "JpegLib Source Files"

# PROP Default_Filter "*.c;*.cpp"
# Begin Source File

SOURCE=.\jpeg\cdjpeg.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcapimin.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcapistd.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jccoefct.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jccolor.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcdctmgr.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jchuff.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcinit.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcmainct.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcmarker.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcmaster.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcomapi.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcparam.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcphuff.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcprepct.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jcsample.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jctrans.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdapimin.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdapistd.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdatadst.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdatasrc.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdcoefct.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdcolor.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jddctmgr.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdhuff.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdinput.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmainct.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmarker.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmaster.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdmerge.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdphuff.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdpostct.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdsample.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jdtrans.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jerror.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jfdctflt.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jfdctfst.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jfdctint.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jidctflt.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jidctfst.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jidctint.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jidctred.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jmemansi.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jmemmgr.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jquant1.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jquant2.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\jutils.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\rdbmp.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\rdcolmap.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\rdgif.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\rdppm.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\rdrle.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\rdswitch.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\rdtarga.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\transupp.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\wrbmp.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\wrgif.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\wrppm.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\wrrle.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\jpeg\wrtarga.c

!IF  "$(CFG)" == "drempels - Win32 Release"

!ELSEIF  "$(CFG)" == "drempels - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\installer\drempels.nsi
# End Source File
# Begin Source File

SOURCE=.\installer\drempels.txt
# End Source File
# End Target
# End Project
