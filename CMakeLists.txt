# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set a cache directory
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project ("PhotoManager")

# Include sub-projects.
file(GLOB_RECURSE INCLUDES src/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(PhotoManager ${SOURCES} ${INCLUDES})

foreach(dir ${INCLUDES})
    get_filename_component(HEADER_DIR ${dir} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${HEADER_DIR})
endforeach()

list(REMOVE_DUPLICATES INCLUDE_DIRS)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

#add_subdirectory ("src" "./bin/")
