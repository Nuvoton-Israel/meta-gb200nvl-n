OBMC_IMAGE_EXTRA_INSTALL:append = " persistent-net-name dpinit program-edid ethtool phytool"


# the bianry size is too large for 64MB flash, temporarily remove them
OBMC_IMAGE_EXTRA_INSTALL:remove:flash-65536 = " openocd"
OBMC_IMAGE_EXTRA_INSTALL:remove:flash-65536 = " nsmd"
IMAGE_FEATURES:remove:flash-65536 = " obmc-ikvm"
IMAGE_FEATURES:remove:flash-65536 = " obmc-webui"
IMAGE_FEATURES:remove:flash-65536 = " obmc-telemetry"
IMAGE_FEATURES:remove:flash-65536 = " obmc-debug-collector"


# Foe apseed bmc only?
OBMC_IMAGE_EXTRA_INSTALL:remove = " nvidia-bmc-compliance"
OBMC_IMAGE_EXTRA_INSTALL:remove = " nvidia-mc-aspeed-lib"
OBMC_IMAGE_EXTRA_INSTALL:remove = " nvidia-otp-monitor"


