#!/bin/bash
if [[ "$1" = "gdb" ]]; then
    echo "initializing qemu with gdb..."
    qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -d int
else 
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 
fi