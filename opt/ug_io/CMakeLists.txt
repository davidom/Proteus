project(ug_io C)

set(UG_IO_HDRS
UG_IO_File_Def.h
UG_IO_LIB.h
UG_IO_LIB_INC.h
UG_IO_Param_Struct.h
cgns_ug_io_read_grid.h
cgns_ug_io_write_grid.h
fv_reader_tags.h
mesh_ug_io_read_2d_grid.h
mesh_ug_io_read_grid.h
mesh_ug_io_write_2d_grid.h
mesh_ug_io_write_grid.h
ug_io_case_name.h
ug_io_cgns.h
ug_io_check_file_name.h
ug_io_convert.h
ug_io_convert_2d.h
ug_io_data.h
ug_io_error_message.h
ug_io_file_format.h
ug_io_file_info.h
ug_io_file_mode.h
ug_io_file_suffix.h
ug_io_file_type.h
ug_io_file_type_suffix.h
ug_io_find_file.h
ug_io_free_2d_grid.h
ug_io_free_2d_node.h
ug_io_free_func.h
ug_io_free_func_flag.h
ug_io_free_grid.h
ug_io_free_node.h
ug_io_free_param.h
ug_io_initialize_param.h
ug_io_malloc_2d_grid.h
ug_io_malloc_2d_node.h
ug_io_malloc_func.h
ug_io_malloc_func_flag.h
ug_io_malloc_grid.h
ug_io_malloc_node.h
ug_io_malloc_param.h
ug_io_mesh.h
ug_io_read_2d_bedge.h
ug_io_read_2d_fgrid.h
ug_io_read_2d_gdata.h
ug_io_read_2d_grid_file.h
ug_io_read_2d_node.h
ug_io_read_2d_node_file.h
ug_io_read_2d_ugrid.h
ug_io_read_cart3d.h
ug_io_read_fgrid.h
ug_io_read_func_file.h
ug_io_read_gdata.h
ug_io_read_grid_file.h
ug_io_read_mesh.h
ug_io_read_nas.h
ug_io_read_node.h
ug_io_read_node_file.h
ug_io_read_nsu3d.h
ug_io_read_psdata_file.h
ug_io_read_snsdata_file.h
ug_io_read_stl.h
ug_io_read_surf.h
ug_io_read_tags.h
ug_io_read_tags_file.h
ug_io_read_tetgen.h
ug_io_read_ufunc.h
ug_io_read_ugrid.h
ug_io_read_vfunc.h
ug_io_read_vgrid.h
ug_io_version.h
ug_io_write_2d_gdata.h
ug_io_write_2d_grid_file.h
ug_io_write_2d_node.h
ug_io_write_2d_node_file.h
ug_io_write_2d_ugrid.h
ug_io_write_cart3d.h
ug_io_write_cfdpp.h
ug_io_write_cobalt.h
ug_io_write_efunc.h
ug_io_write_egrid.h
ug_io_write_fgrid.h
ug_io_write_fluent.h
ug_io_write_func_file.h
ug_io_write_fvuns.h
ug_io_write_gambit.h
ug_io_write_gdata.h
ug_io_write_grid_file.h
ug_io_write_mesh.h
ug_io_write_nas.h
ug_io_write_node.h
ug_io_write_node_file.h
ug_io_write_nsu3d.h
ug_io_write_param_info.h
ug_io_write_poly.h
ug_io_write_stl.h
ug_io_write_surf.h
ug_io_write_ufunc.h
ug_io_write_ugrid.h
ug_io_write_vgrid.h
)

set(UG_IO_SRCS
ug_io_case_name.c
ug_io_cgns.c
ug_io_check_file_name.c
ug_io_convert.c
ug_io_convert_2d.c
ug_io_error_message.c
ug_io_file_format.c
ug_io_file_info.c
ug_io_file_mode.c
ug_io_file_suffix.c
ug_io_file_type.c
ug_io_file_type_suffix.c
ug_io_find_file.c
ug_io_free_2d_grid.c
ug_io_free_2d_node.c
ug_io_free_func.c
ug_io_free_func_flag.c
ug_io_free_grid.c
ug_io_free_node.c
ug_io_free_param.c
ug_io_initialize_param.c
ug_io_malloc_2d_grid.c
ug_io_malloc_2d_node.c
ug_io_malloc_func.c
ug_io_malloc_func_flag.c
ug_io_malloc_grid.c
ug_io_malloc_node.c
ug_io_malloc_param.c
ug_io_mesh.c
ug_io_read_2d_grid_file.c
ug_io_read_2d_node_file.c
ug_io_read_func_file.c
ug_io_read_grid_file.c
ug_io_read_node_file.c
ug_io_read_psdata_file.c
ug_io_read_snsdata_file.c
ug_io_read_tags.c
ug_io_read_tags_file.c
ug_io_version.c
ug_io_write_2d_grid_file.c
ug_io_write_2d_node_file.c
ug_io_write_func_file.c
ug_io_write_grid_file.c
ug_io_write_node_file.c
ug_io_write_param_info.c
)

