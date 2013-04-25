project(ug_cpp CC)

set(UG_CPP_HDRS
UG_CPP_LIB.h
ug_cpp_extract_faces_from_volume_elements.h
ug_cpp_version.h
ug_io_write_OpenFOAM.h
)

set(UG_CPP_SRCS_2
ug_cpp_extract_faces_from_volume_elements.cpp
ug_cpp_version.cpp
ug_io_write_OpenFOAM.cpp
)

add_library(ug_cpp STATIC ${UG_CPP_SRCS_2} ${UG_CPP_HDRS})

if(CMAKE_COMPILER_IS_GNUCC)
  set_target_properties(ug_cpp PROPERTIES COMPILE_FLAGS -fPIC)
endif(CMAKE_COMPILER_IS_GNUCC)

#need to add C++ as compiler

#-------------------------------------
if(AFLR_ADD_INSTALL)
  install(TARGETS ug_cpp EXPORT AFLR
      ARCHIVE DESTINATION ${AFLR_TARGET_BASE}lib
      COMPONENT Development)

  install(FILES
      ${UG_CPP_HDRS}
      ${UG_CPP_SRCS_2}
      CMakeLists.txt
      ug_cpp.info
      Release_Notes_ug_cpp.txt
      UNRESTRICTED_LICENSE.pdf
      DESTINATION src/ug_cpp
      COMPONENT Development)
endif(AFLR_ADD_INSTALL)

