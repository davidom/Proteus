#include "UG3_LIB.h"

void ug3_nnodeb2
 (INT_ nelem,
  INT_ *nnodeb,
  INT_4D * ieliel,
  INT_4D * iniel)

{

/*
 * Determine the maximum node index on the boundary surface from the element
 * neighbor connectivity.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_nnodeb2.c,v 1.2 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ielem, ieln, jelem;

  *nnodeb = 0;

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    for (ieln = 0; ieln <= 3; ++ieln)
    {
      jelem = ieliel[ielem][ieln];

      if (jelem <= 0)
      {
        if (ieln == 0)
        {
          *nnodeb = MAX (*nnodeb, iniel[ielem][1]);
          *nnodeb = MAX (*nnodeb, iniel[ielem][2]);
          *nnodeb = MAX (*nnodeb, iniel[ielem][3]);
        }
        else if (ieln == 1)
        {
          *nnodeb = MAX (*nnodeb, iniel[ielem][0]);
          *nnodeb = MAX (*nnodeb, iniel[ielem][2]);
          *nnodeb = MAX (*nnodeb, iniel[ielem][3]);
        }
        else if (ieln == 2)
        {
          *nnodeb = MAX (*nnodeb, iniel[ielem][0]);
          *nnodeb = MAX (*nnodeb, iniel[ielem][1]);
          *nnodeb = MAX (*nnodeb, iniel[ielem][3]);
        }
        else if (ieln == 3)
        {
          *nnodeb = MAX (*nnodeb, iniel[ielem][0]);
          *nnodeb = MAX (*nnodeb, iniel[ielem][1]);
          *nnodeb = MAX (*nnodeb, iniel[ielem][2]);
        }
      }
    }
  }

  return;

}
