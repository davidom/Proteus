#include "UG_LIB.h"

INT_ ug_read_param
 (char UG_Param_File_Name[],
  INT_ Message_Flag,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Search a UG parameter file for UG parameters to read and re-set.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_read_param.c,v 1.51 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  char *argv[1];

  INT_ argc = 0;
  INT_ Error_Flag;

  Error_Flag = ug_set_params_from_arg_or_file (argv, UG_Param_File_Name, argc, Message_Flag, UG_Param_Struct_Ptr);

  return (Error_Flag);

}
