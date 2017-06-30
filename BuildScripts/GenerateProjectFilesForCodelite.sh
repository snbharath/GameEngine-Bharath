#!/bin/bash

#It generates project files for make files

cd ../code
../ExternalTools/premake/linux/premake5 codelite

echo "Generated codelite project files ..."
