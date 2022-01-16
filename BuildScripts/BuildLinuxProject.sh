#!/bin/bash

#Builds the project
#command goes like this

###
#   ./BuildLinuxProject.sh [debug/release]_[dl/vulkan]_[i386/amd64/arm64]
# currently supported debug_i386, debug_amd64, release_i386, release_amd64, debug_arm64 and release_arm64
###

# Validate if the required environment variable is available
if [[ -z "${ARCH_TYPE}" ]]
then
	echo "Nothing to build. Set the ARCH_TYPE env variable"
	exit
fi

###################################################################
archType="${ARCH_TYPE}"

###################################################################
arr=()

if [[ "$archType" == "aarch64" ]]
then
	# 64 bit GL build
	arr+=("debug_gl_arm64") 
	arr+=("release_gl_arm64")
	# 64 bit vulkan build
	arr+=("debug_vulkan_arm64")
	arr+=("release_vulkan_arm64")
else
	# 64 bit GL build
	arr+=("debug_gl_amd64") 
	arr+=("release_gl_amd64")
	# 64 bit vulkan build
	arr+=("debug_vulkan_amd64")
	arr+=("release_vulkan_amd64")
fi
###################################################################

cd ../code


if [[ "${arr[*]}" == *"$1"* ]]
then
	echo "Building $1 ..."
	make -j4 config=$1
	echo "Done."
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
	echo "Usage : ./BuildLinuxProject.sh  [debug/release]_[dl/vulkan]_[i386/amd64/arm64]"
	echo "Example : ./BuildLinuxProject.sh debug_gl_amd64"
	echo "To build all use : ./BuildLinuxProject.sh all"
fi
