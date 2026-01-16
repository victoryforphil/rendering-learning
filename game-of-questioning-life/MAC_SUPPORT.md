# macOS Support Notes

This project uses Vulkan and GLFW. On macOS you will need MoltenVK (Vulkan-on-Metal) and the Vulkan SDK to provide the required loader, headers, and validation layers.

## Required tools
- Xcode Command Line Tools (`xcode-select --install`)
- Homebrew
- Vulkan SDK (LunarG) with MoltenVK
- GLFW via Bazel (already in MODULE.bazel)

## Recommended setup
1. Install Homebrew: https://brew.sh
2. Install the Vulkan SDK for macOS (includes MoltenVK):
   - https://vulkan.lunarg.com/sdk/home
3. Verify the SDK is visible:
   - `ls "$VULKAN_SDK"` should show `MoltenVK` and `macOS` directories.

## Bazel usage
- The repo enables platform-specific configs with `build --enable_platform_specific_config`.
- macOS builds can be invoked explicitly with `bazel build --config=macos //game:main`.
- If you need custom SDK paths, export `VULKAN_SDK` before running Bazel.

## Notes for Vulkan on macOS
- The Vulkan surface uses Metal via `VK_USE_PLATFORM_METAL_EXT`.
- Vulkan 1.3+ on macOS typically requires `VK_KHR_portability_enumeration` and the create flag `VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR` for full device discovery. If device enumeration fails, the Vulkan device creation layer should add that extension and flag.
