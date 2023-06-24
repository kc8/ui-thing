
@echo off

echo Building UI Thing 

if NOT defined VSCMD_ARG_TGT_ARCH (
    call vcvars64
)

set compilerFlags= -MD -GS -Gm- -nologo -Od -Oi -WX -GR -EHa- -DINTERNAL_BUILD=1 -DSLOW=1 -DSPACETRUCKING_WIN32=1 -FC -W4 -wd4505 -wd4201 -wd4100 -wd4189 -Z7
set linkerFlags= -STACK:0x100000,0x100000 -incremental:no -opt:ref user32.lib gdi32.lib ole32.lib winmm.lib bcrypt.lib kernel32.lib xaudio2.lib mfplat.lib opengl32.lib

set sourceFiles= ../main.cpp 

IF NOT EXIST build mkdir build
pushd build

del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp

rem echo LIBRARY.... COMPILING
rem cl %compilerFlags% %sourceFiles% -Fuithing.map /LD /link -incremental:no -opt:ref -PDB:uithing%random%.pdb 
rem /EXPORT:game_update_and_render /EXPORT:game_get_sound_sample

del lock.tmp 
echo WIN32 EXE... COMPILING
cl %compilerFlags% /I ../includes %sourceFiles% -FUIthing.map /link %linkerFlags% 
rem /NODEFAULTLIB /SUBSYSTEM:windows  TODO some day we can remove the std libs from our linking

popd
