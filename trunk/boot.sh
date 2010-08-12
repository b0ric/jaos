#!/bin/bash

if [ $# -gt 0 ]; then
    if [ $1 == "--debug" ]; then
	qemu -fda boot.img -boot a -s -S &
	sleep 1
	gdb vmlinux -command=gdbcmd
    fi
else
    qemu -fda boot.img -boot a
fi

exit 0
