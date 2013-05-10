project(ug C)

set(UG_HDRS
UG_LIB.h
UG_Param_Struct.h
ug_backup_file.h
ug_case_name.h
ug_check_file.h
ug_check_param.h
ug_compress_file.h
ug_copy_double.h
ug_copy_file.h
ug_copy_int.h
ug_cpu_message.h
ug_date_time.h
ug_error_message.h
ug_find_file_in_path_list.h
ug_fio.h
ug_fio_byte_order.h
ug_free_param.h
ug_get_arch_info.h
ug_get_arg_char_param.h
ug_get_arg_double_param.h
ug_get_arg_double_param_vector.h
ug_get_arg_int_param.h
ug_get_arg_int_param_vector.h
ug_get_char_param.h
ug_get_double_array.h
ug_get_double_data.h
ug_get_double_param.h
ug_get_double_param_vector.h
ug_get_int_array.h
ug_get_int_data.h
ug_get_int_param.h
ug_get_int_param_vector.h
ug_get_items_from_list.h
ug_get_param_from_arg_or_file.h
ug_get_param_from_value.h
ug_get_param_last_label_index.h
ug_get_param_name_from_alt_names.h
ug_get_param_name_from_string.h
ug_get_read_char_param.h
ug_get_read_double_param.h
ug_get_read_double_param_vector.h
ug_get_read_int_param.h
ug_get_read_int_param_vector.h
ug_iaib.h
ug_initialize_param_realloc_char.h
ug_initialize_param_realloc_double.h
ug_initialize_param_realloc_int.h
ug_ivivm.h
ug_kword_error_message.h
ug_lib_error_message.h
ug_lim_double.h
ug_lim_int.h
ug_macro.h
ug_malloc.h
ug_malloc_param.h
ug_max_double.h
ug_max_int.h
ug_message.h
ug_min_double.h
ug_min_int.h
ug_mp.h
ug_random_list.h
ug_read_param.h
ug_realloc_param.h
ug_renumber_int_values.h
ug_rerun.h
ug_round_off_error.h
ug_save_double_array.h
ug_save_double_data.h
ug_save_int_array.h
ug_save_int_data.h
ug_save_param_file.h
ug_set_char_param.h
ug_set_double.h
ug_set_double_param.h
ug_set_double_param_vector.h
ug_set_int.h
ug_set_int_param.h
ug_set_int_param_vector.h
ug_set_param_def.h
ug_set_params_from_arg_or_file.h
ug_sort_double_list.h
ug_sort_int_list.h
ug_system.h
ug_typedef.h
ug_version.h
ug_version_info.h
ug_write_param_file.h
ug_write_param_info.h
)

set(UG_SRCS
ug_backup_file.c
ug_case_name.c
ug_check_file.c
ug_check_param.c
ug_compress_file.c
ug_copy_file.c
ug_cpu_message.c
ug_date_time.c
ug_error_message.c
ug_find_file_in_path_list.c
ug_fio.c
ug_fio_byte_order.c
ug_free_param.c
ug_get_arch_info.c
ug_get_arg_char_param.c
ug_get_arg_double_param.c
ug_get_arg_double_param_vector.c
ug_get_arg_int_param.c
ug_get_arg_int_param_vector.c
ug_get_char_param.c
ug_get_double_param.c
ug_get_double_param_vector.c
ug_get_int_param.c
ug_get_int_param_vector.c
ug_get_items_from_list.c
ug_get_param_from_arg_or_file.c
ug_get_param_from_value.c
ug_get_param_last_label_index.c
ug_get_param_name_from_alt_names.c
ug_get_param_name_from_string.c
ug_get_read_char_param.c
ug_get_read_double_param.c
ug_get_read_double_param_vector.c
ug_get_read_int_param.c
ug_get_read_int_param_vector.c
ug_kword_error_message.c
ug_lib_error_message.c
ug_malloc.c
ug_malloc_param.c
ug_message.c
ug_mp.c
ug_read_param.c
ug_realloc_param.c
ug_rerun.c
ug_round_off_error.c
ug_save_param_file.c
ug_set_char_param.c
ug_set_double_param.c
ug_set_double_param_vector.c
ug_set_int_param.c
ug_set_int_param_vector.c
ug_set_param_def.c
ug_set_params_from_arg_or_file.c
ug_system.c
ug_version.c
ug_version_info.c
ug_write_param_file.c
ug_write_param_info.c
)

