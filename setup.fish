#!/usr/bin/fish
rm -rf build
meson setup --prefix=(pwd)/build/install --bindir=bin --libdir=bin --datadir=data build
