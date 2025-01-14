FILESEXTRAPATHS:append:nuvoton := ":${THISDIR}/${PN}"

SRC_URI:append:gb200nvl-bmc-n = " file://0001-novnc-add-16-bit-hextile-support-for-nuvoton-ece-eng.patch"
SRC_URI:append:gb200nvl-hmc-n = " file://0001-hmc-novnc-add-16-bit-hextile-support-for-nuvoton-ece.patch"

