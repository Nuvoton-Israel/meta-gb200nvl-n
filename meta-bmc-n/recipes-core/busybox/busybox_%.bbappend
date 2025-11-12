FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://head.cfg"
SRC_URI:append = " file://ifconfig.cfg"

