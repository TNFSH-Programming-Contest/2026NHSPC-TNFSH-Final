#!/bin/bash

set -e

input=../examples/01.in
if [ "$#" -ge 1 ]; then
    input="$1"
fi
./Can_You_Blow_My_Whistle < "$input"
