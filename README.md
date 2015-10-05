# X8-MicroController

## What are we using
We are using the mbed libraries with an on your computer compiler called gcc-arm-embedded. The first step is to download the compiller and code flasher 
required to used mbed offline.These steps can be found bellow this description. 

## What are we using
This setup is based on the STM32F4xx_StdPeriph_Driver as well as the excellent Linux port of STLink by [Texane](https://github.com/texane/stlink) 

## How to get setup on Mac: 
Run mac-install.sh in terminal (make sure to reload your terminal so it reloads your path).  

The install file downloads arm-gcc, adds it to your path and installs st-link via homebrew. Check it out if you're familiar with these things and would like to configure it manually. 

## How to get setup on Windows: 
1. Download GCC-ARM for your system from here [here](https://launchpad.net/gcc-arm-embedded/+download)
2. Setup the path on your computer to the location of the downloaded GCC-ARM
4. Download the Make Binaries for your computer from [here](http://gnuwin32.sourceforge.net/packages/make.htm)
5. Setup the path on your computer to the location of the download Make binaries. (Note you can also put these in the same directory as GCC-ARM)
6. Download the STLink program binaries from [here](http://www.emb4fun.de/archive/stlink/index.html)
7. Do the same as step 5 except for the STLink binaries.
8. Clone this directory to your computer
9. go into the starter file
10. run "make"
11. Plug in your board via the mini-usb connector and run "make burn"
12. Voila! Your board will now be running the simple blink program.

