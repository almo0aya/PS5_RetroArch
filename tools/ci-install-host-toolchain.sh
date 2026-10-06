#!/usr/bin/env bash
# Host packages the CI title build needs (clang-18, meson, ninja, …).
set -euo pipefail
sudo apt-get update
sudo apt-get install --yes \
  clang-18 lld-18 llvm-18 llvm-18-dev libclang-rt-18-dev \
  libclc-18-dev llvm-spirv-18 libllvmspirvlib-18-dev \
  spirv-tools libclang-18-dev libclang-cpp18-dev glslang-tools \
  cmake ninja-build python3 python3-pip python3-mako python3-yaml python3-setuptools \
  pkg-config flex bison git curl wget unzip tar xz-utils patch rsync \
  zlib1g-dev nasm autoconf automake libtool g++ make binutils ca-certificates ccache
python3 -m pip install --break-system-packages 'meson>=1.4'
sudo ln -sfn /usr/bin/clang-18 /usr/bin/clang
sudo ln -sfn /usr/bin/clang++-18 /usr/bin/clang++
for tool in llvm-ar llvm-ranlib llvm-nm llvm-objcopy llvm-objdump llvm-strip ld.lld; do
  sudo ln -sfn "/usr/bin/${tool}-18" "/usr/local/bin/${tool}"
done
clang --version | head -n 1
ld.lld --version | head -n 1
meson --version
df -h /
