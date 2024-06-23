#ifndef FlowGraphConfigLoader_hpp
#define FlowGraphConfigLoader_hpp

#include "FlowGraph.hpp"
#include <istream>

/**
* Loads a flow graph configuration from the provided input stream.
*
* @param input The input stream containing the configuration data.
* @return The loaded flow graph configuration.
* @throws std::runtime_error if an error occurs while reading the data.
*/
FlowGraphConfig loadFlowGraphConfig(std::istream& input);

#endif