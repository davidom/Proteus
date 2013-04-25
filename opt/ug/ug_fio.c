#include "UG_LIB.h"

/*
 * Local declarations.
 */

static CHAR_133 Text;

static INT_ File_Status_Monitor = 0;

static ug_stat_struct *stat_struct = NULL;

static FILE * ug_fopen_
 (char[],
  char[]);

/*
 * Local TMP file related declarations.
 */

typedef struct _UG_TMP_File_Struct UG_TMP_File_Struct;

struct _UG_TMP_File_Struct 
{
  INT_ Number_of_TMP_Files;
  CHAR_UG_MAX *TMP_File_Name;
  FILE **TMP_File_Stream;
  INT_1D *TMP_File_Status;
};

static FILE * ug_fopen_tmp_file
 (char[]);

static void ug_free_tmp_file_struct
 (void);

static INT_ ug_malloc_tmp_file_struct
 (UG_TMP_File_Struct **);

static INT_ ug_realloc_tmp_file_struct
 (UG_TMP_File_Struct *);

void ug_rewind_data
 (char[]);

/*
 * Local TMP file related variables.
 */

static UG_TMP_File_Struct *UG_TMP_File_Struct_Ptr = NULL;

static CHAR_UG_MAX UG_TMP_File_Dir_ = "";
static CHAR_UG_MAX UG_TMP_File_PID_Suffix_ = "";

static INT_ UG_TMP_File_Dir_Flag = 0;

/*
 * Basic binary file I/O routines.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_fio.c,v 1.52 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1999-2012, David L. Marcum
 */

INT_ ug_fclose
 (FILE * File_Stream)

{
  
/*
 * Close a binary TMP or regular file.
 * 
 */

  FILE **TMP_File_Stream;

  INT_1D *TMP_File_Status;

  INT_ Close_Flag = 1;
  INT_ Error_Flag, Index;

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_fclose                : closing file with file stream ID=%i", (int) ug_fileno(File_Stream));
    ug_message (Text);
  }

  Index = ug_get_file_stream_tmp_file_index (File_Stream);

  if (Index >= 0)
  {
    TMP_File_Stream = UG_TMP_File_Struct_Ptr->TMP_File_Stream;
    TMP_File_Status = UG_TMP_File_Struct_Ptr->TMP_File_Status;

    Close_Flag = (TMP_File_Status[Index] == 1) ? 1: 0;

    TMP_File_Stream[Index] = NULL;
    TMP_File_Status[Index] = -1;

    if (File_Status_Monitor)
    {
      sprintf (Text, "ug_fclose                : closed TMP file with Index=%i", (int) Index);
      ug_message (Text);
    }
  }

  if (Close_Flag)
  {
    Error_Flag = fclose (File_Stream);

    if (File_Status_Monitor)
      ug_message("ug_fclose                : closed file with fclose");
  }

  return (Error_Flag);

}

INT_ ug_close_data
 (char File_Name[])

{
  
/*
 * Close a binary TMP file by file name.
 */

  FILE **TMP_File_Stream;

  INT_ Index;

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_close_data            : closing TMP data file %s", File_Name);
    ug_message (Text);
  }

  Index = ug_get_file_name_tmp_file_index (File_Name);

  if (Index < 0)
  {
    if (File_Status_Monitor)
      ug_message ("ug_close_data            : no TMP file index found");

    return (-1);
  }

  TMP_File_Stream = UG_TMP_File_Struct_Ptr->TMP_File_Stream;

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_close_data            : closing TMP data file for Index=%i", (int) Index);
    ug_message (Text);
  }

  ug_fclose (TMP_File_Stream[Index]);

  if (File_Status_Monitor)
    ug_message ("ug_close_data            : file closed");

  return (0);

}

INT_ ug_close_dataf
 (char File_Name[])

{

/*
 * Close a binary TMP file by file name but don't delete the file or remove it
 * from the data structure. If the file is not a true file then it is rewound.
 */

  FILE **TMP_File_Stream;

  INT_ Index;

  Index = ug_get_file_name_tmp_file_index (File_Name);

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_close_dataf           : rewinding TMP data file %s with Index=%i", File_Name, (int) Index);
    ug_message (Text);
  }

  if (Index < 0)
    return (-1);

  TMP_File_Stream = UG_TMP_File_Struct_Ptr->TMP_File_Stream;

  ug_rewind (TMP_File_Stream[Index]);

  if (File_Status_Monitor)
    ug_message ("ug_close_dataf           : rewound TMP data file");

  return (0);

}

