#!/usr/bin/sh
if [ ! -e fifo_kiss ]; then 
    mkfifo fifo_kiss
fi

./build/example/transmit > fifo_kiss &
./build/example/receive < fifo_kiss