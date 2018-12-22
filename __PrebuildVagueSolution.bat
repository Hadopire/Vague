
@echo off

set current_folder=%~dp0
set current_folder=%current_folder:~0,-1%

set prebuild_folder=%current_folder%\Prebuild

set premaxe_exe=%prebuild_folder%\premake5.exe
set entry_file=%prebuild_folder%\VagueSolution.lua
set target=vs2017

@echo on
%premaxe_exe% --file="%entry_file%" %target%
@echo off

pause