void ug_file_status_monitor
 (char *Flag)
 
{

/*
 * Turn file status monitor on or off.
 */

  INT_ ierr;

  if (File_Status_Monitor)
  {
    ug_free (stat_struct);

    stat_struct = NULL;
  }

  if (strcmp (Flag, "on") == 0)
  {
    ierr = 0;


    stat_struct = (ug_stat_struct *) ug_malloc (&ierr, sizeof (ug_stat_struct));

    File_Status_Monitor = (ierr == 0) ? 1: 0;
  }
  else
    File_Status_Monitor = 0;

  return;

}

INT_ ug_file_status_monitor_flag
 (void)
 
{

/*
 * Return file status monitor flag.
 */

  return (File_Status_Monitor);

}

FILE * ug_fopen
 (char File_Name[],
  char File_Mode[])

{
  
/*
 * Open a binary TMP or regular file.
 * 
 */

  FILE *File_Stream = NULL;

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_fopen                 : open file name %s with file mode %s", File_Name, File_Mode);
    ug_message (Text);
  }

  if (strcmp (File_Mode, "r_tmp") == 0)
    File_Stream = ug_fopen_tmp_file (File_Name);

  else if (strcmp (File_Mode, "tmp") == 0)
    File_Stream = ug_fopen_tmp_file (File_Name);

  else
    File_Stream = ug_fopen_ (File_Name, File_Mode);

  if (File_Status_Monitor && File_Stream)
  {
    sprintf (Text, "ug_fopen                 : opened file stream with ID=%i for file name %s with file mode %s", (int) ug_fileno(File_Stream), File_Name, File_Mode);
    ug_message (Text);
  }

  return (File_Stream);

}

FILE * ug_fopen_
 (char File_Name[],
  char File_Mode[])

{
  
/*
 * Open a regular file.
 * 
 */

  FILE *File_Stream = NULL;

  CHAR_UG_MAX Binary_File_Mode;

  INT_ ID, ierr;

  strcpy (Binary_File_Mode, ug_get_binary_file_mode (File_Mode));

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_fopen_                : open file name %s with binary file mode %s", File_Name, Binary_File_Mode);
    ug_message (Text);
  }

  File_Stream = fopen (File_Name, Binary_File_Mode);

  if (File_Status_Monitor && File_Stream)
  {
    ID = ug_fileno (File_Stream);

    ierr = ug_fstat (ID, stat_struct);

    if (ierr)
    {
      sprintf (Text, "ug_fopen_                : unable to get status for file %s", File_Name);
      ug_message (Text);
    }
    else
    {
      sprintf (Text, "ug_fopen_                : file name %s file mode %s", File_Name, File_Mode);
      ug_message (Text);
      sprintf (Text, "ug_fopen_                : file device inode=%i file inode=%i", (int) stat_struct->st_dev, (int) stat_struct->st_ino);
      ug_message (Text);
      sprintf (Text, "ug_fopen_                : file stream ID=%i current size %i bytes", (int) ID, (int) stat_struct->st_size);
      ug_message (Text);
    }
  }

  return (File_Stream);

}

FILE * ug_fopen_tmp_file
 (char File_Name[])

