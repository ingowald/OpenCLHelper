
INCLUDE(${clHelper_DIR}/clHelper.cmake)
COMPILE_OPENCL(hello.cl)
ADD_EXECUTABLE(example_hello
  hello.c
  ${EMBEDDED_OPENCL_KERNELS}
  )
TARGET_LINK_LIBRARIES(example_hello
  clHelperLib
  )

