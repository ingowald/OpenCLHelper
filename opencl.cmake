
SET(INTEL_OPENCL_DIR /opt/intel/intel-opencl-1.2-6.3.0.1904/opencl-1.2-sdk-6.3.0.1904)
SET(OCL_EXECUTABLE ${INTEL_OPENCL_DIR}/bin/ioc64)
INCLUDE_DIRECTORIES(${INTEL_OPENCL_DIR}/include)

SET(OPENCL_INCLUDE_DIRS "")
MACRO (INCLUDE_DIRECTORIES_OPENCL)
  SET(OPENCL_INCLUDE_DIRS "${OPENCL_INCLUDE_DIRS} -I${ARGN}")
ENDMACRO ()

SET(OPENCL_LIBRARIES /opt/intel/opencl/libOpenCL.so)

SET(OPENCL_ASM_FILES "")

MACRO (COMPILE_OPENCL)
  FOREACH(src ${ARGN})
    GET_FILENAME_COMPONENT(fname ${src} NAME_WE)
    GET_FILENAME_COMPONENT(abs_path ${CMAKE_CURRENT_SOURCE_DIR}/${src} PATH)
    GET_FILENAME_COMPONENT(rel_path ${src} PATH)

    ADD_CUSTOM_COMMAND(
      OUTPUT ${CMAKE_BINARY_DIR}/${path}$/${fname}.s
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${path}$
      COMMAND ${OCL_EXECUTABLE}
      ${OPENCL_INCLUDE_DIRS}
      -input=${abs_path}/${fname}.cl
      -asm=${CMAKE_BINARY_DIR}/${rel_path}/${fname}.s
      DEPENDS ${input} ${deps}
      COMMENT "OpenCL-compiling ${CMAKE_BINARY_DIR}/${path}$/${fname}.s"
      )
    SET(OPENCL_ASM_FILES ${OPENCL_ASM_FILES} ${CMAKE_BINARY_DIR}/${path}$/${fname}.s)

    ADD_CUSTOM_TARGET(opencl_asm_files ALL DEPENDS ${OPENCL_ASM_FILES})
  ENDFOREACH()
ENDMACRO()

