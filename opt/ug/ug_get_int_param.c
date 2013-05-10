#include "UG_LIB.h"

INT_ ug_get_int_param
 (char _Int_Param_Name[],
  INT_ *_Int_Param_Value,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the value of a UG type int parameter.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_int_param.c,v 1.20 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index;

  Index = ug_get_int_param_ (_Int_Param_Name, _Int_Param_Value, UG_Param_Struct_Ptr);

  if (Index < 0)
    return (Index);

  return (1);

}

INT_ ug_get_int_param_
 (char _Int_Param_Name[],
  INT_ *_Int_Param_Value,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the index and value of a UG type int parameter.
 */

  char *Param_Name_Suffix = NULL;

  INT_ Get_Flag, Index;

  if (UG_Param_Struct_Ptr == NULL)
    return (-2);

  Get_Flag = 0;

  Index = 0;

  do
  {
    Get_Flag = ug_get_param_name_from_alt_names (UG_Param_Struct_Ptr->Int_Param_Name[Index], UG_Param_Struct_Ptr->Int_Param_Alt_Name[Index], _Int_Param_Name);

    ++Index;
  }
  while (Index < UG_Param_Struct_Ptr->Number_of_Int_Params && ! Get_Flag);

  --Index;

  if (! Get_Flag)
    return (-1);

  if (UG_Param_Struct_Ptr->Int_Param_Vector_Entries[Index] >= 0)
    return (-3);

  Param_Name_Suffix = strstr (_Int_Param_Name, "@");

  if (Param_Name_Suffix == NULL)
  {
    *_Int_Param_Value = UG_Param_Struct_Ptr->Int_Param[Index];

    if (UG_Param_Struct_Ptr->Max_Int_Param[Index] > UG_Param_Struct_Ptr->Min_Int_Param[Index])
      *_Int_Param_Value = MAX (MIN (*_Int_Param_Value, UG_Param_Struct_Ptr->Max_Int_Param[Index]),
                                                       UG_Param_Struct_Ptr->Min_Int_Param[Index]);
  }
  else if (strcmp (Param_Name_Suffix, "@def") == 0)
    *_Int_Param_Value = UG_Param_Struct_Ptr->Def_Int_Param[Index];
  else if (strcmp (Param_Name_Suffix, "@max") == 0)
    *_Int_Param_Value = UG_Param_Struct_Ptr->Max_Int_Param[Index];
  else if (strcmp (Param_Name_Suffix, "@min") == 0)
    *_Int_Param_Value = UG_Param_Struct_Ptr->Min_Int_Param[Index];
  else
    return (-1);

  return (Index);

}
