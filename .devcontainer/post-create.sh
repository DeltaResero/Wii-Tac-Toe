#!/bin/sh
set -ex

# Remove installed version of cmake
apt-get purge cmake -y

# Install required cmake version
curl --location https://github.com/Kitware/CMake/releases/download/v4.0.3/cmake-4.0.3-linux-x86_64.sh --output /tmp/cmake.sh
sh /tmp/cmake.sh --skip-license --prefix=/usr
rm -rf /tmp/cmake.sh

# Install required ninja version
curl --location https://github.com/ninja-build/ninja/releases/download/v1.13.0/ninja-linux.zip --output /usr/bin/ninja.gz
gunzip --verbose --force /usr/bin/ninja.gz
chmod a+x /usr/bin/ninja
