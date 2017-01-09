PROJECT(luk-xmr-miner)
cmake_minimum_required(VERSION 2.6)

include(opencl.cmake)

COMPILE_OPENCL(hello.cl)

ADD_EXECUTABLE(hello
  hello.c
  )

TARGET_LINK_LIBRARIES(hello
  ${OPENCL_LIBRARIES}
  )