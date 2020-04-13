@echo off

cd ..\code

:: Remove the existing generated project files
del /S *.sln
del /S *.vcxproj
del /S *.filters
del /S *.VC.db
del /S *.vcxproj.user

..\ExternalTools\premake\windows\premake5.exe vs2019

pause
