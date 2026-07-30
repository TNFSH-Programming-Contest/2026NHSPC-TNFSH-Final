#!/bin/bash

set -e

g++ -std=gnu++14 -O2 -Wall -Wextra -Wshadow -pipe \
    -o Can_You_Blow_My_Whistle \
    grader.cpp Can_You_Blow_My_Whistle.cpp
