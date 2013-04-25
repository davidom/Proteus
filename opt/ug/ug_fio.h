
INT_ ug_close_data
 (char[]);

INT_ ug_close_dataf
 (char[]);

INT_ ug_fclose
 (FILE *);

void ug_file_status_monitor
 (char[]);

INT_ ug_file_status_monitor_flag
 (void);

FILE * ug_fopen
 (char[],
  char[]);

void ug_free_tmp_files
 (void);

INT_ ug_fread
 (void *,
  size_t,
  INT_,
  FILE *);

INT_ ug_fwrite
 (void *,
  size_t,
  INT_,
  FILE *);

INT_ ug_get_file_stream_mode_flag
 (FILE *);

INT_ ug_get_file_name_tmp_file_index
 (char[]);

void ug_get_tmp_file_name
 (char[],
  char[]);

INT_ ug_get_file_stream_tmp_file_index
 (FILE *);

INT_ ug_rewind
 (FILE *);

INT_ ug_set_tmp_file_dir
 (char[]);

FILE * ug_tmpfile
 ( );
