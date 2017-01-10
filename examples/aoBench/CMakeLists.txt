
INCLUDE(${clHelper_DIR}/clHelper.cmake)
COMPILE_OPENCL(aoBench.cl)
ADD_EXECUTABLE(aoBench
  aoBench.cpp
  ${EMBEDDED_OPENCL_KERNELS}
  )
TARGET_LINK_LIBRARIES(aoBench
  clHelperLib
  )

