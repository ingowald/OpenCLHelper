INCLUDE(clHelper.cmake)

ADD_LIBRARY(clHelperLib
  embeddedProgram.cpp
  device.cpp
  buffer.cpp
  program.cpp
  kernel.cpp
  platform.cpp
  context.cpp
  )
TARGET_LINK_LIBRARIES(clHelperLib
  ${OpenCL_LIBRARY}
  dl
  )
SET_PROPERTY(TARGET clHelperLib PROPERTY POSITION_INDEPENDENT_CODE ON)
