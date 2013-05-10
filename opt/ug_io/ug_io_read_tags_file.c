#include "UG_IO_LIB.h"

INT_ ug_io_read_tags_file
 (char Tags_Data_File_Name[],
  char Error_Message[],
  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr,
  UG_Param_Struct * UG_Param_Struct_Ptr,
  INT_ Message_Flag,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_1D * Surf_ID_Flag)

{

/*
 * Read surface grid BC and normal spacing data from a standard UG tags data
 * file.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_tags_file.c,v 1.10 2013/03/16 17:09:09 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX Case_Name, File_Name, drive, dname, fname, ext;
  CHAR_133 Text;
  CHAR_31 Name_Text;
  CHAR_21 File_Type_Suffix_;
  CHAR_11 File_Compression_Suffix_, File_Format_Suffix_, File_Mode_Suffix_;

  FILE * Tags_Data_File;

  INT_ Error_Flag, File_Data_Type_, File_Format_, File_Mode_, File_Name_Length,
       File_Type_Format_Flag_;

  INT_ Check_File_Flag = 0;

  Error_Flag = ug_io_file_info (Tags_Data_File_Name, Case_Name, Error_Message,
                                File_Compression_Suffix_, File_Format_Suffix_,
                                File_Mode_Suffix_, File_Type_Suffix_,
                                UG_IO_Param_Struct_Ptr,
                                Check_File_Flag, &File_Data_Type_,
                                &File_Format_, &File_Mode_,
                                &File_Type_Format_Flag_);

  if (Error_Flag != 0)
    return (Error_Flag);

  if (File_Data_Type_ != UG_IO_TAGS_DATA)
  {
    strcpy (Error_Message, "file is not a tags data file");
    return (-638);
  }

  if (Message_Flag >= 1)
  {
    ug_message (" ");
    ug_message ("UG_IO    : INPUT TAGS DATA");
    ug_message (" ");
    ug_message ("UG_IO    : Reading Data");
    ug_message (" ");

    ug_cpu_message ("");

    ug_splitpath (Tags_Data_File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "UG_IO    : TagsData File Name= %s", Name_Text);
    ug_message (Text);
  }

  File_Name_Length = (INT_) strlen (Tags_Data_File_Name)
                   - (INT_) strlen (File_Compression_Suffix_);

  strcpy (File_Name, "");
  strncat (File_Name, Tags_Data_File_Name, File_Name_Length);

  Error_Flag = ug_uncompress_file (Tags_Data_File_Name);

  if (Error_Flag != 0)
  {
    strcpy (Error_Message, "unable to uncompress tags data file");
    return (638);
  }

  if (strcmp (File_Type_Suffix_, ".tags") == 0)
  {
    Tags_Data_File = ug_fopen (File_Name, "r");

    if (Tags_Data_File == NULL)
    {
      strcpy (Error_Message, "unable to open tags data file");
      return (638);
    }

    Error_Flag = ug_io_read_tags (Tags_Data_File, 
                                  UG_Param_Struct_Ptr,
                                  Message_Flag,
                                  Number_of_Surf_Quads, Number_of_Surf_Trias,
                                  Surf_ID_Flag);

    if (Error_Flag)
    {
      strcpy (Error_Message, "unable to read tags data file and/or properly set BC data");
      return (638);
    }

    Error_Flag = ug_fclose (Tags_Data_File);
  }
  else
  {
    strcpy (Error_Message, "unknown input tags data file type");
    return (638);
  }

  if (Message_Flag >= 1)
  {
    ug_message (" ");
    ug_cpu_message ("UG_IO    :");
  }

  Error_Flag = ug_compress_file (1, File_Compression_Suffix_, File_Name);

  return (0);

}
