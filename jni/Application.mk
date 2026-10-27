# Target modern 64-bit ARM processors exclusively
APP_ABI := arm64-v8a

# Set the minimum system API baseline required by the sandboxing framework
APP_PLATFORM := android-24

# Enforce clean standard C++ optimization configurations
APP_STL := c++_static
