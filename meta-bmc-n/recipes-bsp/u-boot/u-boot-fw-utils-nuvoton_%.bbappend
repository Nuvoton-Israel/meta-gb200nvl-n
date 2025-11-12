FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append:gb200nvl-bmc-n = " file://gb200nvl-bmc-n.cfg"
SRC_URI:append:gb200nvl-scm-n = " file://gb200nvl-scm-n.cfg"

SRC_URI:append:gb200nvl-bmc-n = " file://fw_env-bmc.config"
SRC_URI:append:gb200nvl-scm-n = " file://fw_env-scm.config"

do_install:append:gb200nvl-scm-n () {
	install -m 644 ${UNPACKDIR}/fw_env-scm.config ${D}${sysconfdir}/fw_env.config
}

do_install:append:gb200nvl-bmc-n () {
	install -m 644 ${UNPACKDIR}/fw_env-bmc.config ${D}${sysconfdir}/fw_env.config
}
