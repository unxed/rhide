#!/bin/bash
cd tvision
./configure
make -j$(nproc --all)
cd ../setedit
./configure --libset
make -j$(nproc --all)
make -j$(nproc --all)
make -j$(nproc --all)
cd ../rhide
./configure
make -j$(nproc --all)
