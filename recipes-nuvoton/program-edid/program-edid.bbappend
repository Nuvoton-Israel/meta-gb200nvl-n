FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SYSTEMD_ENVIRONMENT_FILE:${PN} += "obmc/edid/program_edid"
