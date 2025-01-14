# How to build
1. git clone git@github.com:NVIDIA/openbmc.git nvBMC
2. cd nvBMC
3. cd meta-nvidia/meta-prime/meta-graceblackwell/
4. git clone git@github.com:Nuvoton-Israel/meta-gb200nvl-n.git meta-gb200nvl-n
5. cd ~/nvBMC

## bmc target 
7. . setup gb200nvl-bmc-n

## hmc target
8. . setup gb200nvl-hmc-n

## Build
10. bitbake obmc-phosphor-image
