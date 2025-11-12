FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
IGPS_CSVS = "registers_bootblock.csv"

SRCREV = "44769f06590b6bf1d71c21b353fc00e18c8b7fb5"


SRC_URI:append:gb200nvl-scm-n = " file://settings.json"
