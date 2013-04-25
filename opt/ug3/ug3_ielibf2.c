#include "UG3_LIB.h"

INT_ ug3_ielibf2
 (INT_ nbface,
  INT_ nelem,
  INT_4D * ieliel,
  INT_1D * ielibf)

{

/*
 * Determine for each boundary face the element that contains the face from the
 * element to element connectivity.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ielibf2.c,v 1.6 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, ielem, ielem1, ielem2, ielem3, ielem4;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ielibf[ibface] = 0;
  }

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    ielem1 = ieliel[ielem][0];
    ielem2 = ieliel[ielem][1];
    ielem3 = ieliel[ielem][2];
    ielem4 = ieliel[ielem][3];

    if (ielem1 < 0)
    {
      ibface = -ielem1;

      ielibf[ibface] = ielem;
    }

    if (ielem2 < 0)
    {
      ibface = -ielem2;

      ielibf[ibface] = ielem;
    }

    if (ielem3 < 0)
    {
      ibface = -ielem3;

      ielibf[ibface] = ielem;
    }

    if (ielem4 < 0)
    {
      ibface = -ielem4;

      ielibf[ibface] = ielem;
    }
  }

  ibface = 1;

  do
  {
    ielem = ielibf[ibface];

    ++ibface;
  }
  while (ibface <= nbface && ielem > 0);

  if (ielem == 0)
    return (313);

  return (0);

}
