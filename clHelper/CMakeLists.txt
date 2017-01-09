

ADD_LIBRARY(clHelperLib
  embeddedProgram.cpp
  device.cpp
  platform.cpp
  )
TARGET_LINK_LIBRARIES(clHelperLib
  ${OPENCL_LIBRARIES}
  dl
  )
