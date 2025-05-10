# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TraSim_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TraSim_autogen.dir\\ParseCache.txt"
  "TraSim_autogen"
  )
endif()
