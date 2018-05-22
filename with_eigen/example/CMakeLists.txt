project(fivePointTest)

cmake_minimum_required(VERSION 2.8)


# GLOBAL setting
set (NECESSARY_LIBS_DIR C:/tools/cmake_install_libs/pose_estimation)

# five point
set (fivePointLibDIR ${NECESSARY_LIBS_DIR}/fivePointEigen)
include (${fivePointLibDIR}/lib/cmake/fivePointEigen/fivePointEigenConfig.cmake)

message(STATUS "fivePointLibDIR:${fivePointLibDIR}")

include_directories( ${fivePointLibDIR}/include)

add_executable(example ../5point_Eigen/main.cpp)
target_link_libraries(example fivePointEigen::fivePointEigen) 