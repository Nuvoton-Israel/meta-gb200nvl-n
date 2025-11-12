LINUX_VERSION = "6.12.41"
SRCREV = "69083b35ffc25e87691a0d45e2c9657d85c1e4e9"
KSRC = "git://github.com/NVIDIA/linux;protocol=https;branch=develop-6.12"

# From 5.10+ the COPYING file changed
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"
FILESEXTRAPATHS:prepend := "${THISDIR}/linux-nuvoton:"

# Copy from aspeed bbappend
SRC_URI:append = " file://enable-cifs-protocol.cfg"
SRC_URI:append = " file://nxp-rtc-PCFPCF85053A.patch"
SRC_URI:append = " file://0001-Update-shunt-resistor-micro-ohms-for-LTC4286-driver.patch"


# NPCM8XX common dts
SRC_URI:append = " file://0001-arm64-dts-nuvoton-use-the-clock-through-the-reset-no.patch"
SRC_URI:append = " file://0002-arm64-dts-adding-NPCM8xx-modules-node.patch"
SRC_URI:append = " file://0003-arm64-dts-enable-new-NPCM8XX-modules-is-the-NPCM8XX-.patch"
SRC_URI:append = " file://0004-arm64-dts-enable-i3c-driver.patch"
SRC_URI:append = " file://0005-arm64-dts-nuvoton-npcm8xx-add-second-pci-mbox-node.patch"
SRC_URI:append = " file://0006-arm64-dts-npcm8xx-add-SPI1-CS-pin-nodes.patch"
SRC_URI:append = " file://0007-arm64-dts-nuvoton-add-vdm-node.patch"
SRC_URI:append = " file://0008-arm64-dts-nucoton-add-aes-and-sha-nodes.patch"
SRC_URI:append = " file://0009-arm64-dts-nuvoton-Add-siox-gpio-node.patch"

# NPCM8XX NCSI/GMAC driver
SRC_URI:append = " file://0001-stmmac-Add-eee-fixup-disable.patch"
SRC_URI:append = " file://0002-net-ethernet-stmmac-add-sgmii-support.patch"
SRC_URI:append = " file://0003-net-stmmac-Add-NCSI-support-for-STMMAC.patch"
SRC_URI:append = " file://0004-net-ethernet-stmmac-add-sgmii-100-and-10-support.patch"
SRC_URI:append = " file://0005-net-ethernet-stmmac-clearing-interrupt-status-while-.patch"

# NPCM8XX FIU driver
SRC_URI:append = " file://0001-spi-npcm-fiu-add-dual-and-quad-write-support.patch"
SRC_URI:append = " file://0002-spi-npcm-fiu-enable-direct-write-support.patch"
SRC_URI:append = " file://0003-spi-npcm-fiu-fix-rx_dummy-value-in-spix-mode.patch"
SRC_URI:append = " file://0004-fiu_support_generic_spi_transfer.patch"

# NPCM8XX PSPI driver
SRC_URI:append = " file://0001-spi-npcm-pspi-Add-full-duplex-support.patch"
SRC_URI:append = " file://0002-spi-npcm-pspi-Fix-transfer-bits-per-word-issue-389.patch"

# NPCM8XX UDC driver
SRC_URI:append = " file://0001-usb-chipidea-add-SRAM-allocation-support.patch"
SRC_URI:append = " file://0002-usb-chipidea-udc-enforce-write-to-the-memory.patch"

# NPCM8XX i2c driver
SRC_URI:append = " file://0001-i2c-npcm-correct-the-read-write-operation-procedure.patch"
SRC_URI:append = " file://0002-i2c-npcm-use-a-software-flag-to-indicate-a-BER-condi.patch"
SRC_URI:append = " file://0003-i2c-Switch-back-to-struct-platform_driver-remove.patch"
SRC_URI:append = " file://0004-i2c-npcm-Modify-timeout-evaluation-mechanism.patch"
SRC_URI:append = " file://0005-i2c-npcm-Assign-client-address-earlier-for-i2c_recov.patch"
SRC_URI:append = " file://0006-i2c-npcm-use-i2c-frequency-table.patch"
SRC_URI:append = " file://0007-i2c-npcm-Enable-slave-in-eob-interrupt.patch"
SRC_URI:append = " file://0008-i2c-npcm-Add-slave-enable-disable-function.patch"
SRC_URI:append = " file://0009-i2c-npcm-Add-clock-toggle-in-case-of-stuck-bus-durin.patch"
SRC_URI:append = " file://0010-i2c-npcm-clear-the-FIFO-in-master-ber-case.patch"
SRC_URI:append = " file://0011-i2c-npcm-unexpected-SLVRSTR-IRQ-in-master-mode.patch"
SRC_URI:append = " file://0012-i2c-npcm-multi-master.patch"