{
 
/*
 * Create or attach a named binary TMP file and open the file or return file
 * stream only if it exists.
 */

  FILE *File_Stream = NULL;

  FILE **TMP_File_Stream;

  CHAR_UG_MAX *TMP_File_Name;

  INT_1D *TMP_File_Status;

  INT_ Error_Flag = 0;
  INT_ Found_Flag, Index, Number_of_TMP_Files;

  if (UG_TMP_File_Struct_Ptr == NULL)
  {
    Error_Flag = ug_malloc_tmp_file_struct (&UG_TMP_File_Struct_Ptr);

    if (Error_Flag == 0)
    {
      Number_of_TMP_Files = UG_TMP_File_Struct_Ptr->Number_of_TMP_Files;

      TMP_File_Name = UG_TMP_File_Struct_Ptr->TMP_File_Name;
      TMP_File_Stream = UG_TMP_File_Struct_Ptr->TMP_File_Stream;
      TMP_File_Status = UG_TMP_File_Struct_Ptr->TMP_File_Status;

      Index = 0;
    }

    if (File_Status_Monitor)
      ug_message ("ug_fopen_tmp_file        : allocating TMP file structure");
  }

  else
  {
    Number_of_TMP_Files = UG_TMP_File_Struct_Ptr->Number_of_TMP_Files;

    TMP_File_Name = UG_TMP_File_Struct_Ptr->TMP_File_Name;
    TMP_File_Stream = UG_TMP_File_Struct_Ptr->TMP_File_Stream;
    TMP_File_Status = UG_TMP_File_Struct_Ptr->TMP_File_Status;

    Index = ug_get_file_name_tmp_file_index (File_Name);
 
    if (Index >= 0)
    {
      File_Stream = TMP_File_Stream[Index];

      if (File_Status_Monitor)
      {
        sprintf (Text, "ug_fopen_tmp_file        : found previously created TMP file named %s with Index=%i and Status=%i", File_Name, (int) Index, (int) TMP_File_Status[Index]);
        ug_message (Text);
      }

      if (TMP_File_Status[Index] == 1)
        return (File_Stream);
    }

    else
    {
      Found_Flag = 0;

      Index = 0;

      do
      {
        if (TMP_File_Status[Index] == -1)
          Found_Flag = 1;

        ++Index;
      }
      while (Index < Number_of_TMP_Files && Found_Flag == 0);

      --Index;

      if (Found_Flag == 0)
      {
        Error_Flag = ug_realloc_tmp_file_struct (UG_TMP_File_Struct_Ptr);

        Index = UG_TMP_File_Struct_Ptr->Number_of_TMP_Files-1;

        TMP_File_Name = UG_TMP_File_Struct_Ptr->TMP_File_Name;
        TMP_File_Stream = UG_TMP_File_Struct_Ptr->TMP_File_Stream;
        TMP_File_Status = UG_TMP_File_Struct_Ptr->TMP_File_Status;

        if (File_Status_Monitor)
        {
          sprintf (Text, "ug_fopen_tmp_file        : adding file to structure after reallocating TMP file structure with new Index=%i of %i", (int) Index, (int) UG_TMP_File_Struct_Ptr->Number_of_TMP_Files);
          ug_message (Text);
        }
      }
      else if (File_Status_Monitor)
      {
        sprintf (Text, "ug_fopen_tmp_file        : adding file to structure and reusing Index=%i of %i", (int) Index, (int) UG_TMP_File_Struct_Ptr->Number_of_TMP_Files);
        ug_message (Text);
      }
    }
  }

  if (Error_Flag > 0)
  {
    ug_free_tmp_file_struct ();

    if (File_Status_Monitor)
      ug_message ("ug_fopen_tmp_file         : freeing TMP file structure since file could not be added to structure");
    return (NULL);
  }

  File_Stream = ug_tmpfile ();

  strcpy (TMP_File_Name[Index], File_Name);

  TMP_File_Stream[Index] = File_Stream;
  TMP_File_Status[Index] = (File_Stream) ? 1: -1;

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_fopen_tmp_file        : creating TMP file associated with name %s with Index=%i of %i and with TMP_File_Status=%i", File_Name, (int) Index, (int) UG_TMP_File_Struct_Ptr->Number_of_TMP_Files, (int) TMP_File_Status[Index]);
    ug_message (Text);
  }

  return (File_Stream);

}

static void ug_free_tmp_file_struct
 (void)

{

/*
 * Free binary TMP file structure arrays.
 */

  ug_free (UG_TMP_File_Struct_Ptr->TMP_File_Name);
  ug_free (UG_TMP_File_Struct_Ptr->TMP_File_Stream);
  ug_free (UG_TMP_File_Struct_Ptr->TMP_File_Status);

  ug_free (UG_TMP_File_Struct_Ptr);

  UG_TMP_File_Struct_Ptr = NULL;

  return;

}

void ug_free_tmp_files
 (void)

