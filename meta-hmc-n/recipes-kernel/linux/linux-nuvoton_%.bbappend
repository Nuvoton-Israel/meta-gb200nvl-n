FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://gb200nvl-hmc-n.cfg"
SRC_URI:append = " file://npcm-bmc-nvidia-gb200nvl-hmc.dts;subdir=git/arch/${ARCH}/boot/dts/nuvoton"