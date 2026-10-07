/*
 * PS5 RetroArch - configure-only Vulkan stub.
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * RetroArch's ./configure --enable-vulkan probes -lvulkan via vkCreateInstance.
 * The title links RADV (or ps5vk) statically and never loads this archive; the
 * real entry points come from the sibling driver's archives at link time.
 * Bodies are never packaged and never run on the console.
 */
__attribute__((visibility("default")))
int vkCreateInstance(void) { return 0; }