set(UG_SRCS_2
ug_copy_double.c
ug_copy_int.c
ug_get_double_array.c
ug_get_double_data.c
ug_get_int_array.c
ug_get_int_data.c
ug_iaib.c
ug_ivivm.c
ug_lim_double.c
ug_lim_int.c
ug_max_double.c
ug_max_int.c
ug_min_double.c
ug_min_int.c
ug_random_list.c
ug_renumber_int_values.c
ug_save_double_array.c
ug_save_double_data.c
ug_save_int_array.c
ug_save_int_data.c
ug_set_double.c
ug_set_int.c
ug_sort_double_list.c
ug_sort_int_list.c
)

add_library(ug STATIC ${UG_SRCS} ${UG_HDRS} ${UG_SRCS_2})

if(CMAKE_COMPILER_IS_GNUCC)
  set_target_properties(ug PROPERTIES COMPILE_FLAGS -fPIC)
endif(CMAKE_COMPILER_IS_GNUCC)

set_source_files_properties(${UG_SRCS} PROPERTIES COMPILE_FLAGS -O0)


#------------------------------------
set(UG_EXE_SRCS
    ug.c)

if(AFLR_BUILD_TESTS)
  add_executable(ug_exe ${UG_EXE_SRCS})

  set_target_properties(ug_exe PROPERTIES OUTPUT_NAME ug)

  target_link_libraries(ug_exe
      ug)

endif(AFLR_BUILD_TESTS)


#------------------------------------
if(AFLR_ADD_INSTALL)
  if(AFLR_BUILD_TESTS)
    install(TARGETS ug_exe
        RUNTIME DESTINATION ${AFLR_TARGET_BASE}bin
        COMPONENT Runtime)
  endif(AFLR_BUILD_TESTS)

  install(TARGETS ug EXPORT AFLR
      ARCHIVE DESTINATION ${AFLR_TARGET_BASE}lib
      COMPONENT Development)

  install(FILES
      ${UG_SRCS}
      ${UG_SRCS_2}
      ${UG_EXE_SRCS}
CMakeLists.txt
ug.info
UG_LIB.txt
ug_backup_file.txt
ug_case_name.txt
ug_check_file.txt
ug_check_param.txt
ug_compress_file.txt
ug_copy_double.txt
ug_copy_file.txt
ug_copy_int.txt
ug_cpu_message.txt
ug_date_time.txt
ug_error_message.txt
ug_fio.txt
ug_fio_byte_order.txt
ug_free_param.txt
ug_get_char_param.txt
ug_get_double_array.txt
ug_get_double_data.txt
ug_get_double_param.txt
ug_get_double_param_vector.txt
ug_get_int_array.txt
ug_get_int_data.txt
ug_get_int_param.txt
ug_get_int_param_vector.txt
ug_get_read_char_param.txt
ug_get_read_double_param.txt
ug_get_read_double_param_vector.txt
ug_get_read_int_param.txt
ug_get_read_int_param_vector.txt
ug_iaib.txt
ug_kword_error_message.txt
ug_lib_error_message.txt
ug_macro.txt
ug_malloc.txt
ug_malloc_param.txt
ug_max_double.txt
ug_max_int.txt
ug_message.txt
ug_min_double.txt
ug_min_int.txt
ug_mp.txt
ug_random_list.txt
ug_read_param.txt
ug_realloc_param.txt
ug_renumber_int_values.txt
ug_round_off_error.txt
ug_save_double_array.txt
ug_save_double_data.txt
ug_save_int_array.txt
ug_save_int_data.txt
ug_save_param_file.txt
ug_set_char_param.txt
ug_set_double.txt
ug_set_double_param.txt
ug_set_double_param_vector.txt
ug_set_int.txt
ug_set_int_param.txt
ug_set_int_param_vector.txt
ug_set_param_def.txt
ug_sort_double_list.txt
ug_sort_int_list.txt
ug_system.txt
ug_typedef.txt
ug_version.txt
ug_version_info.txt
ug_write_param_file.txt
ug_write_param_info.txt
UNRESTRICTED_LICENSE.pdf
      DESTINATION src/ug
      COMPONENT Development)
endif(AFLR_ADD_INSTALL)

