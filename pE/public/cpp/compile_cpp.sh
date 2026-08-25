#!/bin/bash
set -e
grader=stub.cpp
code=Can_You_Blow_My_Whistle.cpp
problem=Can_You_Blow_My_Whistle
g++ -std=gnu++17 -O2 -Wall -Wextra "$grader" "$code" -o "$problem"
