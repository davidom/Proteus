#include "UG_LIB.h"

void ug_check_param
 (UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Check UG parameters.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_check_param.c,v 1.7 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *Double_Param_Vector_Entries, *Int_Param, *Int_Param_Vector_Entries,
         *Max_Int_Param, *Min_Int_Param;

  DOUBLE_1D *Double_Param, *Max_Double_Param, *Min_Double_Param;

  INT_ Index, Number_of_Double_Params, Number_of_Int_Params;

  Number_of_Double_Params = UG_Param_Struct_Ptr->Number_of_Double_Params;
  Double_Param = UG_Param_Struct_Ptr->Double_Param;
  Double_Param_Vector_Entries = UG_Param_Struct_Ptr->Double_Param_Vector_Entries;
  Max_Double_Param = UG_Param_Struct_Ptr->Max_Double_Param;
  Min_Double_Param = UG_Param_Struct_Ptr->Min_Double_Param;

  Number_of_Int_Params = UG_Param_Struct_Ptr->Number_of_Int_Params;
  Int_Param = UG_Param_Struct_Ptr->Int_Param;
  Int_Param_Vector_Entries = UG_Param_Struct_Ptr->Int_Param_Vector_Entries;
  Max_Int_Param = UG_Param_Struct_Ptr->Max_Int_Param;
  Min_Int_Param = UG_Param_Struct_Ptr->Min_Int_Param;

  for (Index = 0; Index < Number_of_Int_Params; ++Index)
  {
    if (Int_Param_Vector_Entries[Index] == -1)
    {
      if (Max_Int_Param[Index] > Min_Int_Param[Index])
        Int_Param[Index] = MAX (MIN (Int_Param[Index],
                                     Max_Int_Param[Index]),
                                Min_Int_Param[Index]);
    }
  }

  for (Index = 0; Index < Number_of_Double_Params; ++Index)
  {
    if (Double_Param_Vector_Entries[Index] == -1)
    {
      if (Max_Double_Param[Index] > Min_Double_Param[Index])
        Double_Param[Index] = MAX (MIN (Double_Param[Index],
                                        Max_Double_Param[Index]),
                                   Min_Double_Param[Index]);
    }
  }

  return;

}
