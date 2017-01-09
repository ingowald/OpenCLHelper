

ADD_LIBRARY(clHelperLib
  embeddedProgram.cpp
  )
TARGET_LINK_LIBRARIES(clHelperLib
  ${OPENCL_LIBRARIES}
  dl
  )
