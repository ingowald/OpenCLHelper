PROJECT(clHelper-Tools)
cmake_minimum_required(VERSION 2.6)

SET(clHelper_DIR ${CMAKE_CURRENT_SOURCE_DIR}/clHelper)

ADD_SUBDIRECTORY(${clHelper_DIR})


ADD_SUBDIRECTORY(examples)

#COMPILE_OPENCL(hello.cl)
#ADD_EXECUTABLE(helperTest
#  helperTest.cpp
#  ${EMBEDDED_OPENCL_KERNELS}
#  )
#TARGET_LINK_LIBRARIES(helperTest
#  clHelperLib
#  )





