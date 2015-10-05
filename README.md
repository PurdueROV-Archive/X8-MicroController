# X8-MicroController

## What are we using
We are using mbed libraries with the gcc-arm-embedded compiler and st-util or dfu-util to flash the microcontroller.

## Tools Needed
* [Latest GCC Embedded Compiler](https://launchpad.net/gcc-arm-embedded/+download)
* [Texane's St-Link Utilities](https://github.com/texane/stlink) 
* DFU Utilities (Varies per System)
* GNU Make and Core Utilities (For Windows Users)

## How to get setup on Mac: 
Run mac-install.sh in terminal (make sure to reload your terminal so it reloads your path).  

The install file downloads arm-gcc, adds it to your path and installs st-link and dfu-util via homebrew. Check it out if you're familiar with these things and would like to configure it manually. 

## How to get setup on Linux: 
1. Download and install the [latest GCC embedded compiler](https://launchpad.net/gcc-arm-embedded/+download) and add to your path
2. Install st-link and dfu-util through your package manager (instructions vary per distro)
3. If you can't find st-link in your package manager, you can try compiling it manually from [texane's st-link port](https://github.com/texane/stlink)


## How to get setup on Windows: 
1. Download the packaged tools from [IEEE ROV 2016 » Software » ROVTools.zip](https://drive.google.com/open?id=0B0L0G7Vonv4wNjk2T01HZGl4M28)
2. Extract and add the ROVTools\bin folder to your path
3. To edit your path on Windows: go to Control Panel » System » Advanced » Environment Variables. Edit your local path variable and add a ; then C:\path\to\ROVTools\bin 
4. Eventually, [follow the driver install directions here](https://community.particle.io/t/tutorial-installing-dfu-driver-on-windows-24-feb-2015/3518) (We won't be using this until we have the final microcontroller board, so ignore until then)

