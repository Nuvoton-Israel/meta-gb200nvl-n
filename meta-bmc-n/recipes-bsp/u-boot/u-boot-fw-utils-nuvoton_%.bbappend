FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://gb200nvl-bmc-n.cfg"
SRC_URI:append = " file://fw_env.config"

do_install:append () {
	install -m 644 ${WORKDIR}/fw_env.config ${D}${sysconfdir}/fw_env.config
}
