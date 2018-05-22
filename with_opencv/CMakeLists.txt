# Add CMake to [Nghia Ho's 5 points algorithm](http://nghiaho.com/?p=1675)

project(fivePoint_project)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)
set(fivePoint_version 0.1.5)

include(GenerateExportHeader) # for Cmake functions

file(GLOB FIVE_POINT_SRC_FILES ${CMAKE_SOURCE_DIR}/5point/5point.cpp ${CMAKE_SOURCE_DIR}/5point/Rpoly.cpp) # not include the main.cpp
file (GLOB FIVE_POINT_SRC_HEADERS ${CMAKE_SOURCE_DIR}/5point/*.h)

set (FIVE_POINT_FILES ${FIVE_POINT_SRC_FILES} ${FIVE_POINT_SRC_HEADERS})

message(STATUS "FIVE_POINT_SRC_FILES:${FIVE_POINT_SRC_FILES}")
message(STATUS "FIVE_POINT_SRC_HEADERS:${FIVE_POINT_SRC_HEADERS}")
message(STATUS "FIVE_POINT_FILES:${FIVE_POINT_FILES}")

#User options
set(BUILD_SHARED_LIBS false CACHE BOOL "Build shared library.")

# Add the library.
if( BUILD_SHARED_LIBS )
add_library( fivePoint SHARED ${FIVE_POINT_FILES} )
set_target_properties( fivePoint PROPERTIES DEBUG_POSTFIX -d )
else()
add_library( fivePoint ${FIVE_POINT_FILES})
set_target_properties( fivePoint PROPERTIES DEBUG_POSTFIX -s-d )
set_target_properties( fivePoint PROPERTIES RELEASE_POSTFIX -s )
set_target_properties( fivePoint PROPERTIES MINSIZEREL_POSTFIX -s )
endif()

if( WIN32 )
set( SHARE_PATH "." )
set( LIB_PATH "lib" )
elseif(APPLE)
set( SHARE_PATH "${CMAKE_INSTALL_PREFIX}/share/fivePoint" )
set( LIB_PATH "lib" )
else()
set( SHARE_PATH "${CMAKE_INSTALL_PREFIX}/share/fivePoint" )

if( LIB_SUFFIX )
    set( LIB_PATH "lib${LIB_SUFFIX}" )
else()
    set( LIB_PATH "lib" )
endif()
endif()


generate_export_header(fivePoint)
set_property(TARGET fivePoint PROPERTY VERSION ${fivePoint_version})

install(TARGETS fivePoint EXPORT fivePointTargets
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
INCLUDES DESTINATION include
)

install(
    FILES
      ${FIVE_POINT_SRC_HEADERS}
    DESTINATION
      include
    COMPONENT
      Devel
  )


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/fivePoint/fivePointVersion.cmake"
    VERSION ${fivePoint_version}
    COMPATIBILITY AnyNewerVersion
)

export(EXPORT fivePointTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/fivePoint/fivePointTargets.cmake"
    NAMESPACE fivePoint::
)
configure_file(cmake/fivePointConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/fivePoint/fivePointConfig.cmake"
    COPYONLY # not COPY_ONLY
)

set(ConfigPackageLocation lib/cmake/fivePoint)
install(EXPORT fivePointTargets
FILE
    fivePointTargets.cmake
NAMESPACE
    fivePoint::
DESTINATION
    ${ConfigPackageLocation}
)

install(
FILES
    cmake/fivePointConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/fivePoint/fivePointConfig.cmake"
DESTINATION
    ${ConfigPackageLocation}
COMPONENT
    Devel
)


#=============================================================================#
# BUILD WILL GET INVOLVED WITH SPECIFIC VERSION OF OPENCV, THIS IS A DRAWBACK #
# SO BUILD WIHT DIFFERENT OPENCV, PLEASE, OR JUST DRAG IN THE PROJECT         #
#=============================================================================#

# User options
set( USE_CV3 FALSE )
set( USE_CV3 TRUE ) # comment this to use OpenCV 2.x

message(STATUS "USE_CV3 value:${USE_CV3}")
if (USE_CV3)
message(STATUS "Using OpenCV 3.x ...")
include( $ENV{OpenCV3_DIR}/OpenCVConfig.cmake )
add_definitions(-D_CV_VERSION_3) # `#ifdef _CV_VERSION_3`

else()
message(STATUS "Using OpenCV 2.x ...")
include( $ENV{OpenCV2_DIR}/OpenCVConfig.cmake )
endif() # (USE_CV3)

include_directories( ${OpenCV_INCLUDE_DIRS} )


include_directories( ${CMAKE_SOURCE_DIR}/5point)
#add_executable(example ${CMAKE_SOURCE_DIR}/5point/main.cpp)
#target_link_libraries(example fivePoint  ${OpenCV_LIBS})

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/pybind11)
add_subdirectory(${CMAKE_SOURCE_DIR}/python)