# NPCM8XX PCEI driver
SRC_URI:append = " file://0001-pci-npcm-Add-NPCM-PCIe-RC-driver.patch"

# NPCM8XX PCEI mailbox
SRC_URI:append = " file://0001-misc-mbox-add-npcm7xx-pci-mailbox-driver.patch"
SRC_URI:append = " file://0002-misc-npcm-pci-mbox-convert-from-dos-to-unix.patch"
SRC_URI:append = " file://0003-misc-npcm-pci-mbox-add-id-number-for-multiple-pci-mb.patch"

# NPCM8XX Jtag Master driver
SRC_URI:append = " file://0001-misc-npcm8xx-jtag-master-Add-NPCM845-JTAG-master-dri.patch"
SRC_URI:append = " file://0002-misc-npcm8xx-jtag-master-deassert-TRST-for-normal-op.patch"
SRC_URI:append = " file://0003-misc-npcm8xx-jtag-master-Meet-requirements-for-AMD-r.patch"
SRC_URI:append = " file://0004-misc-npcm8xx-jtag-master-fix-initial-tlr-state.patch"
SRC_URI:append = " file://0005-misc-npcm8xx-jtag-master-add-new-JTAG_SIOCSTATE-ioct.patch"
SRC_URI:append = " file://0006-misc-npcm8xx-jtag-master-fix-build-error.patch"

# NPCM8XX WDT driver
SRC_URI:append = " file://0001-watchdog-npcm-Add-DT-restart-priority-and-reset-type.patch"
SRC_URI:append = " file://0002-watchdog-npcm-save-reset-status.patch"
SRC_URI:append = " file://0003-watchdog-npcm-Add-Nuvoton-NPCM8xx-support.patch"

# NPCM8XX media driver
SRC_URI:append = " file://0001-media-nuvoton-Sync-patches-from-NPCM-6.1-OpenBMC-bra.patch"
SRC_URI:append = " file://0002-media-nuvoton-Add-config-for-supporting-resolution-c.patch"
SRC_URI:append = " file://0003-media-nuvoton-Restart-frame-capturing-operation-if-V.patch"
SRC_URI:append = " file://0004-media-nuvoton-Fix-stuck-issue-due-to-no-video-signal.patch"
SRC_URI:append = " file://0005-media-nuvoton-Fix-VCD_STAT_BUSY-timeout-issue-when-E.patch"
SRC_URI:append = " file://0006-media-nuvoton-Add-retry-mechanism-for-detecting-vali.patch"
SRC_URI:append = " file://0007-media-nuvoton-Add-lock-for-npcm_video_start_frame.patch"
SRC_URI:append = " file://0008-media-nuvoton-Reduce-non-critical-logs-verbosity.patch"
SRC_URI:append = " file://0009-media-nuvoton-Adjust-VCD_DVO_DEL-according-to-detect.patch"
SRC_URI:append = " file://0010-media-nuvoton-Capture-full-frames-when-FIFO-overrun.patch"
SRC_URI:append = " file://0011-media-nuvoton-Fix-the-issue-of-continuously-getting-.patch"

# NPCM8XX pinctrl driver
SRC_URI:append = " file://0001-pinctrl-npcm8xx-remove-CTS-and-RTS-pins-from-bmcuart.patch"
SRC_URI:append = " file://0002-pinctrl-nuvoton-npcm8xx-add-rmii-enable-support.patch"
SRC_URI:append = " file://0003-pinctrl-nuvoton-npcm8xx-add-rgmii2-drive-strength-su.patch"
SRC_URI:append = " file://0004-pinctrl-nuvoton-clear-status-and-disable-GPIO-events.patch"
SRC_URI:append = " file://0005-pinctrl-nuvoton-npcm8xx-modify-GPIO7-pin-naming.patch"
SRC_URI:append = " file://0006-pinctrl-nuvoton-npcm8xx-Fix-lockdep-error-in-npcmgpi.patch"
SRC_URI:append = " file://0007-pinctrl-nuvoton-npcm8xx-add-irq_request_resources-su.patch"
SRC_URI:append = " file://0008-pinctrl-nuvoton-npcm8xx-modify-pin-configuration-fla.patch"

# NPCM8XX reset driver
SRC_URI:append = " file://0001-reset-npcm8xx-add-50-us-delay-for-usb-phy-clock-stab.patch"
SRC_URI:append = " file://0002-reset-npcm-reset-USB-hub.patch"

# ipmission SSIF_bmc driver
SRC_URI:append = " file://1340-ipmi-ssif_bmc-add-npcm-slave-disable-enable-method.patch"

# NTC3018Y intrusion detection and timestamp rework
SRC_URI:append = " file://0001-rtc-nuvoton-Add-intrusion-detection-and-timestamp-re.patch"

# GPIO bit-banging SIOX driver
SRC_URI:append = " file://0001-siox-Add-GPIO-bit-banging-SIOX-driver.patch"
