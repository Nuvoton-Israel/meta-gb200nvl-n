FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:remove = " file://gb200nvl-bmc.cfg"

SRC_URI:append:gb200nvl-bmc-n = " file://gb200nvl-bmc-n.cfg"
SRC_URI:append:gb200nvl-bmc-n = " file://npcm-bmc-nvidia-gb200nvl-bmc.dts;subdir=git/arch/${ARCH}/boot/dts/nuvoton"

SRC_URI:append:gb200nvl-scm-n = " file://gb200nvl-scm-n.cfg"
SRC_URI:append:gb200nvl-scm-n = " file://npcm-bmc-nvidia-gb200nvl-scm.dts;subdir=git/arch/${ARCH}/boot/dts/nuvoton"

SRC_URI:append:gb200nvl-scm-n = " file://0001-net-phy-realtek-add-soft_reset-operation-for-rtl8211.patch"
SRC_URI:append:gb200nvl-scm-n = " file://0001-Revert-net-ethernet-stmmac-add-sgmii-100-and-10-supp.patch"
SRC_URI:append:gb200nvl-scm-n = " file://0001-net-stmmac-support-sgmii-auto-negotiation-for-npcm.patch"

#SRC_URI:append:gb200nvl-scm-n = " file://0007-drivers-net-stmmac-npcm-remove-soft-reset.patch"

#SRC_URI:append:gb200nvl-scm-n = " file://0001-add-w25q01nw-support.patch"
#SRC_URI:append:gb200nvl-scm-n = " file://0002-add-w25q512nwfiq-support.patch"
