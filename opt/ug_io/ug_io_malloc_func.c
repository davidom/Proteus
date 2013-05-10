#include "UG_IO_LIB.h"

INT_ ug_io_malloc_func
 (char Error_Message[],
  INT_ Number_of_Nodes,
  INT_ Number_of_U_Scalars,
  INT_ Number_of_U_Vectors,
  INT_1D * U_Scalar_Flags,
  INT_1D * U_Vector_Flags,
  DOUBLE_1D ** U_Scalars,
  DOUBLE_3D ** U_Vectors)

{

/*
 * Malloc function data arrays.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_malloc_func.c,v 1.2 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Error_Flag, Index, Number_of_U_Scalars_Active,
       Number_of_U_Vectors_Active;

  Error_Flag = 0;

  Number_of_U_Scalars_Active = 0;

  for (Index = 0; Index < Number_of_U_Scalars; ++Index)
  {
    if (U_Scalar_Flags[Index] != 0)
      ++Number_of_U_Scalars_Active;
  }

  Number_of_U_Vectors_Active = 0;

  for (Index = 0; Index < Number_of_U_Vectors; ++Index)
  {
    if (U_Vector_Flags[Index] != 0)
      ++Number_of_U_Vectors_Active;
  }

  *U_Scalars = (DOUBLE_1D *) ug_malloc (&Error_Flag,
                                        Number_of_U_Scalars_Active
                                      * (Number_of_Nodes+1)
                                      * sizeof (DOUBLE_1D));
  *U_Vectors = (DOUBLE_3D *) ug_malloc (&Error_Flag,
                                        Number_of_U_Vectors_Active
                                      * (Number_of_Nodes+1)
                                      * sizeof (DOUBLE_3D));

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc function data arrays");
    return (611);
  }

  return (0);

}
