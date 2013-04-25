#include "UG3_LIB.h"

INT_ ug3_chkvol
 (INT_ mmsg,
  INT_ nelem,
  INT_ nnode,
  INT_4D * iniel,
  double tol,
  DOUBLE_3D * x)

{

/*
 * Check tetrahedral element volume. Element volume (vol) is checked for zero
 * or negative volumes.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkvol.c,v 1.11 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_5D *inielc5 = NULL;
  INT_6D *inielc6 = NULL;
  INT_8D *inielc8 = NULL;

  INT_ ierr;
  INT_ nelemc5 = 0;
  INT_ nelemc6 = 0;
  INT_ nelemc8 = 0;

  ierr = ug3_chkvolm (mmsg, nelem, nelemc5, nelemc6, nelemc8, nnode,
                      iniel, inielc5, inielc6, inielc8,
                      tol, x);

  return (ierr);

}
