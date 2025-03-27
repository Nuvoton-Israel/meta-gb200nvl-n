FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://gb200nvl-hmc-n.cfg"

SRC_URI:append:gb200nvl-hmc-scm-n = " file://nuvoton-npcm845-gb200nvl-scm-pincfg.dtsi;subdir=git/arch/arm/dts/"
SRC_URI:append:gb200nvl-hmc-scm-n = " file://nuvoton-npcm845-gb200nvl-scm.dts;subdir=git/arch/arm/dts/"
SRC_URI:append:gb200nvl-hmc-scm-n = " file://0001-uboot-gb200nvl-scm-dts.patch"
