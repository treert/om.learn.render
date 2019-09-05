#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class XUtils
{
public:
	static std::string GetRealPath(const char* path, const char* dir = nullptr);
};