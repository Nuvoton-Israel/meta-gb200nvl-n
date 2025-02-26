FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://0001-correct-property-type.patch"

EXTRA_OEMESON:append = " -Dfw-debug=disabled"
EXTRA_OEMESON:append = " -Dpldm-package-verification=disabled"
