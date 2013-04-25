#include "UG_LIB.h"

INT_ ug_get_arg_double_param_vector
 (char *argv[],
  char Param_Name[],
  char UG_Param_File_Name[],
  INT_ argc,
  INT_ *Param_Vector_Entries,
  DOUBLE_1D **Double_Param_Vector,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get the values for a UG type double parameter vector from an array of
 * arguments, the UG input parameter file, or the UG parameter structure.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_arg_double_param_vector.c,v 1.6 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  char *Char_Param_Value = NULL;

  INT_1D **Int_Param_Vector = NULL;

  INT_ Param_Type_Flag;
  INT_ Get_Flag = 0;
  INT_ *Int_Param_Value = NULL;

  double *Double_Param_Value = NULL;

  if (strcmp (UG_Param_File_Name, ""))
  {
    Param_Type_Flag = DOUBLE_PARAM_VECTOR_TYPE_FLAG;

    Get_Flag = ug_get_param_from_arg_or_file (argv, Char_Param_Value, Param_Name, UG_Param_File_Name,
                                              Int_Param_Vector, argc, Int_Param_Value, Param_Type_Flag, Param_Vector_Entries,
                                              Double_Param_Vector, Double_Param_Value);

    if (Get_Flag == 1) Get_Flag = 2;
  }

  if (Get_Flag == 0 && UG_Param_Struct_Ptr)
    Get_Flag = ug_get_double_param_vector (Param_Name, Param_Vector_Entries, Double_Param_Vector, UG_Param_Struct_Ptr);

  return (Get_Flag);

}
