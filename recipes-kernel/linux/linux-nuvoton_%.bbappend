# Override some values in linux-aspeed.inc and linux-aspeed_git.bb
# with specifics of our Git repo, branch names, and Linux version
#
LINUX_VERSION = "6.6.58"
SRCREV="c0e89f5f7ef7c9053922055c24211bbcfd033fee"
KSRC = "git://github.com/NVIDIA/linux;protocol=https;branch=develop-6.6"
# From 5.10+ the COPYING file changed
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# Fix build issues
SRC_URI:append = " file://1001-fix-duplicated-function.patch"
SRC_URI:append = " file://1002-drivers-iommu-Kconfig-missing-IOMMU_DMA.patch"
SRC_URI:append = " file://1003-arch-arm64-mmu-fix-porting-issue-don-t-acquire-mutex.patch"

# NPCM8XX common dts
SRC_URI:append = " file://1050-arm64-dts-adding-NPCM8xx-modules-node.patch"
SRC_URI:append = " file://1051-arm64-dts-enable-new-NPCM8XX-modules-is-the-NPCM8XX-.patch"
SRC_URI:append = " file://1052-arm64-dts-nuvoton-modify-pin-name-pspi2-to-pspi.patch"
SRC_URI:append = " file://1053-arm64-dts-nuvoton-Add-vcd-ece-gfxi-nodes-for-V4L2.patch"
SRC_URI:append = " file://1054-arm64-dts-npcm8xx-Add-i3c-nodes.patch"
SRC_URI:append = " file://1055-arm64-dts-nuvoton-add-npcm8xx-nodes.patch"
SRC_URI:append = " file://1056-arm64-dts-nuvoton-enable-npcm8xx-nodes-in-NPCM845-EV.patch"
SRC_URI:append = " file://1057-arm64-dts-nuvoton-corrects-lpc_bpc-compatible.patch"
SRC_URI:append = " file://1058-arm64-dts-npcm845-evb-Enable-i3c0-DMA.patch"
SRC_URI:append = " file://1059-USB-host-Add-USB-ohci-support-for-nuvoton-npcm7xx-pl.patch"
SRC_URI:append = " file://1060-arm64-dts-npcm845-evb-Fix-spi-gpio-gpio-names.patch"
SRC_URI:append = " file://1061-arm64-dts-npcm8xx-modify-ohci-compatible-name.patch"
SRC_URI:append = " file://1062-arm64-dts-npcm8xx-correct-pinctrl-unit-address.patch"
SRC_URI:append = " file://1063-arm64-dts-npcm845-evb-Add-spi-tx-bus-width-property-.patch"
SRC_URI:append = " file://1064-arm64-dts-add-bu4b-and-bu5b-pins-support.patch"
SRC_URI:append = " file://1065-pinctrl-pinctrl-npcm8xx-update-pin-names.patch"
SRC_URI:append = " file://1066-arm64-dts-npcm8xx-add-clock-properties-to-reset-node.patch"
SRC_URI:append = " file://1067-arm64-dts-npmc8xx-move-the-clk-handler-node-to-the-r.patch"
SRC_URI:append = " file://1068-dts-arm64-npcm8xx-add-simple-mfd-in-gcr-node.patch"
SRC_URI:append = " file://1069-dts-npcm8xx-add-fiu-tip-node.patch"
SRC_URI:append = " file://1070-dts-arm64-nuvoton-Add-SMB16B-pin-support.patch"
SRC_URI:append = " file://1071-arm64-dts-nuvoton-remove-npcm750-compatible-WD-node-.patch"
SRC_URI:append = " file://1072-arm64-dts-nuvoton-npcm845-set-FIU-UMA-read.patch"
SRC_URI:append = " file://1073-dts-nuvoton-evb-npcm845-support-openbmc-partition.patch"
SRC_URI:append = " file://1074-dts-arm64-npcm845-evb-update-partition-for-nv-openbm.patch"

# NPCM8XX NCSI driver
SRC_URI:append = " file://1200-net-stmmac-Add-NCSI-support-for-STMMAC.patch"

