#include "XUtils.h"

std::string XUtils::GetRealPath(const char* path, const char* dir /* = nullptr */)
{
	auto *p = strchr(path, '/');
	if (!p) {
		p = strchr(path, '\\');
	}
	if (p) {
		return path;
	}

	std::ifstream f(path);
	if (f.good()) {
		return path;
	}

	std::string np = path;
	if (dir) {
		np = dir + np;
	}

	int num = 5;
	while (num-- > 0)
	{
		f.open(np);
		if (f.good()) {
			return np;
		}
		np = std::string("../") + np;
	}
	return "";// error not found
}