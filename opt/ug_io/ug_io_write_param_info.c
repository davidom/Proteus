#include "UG_IO_LIB.h"

void ug_io_write_param_info
(INT_ UG_IO_Param_Output_Flag,
  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr)

{

/*
 * Write information on all UG_IO parameters to standard output.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_param_info.c,v 1.19 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *File_Type_Format_Flag;      

  CHAR_133 Text;
  CHAR_41 *File_Compression_Label, *File_Format_Label, *File_Mode_Label,
          *File_Type_Label;
  CHAR_21 *File_Type_Suffix;
  CHAR_11 *File_Compression_Suffix, *File_Format_Suffix, *File_Mode_Suffix;

  INT_ File_Type_Format_Flag_, Format_Option, Index, Input_Option, Label_Index,
       Max_IO_Param_Labels, Max_Labels_Output, Mode_Option,
       Number_of_File_Compressions, Number_of_File_Formats,
       Number_of_File_Modes, Number_of_File_Types, Output_Option;

  if (UG_IO_Param_Output_Flag == 0)
    return;

  Max_IO_Param_Labels = UG_IO_Param_Struct_Ptr->Max_IO_Param_Labels;

  Max_Labels_Output = (UG_IO_Param_Output_Flag == 2) ? Max_IO_Param_Labels : 1;
  
  Number_of_File_Types = UG_IO_Param_Struct_Ptr->Number_of_File_Types;
  File_Type_Format_Flag = UG_IO_Param_Struct_Ptr->File_Type_Format_Flag;
  File_Type_Label = UG_IO_Param_Struct_Ptr->File_Type_Label;
  File_Type_Suffix = UG_IO_Param_Struct_Ptr->File_Type_Suffix;

  Number_of_File_Formats = UG_IO_Param_Struct_Ptr->Number_of_File_Formats;
  File_Format_Label = UG_IO_Param_Struct_Ptr->File_Format_Label;
  File_Format_Suffix = UG_IO_Param_Struct_Ptr->File_Format_Suffix;

  Number_of_File_Modes = UG_IO_Param_Struct_Ptr->Number_of_File_Modes;
  File_Mode_Label = UG_IO_Param_Struct_Ptr->File_Mode_Label;
  File_Mode_Suffix = UG_IO_Param_Struct_Ptr->File_Mode_Suffix;
  
  Number_of_File_Compressions = UG_IO_Param_Struct_Ptr->Number_of_File_Compressions;
  File_Compression_Label = UG_IO_Param_Struct_Ptr->File_Compression_Label;
  File_Compression_Suffix = UG_IO_Param_Struct_Ptr->File_Compression_Suffix;

  ug_message (" ");
  ug_message ("UG_IO file names are specified as");
  ug_message (" ");
  ug_message ("   case_name.mode.type.format.compression");
  ug_message (" ");
  ug_message ("where case_name is the file case name, .mode is the file mode, .type is the");
  ug_message ("type suffix, .format is the format suffix, and .compression is the compression");
  ug_message ("suffix.");

  ug_message (" ");
  ug_message ("Suffix              File Type Description");
  ug_message ("-----------------   --------------------");

  for (Index = 0; Index < Number_of_File_Types; ++Index)
  {
    sprintf (Text, "%-20s%-40s", File_Type_Suffix[Index],
                                 File_Type_Label[Index*Max_IO_Param_Labels]);
    ug_message (Text);

    Label_Index = 1;

    while (Label_Index < Max_Labels_Output &&
           strcmp (File_Type_Label[Label_Index+Index*Max_IO_Param_Labels], "") != 0)
    {
      sprintf (Text, "                    %-40s",
              File_Type_Label[Label_Index+Index*Max_IO_Param_Labels]);
      ug_message (Text);

      ++Label_Index;
    }

    if (Max_Labels_Output > 1)
    {
      File_Type_Format_Flag_ = File_Type_Format_Flag[Index];

      Format_Option = File_Type_Format_Flag_ / 8;
      Mode_Option = (File_Type_Format_Flag_ - Format_Option * 8) / 4;
      Output_Option = (File_Type_Format_Flag_
                     - Format_Option * 8 - Mode_Option * 4) / 2;
      Input_Option = File_Type_Format_Flag_
                   - Format_Option * 8 - Mode_Option * 4 - Output_Option * 2;

      if (Format_Option == 0)
      {
        ug_message ("                    This file type uses a fixed file format");
        ug_message ("                    that is either a standard ASCII file");
        ug_message ("                    format or a binary format specific to");
        ug_message ("                    the file type.");
      }

      if (Mode_Option == 1)
      {
        ug_message ("                    This file type can use socket or TMP");
        ug_message ("                    file mode if a C binary double file");
        ug_message ("                    format is also used.");
      }

      if (Input_Option == 0)
      {
        ug_message ("                    This file type may only be used for");
        ug_message ("                    output files.");
      }

      if (Output_Option == 0)
      {
        ug_message ("                    This file type may only be used for");
        ug_message ("                    input files.");
      }

      ug_message (" ");
    }
  }

  ug_message (" ");
  ug_message ("Suffix    File Format Description");
  ug_message ("------    -----------------------");

  for (Index = 0; Index < Number_of_File_Formats; ++Index)
  {
    sprintf (Text, "%-10s%-40s", File_Format_Suffix[Index],
                                 File_Format_Label[Index*Max_IO_Param_Labels]);
    ug_message (Text);

    Label_Index = 1;

    while (Label_Index < Max_IO_Param_Labels &&
           strcmp (File_Format_Label[Label_Index+Index*Max_IO_Param_Labels], "") != 0)
    {
      sprintf (Text, "          %-40s",
               File_Format_Label[Label_Index+Index*Max_IO_Param_Labels]);
      ug_message (Text);

      ++Label_Index;
    }

    if (Max_Labels_Output > 1)
      ug_message (" ");
  }

  ug_message (" ");
  ug_message ("Suffix    File Mode Description");
  ug_message ("------    ---------------------");

  for (Index = 0; Index < Number_of_File_Modes; ++Index)
  {
    sprintf (Text, "%-10s%-40s", File_Mode_Suffix[Index],
                                 File_Mode_Label[Index*Max_IO_Param_Labels]);
    ug_message (Text);

    Label_Index = 1;

    while (Label_Index < Max_Labels_Output &&
           strcmp (File_Mode_Label[Label_Index+Index*Max_IO_Param_Labels], "") != 0)
    {
      sprintf (Text, "          %-40s",
               File_Mode_Label[Label_Index+Index*Max_IO_Param_Labels]);
      ug_message (Text);

      ++Label_Index;
    }

    if (Max_Labels_Output > 1)
      ug_message (" ");
  }

  ug_message (" ");
  ug_message ("Suffix    File Compression Description");
  ug_message ("------    ----------------------------");

  for (Index = 0; Index < Number_of_File_Compressions; ++Index)
  {
    sprintf (Text, "%-10s%-40s", File_Compression_Suffix[Index],
                                 File_Compression_Label[Index*Max_IO_Param_Labels]);
    ug_message (Text);

    Label_Index = 1;

    while (Label_Index < Max_IO_Param_Labels &&
           strcmp (File_Compression_Label[Label_Index+Index*Max_IO_Param_Labels],
                   "") != 0)
    {
      sprintf (Text, "          %-40s",
               File_Compression_Label[Label_Index+Index*Max_IO_Param_Labels]);
      ug_message (Text);

      ++Label_Index;
    }

    if (Max_Labels_Output > 1)
      ug_message (" ");
  }

  return;

}
