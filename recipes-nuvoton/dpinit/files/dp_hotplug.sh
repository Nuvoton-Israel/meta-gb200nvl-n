#!/bin/sh

# dump edid
/usr/bin/dpinit -d /usr/share/edid/edid.bin

#program edid
/bin/systemctl start program-edid.service

#sleep 1
# trigger gfx interrupt
#/usr/sbin/devmem 0xf080003c 32 0x081010DE
#/usr/sbin/devmem 0xf080003c 32 0x0810005E

# force dp init
/usr/bin/dpinit -f
