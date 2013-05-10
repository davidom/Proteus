#include "UG_IO_LIB.h"

INT_ ug_io_read_node_file
 (char Node_Data_File_Name[],
  char Error_Message[],
  UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr,
  INT_ Message_Flag,
  INT_ *Number_of_Nodes,
  DOUBLE_3D * Coordinates,
  DOUBLE_1D * Spacing,
  DOUBLE_4D * Vector1,
  DOUBLE_4D * Vector2)

{

/*
 * Read data from a standard UG node data file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_node_file.c,v 1.10 2013/03/16 17:09:09 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX Case_Name, File_Name, drive, dname, fname, ext;
  CHAR_133 Text;
  CHAR_31 Name_Text;
  CHAR_21 File_Type_Suffix_;
  CHAR_11 File_Compression_Suffix_, File_Format_Suffix_, File_Mode_Suffix_;

  FILE * Node_Data_File;

  INT_ Error_Flag, File_Data_Type_, File_Format_, File_Mode_, File_Name_Length,
       File_Type_Flag, File_Type_Format_Flag_;

  INT_ Check_File_Flag = 0;

  static INT_ Read_Task_Flag = 0;

  ++Read_Task_Flag;

  if (Read_Task_Flag > 2)
    Read_Task_Flag = 1;
  
  Error_Flag = ug_io_file_info (Node_Data_File_Name, Case_Name, Error_Message,
                                File_Compression_Suffix_, File_Format_Suffix_,
                                File_Mode_Suffix_, File_Type_Suffix_,
                                UG_IO_Param_Struct_Ptr,
                                Check_File_Flag, &File_Data_Type_,
                                &File_Format_, &File_Mode_,
                                &File_Type_Format_Flag_);
    
  if (Error_Flag != 0) 
  {
    Read_Task_Flag = 0;
    return (Error_Flag);
  }

  if (File_Data_Type_ != UG_IO_NODE_DATA)
  {
    Read_Task_Flag = 0;
    strcpy (Error_Message, "file is not a 3D node data file");
    return (-638);
  }

  File_Format_ = abs (File_Format_);

  if (Read_Task_Flag == 1 && Message_Flag >= 1)
  {
    ug_message (" ");
    ug_message ("UG_IO    : INPUT NODE DATA");
    ug_message (" ");
    ug_message ("UG_IO    : Reading Data");
    ug_message (" ");

    ug_cpu_message ("");
  }

  File_Name_Length = (INT_) strlen (Node_Data_File_Name)
                   - (INT_) strlen (File_Compression_Suffix_);

  strcpy (File_Name, "");
  strncat (File_Name, Node_Data_File_Name, File_Name_Length);

  if (Read_Task_Flag == 1)
  {
    Error_Flag = ug_uncompress_file (Node_Data_File_Name);

    if (Error_Flag != 0)
    {
      Read_Task_Flag = 0;
      strcpy (Error_Message, "unable to uncompress 3D node data file");
      return (638);
    }
  
    *Number_of_Nodes = 0;
  }

  File_Type_Flag = (strcmp (File_Type_Suffix_,  ".node") == 0) ? 1:
                   (strcmp (File_Type_Suffix_, ".snode") == 0) ? 2:
                   (strcmp (File_Type_Suffix_, ".vnode") == 0) ? 3: 0;

  if (File_Type_Flag > 0)
  {
    Node_Data_File = ug_fopen (File_Name, "r");

    if (Node_Data_File == NULL)
    {
      Read_Task_Flag = 0;
      strcpy (Error_Message, "unable to open 3D node data file");
      return (638);
    }

    Error_Flag = ug_io_read_node (Node_Data_File, Error_Message, 
                                  File_Type_Flag, Read_Task_Flag,
                                  Number_of_Nodes,
                                  Coordinates, Spacing, Vector1, Vector2);

    if (Error_Flag > 0)
    {
      Read_Task_Flag = 0;
      Error_Flag = ug_fclose (Node_Data_File);
      return (638);
    }

    Error_Flag = ug_fclose (Node_Data_File);
  }

  else
  {
    Read_Task_Flag = 0;
    strcpy (Error_Message, "unknown input 3D node data file type");
    return (638);
  }

  if (Read_Task_Flag == 1 && Message_Flag >= 1)
  {
    ug_splitpath (Node_Data_File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "UG_IO    : NodeData File Name= %s", Name_Text);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Nodes             =%10i", *Number_of_Nodes);
    ug_message (Text);
  }

  if (Read_Task_Flag == 2 && Message_Flag >= 1)
  {
    ug_message (" ");
    ug_cpu_message ("UG_IO    :");
  }

  if (Read_Task_Flag == 2)
    Error_Flag = ug_compress_file (1, File_Compression_Suffix_, File_Name);

  return (0);

}
