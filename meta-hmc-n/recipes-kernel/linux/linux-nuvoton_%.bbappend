FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:remove = " file://gb200nvl-hmc-n.cfg"

SRC_URI:append:gb200nvl-hmc-n = " file://gb200nvl-hmc-n.cfg"
SRC_URI:append:gb200nvl-hmc-n = " file://npcm-bmc-nvidia-gb200nvl-hmc.dts;subdir=git/arch/${ARCH}/boot/dts/nuvoton"

SRC_URI:append:gb200nvl-hmc-scm-n = " file://gb200nvl-hmc-scm-n.cfg"
SRC_URI:append:gb200nvl-hmc-scm-n = " file://npcm-bmc-nvidia-gb200nvl-hmc-scm.dts;subdir=git/arch/${ARCH}/boot/dts/nuvoton"
