#include "UG_LIB.h"

INT_ ug_get_read_int_param_vector
 (char Param_Name[],
  char UG_Param_File_Name[],
  INT_ *Param_Vector_Entries,
  INT_1D **Int_Param_Vector,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the values for a UG type int parameter vector from either the UG
 * parameter file or if not found there from the UG parameter structure.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_read_int_param_vector.c,v 1.19 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  char *argv[1];

  char *Char_Param_Value = NULL;

  DOUBLE_1D **Double_Param_Vector = NULL;

  INT_ Param_Type_Flag;
  INT_ argc = 0;
  INT_ Get_Flag = 0;
  INT_ *Int_Param_Value = NULL;

  double *Double_Param_Value = NULL;

  if (strcmp (UG_Param_File_Name, ""))
  {
    Param_Type_Flag = INT_PARAM_VECTOR_TYPE_FLAG;

    Get_Flag = ug_get_param_from_arg_or_file (argv, Char_Param_Value, Param_Name, UG_Param_File_Name,
                                              Int_Param_Vector, argc, Int_Param_Value, Param_Type_Flag, Param_Vector_Entries,
                                              Double_Param_Vector, Double_Param_Value);

    if (Get_Flag == 1) Get_Flag = 2;
  }

  if (Get_Flag == 0 && UG_Param_Struct_Ptr)
    Get_Flag = ug_get_int_param_vector (Param_Name, Param_Vector_Entries, Int_Param_Vector, UG_Param_Struct_Ptr);

  return (Get_Flag);

}
