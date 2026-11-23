#ifndef DATAPATH_H
#define DATAPATH_H

#include <string>

// Absolute path to the game's data/ directory, set by CMake (GE_DATA_DIR cache
// variable). Falls back to the old working-directory-relative location.
#ifndef GE_DATA_DIR
#define GE_DATA_DIR "../data"
#endif

namespace escape {

/*
* Full path to a file in the data directory, independent of the working directory.
* @param file: file name relative to data/, e.g. "thump.wav"
*/
inline std::string dataPath(const std::string& file){
	return std::string(GE_DATA_DIR) + "/" + file;
}

}

#endif
