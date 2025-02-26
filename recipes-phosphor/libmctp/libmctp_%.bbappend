
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://0001-increase-mctp-pkt-buffer-size.patch"

SYSTEMD_SERVICE:${PN}:remove = " mctp-spi0-ctrl.service \
                                 mctp-spi0-demux.service \
                                 mctp-spi0-demux.socket \
                                 mctp-spi2-ctrl.service \
                                 mctp-spi2-demux.service \
                                 fpga0-ap-recovery.target \
                               "

do_install:append() {
    rm -f ${D}${nonarch_base_libdir}/systemd/system/mctp-spi0-ctrl.service
    rm -f ${D}${nonarch_base_libdir}/systemd/system/mctp-spi0-demux.service
    rm -f ${D}${nonarch_base_libdir}/systemd/system/mctp-spi0-demux.socket
    rm -f ${D}${nonarch_base_libdir}/systemd/system/mctp-spi2-ctrl.service
    rm -f ${D}${nonarch_base_libdir}/systemd/system/mctp-spi2-demux.service
    rm -f ${D}${nonarch_base_libdir}/systemd/system/fpga0-ap-recovery.target
}