# How to build
1. git clone git@github.com:NVIDIA/openbmc.git nvBMC
2. cd nvBMC
3. cd meta-nvidia/meta-prime/meta-graceblackwell/meta-gb200nvl/meta-bmc
4. git clone git@github.com:Nuvoton-Israel/meta-bmc-n.git meta-bmc-n
5. cd ~/nvBMC
6. . setup gb200nvl-bmc-n
7. bitbake obmc-phosphor-image
