#!/bin/bash

#Cleans the project
#command goes like this

###
#   ./CleanLinuxProject.sh <type-of-build>
# currently supported debug_i386, debug_amd64, release_i386 and release_amd64
###

arr=()
# 64 bit GL build
arr+=("debug_gl_amd64") 
arr+=("release_gl_amd64")
# 64 bit vulkan build
arr+=("debug_vulkan_amd64")
arr+=("release_vulkan_amd64")

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
	echo "Supported configurations are debug_gl_amd64, release_gl_amd64, debug_vulkan_amd64, release_vulkan_amd64" 
	echo "Usage : ./CleanLinuxProject.sh <type-of-build>"
	echo "Example : ./CleanLinuxProject.sh debug_gl_amd64"
	echo "To clear all builds use ./CleanLinuxProject.sh all"
fi
