tortr

universal linux package installer

tortr installs software using the system package manager and if that fails it tries other package systems

goal

install packages across linux distributions with one command

supported package managers

pacman  arch linux and arch based distros
apt  debian ubuntu
dnf  fedora
zypper  opensuse
xbps  void linux

fallback package systems

flatpak
snap

usage

install package

tortr install <package>

remove package

tortr remove <package>

search package

tortr search <package>

update system

tortr update

show version

tortr version

help

tortr help

example

tortr install firefox
tortr search neovim
tortr update

installation

compile

g++ tortr.cpp -o tortr

optional install system wide

./tortr setup

tested on

arch linux
ubuntu

author

torter
koubbamohamedrayan
