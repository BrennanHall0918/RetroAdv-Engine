#!/usr/bin/fish
rm -rf build
meson setup --prefix=/home/aaron/RetroAdv-Engine/build/install --bindir=bin --libdir=bin --datadir=data build
