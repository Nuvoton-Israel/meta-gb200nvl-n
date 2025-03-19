FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:remove = " file://gb200nvl-bmc.cfg"

SRC_URI:append:gb200nvl-bmc-n = " file://gb200nvl-bmc-n.cfg"
SRC_URI:append:gb200nvl-bmc-n = " file://npcm-bmc-nvidia-gb200nvl-bmc.dts;subdir=git/arch/${ARCH}/boot/dts/nuvoton"

SRC_URI:append:gb200nvl-scm-n = " file://gb200nvl-scm-n.cfg"
SRC_URI:append:gb200nvl-scm-n = " file://npcm-bmc-nvidia-gb200nvl-scm.dts;subdir=git/arch/${ARCH}/boot/dts/nuvoton"
