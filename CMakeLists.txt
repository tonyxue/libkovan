#################################
#   libkovan CMake Build File   #
#################################

project(libkovan)

cmake_minimum_required(VERSION 2.8.0)

set(INCLUDE ${libkovan_SOURCE_DIR}/include)
set(SRC ${libkovan_SOURCE_DIR}/src)
set(PIXELTOASTER ${SRC}/pixeltoaster)

include_directories(${INCLUDE})

file(GLOB INCLUDES ${INCLUDE}/kovan/*.h ${INCLUDE}/kovan/*.hpp)
file(GLOB SOURCES ${SRC}/*.cpp ${PIXELTOASTER}/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

#############################
#   PixelToaster settings   #
#############################

if(APPLE)
	file(GLOB OBJC_SOURCES ${PIXELTOASTER}/*.mm)
elseif(WIN32)
	add_definitions(-fno-check-new)
endif()

############################################
#   Resolve external library directories   #
############################################

if(DEFINED ENV{OPENNI2_INCLUDE_DIR})
  set(OPENNI2_INCLUDE $ENV{OPENNI2_INCLUDE_DIR})
elseif(WIN32)
  set(OPENNI2_INCLUDE "C:/Program Files (x86)/OpenNI2/Include")
else()
  set(OPENNI2_INCLUDE "${CMAKE_SOURCE_DIR}/../OpenNI2/Include")
endif()

if(DEFINED ENV{OPENNI2_LIB_DIR})
  set(OPENNI2_LIB $ENV{OPENNI2_LIB_DIR})
elseif(WIN32)
  set(OPENNI2_LIB "C:/Program Files (x86)/OpenNI2/Lib")
else()
  set(OPENNI2_LIB "${CMAKE_SOURCE_DIR}/../OpenNI2/Bin/x64-Release")
endif()

if(DEFINED ENV{OPENCV_INCLUDE_DIR})
  set(OPENCV_INCLUDE $ENV{OPENCV_INCLUDE_DIR})
elseif(WIN32)
  set(OPENCV_INCLUDE ${CMAKE_SOURCE_DIR}/../opencv/build/install/include)
endif()

if(DEFINED ENV{OPENCV_LIB_DIR})
  set(OPENCV_LIB $ENV{OPENCV_LIB_DIR})
elseif(WIN32)
  set(OPENCV_LIB ${CMAKE_SOURCE_DIR}/../opencv/build/install/x86/vc11/lib)
endif()

if(DEFINED ENV{ZBAR_DIR})
  set(ZBAR_DIR $ENV{ZBAR_DIR})
elseif(WIN32)
  set(ZBAR_DIR "C:/Program Files (x86)/ZBar")
endif()

################################
#   Include/link directories   #
################################

include_directories(${OPENNI2_INCLUDE})
link_directories(${OPENNI2_LIB})
if(WIN32)
  include_directories(${CMAKE_SOURCE_DIR}/../prefix/include)
  include_directories(${OPENCV_INCLUDE})
  include_directories(${ZBAR_DIR}/include)

  link_directories(${CMAKE_SOURCE_DIR}/../prefix/lib)
  link_directories(${OPENCV_LIB})
  link_directories(${ZBAR_DIR}/lib)  
endif()

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g")
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
elseif(WIN32)
  set(SOURCES ${SOURCES} ${CMAKE_SOURCE_DIR}/compat/mingw.cpp)
elseif(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# add_definitions(-O3)

###################################
#   Create the libkovan library   #
###################################

add_library(kovan SHARED ${SOURCES} ${OBJC_SOURCES} ${INCLUDES})
if(WIN32)
  # TODO: Make sure these are current
  target_link_libraries(kovan opencv_core300 opencv_highgui300 opencv_imgproc300 libzbar-0 OpenNI2)
  target_link_libraries(kovan d3d9)
else()
  target_link_libraries(kovan pthread opencv_core opencv_highgui opencv_imgproc zbar avcodec avformat avutil z swscale bz2 OpenNI2)
endif()

if(APPLE)
# set(CMAKE_OSX_SYSROOT "${OSX_DEVELOPER_ROOT}/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk")
  find_library(OPENGL_LIBRARY OpenGL)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(CORESERVICE_LIBRARY CoreServices)
  target_link_libraries(kovan ${OPENGL_LIBRARY} ${COCOA_LIBRARY} ${CORESERVICE_LIBRARY})
endif()

if(KOVAN)
  target_link_libraries(kovan i2c_wrapper)
  add_definitions(-DKOVAN)
endif()

# add_definitions(-DARDRONE_DEBUG)

if(WIN32)
  target_link_libraries(kovan ws2_32)
  install(FILES ${INCLUDES} DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/include/kovan)
  install(TARGETS kovan
          ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib
          RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib)
else()
  install(FILES ${INCLUDES} DESTINATION include/kovan)
  install(TARGETS kovan LIBRARY DESTINATION lib)
endif()

########################################
#   Doxygen documentation (optional)   #
########################################

option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" ON)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  file(MAKE_DIRECTORY doc)
  set(DOXYGEN_DIR ${PROJECT_SOURCE_DIR}/doxygen)
  add_custom_target (Docs++ ALL 
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_DIR}/Doxyfile++ SOURCES ${DOXYGEN_DIR}/Doxyfile++
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  add_custom_target (Docs ALL 
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_DIR}/Doxyfile SOURCES ${DOXYGEN_DIR}/Doxyfile
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  add_custom_target(DocMetadata ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${DOXYGEN_DIR}/metadata.ini ${PROJECT_SOURCE_DIR}/doc)
endif()

##################
#   Depth test   #
##################

add_executable(test_depth ${CMAKE_SOURCE_DIR}/test.cpp)
target_link_libraries(test_depth kovan)


