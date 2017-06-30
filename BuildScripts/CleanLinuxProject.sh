#!/bin/bash

#Cleans the project
#command goes like this

###
#   ./CleanLinuxProject.sh <type-of-build>
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
	echo "Cleaning $1 ..."
	make config=$1 clean
	echo "Done."
elif [ "all" == $1 ]
then
	echo "Cleaning all builds..."

	for str in "${arr[@]}"
	do
		echo "Cleaning $str..."
		make config=$str clean
		echo -e "\n"
	done

	echo "Done."
else
	echo "The specified configuration isn't supported/available."
	echo "Supported configurations are debug_i386, debug_amd64, release_i386, release_amd64"
	echo "Usage : ./CleanLinuxProject.sh <type-of-build>"
	echo "Example : ./CleanLinuxProject.sh release_i386"
	echo "To clear all builds use ./CleanLinuxProject.sh all"
fi
