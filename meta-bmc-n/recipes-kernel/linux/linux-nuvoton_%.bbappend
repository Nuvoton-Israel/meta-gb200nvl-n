FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://gb200nvl-bmc-n.cfg"
SRC_URI:remove = " file://gb200nvl-bmc.cfg"
SRC_URI:append = " file://npcm-bmc-nvidia-gb200nvl-bmc.dts;subdir=git/arch/${ARCH}/boot/dts/nuvoton"