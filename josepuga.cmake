set(Compiler "clang++")
set(BuildType "Debug") # Debug or Release
set(DebugFlags "-g")
set(ReleaseFlags "-O3")
set(Flags "-Wall -Wextra -Wpedantic") # -Werror")
set(Standard "20")

message("José Puga's Settings...")

# Compiler Type. IMPORTANT!!! BE SURE CMAKE_CXX_COMPILER IS *NOT SET*
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER ${Compiler})
endif()

# Build Type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE ${BuildType})
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG ${DebugFlags})
set(CMAKE_CXX_FLAGS_RELEASE ${ReleaseFlags})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Flags}")

# C++ Standard
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD ${Standard} CACHE STRING "")
endif()
if(CMAKE_CXX_STANDARD LESS ${Standard})
    message(FATAL_ERROR " C++ incompatible standard ${CMAKE_CXX_STANDARD}. Required C++${Standard}")
endif()
