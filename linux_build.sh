#!/bin/bash
if ! command -v conan >> /dev/null
then
    echo "First Build ? Installing conan using pip"
    pip install conan
fi

mkdir -p build && cd build
conan profile update settings.compiler.libcxx=libstdc++11 default
conan install .. --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
cmake .. -G "Unix Makefiles"
cmake --build .
