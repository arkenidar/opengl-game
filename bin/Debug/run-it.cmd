@echo off

set PATH=%PATH%;C:\msys64\usr\bin
set BUILD_TYPE=Debug

pushd %cd%
cd ../..
start %cd%/bin/%BUILD_TYPE%/opengl.exe %*
popd
