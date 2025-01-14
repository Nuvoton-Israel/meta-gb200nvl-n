# How to build
1. git clone git@github.com:NVIDIA/openbmc.git nvBMC
2. cd nvBMC
3. cd meta-nvidia/meta-prime/meta-graceblackwell/
4. git clone git@github.com:Nuvoton-Israel/meta-gb200nvl-n.git meta-gb200nvl-n
5. cd ~/nvBMC
6. . setup gb200nvl-bmc-n  or  . setup gb200nvl-hmc-n
7. bitbake obmc-phosphor-image
