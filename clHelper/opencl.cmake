
SET(INTEL_OPENCL_DIR /opt/intel/intel-opencl-1.2-6.3.0.1904/opencl-1.2-sdk-6.3.0.1904)
SET(OCL_EXECUTABLE ioc64)
#SET(OCL_EXECUTABLE ${INTEL_OPENCL_DIR}/bin/ioc64)
INCLUDE_DIRECTORIES(${INTEL_OPENCL_DIR}/include)

SET(OPENCL_INCLUDE_DIRS "")
MACRO (OPENCL_INCLUDE_DIRECTORIES)
  SET(OPENCL_INCLUDE_DIRS "${OPENCL_INCLUDE_DIRS} -I${ARGN}")
ENDMACRO ()
SET(OPENCL_DEFINITIONS "")
MACRO (OPENCL_ADD_DEFINITION)
  SET(OPENCL_DEFINITIONS "${OPENCL_DEFINITIONS} ${ARGN}")
ENDMACRO ()

SET(C_PREPROCESSOR cpp)

SET(OPENCL_LIBRARIES /opt/intel/opencl/libOpenCL.so)

SET(OPENCL_ASM_FILES "")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")

MACRO (COMPILE_OPENCL)
  SET(OPENCL_ASM_FILES "")
  SET(OPENCL_DEP_FILES "")
  SET(EMBEDDED_OPENCL_KERNELS "")

  FOREACH(src ${ARGN})
    GET_FILENAME_COMPONENT(fname ${src} NAME_WE)
    GET_FILENAME_COMPONENT(abs_path ${CMAKE_CURRENT_SOURCE_DIR}/${src} PATH)
    GET_FILENAME_COMPONENT(rel_path ${src} PATH)

    # =======================================================
    # load dependencies from .dep file, IF this exists
    # =======================================================
    SET(dep_file ${CMAKE_BINARY_DIR}/.expanded_opencl/${fname}.dep)
    message("dep_file ${dep_file}")
    SET(deps "")
    IF (EXISTS ${dep_file})
      FILE(READ ${dep_file} contents)
      message("initial contents ${contents}")
      STRING(REPLACE " " ";"     contents "${contents}")
      STRING(REPLACE "\\" ""     contents "${contents}")
      STRING(REPLACE "\n" ";"    contents "${contents}")
      message("contents after replaces ${contents}")
      # remove first element - this is the outfile file name
      LIST(REMOVE_AT contents 0)
      message("contents after removing first element ${contents}")
      FOREACH(dep ${contents})
	message("dep ${dep}")
        IF (EXISTS ${dep})
          SET(deps ${deps} ${dep})
        ENDIF (EXISTS ${dep})
      ENDFOREACH(dep ${contents})
    ENDIF ()
    message("all deps ${deps}")
    # command to generate a 'dep' file
    GET_FILENAME_COMPONENT(dep_file_path ${dep_file} PATH)
    SET(input_file ${abs_path}/${fname}.cl)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${dep_file}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${dep_file_path}
      COMMAND ${C_PREPROCESSOR}
      -MM
      ${OPENCL_INCLUDE_DIRS}
      ${OPENCL_DEFINITIONS}
      ${input_file}
      -o ${dep_file}
      DEPENDS ${input_file} ${deps}
      COMMENT "Generated dependencies for ${src} -> ${dep_file}"
      )

    # command to generate a #include-expanded '.cl' file
    SET(preprocessed_file ${CMAKE_BINARY_DIR}/.expanded_opencl/${path}/${fname}.cl)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${preprocessed_file}
      COMMAND ${C_PREPROCESSOR}
#      -P
      ${OPENCL_INCLUDE_DIRS}
      ${OPENCL_DEFINITIONS}
      ${input_file}
      -o ${preprocessed_file}
      DEPENDS ${input_file} ${deps} ${dep_file}
      COMMENT "Run pre-processor on ${src} -> ${preprocessed_file}"
      )

    # command to generate a cmdline-compiled '.s' file
    SET(asm_file ${CMAKE_BINARY_DIR}/.expanded_opencl/${path}/${fname}.s)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${asm_file}
      COMMAND ${OCL_EXECUTABLE}
      -cmd=build
      -input=${preprocessed_file}
      -asm=${asm_file}
      DEPENDS ${preprocessed_file} ${deps}
      COMMENT "OpenCL-compiling ${preprocessed_file} -> ${asm_file}"
      COMMENT "(this is done as a sanity check to make sure opencl understands this code)"
      )

    # command to generate 'embedded' c file that contains the preprocessed kernel as a string
    SET(embedded_file ${CMAKE_BINARY_DIR}/.expanded_opencl/${path}/${fname}.c)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${embedded_file}
      COMMAND xxd 
      -i .expanded_opencl/${path}/${fname}.cl
      ${embedded_file}
      DEPENDS ${preprocessed_file} ${asm_file} ${deps}
      COMMENT "embedded opencl code from ${src} -> ${embedded_file}"
      )
    SET(OPENCL_ASM_FILES ${OPENCL_ASM_FILES} ${asm_file})
    SET(OPENCL_DEP_FILES ${OPENCL_DEP_FILES} ${dep_file})
    SET(EMBEDDED_OPENCL_KERNELS ${EMBEDDED_OPENCL_KERNELS} ${embedded_file})

    ADD_CUSTOM_TARGET(opencl_asm_files ALL DEPENDS ${OPENCL_ASM_FILES})
    ADD_CUSTOM_TARGET(opencl_dep_files ALL DEPENDS ${OPENCL_DEP_FILES})
  ENDFOREACH()
ENDMACRO()

