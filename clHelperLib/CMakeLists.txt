

ADD_LIBRARY(clHelperLib
  embeddedOpenCL.cpp
  )
TARGET_LINK_LIBRARIES(clHelperLib
  ${OPENCL_LIBRARIES}
  dl
  )
