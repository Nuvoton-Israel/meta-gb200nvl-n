SUMMARY = "Display port init app"
DESCRIPTION = "Display port init app"
HOMEPAGE = ""
LICENSE = "CLOSED"

inherit autotools pkgconfig systemd obmc-phosphor-systemd

DEPENDS += "phosphor-logging"

S = "${WORKDIR}/sources"
UNPACKDIR = "${S}"

SRC_URI = " \
    file://configure.ac \
    file://main.cpp \
    file://Makefile.am \
    file://anx9807.h \
    file://dpinit.service \
    file://dp_hotplug.sh \
    file://dp_hotplug.service \
"

# Use the modern _${PN} syntax instead of the deprecated :${PN}
SYSTEMD_SERVICE_${PN} = "dpinit.service dp_hotplug.service"

# The autotools class's do_install will install the dpinit binary.
# We only need to append to it to install the script.
do_install:append() {
    # Look for the script inside the source directory ${S}, as the
    # error log shows it's not in ${WORKDIR}.
    install -m 0755 ${S}/dp_hotplug.sh ${D}${bindir}/
}