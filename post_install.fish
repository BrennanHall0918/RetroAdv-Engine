#!/usr/bin/fish

env | grep MESON

if test -z $MESON_INSTALL_DESTDIR_PREFIX
    echo Could not find install location. >>/dev/stderr
    exit
end

cd (string replace "C:" "/c/" $MESON_INSTALL_DESTDIR_PREFIX)
pwd

cd bin
cp /clang64/bin/SDL3*.dll .
