#!/usr/bin/fish

env | grep MESON

if test -z $MESON_INSTALL_DESTDIR_PREFIX
    echo Could not find install location. >>/dev/stderr
    exit
end

cd (string replace "C:" "/c/" $MESON_INSTALL_DESTDIR_PREFIX)
pwd

rm -vf bin/*.a
rm -vf bin/showanim.exe
rm -vf bin/showimage.exe
#rm -rvf lib
mv lib/* bin
rmdir lib

#Why is this needed??
cp bin/libSDL3_shared.dll bin/SDL3.dll

printf "@echo off\ncd bin\nretroadv.exe >log.txt 2>&1 5>&1" >launch.bat
