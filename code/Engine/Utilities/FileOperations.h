#pragma once

#include "../EngineCommonIncludes.h"

namespace GE
{
	class FileOperations
	{
	public :
		
		// return char array
		static std::vector<char> ReadFile(const std::string& fileName);
	};
}