@echo off

g++ -std=gnu++17 -O2 -Wall -Wextra -Wshadow -pipe -o Can_You_Blow_My_Whistle.exe stub.cpp Can_You_Blow_My_Whistle.cpp

if %errorlevel% neq 0 (
    exit /b %errorlevel%
)