#!/bin/bash

# If you have trouble downloading the gcc files, follow the link below for the newer releases
# https://launchpad.net/gcc-arm-embedded

# Get gcc-arm
wget https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update/+download/gcc-arm-none-eabi-4_9-2015q3-20150921-mac.tar.bz2

# Install into /usr/local/gcc-arm
tar -xvf gcc-arm-none-eabi-4_9-2015q3-20150921-mac.tar.bz2 -C /usr/local

# Add /usr/local/gcc-arm into path
echo "export PATH=$PATH:/usr/local/gcc-arm-none-eabi-4_9-2015q3/bin" >> ~/.bashrc
echo "source ~/.bashrc" >> ~/.bash_profile

# Get and install homebrew
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

# Get and install stlink and dfu-util
brew install stlink
brew install dfu-util

# Remove gcc tar file
rm gcc-arm-none-eabi-4_9-2015q3-20150921-mac.tar.bz2
