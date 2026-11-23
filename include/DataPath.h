#ifndef DATAPATH_H
#define DATAPATH_H

#include <string>

namespace escape {

/*
* Full path to a file in the game's data directory, independent of the
* working directory. The directory is located once, on first use; see
* DataPath.cpp for the search order.
* @param file: file name relative to data/, e.g. "thump.wav"
*/
std::string dataPath(const std::string& file);

}

#endif
