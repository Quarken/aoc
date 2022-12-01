@echo off
setlocal enabledelayedexpansion

where /Q cl.exe || (
    set __VSCMD_ARG_NO_LOGO=1
    for /f "tokens=*" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath ') do (
        set InstallDir=%%i
    )
    if "!InstallDir!" equ "" (
        echo Couldn't find cl.exe or a Visual Studio installation! exiting without building...
        exit /b 1
    )
    call "!InstallDir!\VC\Auxiliary\Build\vcvarsall.bat" amd64 || exit /b 1
)

if "%1" equ "debug" (
    set CL=/MTd /Od /Zi /RTC1 /fsanitize=address
    set LINK=/DEBUG:FULL
) else if "%1" equ "profile" (
    set CL=/MTd /DNDEBUG /GL /O2 /Zi /GS-
    set LINK=/LTCG /OPT:REF /OPT:ICF
) else (
    set CL=/MT /GL /O2 /DNDEBUG /GS-
    set LINK=/LTCG /OPT:REF /OPT:ICF
)

if not exist build mkdir build
pushd build
cl.exe /D_CRT_SECURE_NO_WARNINGS /nologo /std:c11 /W3 /WX /MP /I .. ..\aoc.c ..\aoc_win32.c ..\2022\solutions\*.c /link /INCREMENTAL:NO /FIXED
del *.obj *.lib *.exp vc*.pdb >nul
popd
