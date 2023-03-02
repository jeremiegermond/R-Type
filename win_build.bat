if not exist python.exe (
    @echo Installing python
    curl -O https://www.python.org/ftp/python/3.9.7/python-3.9.7-amd64.exe
    python-3.9.7-amd64.exe /quiet InstallAllUsers=1 PrependPath=1
    del python-3.9.7-amd64.exe
)

if not exist conan.exe (
    @echo Installing conan
    python -m pip install conan==1.59.0
)

mkdir bin
mkdir build
cd build
conan install .. --build=missing --profile ../conanprofile_win.txt
cmake .. -G "Visual Studio 17 2022"
cmake --build .
