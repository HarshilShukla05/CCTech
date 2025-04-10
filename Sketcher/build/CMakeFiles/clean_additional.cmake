# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\Sketcher_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Sketcher_autogen.dir\\ParseCache.txt"
  "Sketcher_autogen"
  )
endif()
