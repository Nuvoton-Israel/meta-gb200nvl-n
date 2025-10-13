FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

PACKAGECONFIG:remove = " \
    ipmbsensor \
    mcutempsensor \
    intelcpusensor \
    intrusionsensor \
    exitairtempsensor \
    external \
    adcsensor \ 
"


SRC_URI:append = " file://0001-utils-build-the-string-safely-in-setupPropertiesChan.patch" 
