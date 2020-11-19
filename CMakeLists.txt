# cmake version to be used
cmake_minimum_required( VERSION 3.0 )

# project name
project( tftp_client )

# flags

# files

# include
include_directories( ./include ./src ./libs )

# target
add_executable( 
    binary 
    ./src/main.cpp 
    ./src/encoderDecoder.cpp 
    ./src/packetStructure.cpp
    ./src/supportFunctions.cpp 
)

# external libs


#add_library(randomize STATIC IMPORTED)
#set_property(TARGET randomize PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/Randomize/librandomize.a)
#target_link_libraries( binary randomize )