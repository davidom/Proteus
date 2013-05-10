#ifndef __UG_LIB_H__

#define __UG_LIB_H__

#define UG_FIO_STANDARD_FILE_MODE 0
#define UG_FIO_TMP_FILE_MODE 1

#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>

#if defined (_WIN32)

typedef __int32 LONG_int;
typedef __int64 LONG_LONG_int;
typedef struct _stat ug_stat_struct;

#define UG_EXE_EXT ".exe"
#define UG_MAX_CHAR_STRING_LENGTH 512
#define UG_MODE_IRWXU _S_IWRITE;
#define UG_PATH_SEP "\\"
#define UG_PATH_DIR_SEP ";"
#define UG_PATH_DRIVE_SEP ":"
#define UG_PATH_EXT_SEP "."
#define UG_SHELL_COMMAND_CAT "TYPE"
#define UG_SHELL_COMMAND_CP "COPY"
#define UG_SHELL_COMMAND_BG " "
#define UG_SHELL_COMMAND_LS "DIR"
#undef UG_SHELL_COMMAND_BZIP 
#undef UG_SHELL_COMMAND_BUNZIP 
#undef UG_SHELL_COMMAND_BZIP_EXT 
#undef UG_SHELL_COMMAND_COMPRESS 
#undef UG_SHELL_COMMAND_UNCOMPRESS 
#undef UG_SHELL_COMMAND_COMPRESS_EXT 
#undef UG_SHELL_COMMAND_GZIP 
#undef UG_SHELL_COMMAND_GUNZIP 
#undef UG_SHELL_COMMAND_GZIP_EXT 

#include <process.h>
#include <direct.h>
#include <io.h> 

#else

typedef long int LONG_int;
typedef long long int LONG_LONG_int;
typedef struct stat ug_stat_struct;

#undef UG_EXE_EXT
#define UG_MAX_CHAR_STRING_LENGTH 1024
#define UG_MODE_IRWXU S_IRWXU;
#define UG_PATH_SEP "/"
#define UG_PATH_DIR_SEP ":"
#undef UG_PATH_DRIVE_SEP
#define UG_PATH_EXT_SEP "."
#define UG_SHELL_COMMAND_CAT "cat"
#define UG_SHELL_COMMAND_CP "cp"
#define UG_SHELL_COMMAND_BG "&"
#define UG_SHELL_COMMAND_LS "ls -ls"
#define UG_SHELL_COMMAND_BZIP "bzip2 -f"
#define UG_SHELL_COMMAND_BUNZIP "bzip2 -d -f"
#define UG_SHELL_COMMAND_BZIP_EXT ".bz2"
#undef UG_SHELL_COMMAND_COMPRESS
#define UG_SHELL_COMMAND_UNCOMPRESS "gzip -d -f"
#define UG_SHELL_COMMAND_COMPRESS_EXT ".Z"
#define UG_SHELL_COMMAND_GZIP "gzip -f"
#define UG_SHELL_COMMAND_GUNZIP "gzip -d -f"
#define UG_SHELL_COMMAND_GZIP_EXT ".gz"

#include <libgen.h>
#include <unistd.h>
#include <sys/utsname.h>

#endif

#include "ug_macro.h"
#include "ug_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "UG_Param_Struct.h"
#include "ug_backup_file.h"
#include "ug_case_name.h"
#include "ug_check_file.h"
#include "ug_check_param.h"
#include "ug_compress_file.h"
#include "ug_copy_double.h"
#include "ug_copy_file.h"
#include "ug_copy_int.h"
#include "ug_cpu_message.h"
#include "ug_date_time.h"
#include "ug_error_message.h"
#include "ug_find_file_in_path_list.h"
#include "ug_fio.h"
#include "ug_fio_byte_order.h"
#include "ug_free_param.h"
#include "ug_get_arch_info.h"
#include "ug_get_arg_char_param.h"
#include "ug_get_arg_double_param.h"
#include "ug_get_arg_double_param_vector.h"
#include "ug_get_arg_int_param.h"
#include "ug_get_arg_int_param_vector.h"
#include "ug_get_char_param.h"
#include "ug_get_double_array.h"
#include "ug_get_double_data.h"
#include "ug_get_double_param.h"
#include "ug_get_double_param_vector.h"
#include "ug_get_int_array.h"
#include "ug_get_int_data.h"
#include "ug_get_int_param.h"
#include "ug_get_int_param_vector.h"
#include "ug_get_items_from_list.h"
#include "ug_get_param_from_arg_or_file.h"
#include "ug_get_param_from_value.h"
#include "ug_get_param_last_label_index.h"
#include "ug_get_param_name_from_alt_names.h"
#include "ug_get_param_name_from_string.h"
#include "ug_get_read_char_param.h"
#include "ug_get_read_double_param.h"
#include "ug_get_read_double_param_vector.h"
#include "ug_get_read_int_param.h"
#include "ug_get_read_int_param_vector.h"
#include "ug_iaib.h"
#include "ug_ivivm.h"
#include "ug_kword_error_message.h"
#include "ug_lib_error_message.h"
#include "ug_lim_double.h"
#include "ug_lim_int.h"
#include "ug_malloc.h"
#include "ug_malloc_param.h"
#include "ug_max_double.h"
#include "ug_max_int.h"
#include "ug_message.h"
#include "ug_min_double.h"
#include "ug_min_int.h"
#include "ug_mp.h"
#include "ug_random_list.h"
#include "ug_read_param.h"
#include "ug_realloc_param.h"
#include "ug_renumber_int_values.h"
#include "ug_rerun.h"
#include "ug_round_off_error.h"
#include "ug_save_double_array.h"
#include "ug_save_double_data.h"
#include "ug_save_int_array.h"
#include "ug_save_int_data.h"
#include "ug_save_param_file.h"
#include "ug_set_char_param.h"
#include "ug_set_double.h"
#include "ug_set_double_param.h"
#include "ug_set_double_param_vector.h"
#include "ug_set_int.h"
#include "ug_set_int_param.h"
#include "ug_set_int_param_vector.h"
#include "ug_set_param_def.h"
#include "ug_set_params_from_arg_or_file.h"
#include "ug_sort_double_list.h"
#include "ug_sort_int_list.h"
#include "ug_system.h"
#include "ug_version.h"
#include "ug_version_info.h"
#include "ug_write_param_file.h"
#include "ug_write_param_info.h"

#ifdef __cplusplus
}
#endif

#endif
