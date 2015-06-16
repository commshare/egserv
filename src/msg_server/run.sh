#!/bin/sh

cd build
make
cd ..
./msg_server
