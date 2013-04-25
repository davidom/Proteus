#include "UG_LIB.h"

INT_ ug_get_param_last_label_index
 (char Param_Name[],
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the last label index of a UG parameter.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_param_last_label_index.c,v 1.3 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_41 *Param_Label;

  CHAR_UG_MAX Char_Param_Value;

  INT_1D *Int_Param_Vector;

  DOUBLE_1D *Double_Param_Vector;

  INT_ Entries, Index, Int_Param_Value, Last_Label_Index;

  double Double_Param_Value;

  Index = ug_get_char_param_ (Param_Name, Char_Param_Value, UG_Param_Struct_Ptr);

  Param_Label = UG_Param_Struct_Ptr->Char_Param_Label;

  if (Index < 0)
  {
    Index = ug_get_double_param_ (Param_Name, &Double_Param_Value, UG_Param_Struct_Ptr);

    Param_Label = UG_Param_Struct_Ptr->Double_Param_Label;
  }

  if (Index < 0)
  {
    Index = ug_get_double_param_vector_ (Param_Name, &Entries, &Double_Param_Vector, UG_Param_Struct_Ptr);

    Param_Label = UG_Param_Struct_Ptr->Double_Param_Label;
  }

  if (Index < 0)
  {
    Index = ug_get_int_param_ (Param_Name, &Int_Param_Value, UG_Param_Struct_Ptr);

    Param_Label = UG_Param_Struct_Ptr->Int_Param_Label;
  }

  if (Index < 0)
  {
    Index = ug_get_int_param_vector_ (Param_Name, &Entries, &Int_Param_Vector, UG_Param_Struct_Ptr);

    Param_Label = UG_Param_Struct_Ptr->Int_Param_Label;
  }

  if (Index < 0)
    return (-1);

  Last_Label_Index = 0;

  do
  {
    if (strcmp (Param_Label[Last_Label_Index+Index*UG_Param_Struct_Ptr->Max_Param_Labels], "") == 0)
      return (Last_Label_Index-1);

    ++Last_Label_Index;
  }
  while (Last_Label_Index < UG_Param_Struct_Ptr->Max_Param_Labels);

  return (-2);
}
