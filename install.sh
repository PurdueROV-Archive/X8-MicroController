#!/bin/bash

#Get and install gcc
wget https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update/+download/gcc-arm-none-eabi-4_9-2015q3-20150921-mac.tar.bz2 ~/Documnets/
tar -xvf gcc-arm-none-eabi-4_9-2015q3-20150921-mac.tar.bz2
echo "export PATH=$PATH:gcc-arm-none-eabi-4_9-2015q3-20150921-mac/bin" >> ~/.bashrc
chmod +x ~/.bashrc
echo "source ~/.bashrc" >> ~/.bash_profile


#Get and install stlink
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
brew install stlink



