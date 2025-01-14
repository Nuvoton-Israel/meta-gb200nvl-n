require recipes-bsp/u-boot/u-boot-configure.inc
DEPENDS += "mtd-utils bison-native"

SRCREV = "287358543d056b070bda18b5fe1eb4806d081a5d"

do_compile () {
  oe_runmake envtools
}

