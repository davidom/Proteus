#include "UG_LIB.h"

INT_ ug_get_int_param_vector
 (char _Int_Param_Name[],
  INT_ *_Int_Param_Vector_Entries,
  INT_1D **_Int_Param_Vector,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the values for a UG type int parameter vector.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_int_param_vector.c,v 1.11 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index;

  Index = ug_get_int_param_vector_ (_Int_Param_Name, _Int_Param_Vector_Entries, _Int_Param_Vector, UG_Param_Struct_Ptr);

  if (Index < 0)
    return (Index);

  return (1);

}

INT_ ug_get_int_param_vector_
 (char _Int_Param_Name[],
  INT_ *_Int_Param_Vector_Entries,
  INT_1D **_Int_Param_Vector,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the index and values for a UG type int parameter vector.
 */

  INT_ Get_Flag, Index, Loc;

  if (UG_Param_Struct_Ptr == NULL)
    return (-2);

  Get_Flag = 0;

  Index = 0;

  do
  {
    Get_Flag = ug_get_param_name_from_alt_names (UG_Param_Struct_Ptr->Int_Param_Name[Index], UG_Param_Struct_Ptr->Int_Param_Alt_Name[Index], _Int_Param_Name);

    ++Index;
  }
  while (Index < UG_Param_Struct_Ptr->Number_of_Int_Params && Get_Flag != 1);

  --Index;

  if (Get_Flag != 1)
    return (-1);

  if (UG_Param_Struct_Ptr->Int_Param_Vector_Entries[Index] < 0)
    return (-3);

  *_Int_Param_Vector_Entries = UG_Param_Struct_Ptr->Int_Param_Vector_Entries[Index];

  if (*_Int_Param_Vector_Entries < 0)
    return (-4);

  Loc = UG_Param_Struct_Ptr->Int_Param_Vector_Loc[Index];

  *_Int_Param_Vector = &(UG_Param_Struct_Ptr->Int_Param_Vector[Loc]);

  return (Index);

}
