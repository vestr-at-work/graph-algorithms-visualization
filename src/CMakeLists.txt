cmake_minimum_required (VERSION 3.20)

project("grid_algorithm_visualiser")

set(MY_EXE "grid_algorithm_visualiser")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${MY_EXE}
	"BFSGraphAlgorithm.cpp"
	"DrawHelpers.cpp"
	"gif.cpp"
	"FlowGraph.cpp"
	"FlowGraphConfigLoader.cpp"
	"FordFulkersonGraphAlgorithm.cpp"
	"GIFFrame.cpp"
	"GIFRenderer.cpp"
	"main.cpp"
	"PathGraph.cpp"
	"PathGraphConfigLoader.cpp"
	)

set_property(TARGET ${MY_EXE} PROPERTY CXX_STANDARD 23)
