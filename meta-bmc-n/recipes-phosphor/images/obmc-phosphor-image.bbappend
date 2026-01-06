OBMC_IMAGE_EXTRA_INSTALL:remove = " openocd"
OBMC_IMAGE_EXTRA_INSTALL:remove = " nsmd"
OBMC_IMAGE_EXTRA_INSTALL:remove = " remote-media"

# Remove additional features to reduce image size
IMAGE_FEATURES:remove = " obmc-ikvm"
IMAGE_FEATURES:remove = " obmc-webui"
IMAGE_FEATURES:remove = " obmc-telemetry"
IMAGE_FEATURES:remove = " obmc-debug-collector"
