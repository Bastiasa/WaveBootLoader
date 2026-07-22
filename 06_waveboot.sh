#!/bin/sh
set -e
cat << 'EOF'
insmod search
insmod fat
search --no-floppy --set=waveboot_flag_dev --file /waveboot_direct.flag
if [ -n "$waveboot_flag_dev" ]; then
  set timeout=0
  set timeout_style=hidden
else
  set timeout=5
  set timeout_style=menu
fi
EOF