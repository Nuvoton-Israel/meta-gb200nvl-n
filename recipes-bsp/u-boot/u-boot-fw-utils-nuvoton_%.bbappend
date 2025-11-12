require recipes-bsp/u-boot/u-boot-configure.inc
DEPENDS += "mtd-utils bison-native"

SRCREV = "bb363ca1706a0f9f03f25dbbde4f14936883f78d"

do_compile () {
  oe_runmake envtools
}

