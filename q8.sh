#!/bin/bash

./cache -s $1 -b 4 -w $2 -f $3
./cache -s $1 -b 8 -w $2 -f $3
./cache -s $1 -b 16 -w $2 -f $3
./cache -s $1 -b 32 -w $2 -f $3
./cache -s $1 -b 64 -w $2 -f $3
./cache -s $1 -b 128 -w $2 -f $3
./cache -s $1 -b 256 -w $2 -f $3
./cache -s $1 -b 512 -w $2 -f $3
./cache -s $1 -b 1024 -w $2 -f $3

