@echo off
setlocal

set "input=..\examples\01.in"

if not "%~1"=="" (
    set "input=%~1"
)

Can_You_Blow_My_Whistle.exe < "%input%"

if %errorlevel% neq 0 (
    exit /b %errorlevel%
)