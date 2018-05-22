message( STATUS "    In classes cv, link libs are: ${LINK_LIBS}" )


set(GET_RT_ALGO ${CMAKE_CURRENT_SOURCE_DIR}/getRTAlgo)
include_directories(${GET_RT_ALGO})

file(GLOB getRTAlgo_FILE ${GET_RT_ALGO}/getRTAlgo.cpp)

set(LIBNAME RelativePose5Point)
add_library(${LIBNAME} MODULE classes_cv.cpp ${getRTAlgo_FILE} ${FIVE_POINT_FILES})
target_link_libraries(${LIBNAME} PRIVATE pybind11::module ${OpenCV_LIBS} )
message( STATUS "    include path: ${OpenCV_INCLUDE_DIRS}" )


if( MSVC )
set_target_properties(${LIBNAME} PROPERTIES
    SUFFIX ".pyd"
)
else()
set_target_properties(${LIBNAME} PROPERTIES
    SUFFIX ".so"
)
endif()