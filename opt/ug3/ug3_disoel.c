#include "UG3_LIB.h"

void ug3_disoel
 (INT_ nbface,
  INT_ *nelem,
  INT_1D * ielibf,
  INT_4D * ieliel,
  INT_4D * iniel,
  INT_1D * jeliel)

{

/*
 * Search for and delete isolated elements with no neighbors.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_disoel.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, ielem, ielem1, ielem2, ielem3, ielem4, jelem, nelemi;

  jelem = 0;

  for (ielem = 1; ielem <= *nelem; ++ielem)
  {
    ielem1 = ieliel[ielem][0];
    ielem2 = ieliel[ielem][1];
    ielem3 = ieliel[ielem][2];
    ielem4 = ieliel[ielem][3];

    if (ielem1 > 0 || ielem2 > 0 || ielem3 > 0 || ielem4 > 0)
    {
      ++jelem;

      jeliel[ielem] = jelem;
    }
    else
      jeliel[ielem] = 0;
  }

  if (jelem < *nelem)
  {
    nelemi = *nelem;

    *nelem = jelem;

    for (ielem = 1; ielem <= nelemi; ++ielem)
    {
      jelem = jeliel[ielem];

      if (jelem > 0)
      {
        ielem1 = ieliel[ielem][0];
        ielem2 = ieliel[ielem][1];
        ielem3 = ieliel[ielem][2];
        ielem4 = ieliel[ielem][3];

        ieliel[jelem][0] = (ielem1 > 0) ? jeliel[ielem1] : ielem1;
        ieliel[jelem][1] = (ielem2 > 0) ? jeliel[ielem2] : ielem2;
        ieliel[jelem][2] = (ielem3 > 0) ? jeliel[ielem3] : ielem3;
        ieliel[jelem][3] = (ielem4 > 0) ? jeliel[ielem4] : ielem4;

        iniel[jelem][0] = iniel[ielem][0];
        iniel[jelem][1] = iniel[ielem][1];
        iniel[jelem][2] = iniel[ielem][2];
        iniel[jelem][3] = iniel[ielem][3];
      }
    }

    if (ielibf != NULL)
    {
      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        ielem = ielibf[ibface];

        if (ielem > 0)
          ielibf[ibface] = jeliel[ielem];
      }
    }
  }

  return;

}
