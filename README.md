# How to build
```ruby
git clone git@github.com:NVIDIA/openbmc.git nvBMC
cd nvBMC
cd meta-nvidia/meta-prime/meta-graceblackwell/
git clone git@github.com:Nuvoton-Israel/meta-gb200nvl-n.git meta-gb200nvl-n
cd ~/nvBMC
```

## bmc target
```ruby
. setup gb200nvl-bmc-n
```
## hmc target
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
