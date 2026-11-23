#include "resources/DataPath.h"

#include <fstream>
#include <vector>

#if defined(_WIN32)
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #include <windows.h>
#elif defined(__APPLE__)
  #include <mach-o/dyld.h>
#elif defined(__linux__)
  #include <unistd.h>
#endif

// Both set by CMake.
// GE_DATA_DIR:          absolute path to the source tree's data/ (development builds).
// GE_DATA_DIR_FROM_BIN: install layout's data dir relative to the executable's dir.
#ifndef GE_DATA_DIR
#define GE_DATA_DIR "../data"
#endif
#ifndef GE_DATA_DIR_FROM_BIN
#define GE_DATA_DIR_FROM_BIN "data"
#endif

using namespace escape;

namespace {

// File that every valid data directory contains.
const char* const MARKER = "GreatEscape.xml";

/*
* Directory containing the running executable (no trailing separator),
* or "" if it cannot be determined.
*/
std::string executableDir(){
	std::string path;
#if defined(_WIN32)
	std::vector<char> buf(MAX_PATH);
	for (;;) {
		DWORD n = GetModuleFileNameA(NULL, buf.data(), (DWORD) buf.size());
		if (n == 0) return "";
		if (n < buf.size()) { path.assign(buf.data(), n); break; }
		buf.resize(buf.size() * 2);
	}
#elif defined(__APPLE__)
	uint32_t size = 0;
	_NSGetExecutablePath(NULL, &size);
	std::vector<char> buf(size);
	if (_NSGetExecutablePath(buf.data(), &size) != 0) return "";
	path = buf.data();
#elif defined(__linux__)
	std::vector<char> buf(4096);
	ssize_t n = readlink("/proc/self/exe", buf.data(), buf.size());
	if (n <= 0 || (size_t) n >= buf.size()) return "";
	path.assign(buf.data(), (size_t) n);
#endif
	std::string::size_type slash = path.find_last_of("/\\");
	return slash == std::string::npos ? "" : path.substr(0, slash);
}

bool isDataDir(const std::string& dir){
	return std::ifstream((dir + "/" + MARKER).c_str()).good();
}

/*
* Search order:
*  1. <exe dir>/<install layout's data dir>  - installed/packaged build
*  2. <exe dir>/data                         - portable folder (data next to exe)
*  3. GE_DATA_DIR                            - development build from the source tree
* Falls back to GE_DATA_DIR so load errors name a sensible path.
*/
std::string findDataDir(){
	std::vector<std::string> candidates;
	std::string exeDir = executableDir();
	if (!exeDir.empty()) {
		candidates.push_back(exeDir + "/" + GE_DATA_DIR_FROM_BIN);
		candidates.push_back(exeDir + "/data");
	}
	candidates.push_back(GE_DATA_DIR);

	for (size_t i = 0; i < candidates.size(); ++i) {
		if (isDataDir(candidates[i])) return candidates[i];
	}
	return GE_DATA_DIR;
}

}

std::string escape::dataPath(const std::string& file){
	static const std::string dir = findDataDir();
	return dir + "/" + file;
}
