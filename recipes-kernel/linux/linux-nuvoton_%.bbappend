# Override some values in linux-aspeed.inc and linux-aspeed_git.bb
# with specifics of our Git repo, branch names, and Linux version
#
LINUX_VERSION = "6.6.58"
SRCREV="dd2dd05d0ca967174db5e7c84fb6dd0b0389de3b"
KSRC = "git://github.com/NVIDIA/linux;protocol=https;branch=develop-6.6"
# From 5.10+ the COPYING file changed
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# Fix build issues
SRC_URI:append = " file://1001-fix-duplicated-function.patch"
SRC_URI:append = " file://1002-drivers-iommu-Kconfig-missing-IOMMU_DMA.patch"
SRC_URI:append = " file://1003-arch-arm64-mmu-fix-porting-issue-don-t-acquire-mutex.patch"

# NPCM8XX common dts
SRC_URI:append = " file://1010-dt-bindings-clock-npcm845-Add-reference-25m-clock-pr.patch"
SRC_URI:append = " file://1011-arm64-dts-modify-clock-property-in-modules-node.patch"
SRC_URI:append = " file://1012-arm64-dts-npmc8xx-move-the-clk-handler-node-to-the-r.patch"
SRC_URI:append = " file://1013-arm64-dts-nuvoton-npcm8xx-add-pin-and-gpio-controller-nodes.patch"
SRC_URI:append = " file://1014-arm64-dts-nuvoton-npcm8xx-add-modules-node.patch"

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
SRC_URI:append = " file://1241-i2c-Remove-redundant-comparison-in-npcm_i2c_reg_slav.patch"
SRC_URI:append = " file://1242-i2c-npcm-restore-slave-addresses-array-length.patch"
SRC_URI:append = " file://1243-i2c-npcm-correct-the-read-write-operation-procedure.patch"
SRC_URI:append = " file://1244-i2c-npcm-use-a-software-flag-to-indicate-a-BER-condi.patch"
SRC_URI:append = " file://1245-i2c-npcm-Modify-timeout-evaluation-mechanism.patch"
SRC_URI:append = " file://1246-i2c-npcm-Assign-client-address-earlier-for-i2c_recov.patch"
SRC_URI:append = " file://1247-i2c-npcm-use-i2c-frequency-table.patch"
SRC_URI:append = " file://1248-i2c-npcm-Enable-slave-in-eob-interrupt.patch"

# NPCM8XX i3c driver
SRC_URI:append = " file://1250-i3c-sync-latest-svc-i3c-driver-and-header.patch"

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

# NPCM8XX pinctrl driver
SRC_URI:append = " file://1330-pinctrl-npcm8xx-remove-CTS-and-RTS-pins-from-bmcuart.patch"
SRC_URI:append = " file://1331-driver-pinctrl-npcm8xx-Set-strict-as-true.patch"
