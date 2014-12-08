#!/bin/bash

MODE=$(echo ${PWD##*/} | sed 's/[^ ]\+/\L\u&/g')

cmake -DCMAKE_BUILD_TYPE=$MODE ../.. #-DBUILD_SHARED_LIBS:BOOL=ON

make -j2
