#include "UG_IO_LIB.h"

INT_ ug_io_malloc_param
 (char Error_Message[],
  UG_IO_Param_Struct ** UG_IO_Param_Struct_Ptr)

{

/*
 * Malloc UG_IO parameter structure arrays.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_malloc_param.c,v 1.16 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  UG_IO_Param_Struct *Struct_Ptr_Ptr;

  INT_ Max_File_Compressions = 10;
  INT_ Max_File_Formats = 10;
  INT_ Max_File_Modes = 10;
  INT_ Max_File_Types = 100;
  INT_ Max_IO_Param_Labels = 21;

  INT_ Error_Flag = 0;

  *UG_IO_Param_Struct_Ptr = (UG_IO_Param_Struct *)
                            ug_malloc (&Error_Flag,
                                       sizeof (UG_IO_Param_Struct));

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc UG_IO parameter structure");
    return (616);
  }

  Struct_Ptr_Ptr = *UG_IO_Param_Struct_Ptr;

  Struct_Ptr_Ptr->Max_IO_Param_Labels = Max_IO_Param_Labels;

  Struct_Ptr_Ptr->Max_File_Types = Max_File_Types;
  Struct_Ptr_Ptr->Number_of_File_Types = 0;

  Struct_Ptr_Ptr->Max_File_Formats = Max_File_Formats;
  Struct_Ptr_Ptr->Number_of_File_Formats = 0;

  Struct_Ptr_Ptr->Max_File_Modes = Max_File_Modes;
  Struct_Ptr_Ptr->Number_of_File_Modes = 0;

  Struct_Ptr_Ptr->Max_File_Compressions = Max_File_Compressions;
  Struct_Ptr_Ptr->Number_of_File_Compressions = 0;

  Struct_Ptr_Ptr->File_Data_Type = (INT_1D *)
                                   ug_malloc (&Error_Flag,
                                              Max_File_Types
                                            * sizeof (INT_1D));
  Struct_Ptr_Ptr->File_Type_Format_Flag = (INT_1D *)
                                          ug_malloc (&Error_Flag,
                                                     Max_File_Types
                                                   * sizeof (INT_1D));
  Struct_Ptr_Ptr->File_Type_Label = (CHAR_41 *)
                                    ug_malloc (&Error_Flag,
                                               Max_IO_Param_Labels
                                             * Max_File_Types
                                             * sizeof (CHAR_41));
  Struct_Ptr_Ptr->File_Type_Suffix = (CHAR_21 *)
                                     ug_malloc (&Error_Flag,
                                                Max_File_Types
                                              * sizeof (CHAR_21));

  Struct_Ptr_Ptr->File_Format = (INT_1D *)
                                ug_malloc (&Error_Flag,
                                           Max_IO_Param_Labels
                                         * sizeof (INT_1D));
  Struct_Ptr_Ptr->File_Format_Label = (CHAR_41 *)
                                      ug_malloc (&Error_Flag,
                                                 Max_IO_Param_Labels
                                               * Max_File_Formats
                                               * sizeof (CHAR_41));
  Struct_Ptr_Ptr->File_Format_Suffix = (CHAR_11 *)
                                       ug_malloc (&Error_Flag,
                                                  Max_File_Formats
                                                * sizeof (CHAR_11));

  Struct_Ptr_Ptr->File_Mode = (INT_1D *)
                              ug_malloc (&Error_Flag,
                                         Max_IO_Param_Labels
                                       * sizeof (INT_1D));
  Struct_Ptr_Ptr->File_Mode_Label = (CHAR_41 *)
                                    ug_malloc (&Error_Flag,
                                               Max_IO_Param_Labels
                                             * Max_File_Modes
                                             * sizeof (CHAR_41));
  Struct_Ptr_Ptr->File_Mode_Suffix = (CHAR_11 *)
                                     ug_malloc (&Error_Flag,
                                                Max_File_Modes
                                              * sizeof (CHAR_11));

  Struct_Ptr_Ptr->File_Compression_Label = (CHAR_41 *)
                                           ug_malloc (&Error_Flag,
                                                      Max_IO_Param_Labels
                                                    * Max_File_Compressions
                                                    * sizeof (CHAR_41));
  Struct_Ptr_Ptr->File_Compression_Suffix = (CHAR_11 *)
                                            ug_malloc (&Error_Flag,
                                                       Max_File_Compressions
                                                     * sizeof (CHAR_11));

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc UG_IO parameter arrays");
    return (616);
  }

  return (0);

}
