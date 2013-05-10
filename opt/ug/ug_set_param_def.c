#include "UG_LIB.h"

void ug_set_param_def
 (INT_ Number_of_Char_Params_i,
  INT_ Number_of_Double_Params_i,
  INT_ Number_of_Int_Params_i,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Set UG parameter variables to default values.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_set_param_def.c,v 1.10 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum 
 */

  INT_ Index;

  for (Index = Number_of_Int_Params_i; Index < UG_Param_Struct_Ptr->Number_of_Int_Params; ++Index)
  {
    if ((UG_Param_Struct_Ptr->Int_Param_Vector_Entries)[Index] == -1)
      (UG_Param_Struct_Ptr->Int_Param)[Index] = (UG_Param_Struct_Ptr->Def_Int_Param)[Index];
  }

  for (Index = Number_of_Double_Params_i; Index < UG_Param_Struct_Ptr->Number_of_Double_Params; ++Index)
  {
    if ((UG_Param_Struct_Ptr->Double_Param_Vector_Entries)[Index] == -1)
      (UG_Param_Struct_Ptr->Double_Param)[Index] = (UG_Param_Struct_Ptr->Def_Double_Param)[Index];
  }

  for (Index = Number_of_Char_Params_i; Index < UG_Param_Struct_Ptr->Number_of_Char_Params; ++Index)
  {
    strcpy ((UG_Param_Struct_Ptr->Char_Param)[Index], (UG_Param_Struct_Ptr->Def_Char_Param)[Index]);
  }

  return;

}