{
  
/*
 * Close all binary TMP files and free TMP file arrays and remove TMP directory.
 *
 */

  CHAR_UG_MAX *TMP_File_Name;

  INT_1D *TMP_File_Status;

  INT_ Error_Flag, Index, Number_of_TMP_Files;

  if (UG_TMP_File_Struct_Ptr)
  {
    Number_of_TMP_Files = UG_TMP_File_Struct_Ptr->Number_of_TMP_Files;

    if (Number_of_TMP_Files > 0)
    {
      TMP_File_Name = UG_TMP_File_Struct_Ptr->TMP_File_Name;
      TMP_File_Status = UG_TMP_File_Struct_Ptr->TMP_File_Status;

      for (Index = 0; Index < Number_of_TMP_Files; ++Index)
      {
        if (TMP_File_Status[Index] >= 1)
          ug_close_data (TMP_File_Name[Index]);
      }
    }

    ug_free_tmp_file_struct ();
  }

  if (UG_TMP_File_Dir_Flag == 2)
  {
    UG_TMP_File_Dir_Flag = 0;

    ug_remove_dir (UG_TMP_File_Dir_);

    Error_Flag = ug_set_tmp_file_dir ("NULL");
  }

  return;

}

INT_ ug_fread
 (void * Data,
  size_t Size,
  INT_ Number_of_Items,
  FILE * File_Stream)

{
  
/*
 * Read from binary TMP or regular file.
 * 
 */

  INT_ Number_of_Items_Read = 0;

  Number_of_Items_Read = (INT_) fread (Data, Size,
                                       (size_t) Number_of_Items,
                                       File_Stream);

  ug_reverse_byte_order (Data, Size, Number_of_Items);

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_fread                 : read %i items of %i from file with file stream ID=%i", (int) Number_of_Items_Read, (int) Number_of_Items, (int) ug_fileno (File_Stream));
    ug_message (Text);
  }

  return (Number_of_Items_Read);

}

INT_ ug_fwrite
 (void * Data,
  size_t Size,
  INT_ Number_of_Items,
  FILE * File_Stream)

{
  
/*
 * Write to binary TMP or regular file.
 * 
 */

  static INT_ previous_size = -1;

  INT_ ID, ierr, size;
  INT_ Number_of_Items_Written = 0;

  ug_reverse_byte_order (Data, Size, Number_of_Items);

  Number_of_Items_Written = (INT_) fwrite (Data, Size,
                                           (size_t) Number_of_Items,
                                           File_Stream);

  ug_reverse_byte_order (Data, Size, Number_of_Items);

  if (File_Status_Monitor)
  {
    ID = ug_fileno(File_Stream);

    if (Number_of_Items > 1)
    {
      sprintf (Text, "ug_fwrite                : wrote %i items of %i from file with file stream ID=%i", (int) Number_of_Items_Written, (int) Number_of_Items, (int) ID);
      ug_message (Text);
    }

    if (stat_struct)
    {
      ierr = ug_fstat (ID, stat_struct);

      size = (INT_) stat_struct->st_size;

      if (ierr == 0 && previous_size != size)
      {
        sprintf (Text, "ug_fwrite                : file stream ID=%i current size=%i bytes", (int) ID, (int) size);
        ug_message (Text);

        previous_size = size;
      }
    }
  }

  return (Number_of_Items_Written);

}

INT_ ug_get_file_name_tmp_file_index
 (char File_Name[])

{
  
/*
 * Get index for a named binary TMP file that corresponds to a file name.
 */

  CHAR_UG_MAX *TMP_File_Name;

  INT_ Index, Number_of_TMP_Files;

  if (! UG_TMP_File_Struct_Ptr)
    return (-1);

  Number_of_TMP_Files = UG_TMP_File_Struct_Ptr->Number_of_TMP_Files;

  TMP_File_Name = UG_TMP_File_Struct_Ptr->TMP_File_Name;

  for (Index = 0; Index < Number_of_TMP_Files; ++Index)
  {
    if (strcmp (TMP_File_Name[Index], File_Name) == 0)
      return (Index);
  }

  return (-1);

}

INT_ ug_get_file_stream_mode_flag
 (FILE * File_Stream)

{
  
/*
 * Determine if a file stream is associated with a binary TMP file.
 * 
 */

  INT_ Return_Flag = 0;

  if (ug_get_file_stream_tmp_file_index (File_Stream) >= 0)
    Return_Flag = UG_FIO_TMP_FILE_MODE;

  return (Return_Flag);

}

INT_ ug_get_file_stream_tmp_file_index
 (FILE * File_Stream)

