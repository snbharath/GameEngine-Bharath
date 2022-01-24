#include "FileOperations.h"

#include <fstream>
#include <iostream>

namespace GE {

// ---------------------------------------------------------------------------------

std::vector<char> FileOperations::ReadFile(const std::string& fileName)
{
	// Start reading the file from the end of it ::ate, so that we can get the size of the buffer to allocate
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file !");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize); // allocate a buffer to return

	// Go to the beginning of the file
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

// ---------------------------------------------------------------------------------



}