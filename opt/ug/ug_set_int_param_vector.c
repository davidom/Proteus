#include "UG_LIB.h"

INT_ ug_set_int_param_vector
 (char Name[],
  INT_ Entries,
  INT_1D *_Vector,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Set the values for a UG type int parameter vector.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_set_int_param_vector.c,v 1.17 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *Vector_ptr = NULL;

  INT_ Current_Entries, Error_Flag, Index;
  INT_ Message_Flag=0;

  Index = ug_get_int_param_vector_ (Name, &Current_Entries, &Vector_ptr, UG_Param_Struct_Ptr);

  if (Index < 0)
    return (-1);

  Error_Flag = ug_set_int_param_vector_ (Index, Message_Flag, Entries, _Vector, UG_Param_Struct_Ptr);

  if (Error_Flag == 0) Error_Flag = 1;

  return (Error_Flag);

}

INT_ ug_set_int_param_vector_
 (INT_ Index,
  INT_ Message_Flag,
  INT_ Entries,
  INT_1D *_Vector,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{
  CHAR_133 Text;

  INT_ Error_Flag, Loc, Total_Entries, Vector_Index;
  INT_ Max_Output_Entries = 100;

  if (UG_Param_Struct_Ptr == NULL)
    return (-1);

  if (Entries < 0)
    return (-1);

  Error_Flag = 0;

  Total_Entries = UG_Param_Struct_Ptr->Number_of_Int_Param_Vector_Entries + Entries;

  UG_Param_Struct_Ptr->Int_Param_Vector = (INT_1D *) ug_realloc (&Error_Flag,
                                                                       UG_Param_Struct_Ptr->Int_Param_Vector,
                                                                       Total_Entries * sizeof (INT_1D));

  if (Error_Flag > 0)
    return (100407);

  UG_Param_Struct_Ptr->Int_Param_Vector_Loc[Index] = UG_Param_Struct_Ptr->Number_of_Int_Param_Vector_Entries;
  UG_Param_Struct_Ptr->Number_of_Int_Param_Vector_Entries = Total_Entries;

  Loc = UG_Param_Struct_Ptr->Int_Param_Vector_Loc[Index];

  for (Vector_Index = 0; Vector_Index < Entries; ++Vector_Index)
  {
    UG_Param_Struct_Ptr->Int_Param_Vector[Loc] = _Vector[Vector_Index];

    if (Message_Flag >= 1 && (Vector_Index < Max_Output_Entries || Vector_Index == Entries-1))
    {
      sprintf (Text, "UG PARAM : %-24s[%2i]= %-10i", UG_Param_Struct_Ptr->Int_Param_Name[Index], (int) Vector_Index, (int) _Vector[Vector_Index]);
      ug_message (Text);
    }

    ++Loc;
  }

  UG_Param_Struct_Ptr->Int_Param_Vector_Entries[Index] = Entries;

  return (0);

}
