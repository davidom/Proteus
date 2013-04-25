#include "UG_LIB.h"

void ug_free_param
 (UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Free UG parameter structure arrays.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_free_param.c,v 1.10 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  if (UG_Param_Struct_Ptr)
  {
    ug_free (UG_Param_Struct_Ptr->Char_Param_Flag);
    ug_free (UG_Param_Struct_Ptr->Char_Param_Name);
    ug_free (UG_Param_Struct_Ptr->Char_Param_Alt_Name);
    ug_free (UG_Param_Struct_Ptr->Char_Param_Label);
    ug_free (UG_Param_Struct_Ptr->Char_Param);
    ug_free (UG_Param_Struct_Ptr->Def_Char_Param);
    ug_free (UG_Param_Struct_Ptr->Char_Param_Type);
    ug_free (UG_Param_Struct_Ptr->Double_Param_Flag);
    ug_free (UG_Param_Struct_Ptr->Double_Param_Name);
    ug_free (UG_Param_Struct_Ptr->Double_Param_Alt_Name);
    ug_free (UG_Param_Struct_Ptr->Double_Param_Label);
    ug_free (UG_Param_Struct_Ptr->Double_Param_Vector_Entries);
    ug_free (UG_Param_Struct_Ptr->Double_Param_Vector_Loc);
    ug_free (UG_Param_Struct_Ptr->Double_Param);
    ug_free (UG_Param_Struct_Ptr->Double_Param_Vector);
    ug_free (UG_Param_Struct_Ptr->Def_Double_Param);
    ug_free (UG_Param_Struct_Ptr->Max_Double_Param);
    ug_free (UG_Param_Struct_Ptr->Min_Double_Param);
    ug_free (UG_Param_Struct_Ptr->Int_Param_Flag);
    ug_free (UG_Param_Struct_Ptr->Int_Param_Name);
    ug_free (UG_Param_Struct_Ptr->Int_Param_Alt_Name);
    ug_free (UG_Param_Struct_Ptr->Int_Param_Label);
    ug_free (UG_Param_Struct_Ptr->Int_Param_Vector_Entries);
    ug_free (UG_Param_Struct_Ptr->Int_Param_Vector_Loc);
    ug_free (UG_Param_Struct_Ptr->Int_Param);
    ug_free (UG_Param_Struct_Ptr->Int_Param_Vector);
    ug_free (UG_Param_Struct_Ptr->Def_Int_Param);
    ug_free (UG_Param_Struct_Ptr->Max_Int_Param);
    ug_free (UG_Param_Struct_Ptr->Min_Int_Param);

    ug_free (UG_Param_Struct_Ptr);
  }

  return;

}
