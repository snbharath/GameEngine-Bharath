#!/bin/bash

#It generates project files for make files

cd ../code
../ExternalTools/premake/linux/premake5 gmake

echo "Generated make files ..."