{
  
/*
 * Get index for a named binary TMP file that corresponds to a file stream.
 */

  FILE **TMP_File_Stream;

  INT_ Index, Number_of_TMP_Files;

  if (! UG_TMP_File_Struct_Ptr)
    return (-1);

  Number_of_TMP_Files = UG_TMP_File_Struct_Ptr->Number_of_TMP_Files;

  TMP_File_Stream = UG_TMP_File_Struct_Ptr->TMP_File_Stream;

  for (Index = 0; Index < Number_of_TMP_Files; ++Index)
  {
    if (TMP_File_Stream[Index] == File_Stream)
      return (Index);
  }

  return (-1);
}

void ug_get_tmp_file_name
 (char File_Name[],
  char UG_TMP_File_Name[])

{

/*
 * Set binary TMP file name including directory path.
 */

  if (UG_TMP_File_Dir_Flag)
  {
    strcpy (UG_TMP_File_Name, UG_TMP_File_Dir_);

    if (strcmp (File_Name, ""))
    {
      strcat (UG_TMP_File_Name, UG_PATH_SEP);
      strcat (UG_TMP_File_Name, File_Name);

      if (strcspn (File_Name, "+") == strlen (File_Name) - 1)
      {
        if (strcmp (UG_TMP_File_PID_Suffix_, "") == 0)
          sprintf (UG_TMP_File_PID_Suffix_, "%i", (int) ug_getpid ());

        strcat (UG_TMP_File_Name, UG_TMP_File_PID_Suffix_);
      }
    }
  }
  else
    strcpy (UG_TMP_File_Name, File_Name);

  return;

}

static INT_ ug_malloc_tmp_file_struct
 (UG_TMP_File_Struct ** UG_TMP_File_Struct_Ptr)
{

/*
 * Malloc binary TMP file structure arrays.
 */

  UG_TMP_File_Struct *Struct_Ptr_Ptr;

  INT_ Error_Flag = 0;

  *UG_TMP_File_Struct_Ptr = (UG_TMP_File_Struct *)
                            ug_malloc (&Error_Flag,
                                       sizeof (UG_TMP_File_Struct));

  if (Error_Flag > 0)
    return (-1);

  Struct_Ptr_Ptr = *UG_TMP_File_Struct_Ptr;

  Struct_Ptr_Ptr->Number_of_TMP_Files = 1;

  Struct_Ptr_Ptr->TMP_File_Name = (CHAR_UG_MAX *) ug_malloc (&Error_Flag,
                                                           sizeof (CHAR_UG_MAX));
  Struct_Ptr_Ptr->TMP_File_Stream = (FILE **) ug_malloc (&Error_Flag,
                                                         sizeof (FILE));
  Struct_Ptr_Ptr->TMP_File_Status = (INT_1D *) ug_malloc (&Error_Flag,
                                                          sizeof (INT_1D));

  if (Error_Flag > 0)
    return (-1);

  return (0);

}

static INT_ ug_realloc_tmp_file_struct
 (UG_TMP_File_Struct * UG_TMP_File_Struct_Ptr)

{

/*
 * Realloc binary TMP file structure arrays.
 */

  INT_ Error_Flag = 0;
  INT_ Number_of_TMP_Files;

  Number_of_TMP_Files = UG_TMP_File_Struct_Ptr->Number_of_TMP_Files;

  ++Number_of_TMP_Files;

  UG_TMP_File_Struct_Ptr->Number_of_TMP_Files = Number_of_TMP_Files;

  UG_TMP_File_Struct_Ptr->TMP_File_Name =
    (CHAR_UG_MAX *) ug_realloc (&Error_Flag,
                              UG_TMP_File_Struct_Ptr->TMP_File_Name,
                              Number_of_TMP_Files * sizeof (CHAR_UG_MAX));
  UG_TMP_File_Struct_Ptr->TMP_File_Stream =
    (FILE **) ug_realloc (&Error_Flag,
                          UG_TMP_File_Struct_Ptr->TMP_File_Stream,
                          Number_of_TMP_Files * sizeof (FILE));
  UG_TMP_File_Struct_Ptr->TMP_File_Status =
    (INT_1D *) ug_realloc (&Error_Flag,
                           UG_TMP_File_Struct_Ptr->TMP_File_Status,
                           Number_of_TMP_Files * sizeof (INT_1D));

  if (Error_Flag > 0)
    return (-1);

  return (0);

}

INT_ ug_rewind
 (FILE * File_Stream)

