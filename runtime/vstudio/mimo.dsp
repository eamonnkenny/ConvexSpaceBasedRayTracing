# Microsoft Developer Studio Project File - Name="mimo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=mimo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mimo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mimo.mak" CFG="mimo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mimo - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "mimo - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mimo___Win32_Release"
# PROP BASE Intermediate_Dir "mimo___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x1809 /d "NDEBUG"
# ADD RSC /l 0x1809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mimo___Win32_Debug"
# PROP BASE Intermediate_Dir "mimo___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x1809 /d "_DEBUG"
# ADD RSC /l 0x1809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mimo - Win32 Release"
# Name "mimo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\boundary.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\box.cpp
# End Source File
# Begin Source File

SOURCE=..\bts.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\building.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\complex.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ComplexVector.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\convex.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\diagram.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\diffcoeff.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\diffraction.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\edge.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\image.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\intrinsic.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\main-mimo.cpp"
# End Source File
# Begin Source File

SOURCE=..\matlab.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\path-loss.cpp"

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\plane.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\point3d.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\pointinfo.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\raypath.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\raytracing.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\reflection.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\reloader.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\room.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\segment3d.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\storage.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\string.cpp

!IF  "$(CFG)" == "mimo - Win32 Release"

# PROP Intermediate_Dir "Release"

!ELSEIF  "$(CFG)" == "mimo - Win32 Debug"

# PROP Intermediate_Dir "Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\boolean.hh
# End Source File
# Begin Source File

SOURCE=..\boundary.hh
# End Source File
# Begin Source File

SOURCE=..\bts.hh
# End Source File
# Begin Source File

SOURCE=..\building.hh
# End Source File
# Begin Source File

SOURCE=..\complex.hh
# End Source File
# Begin Source File

SOURCE=..\ComplexVector.hh
# End Source File
# Begin Source File

SOURCE=..\contact.hh
# End Source File
# Begin Source File

SOURCE=..\convex.hh
# End Source File
# Begin Source File

SOURCE=..\diagram.hh
# End Source File
# Begin Source File

SOURCE=..\diffcoeff.hh
# End Source File
# Begin Source File

SOURCE=..\diffraction.hh
# End Source File
# Begin Source File

SOURCE=..\edge.hh
# End Source File
# Begin Source File

SOURCE=..\edgelist.hh
# End Source File
# Begin Source File

SOURCE=..\floor.hh
# End Source File
# Begin Source File

SOURCE=..\image.hh
# End Source File
# Begin Source File

SOURCE=..\intrinsic.hh
# End Source File
# Begin Source File

SOURCE=..\item.hh
# End Source File
# Begin Source File

SOURCE=..\list.hh
# End Source File
# Begin Source File

SOURCE=..\matlab.hh
# End Source File
# Begin Source File

SOURCE="..\path-loss.hh"
# End Source File
# Begin Source File

SOURCE=..\plane.hh
# End Source File
# Begin Source File

SOURCE=..\point3d.hh
# End Source File
# Begin Source File

SOURCE=..\pointinfo.hh
# End Source File
# Begin Source File

SOURCE=..\raypath.hh
# End Source File
# Begin Source File

SOURCE=..\raytracing.hh
# End Source File
# Begin Source File

SOURCE=..\reflection.hh
# End Source File
# Begin Source File

SOURCE=..\reloader.hh
# End Source File
# Begin Source File

SOURCE=..\room.hh
# End Source File
# Begin Source File

SOURCE=..\segment3d.hh
# End Source File
# Begin Source File

SOURCE=..\storage.hh
# End Source File
# Begin Source File

SOURCE=..\story.hh
# End Source File
# Begin Source File

SOURCE=..\string.hh
# End Source File
# Begin Source File

SOURCE=..\userconst.hh
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
