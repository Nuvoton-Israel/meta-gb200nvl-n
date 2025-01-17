# How to build
- git clone git@github.com:NVIDIA/openbmc.git nvBMC
- cd nvBMC
- cd meta-nvidia/meta-prime/meta-graceblackwell/
- git clone git@github.com:Nuvoton-Israel/meta-gb200nvl-n.git meta-gb200nvl-n
- cd ~/nvBMC

## bmc target 
- . setup gb200nvl-bmc-n

## hmc target
- . setup gb200nvl-hmc-n

## Build
-  bitbake obmc-phosphor-image

## First Use
### Under u-boot, erase the bottom half of the flash for rwfs and log
#### BMC
- sf probe 0:0
- sf erase 0x4C00000 0x03400000
#### HMC
- sf probe 0:0
- sf erase 0x04000000 0x04000000
### Under u-boot, erase the emmc to create a new partition table in the openbmc
- mmc erase 0 0x1000000