set(UG_IO_SRCS_2
ug_io_data.c
ug_io_read_2d_bedge.c
ug_io_read_2d_fgrid.c
ug_io_read_2d_gdata.c
ug_io_read_2d_node.c
ug_io_read_2d_ugrid.c
ug_io_read_cart3d.c
ug_io_read_fgrid.c
ug_io_read_gdata.c
ug_io_read_mesh.c
ug_io_read_nas.c
ug_io_read_node.c
ug_io_read_nsu3d.c
ug_io_read_stl.c
ug_io_read_surf.c
ug_io_read_tetgen.c
ug_io_read_ufunc.c
ug_io_read_ugrid.c
ug_io_read_vgrid.c
ug_io_write_2d_gdata.c
ug_io_write_2d_node.c
ug_io_write_2d_ugrid.c
ug_io_write_cart3d.c
ug_io_write_cfdpp.c
ug_io_write_cobalt.c
ug_io_write_efunc.c
ug_io_write_egrid.c
ug_io_write_fgrid.c
ug_io_write_fluent.c
ug_io_write_fvuns.c
ug_io_write_gambit.c
ug_io_write_gdata.c
ug_io_write_mesh.c
ug_io_write_nas.c
ug_io_write_node.c
ug_io_write_nsu3d.c
ug_io_write_poly.c
ug_io_write_stl.c
ug_io_write_surf.c
ug_io_write_ufunc.c
ug_io_write_ugrid.c
ug_io_write_vgrid.c
)

add_library(ug_io STATIC ${UG_IO_SRCS} ${UG_IO_HDRS} ${UG_IO_SRCS_2})

if(CMAKE_COMPILER_IS_GNUCC)
  set_target_properties(ug_io PROPERTIES COMPILE_FLAGS -fPIC)
endif(CMAKE_COMPILER_IS_GNUCC)

set_source_files_properties(${UG_IO_SRCS} PROPERTIES COMPILE_FLAGS -O0)


#-----------------------------------------------------
set(UGC_EXE_SRCS
ugc.c
)

if(AFLR_BUILD_TESTS)
  add_executable(ugc ${UGC_EXE_SRCS})

  target_link_libraries(ugc
      ug_io
      ug3
      ug)

  if(AFLR_BUILD_CGNS)
    target_link_libraries(ugc cgns)
  endif(AFLR_BUILD_CGNS)

  if(AFLR_BUILD_LM5)
    target_link_libraries(ugc lm5)
  endif(AFLR_BUILD_LM5)

endif(AFLR_BUILD_TESTS)


#-----------------------------------------------------
if(AFLR_ADD_INSTALL)
  if(AFLR_BUILD_TESTS)
    install(TARGETS ugc
        RUNTIME DESTINATION ${AFLR_TARGET_BASE}bin
        COMPONENT Runtime)
  endif(AFLR_BUILD_TESTS)

  install(TARGETS ug_io EXPORT AFLR
      ARCHIVE DESTINATION ${AFLR_TARGET_BASE}lib
      COMPONENT Development)

  install(FILES
      ${UG_IO_SRCS}
      ${UG_IO_HDRS}
      ${UG_IO_SRCS_2}
      ${UGC_EXE_SRCS}
      3rd_PARTY_SOFTWARE_STATEMENT.pdf
      CMakeLists.txt
      ug_io.info
      UG_IO_LIB.txt
      Release_Notes_ug_io.txt
      ug_io_case_name.txt
      ug_io_check_file_name.txt
      ug_io_convert.txt
      ug_io_convert_2d.txt
      ug_io_error_message.txt
      ug_io_file_format.txt
      ug_io_file_info.txt
      ug_io_file_mode.txt
      ug_io_file_suffix.txt
      ug_io_file_type.txt
      ug_io_file_type_suffix.txt
      ug_io_find_file.txt
      ug_io_free_2d_grid.txt
      ug_io_free_2d_node.txt
      ug_io_free_func.txt
      ug_io_free_func_flag.txt
      ug_io_free_grid.txt
      ug_io_free_node.txt
      ug_io_free_param.txt
      ug_io_initialize_param.txt
      ug_io_malloc_2d_grid.txt
      ug_io_malloc_2d_node.txt
      ug_io_malloc_func.txt
      ug_io_malloc_func_flag.txt
      ug_io_malloc_grid.txt
      ug_io_malloc_node.txt
      ug_io_malloc_param.txt
      ug_io_read_2d_grid_file.txt
      ug_io_read_2d_node_file.txt
      ug_io_read_grid_file.txt
      ug_io_read_node_file.txt
      ug_io_read_tags_file.txt
      ug_io_version.txt
      ug_io_write_2d_grid_file.txt
      ug_io_write_2d_node_file.txt
      ug_io_write_OpenFOAM.txt
      ug_io_write_grid_file.txt
      ug_io_write_node_file.txt
      ug_io_write_param_info.txt
      UNRESTRICTED_LICENSE.pdf
      DESTINATION src/ug_io
      COMPONENT Development)
endif(AFLR_ADD_INSTALL)

