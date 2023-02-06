mkdir build
cd build
conan install .. --build=missing --profile ../conanprofile_win.txt
cmake .. -G "Visual Studio 17 2022"
cmake --build .