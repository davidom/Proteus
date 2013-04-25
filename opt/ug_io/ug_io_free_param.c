#include "UG_IO_LIB.h"

void ug_io_free_param
 (UG_IO_Param_Struct * UG_IO_Param_Struct_Ptr)

{

/*
 * Free UG_IO parameter structure arrays.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_free_param.c,v 1.4 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  if (UG_IO_Param_Struct_Ptr != NULL)
  {
    ug_free (UG_IO_Param_Struct_Ptr->File_Data_Type);
    ug_free (UG_IO_Param_Struct_Ptr->File_Type_Format_Flag);
    ug_free (UG_IO_Param_Struct_Ptr->File_Type_Label);
    ug_free (UG_IO_Param_Struct_Ptr->File_Type_Suffix);

    ug_free (UG_IO_Param_Struct_Ptr->File_Format);
    ug_free (UG_IO_Param_Struct_Ptr->File_Format_Label);
    ug_free (UG_IO_Param_Struct_Ptr->File_Format_Suffix);

    ug_free (UG_IO_Param_Struct_Ptr->File_Mode);
    ug_free (UG_IO_Param_Struct_Ptr->File_Mode_Label);
    ug_free (UG_IO_Param_Struct_Ptr->File_Mode_Suffix);

    ug_free (UG_IO_Param_Struct_Ptr->File_Compression_Label);
    ug_free (UG_IO_Param_Struct_Ptr->File_Compression_Suffix);

    ug_free (UG_IO_Param_Struct_Ptr);
  }

  return;

}
