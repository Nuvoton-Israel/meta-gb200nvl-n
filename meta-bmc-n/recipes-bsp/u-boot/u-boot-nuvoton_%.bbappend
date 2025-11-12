FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append:gb200nvl-bmc-n = " file://gb200nvl-bmc-n.cfg"
SRC_URI:append:gb200nvl-scm-n = " file://gb200nvl-scm-n.cfg"
SRC_URI:append:gb200nvl-scm-n = " file://nuvoton-npcm845-gb200nvl-scm-pincfg.dtsi;subdir=git/arch/arm/dts/"
SRC_URI:append:gb200nvl-scm-n = " file://nuvoton-npcm845-gb200nvl-scm.dts;subdir=git/arch/arm/dts/"
SRC_URI:append:gb200nvl-scm-n = " file://0001-uboot-gb200nvl-scm-dts.patch"
