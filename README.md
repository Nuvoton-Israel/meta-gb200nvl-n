# GB200NVL-N OpenBMC

This document provides instructions for building and deploying the OpenBMC image for the Nuvoton BMC.

## Prerequisites

Before you begin, ensure you have cloned the necessary repositories and applied the required patches.

1.  **Clone Repositories**

    Start by cloning the `openbmc` repository and the platform-specific `meta-gb200nvl-n` layer.
```sh
git clone git@github.com:NVIDIA/openbmc.git openbmc
cd openbmc
git clone git@github.com:Nuvoton-Israel/meta-gb200nvl-n.git meta-nvidia/meta-prime/meta-graceblackwell/meta-gb200nvl-n
```

2.  Apply the necessary patch from the top-level `openbmc` directory:

```sh
cd ~/openbmc # Or the path to your top-level openbmc directory
git reset --hard fa1c3b95cada90ea4fc8ad22b9bf6bd24b4dc91c
git am ./meta-nvidia/meta-prime/meta-graceblackwell/meta-gb200nvl-n/0001-fix-meta-arm-and-meta-nuvoton-break.patch
```

## npcm8xx dc-scm target
```ruby
. setup gb200nvl-scm-n
```

## npcm8xx evb for bmc target
```ruby
. setup gb200nvl-bmc-n
```

## npcm8xx evb for hmc target
```ruby
. setup gb200nvl-hmc-n
```

## Build
```ruby
bitbake obmc-phosphor-image
```
## Image programming

* Flash full openbmc image
```ruby
setenv ethact gmac2
tftp 10000000 image-bmc
sf probe 0:0
sf update 0x10000000 0x0 ${filesize}
```
## First Use
### Under u-boot, erase the bottom half of the flash for rwfs and log if the flash size is 128MB
```ruby
sf probe 0:0
sf erase 0x04000000 0x04000000
```
### Under u-boot, erase the emmc to create a new partition table in the openbmc
```ruby
mmc erase 0 0x1000000
```

## QEMU

### Build QEMU

```ruby
$ sudo apt install nettle-dev
$ git clone git@github.com:Nuvoton-Israel/qemu.git
$ cd qemu
$ ./configure --target-list=aarch64-softmmu --enable-nettle
$ make -j $(nproc)    // will generate a qemu-system-aarch64 binary file in the build floder
```

### Run BMC image with QEMU

```ruby
$ cd build
$ ./qemu-system-aarch64 -machine npcm845-evb -nographic \
	-drive file=workdir/image-bmc,if=mtd,bus=0,unit=0,format=raw \
	-device loader,force-raw=on,addr=0x2000000,file=workdir/bl31.bin \
	-device loader,cpu-num=3,addr=0x2000000 \
	-device loader,cpu-num=2,addr=0x2000000 \
	-device loader,cpu-num=1,addr=0x2000000
```