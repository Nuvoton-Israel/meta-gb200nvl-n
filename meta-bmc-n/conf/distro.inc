BUILD_TYPES_WITH_SECURE_SHELL = "prod debug"
DISTRO_FEATURES:append = "${@bb.utils.contains('BUILD_TYPES_WITH_SECURE_SHELL', '${BUILD_TYPE}', 'nvidia-secure-shell', '', d)}"
DISTRO_FEATURES:append = " mctp-inkernel"
DISTRO_FEATURES:append = " erotless-bmc"
