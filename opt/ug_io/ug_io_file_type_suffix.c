#include "UG_IO_LIB.h"

INT_ ug_io_file_type_suffix
 (char File_Name[],
  char File_Type_Suffix[],
  char Error_Message[],
  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr)

{

/*
 * Extract file type suffix from a standard UG file name.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_file_type_suffix.c,v 1.3 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX Case_Name;

  CHAR_21 File_Type_Suffix_;
  CHAR_11 File_Compression_Suffix_, File_Format_Suffix_, File_Mode_Suffix_;

  INT_ Error_Flag, File_Data_Type_, File_Format_, File_Mode_,
       File_Type_Format_Flag_;

  Error_Flag = ug_io_file_info (File_Name, Case_Name, Error_Message,
                                File_Compression_Suffix_, File_Format_Suffix_,
                                File_Mode_Suffix_, File_Type_Suffix_,
                                UG_IO_Param_Struct_Ptr,
                                0, &File_Data_Type_, &File_Format_, &File_Mode_,
                                &File_Type_Format_Flag_);

  strcpy (File_Type_Suffix, "");
  strcat (File_Type_Suffix, File_Type_Suffix_);

  return (Error_Flag);

}
