FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

LIC_FILES_CHKSUM = "file://LICENSE;md5=75859989545e37968a99b631ef42722e"
SRC_URI := "git://github.com/Nuvoton-Israel/obmc-ikvm.git;branch=upstream-v4l2;protocol=https"
SRCREV := "0f73cf51b823e3e7a0194fe49844ca9c1b75df1b"

SYSTEMD_SERVICE:${PN}:remove = "start-ipkvm.service"
SYSTEMD_SERVICE:${PN}:append += "obmc-ikvm.service"

do_patch() {
	sed -i -e 's/ci_hdrc.0/ci_hdrc.1/g' ${S}/create_usbhid.sh \
		${S}/create_usbhid.sh
}

