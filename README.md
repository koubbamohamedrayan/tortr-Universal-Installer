tortr

tortr is a cross distro package installer written in C++

it detects the system package manager and installs packages using it

supported package managers

pacman
apt
dnf
zypper
xbps
nix

fallback sources

flatpak
snap

example

tortr install firefox
tortr search neovim
tortr update

how it works

tortr checks which package manager exists on the system
when installing a package it uses the native package manager first
if the package is not found it tries fallback sources

building

requires g++

clone the repository and run

make

this will build the tortr binary

install

move the binary somewhere in your path for example

sudo mv tortr /usr/local/bin

license

GPL