{
  
/*
 * Rewind a binary TMP or regular file.
 * 
 */

  rewind (File_Stream);

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_rewind                : rewound file with file stream ID=%i", (int) ug_fileno(File_Stream));
    ug_message (Text);
  }

  return (0);

}

void ug_rewind_data
 (char File_Name[])

{

/*
 * Rewind a binary TMP file by file name.
 */

  FILE *File_Stream = NULL;

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_rewind_data           : rewinding TMP file with associated name %s", File_Name);
    ug_message (Text);
  }

  File_Stream = ug_fopen (File_Name, "r_tmp");

  if (File_Stream)
  {
    ug_rewind (File_Stream);

    if (File_Status_Monitor)
    {
      sprintf (Text, "ug_rewind_data           : rewound TMP file with associated name %s", File_Name);
      ug_message (Text);
    }
  }

  return;

}

INT_ ug_set_tmp_file_dir
 (char UG_TMP_File_Dir[])

{

/*
 * Set binary TMP file directory and create it if directory doesn't exist.
 */

  INT_ mode;
  INT_ Error_Flag = -1;

  if (File_Status_Monitor)
  {
    sprintf (Text, "ug_set_tmp_file_dir      : setting TMP directory to %s", UG_TMP_File_Dir);
    ug_message (Text);
  }

  if (strcmp (UG_TMP_File_Dir, "NULL") == 0)
  {
    UG_TMP_File_Dir_Flag = 0;

    strcpy (UG_TMP_File_Dir_, "");

    return (0);
  }

  else
  {
    UG_TMP_File_Dir_Flag = 0;

    if (strcmp (UG_TMP_File_Dir, UG_PATH_SEP) && strcmp (UG_TMP_File_Dir, ""))
    {
      Error_Flag = ug_check_file (UG_TMP_File_Dir, 0);

      if (Error_Flag)
      {
        mode = UG_MODE_IRWXU;

        Error_Flag = ug_mkdir (UG_TMP_File_Dir, mode);

        if (Error_Flag == 0)
          UG_TMP_File_Dir_Flag = 2;

        if (File_Status_Monitor)
        {
          sprintf (Text, "ug_set_tmp_file_dir      : created TMP directory named %s", UG_TMP_File_Dir);
          ug_message (Text);
        }
      }
      else
        UG_TMP_File_Dir_Flag = 1;

      if (UG_TMP_File_Dir_Flag)
        strcpy (UG_TMP_File_Dir_, UG_TMP_File_Dir);
    }
  }

  if (UG_TMP_File_Dir_Flag == 0)
    return (408);

  return (0);

}

FILE * ug_tmpfile ( )
{

 /*
  * Open a temporary file.
  */

  FILE *TMP_File_Stream = NULL;

  char *tmpdir;

  CHAR_UG_MAX TMP_File_Dir;
  CHAR_UG_MAX TMP_File_Name;

  if (UG_TMP_File_Dir_Flag == 0)
  {
    tmpdir = ug_get_tmpdir ();

    if (! tmpdir)
      return (NULL);

    strcpy (TMP_File_Dir, tmpdir);

    if (File_Status_Monitor)
    {
      sprintf (Text, "ug_tmpfile               : TMP file directory %s set from environment", TMP_File_Dir);
      ug_message (Text);
    }
  }
  else
  {
    strcpy (TMP_File_Dir, UG_TMP_File_Dir_);

    if (File_Status_Monitor)
    {
      sprintf (Text, "ug_tmpfile               : TMP file directory %s set from input parameters", TMP_File_Dir);
      ug_message (Text);
    }
  }

  strcpy (TMP_File_Name, TMP_File_Dir);
  strcat (TMP_File_Name, UG_PATH_SEP);
  strcat (TMP_File_Name,"ug_tmpfile_XXXXXX");

  TMP_File_Stream = ug_open_tmpdir (TMP_File_Name);

  if (TMP_File_Stream)
  {
    if (File_Status_Monitor)
    {
      sprintf (Text, "ug_tmpfile               : opened TMP file name %s with file stream ID=%i", TMP_File_Name, (int) ug_fileno(TMP_File_Stream));
      ug_message (Text);
    }

    ug_unlink (TMP_File_Name);

    if (File_Status_Monitor)
    {
      sprintf (Text, "ug_tmpfile               : unlink TMP file name %s", TMP_File_Name);
      ug_message (Text);
    }
  }

  return (TMP_File_Stream);
}
