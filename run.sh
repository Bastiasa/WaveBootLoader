#!/bin/sh
# run.sh

env -i \
PATH=/usr/bin:/bin \
HOME="$HOME" \
DISPLAY="$DISPLAY" \
XAUTHORITY="$XAUTHORITY" \
XDG_RUNTIME_DIR="$XDG_RUNTIME_DIR" \
qemu-system-x86_64 \
    -monitor stdio \
    -bios /usr/share/OVMF/OVMF_CODE.fd \
    -drive format=raw,file=fat:rw:test \
    -usb \
    -device usb-tablet,bus=usb-bus.0