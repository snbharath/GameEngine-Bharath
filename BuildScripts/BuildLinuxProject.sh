#!/bin/bash

#Builds the project
#command goes like this

###
#   ./BuildLinuxProject.sh <type-of-build>
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


if [[ "${arr[*]}" == *"$1"* ]]
then
	echo "Building $1 ..."
	make config=$1
	echo "Done1."
	echo ${#arr[@]}
elif [ 'all' == "$1" ]
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
	echo "Supported configurations are debug_gl_amd64, release_gl_amd64, debug_vulkan_amd64, release_vulkan_amd64"
	echo "Usage : ./BuildLinuxProject.sh <type-of-build>"
	echo "Example : ./BuildLinuxProject.sh debug_gl_amd64"
	echo "To build all use : ./BuildLinuxProject.sh all"
fi
