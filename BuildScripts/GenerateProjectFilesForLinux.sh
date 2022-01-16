#!/bin/bash

#It generates project files for make files

cd ../code

#what is the architecture ? for rpi it should be aarch64
if [[ $(arch) == "aarch64" ]] 
then
   echo "Generating project files for arm64(rpi)...."
   ../ExternalTools/premake/linux-rpi/premake5 gmake
else
   echo "Generating project files for linux...."
   ../ExternalTools/premake/linux/premake5 gmake
fi
