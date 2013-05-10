#include "UG_IO_LIB.h"

INT_ ug_io_malloc_func_flag
 (char Error_Message[],
  INT_ Number_of_U_Scalars,
  INT_ Number_of_U_Vectors,
  CHAR_21 ** U_Scalar_Labels,
  CHAR_21 ** U_Vector_Labels,
  INT_1D ** U_Scalar_Flags,
  INT_1D ** U_Vector_Flags)

{

/*
 * Malloc function label and flag arrays.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_malloc_func_flag.c,v 1.2 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Error_Flag;

  Error_Flag = 0;

  *U_Scalar_Labels = (CHAR_21 *) ug_malloc (&Error_Flag,
                                            Number_of_U_Scalars
                                          * sizeof (CHAR_21));
  *U_Vector_Labels = (CHAR_21 *) ug_malloc (&Error_Flag,
                                            Number_of_U_Vectors
                                          * sizeof (CHAR_21));
  *U_Scalar_Flags = (INT_1D *) ug_malloc (&Error_Flag,
                                          Number_of_U_Scalars
                                        * sizeof (INT_1D));
  *U_Vector_Flags = (INT_1D *) ug_malloc (&Error_Flag,
                                          Number_of_U_Vectors
                                        * sizeof (INT_1D));

  if (Error_Flag > 0)
  {
    strcpy (Error_Message, "unable to malloc function label and flag arrays");
    return (612);
  }

  return (0);

}
