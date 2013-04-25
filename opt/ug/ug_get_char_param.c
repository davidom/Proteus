#include "UG_LIB.h"

INT_ ug_get_char_param
 (char _Char_Param_Name[],
  char _Char_Param_Value[],
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the value of a UG type char parameter.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_char_param.c,v 1.15 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index;

  Index = ug_get_char_param_ (_Char_Param_Name, _Char_Param_Value, UG_Param_Struct_Ptr);

  if (Index < 0)
    return (Index);

  return (1);

}

INT_ ug_get_char_param_
 (char _Char_Param_Name[],
  char _Char_Param_Value[],
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the index and value of a UG type char parameter.
 */

  char *Param_Name_Suffix = NULL;

  INT_ Get_Flag, Index;

  if (UG_Param_Struct_Ptr == NULL)
    return (-2);

  Get_Flag = 0;

  Index = 0;

  do
  {
    Get_Flag = ug_get_param_name_from_alt_names (UG_Param_Struct_Ptr->Char_Param_Name[Index], UG_Param_Struct_Ptr->Char_Param_Alt_Name[Index], _Char_Param_Name);

    ++Index;
  }
  while (Index < UG_Param_Struct_Ptr->Number_of_Char_Params && Get_Flag != 1);

  --Index;

  if (Get_Flag != 1)
    return (-1);

  Param_Name_Suffix = strstr (_Char_Param_Name, "@");

  if (Param_Name_Suffix == NULL)
  {
    if (strcmp (UG_Param_Struct_Ptr->Char_Param[Index], "NULL") == 0)
      return (-3);

    strcpy (_Char_Param_Value, UG_Param_Struct_Ptr->Char_Param[Index]);
  }
  else if (strcmp (Param_Name_Suffix, "@def") == 0)
    strcpy (_Char_Param_Value, UG_Param_Struct_Ptr->Def_Char_Param[Index]);
  else
    return (-1);

  return (Index);

}
