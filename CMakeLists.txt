PROJECT(luk-xmr-miner)
cmake_minimum_required(VERSION 2.6)

INCLUDE(clHelperLib/opencl.cmake)
ADD_SUBDIRECTORY(clHelperLib)

COMPILE_OPENCL(hello.cl)

ADD_EXECUTABLE(hello
  hello.c
  ${EMBEDDED_OPENCL_KERNELS}
  )

TARGET_LINK_LIBRARIES(hello
  clHelperLib
  )



