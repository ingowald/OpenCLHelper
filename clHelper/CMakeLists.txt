INCLUDE(clHelper.cmake)

ADD_LIBRARY(clHelperLib
  embeddedProgram.cpp
  device.cpp
  buffer.cpp
  platform.cpp
  context.cpp
  )
TARGET_LINK_LIBRARIES(clHelperLib
  ${OpenCL_LIBRARY}
  dl
  )
