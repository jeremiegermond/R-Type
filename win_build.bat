where python > nul 2>&1
if %errorlevel% neq 0 (
    @echo Installing python
    curl -O https://www.python.org/ftp/python/3.9.7/python-3.9.7-amd64.exe
    python-3.9.7-amd64.exe /quiet InstallAllUsers=1 PrependPath=1
    del python-3.9.7-amd64.exe
)

where conan > nul 2>&1
if %errorlevel% neq 0 (
    @echo Installing conan
    curl -L https://github.com/conan-io/conan/releases/download/1.59.0/conan-win-64.exe > conan-win-64.exe
    conan-win-64.exe \quiet
    del conan-win-64.exe
)

mkdir bin
mkdir build
cd build
conan install .. --build=missing --profile:build ../conanprofile_win.txt --profile:host ../conanprofile_win.txt
cmake .. -G "Visual Studio 17 2022"
cmake --build .
