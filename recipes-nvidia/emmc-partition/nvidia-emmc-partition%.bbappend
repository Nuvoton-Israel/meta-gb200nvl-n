FILESEXTRAPATHS:append := "${THISDIR}/files:"

SRC_URI:append = " file://create-partition.sh"

do_install:append() {
    install -m 0755 ${WORKDIR}/create-partition.sh ${D}/${bindir}/
}
