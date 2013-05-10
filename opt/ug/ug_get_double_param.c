#include "UG_LIB.h"

INT_ ug_get_double_param
 (char _Double_Param_Name[],
  double *_Double_Param_Value,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the value of a UG type double parameter.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_double_param.c,v 1.20 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index;

  Index = ug_get_double_param_ (_Double_Param_Name, _Double_Param_Value, UG_Param_Struct_Ptr);

  if (Index < 0)
    return (Index);

  return (1);

}

INT_ ug_get_double_param_
 (char _Double_Param_Name[],
  double *_Double_Param_Value,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the index and value of a UG type double parameter.
 */

  char *Param_Name_Suffix = NULL;

  INT_ Get_Flag, Index;

  if (UG_Param_Struct_Ptr == NULL)
    return (-2);

  Get_Flag = 0;

  Index = 0;

  do
  {
    Get_Flag = ug_get_param_name_from_alt_names (UG_Param_Struct_Ptr->Double_Param_Name[Index], UG_Param_Struct_Ptr->Double_Param_Alt_Name[Index], _Double_Param_Name);

    ++Index;
  }
  while (Index < UG_Param_Struct_Ptr->Number_of_Double_Params && Get_Flag != 1);

  --Index;

  if (Get_Flag != 1)
    return (-1);

  if (UG_Param_Struct_Ptr->Double_Param_Vector_Entries[Index] >= 0)
    return (-3);

  Param_Name_Suffix = strstr (_Double_Param_Name, "@");

  if (Param_Name_Suffix == NULL)
  {
    *_Double_Param_Value = UG_Param_Struct_Ptr->Double_Param[Index];

    if (UG_Param_Struct_Ptr->Max_Double_Param[Index] > UG_Param_Struct_Ptr->Min_Double_Param[Index])
      *_Double_Param_Value = MAX (MIN (*_Double_Param_Value, UG_Param_Struct_Ptr->Max_Double_Param[Index]),
                                                             UG_Param_Struct_Ptr->Min_Double_Param[Index]);
  }
  else if (strcmp (Param_Name_Suffix, "@def") == 0)
    *_Double_Param_Value = UG_Param_Struct_Ptr->Def_Double_Param[Index];
  else if (strcmp (Param_Name_Suffix, "@max") == 0)
    *_Double_Param_Value = UG_Param_Struct_Ptr->Max_Double_Param[Index];
  else if (strcmp (Param_Name_Suffix, "@min") == 0)
    *_Double_Param_Value = UG_Param_Struct_Ptr->Min_Double_Param[Index];
  else
    return (-1);

  return (Index);

}
