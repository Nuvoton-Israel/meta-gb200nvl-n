FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRCREV = "44769f06590b6bf1d71c21b353fc00e18c8b7fb5"

IGPS_CSVS = "registers_bootblock.csv"

SRC_URI:append = " file://settings.json"