#include "UG_IO_LIB.h"

INT_ ug_io_check_file_name
 (char Case_Name[],
  char Error_Message[],
  char File_Name[],
  INT_ Check_File_Flag,
  INT_ File_Data_Type_1,
  INT_ Output_File_Check,
  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr)
  
{
  
/*
 * Check a file name and extract a case name.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_check_file_name.c,v 1.21 2013/03/16 17:09:09 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX drive, dname, fname, ext;
  CHAR_31 Name_Text;
  CHAR_21 File_Type_Suffix_;
  CHAR_11 File_Compression_Suffix_, File_Format_Suffix_, File_Mode_Suffix_;

  CHAR_UG_MAX New_File_Name;
  CHAR_UG_MAX Org_Case_Name;

  INT_ Error_Flag, File_Data_Type_, File_Data_Type_2, File_Format_,
       File_Mode_, File_Type_Format_Flag_, Format_Option, Input_Option,
       Mode_Option, Output_Option;

/*
 * -----------------------------------------------------------------------------
 * Set secondary file type.
 * -----------------------------------------------------------------------------
 */

         if (File_Data_Type_1 == UG_IO_2D_EDGE_GRID)
      File_Data_Type_2 = UG_IO_2D_GRID;
    else if (File_Data_Type_1 == UG_IO_2D_GRID)
      File_Data_Type_2 = UG_IO_2D_EDGE_GRID;
    else if (File_Data_Type_1 == UG_IO_SURFACE_GRID)
      File_Data_Type_2 = UG_IO_VOLUME_GRID;
    else if (File_Data_Type_1 == UG_IO_VOLUME_GRID)
      File_Data_Type_2 = UG_IO_SURFACE_GRID;
    else
      File_Data_Type_2 = File_Data_Type_1;

/*
 * -----------------------------------------------------------------------------
 * CHECK 1 & 3
 * -----------------------------------------------------------------------------
 * Check to see if the file name string contains a full file name or file name
 * suffix. If it is a suffix then use the input argument case name as the
 * case name. The following two possible cases are allowed.
 *
 * 1) If the file name string is a full file name of the correct data type then
 *    the returned case name string will be derived from the full file name and
 *    the returned file name string will be unchanged.
 *
 * 2) If the file name string is a suffix of the correct data type (full file
 *    name minus case name) and the case name string is not blank then the
 *    returned case name string will be unchanged and the returned file name
 *    string will be unchanged with Check_File_Flag = 1 or derived from the
 *    input case and file name strings with Check_File_Flag = 3.
 * -----------------------------------------------------------------------------
 */

  if (Check_File_Flag == 1 || Check_File_Flag == 3)
  {
    strcpy (Org_Case_Name, Case_Name);

    Error_Flag = ug_io_case_name (File_Name, Case_Name, Error_Message,
                                  UG_IO_Param_Struct_Ptr);

    if (Error_Flag == 0)
    {
      if (Check_File_Flag == 3 && strcmp (Case_Name, "") == 0)
      {
        if (strcmp (Org_Case_Name, "") != 0)
        {
          strcpy (Case_Name, Org_Case_Name);
          strcpy (New_File_Name, Case_Name);
          strcat (New_File_Name, File_Name);
          strcpy (File_Name, New_File_Name);
        }
        else
        {
          strcpy (Error_Message,
           "file name string has no case name and case name string is blank");
          return (628);
        }
      }

      Error_Flag = ug_io_file_type (File_Name, Error_Message,
                                    UG_IO_Param_Struct_Ptr, &File_Data_Type_);

      if (Error_Flag != 0)
        return (Error_Flag);

      if (File_Data_Type_ != File_Data_Type_1 &&
          File_Data_Type_ != File_Data_Type_2)
      {
        ug_splitpath (File_Name, drive, dname, fname, ext);

        strcat (fname, ext);
        strcpy (Name_Text, "");
        strncat (Name_Text, fname, 29);

        sprintf (Error_Message, "file %s has an illegal file data type",
                 Name_Text);
        return (Error_Flag);
      }

      if (Output_File_Check != 0)
      {
        Error_Flag = ug_io_file_info (File_Name, Case_Name, Error_Message,
                                      File_Compression_Suffix_,
                                      File_Format_Suffix_,
                                      File_Mode_Suffix_, File_Type_Suffix_,
                                      UG_IO_Param_Struct_Ptr,
                                      0, &File_Data_Type_, &File_Format_,
                                      &File_Mode_, &File_Type_Format_Flag_);

        Format_Option = File_Type_Format_Flag_ / 8;
        Mode_Option = (File_Type_Format_Flag_ - Format_Option * 8) / 4;
        Output_Option = (File_Type_Format_Flag_
                       - Format_Option * 8 - Mode_Option * 4) / 2;
        Input_Option = File_Type_Format_Flag_
                     - Format_Option * 8 - Mode_Option * 4
                     - Output_Option * 2;

        if (Output_File_Check == -1 && Input_Option == 0)
        {
          ug_splitpath (File_Name, drive, dname, fname, ext);

          strcat (fname, ext);
          strcpy (Name_Text, "");
          strncat (Name_Text, fname, 29);

          sprintf (Error_Message,
                   "file type of file %s is not supported for input files",
                   Name_Text);
          return (629);
        }

        if (Output_File_Check == 1 && Output_Option == 0)
        {
          ug_splitpath (File_Name, drive, dname, fname, ext);

          strcat (fname, ext);
          strcpy (Name_Text, "");
          strncat (Name_Text, fname, 29);

          sprintf (Error_Message,
                   "file type of file %s is not supported for output files",
                   Name_Text);
          return (629);
        }
      }
    }
    else
    {
      Error_Flag = abs (Error_Flag);
      return (Error_Flag);
    }
  }

/*
 * -----------------------------------------------------------------------------
 * CHECK 2
 * -----------------------------------------------------------------------------
 * Search for a matching file name to either the case name string or file name
 * string. The following three possible cases are allowed.
 *
 * 1) If the file name string is empty then search for an existing file with a
 *    matching case name string. The returned case name string will be unchanged
 *    and the returned file name string will be the matching file name.
 *
 * 2) If the file name string is a case name (with no suffix) then search for
 *    an existing file with a matching case name string. The returned file name
 *    string will be the matching file name and the returned case name string
 *    will be derived from the returned file name.
 *
 * 3) If the file name string is a full file name of the correct type then
 *    search for an existing file (with or without compression) which matches
 *    that string. The returned file name string will be the matching file name
 *    (unchanged unless only a compressed version of the file matches) and the
 *    returned case name string will be derived from the returned file name.
 * -----------------------------------------------------------------------------
 */

  if (Check_File_Flag == 2)
  {
    if (strcmp (Case_Name, "") == 0 && strcmp (File_Name, "") == 0)
    {
      strcpy (Error_Message, "both file and case name strings are blank");
      return (630);
    }

    if (strcmp (Case_Name, "") == 0)
    {
      Error_Flag = ug_io_file_info (File_Name, Case_Name, Error_Message,
                                    File_Compression_Suffix_,
                                    File_Format_Suffix_,
                                    File_Mode_Suffix_, File_Type_Suffix_,
                                    UG_IO_Param_Struct_Ptr,
                                    1, &File_Data_Type_, &File_Format_,
                                    &File_Mode_, &File_Type_Format_Flag_);

      if (Error_Flag != 0)
      {
        strcpy (Case_Name, File_Name);
        strcpy (File_Name, "");
      }
    }
    else
      strcpy (File_Name, "");

    if (strcmp (File_Name, "") == 0)
    {
      Error_Flag = ug_io_find_file (Case_Name, Error_Message, File_Name,
                                    UG_IO_Param_Struct_Ptr, File_Data_Type_1);

      if (Error_Flag != 0)
        Error_Flag = ug_io_find_file (Case_Name, Error_Message, File_Name,
                                      UG_IO_Param_Struct_Ptr, File_Data_Type_2);

      if (Error_Flag != 0)
      {
        strcpy (File_Name, Case_Name);

        Error_Flag = ug_io_case_name (File_Name, Case_Name, Error_Message,
                                      UG_IO_Param_Struct_Ptr);

        if (Error_Flag == 0 && Output_File_Check == 0)
          Error_Flag = ug_io_file_info (File_Name, Case_Name, Error_Message,
                                        File_Compression_Suffix_,
                                        File_Format_Suffix_,
                                        File_Mode_Suffix_, File_Type_Suffix_,
                                        UG_IO_Param_Struct_Ptr,
                                        1, &File_Data_Type_, &File_Format_,
                                        &File_Mode_, &File_Type_Format_Flag_);
        else
        {
          Error_Flag = ug_io_find_file (Case_Name, Error_Message, File_Name,
                                        UG_IO_Param_Struct_Ptr,
                                        File_Data_Type_1);

          if (Error_Flag != 0)
            Error_Flag = ug_io_find_file (Case_Name, Error_Message, File_Name,
                                          UG_IO_Param_Struct_Ptr,
                                          File_Data_Type_2);
        }
      }

      if (Error_Flag != 0)
      {
        Error_Flag = abs (Error_Flag);
        return (Error_Flag);
      }
    }

    if (Output_File_Check != 0)
    {
      Error_Flag = ug_io_file_info (File_Name, Case_Name, Error_Message,
                                    File_Compression_Suffix_,
                                    File_Format_Suffix_,
                                    File_Mode_Suffix_, File_Type_Suffix_,
                                    UG_IO_Param_Struct_Ptr,
                                    0, &File_Data_Type_, &File_Format_,
                                    &File_Mode_, &File_Type_Format_Flag_);

      Format_Option = File_Type_Format_Flag_ / 8;
      Mode_Option = (File_Type_Format_Flag_ - Format_Option * 8) / 4;
      Output_Option = (File_Type_Format_Flag_
                     - Format_Option * 8 - Mode_Option * 4) / 2;
      Input_Option = File_Type_Format_Flag_
                   - Format_Option * 8 - Mode_Option * 4
                   - Output_Option * 2;

      if (Output_File_Check == -1 && Input_Option == 0)
      {
        ug_splitpath (File_Name, drive, dname, fname, ext);

        strcat (fname, ext);
        strcpy (Name_Text, "");
        strncat (Name_Text, fname, 29);

        sprintf (Error_Message,
                 "file type of file %s is not supported for input files",
                 Name_Text);
        return (631);
      }

      if (Output_File_Check == 1 && Output_Option == 0)
      {
        ug_splitpath (File_Name, drive, dname, fname, ext);

        strcat (fname, ext);
        strcpy (Name_Text, "");
        strncat (Name_Text, fname, 29);

        sprintf (Error_Message,
                 "file type of file %s is not supported for output files",
                 Name_Text);
        return (631);
      }
    }
  }

  return (0);

}
