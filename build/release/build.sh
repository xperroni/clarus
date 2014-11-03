#!/bin/bash

cmake -DCMAKE_BUILD_TYPE=Release ../..
#cmake -DBUILD_SHARED_LIBS:BOOL=ON -DCMAKE_BUILD_TYPE=Release ../..

make
