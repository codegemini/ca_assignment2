#!/bin/bash

./cache -s 256 -b $1 -w $2 -f $3
./cache -s 512 -b $1 -w $2 -f $3
./cache -s 1024 -b $1 -w $2 -f $3
./cache -s 16384 -b $1 -w $2 -f $3
./cache -s 32768 -b $1 -w $2 -f $3
./cache -s 65536 -b $1 -w $2 -f $3
./cache -s 1048576 -b $1 -w $2 -f $3
./cache -s 2097152 -b $1 -w $2 -f $3
./cache -s 4194304 -b $1 -w $2 -f $3

