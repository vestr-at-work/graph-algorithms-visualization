#ifndef PathGraphConfigLoader_hpp
#define PathGraphConfigLoader_hpp

#include "PathGraph.hpp"
#include <istream>

/**
* Loads a path graph configuration from the provided input stream.
*
* @param input The input stream containing the configuration data.
* @return The loaded path graph configuration.
* @throws std::runtime_error if an error occurs while reading the data.
*/
PathGraphConfig loadPathGraphConfig(std::istream& input);

#endif