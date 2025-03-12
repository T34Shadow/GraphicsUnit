#include "Utilities.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string LoadFileAsString(std::string filename)
{
	std::ifstream fileStream(filename);

	if (fileStream.is_open())
	{
		std::stringstream read;
		read << fileStream.rdbuf();
		return read.str();
	}

	std::cout << "Error, failed to read file \"" + filename + "\"\n";
	return std::string();
}