# NPCM8XX FIU driver
SRC_URI:append = " file://1210-spi-npcm-fiu-add-dual-and-quad-write-support.patch"

# NPCM8XX PSPI driver
SRC_URI:append = " file://1220-spi-npcm-pspi-Fix-transfer-bits-per-word-issue-389.patch"

# NPCM8XX UDC driver
SRC_URI:append = " file://1230-usb-chipidea-udc-enforce-write-to-the-memory.patch"
SRC_URI:append = " file://1231-usb-chipidea-add-SRAM-allocation-support.patch"

# NPCM8XX i2c driver
SRC_URI:append = " file://1240-i2c-npcm-disable-interrupt-enable-bit-before-devm_re.patch"
SRC_URI:append = " file://1241-i2c-npcm-use-i2c-frequency-table.patch"
SRC_URI:append = " file://1242-i2c-npcm-use-a-software-flag-to-indicate-a-BER-condi.patch"
SRC_URI:append = " file://1243-i2c-npcm-Modify-timeout-evaluation-mechanism.patch"
SRC_URI:append = " file://1244-i2c-npcm-Modify-the-client-address-assignment.patch"
SRC_URI:append = " file://1245-i2c-npcm7xx.c-Enable-slave-in-eob-interrupt.patch"
SRC_URI:append = " file://1246-i2c-npcm-correct-the-read-write-operation-procedure.patch"

# NPCM8XX PCEI driver
SRC_URI:append = " file://1260-pci-npcm-Add-NPCM-PCIe-RC-driver.patch"

# NPCM8XX PCEI mailbox
SRC_URI:append = " file://1270-misc-mbox-add-npcm7xx-pci-mailbox-driver.patch"

# NPCM8XX Jtag Master driver
SRC_URI:append = " file://1280-misc-npcm8xx-jtag-master-Add-NPCM845-JTAG-master-dri.patch"
SRC_URI:append = " file://1281-misc-npcm8xx-jtag-master-deassert-TRST-for-normal-op.patch"
SRC_URI:append = " file://1282-misc-npcm8xx-jtag-master-Meet-requirements-for-AMD-r.patch"
SRC_URI:append = " file://1283-misc-npcm8xx-jtag-master-fix-initial-tlr-state.patch"
SRC_URI:append = " file://1284-misc-npcm8xx-jtag-master-add-new-JTAG_SIOCSTATE-ioct.patch"

# NPCM8XX WDT driver
SRC_URI:append = " file://1290-drivers-watchdog-npcm-fix-missing-variables.patch"
SRC_URI:append = " file://1291-watchdog-npcm-Add-Nuvoton-NPCM8xx-support.patch"
SRC_URI:append = " file://1292-watchdog-npcm-Add-DT-restart-priority-and-reset-type.patch"
SRC_URI:append = " file://1293-watchdog-npcm-save-reset-status.patch"

# NPCM8XX ADC driver
SRC_URI:append = " file://1300-iio-adc-npcm-fix-inappropriate-error-log.patch"
SRC_URI:append = " file://1301-iio-adc-fix-adc-driver-issue.patch"
SRC_URI:append = " file://1302-iio-adc-npcm-cover-more-module-reset-cases.patch"
SRC_URI:append = " file://1303-iio-adc-modify-wake-up-function.patch"
SRC_URI:append = " file://1304-iio-adc-npcm-clear-interrupt-status-at-probe.patch"
SRC_URI:append = " file://1305-iio-adc-npcm-add-reset-method-to-fix-get-value-faile.patch"
SRC_URI:append = " file://1306-iio-adc-npcm-remove-reset-method-flag.patch"

# NPCM8XX network driver
SRC_URI:append = " file://1310-stmmac-Add-eee-fixup-disable.patch"
SRC_URI:append = " file://1311-net-ethernet-stmmac-add-sgmii-support.patch"

# NPCM8XX media driver
SRC_URI:append = " file://1320-media-nuvoton-Add-head1-hsync-support.patch"
