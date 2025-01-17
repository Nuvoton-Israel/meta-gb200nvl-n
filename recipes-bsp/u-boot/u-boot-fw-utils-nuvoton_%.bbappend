require recipes-bsp/u-boot/u-boot-configure.inc
DEPENDS += "mtd-utils bison-native"

SRCREV = "4b7dd7dd317115fa1978dc0a89ca313cfd036294"

do_compile () {
  oe_runmake envtools
}

