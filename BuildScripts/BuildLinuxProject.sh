#!/bin/bash

#Builds the project
#command goes like this

###
#   ./BuildLinuxProject.sh <type-of-build>
# currently supported debug_i386, debug_amd64, release_i386 and release_amd64
###

arr=()
arr+=("debug_i386") # 32 bit debug build
arr+=("debug_amd64") # 64 bit debug build
arr+=("release_i386") # 32 bit release build
arr+=("release_amd64") # 64 bit release build

cd ../code


if [[ " ${arr[*]} " == *"$1"* ]];
then
	echo "Building $1 ..."
	make config=$1
	echo "Done."
elif [ "all" == $1 ]
then
	echo "Building all configurations..."

	for str in "${arr[@]}"
	do
		echo "Building $str..."
		make config=$str
		echo -e "\n"
	done

	echo "Done."
else
	echo "The specified configuration isn't supported/available."
	echo "Supported configurations are debug_i386, debug_amd64, release_i386, release_amd64"
	echo "Usage : ./BuildLinuxProject.sh <type-of-build>"
	echo "Example : ./BuildLinuxProject.sh release_i386"
	echo "To build all use : ./BuildLinuxProject.sh all"
fi
