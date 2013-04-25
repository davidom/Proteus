#include "UG3_LIB.h"

INT_ ug3_rmtbf
 (INT_ *nbface,
  INT_ nelem,
  INT_1D * ibcibf,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_1D * irfibf,
  INT_4D * ieliel)

{

/*
 * 
 * Remove internal transparent/embedded surface faces from a volume grid, delete
 * boundary surface internal transparent/embedded surface face connectivity, and
 * reset element neighbor connectivity.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_rmtbf.c,v 1.8 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *ielibf, *jbfibf;

  INT_ ibc, ibface, ielem, ieln, ierr, jbface, jelem, jeln;

  if (nelem == 0)
    return (0);

  ibface = 1;

  do
  {
    ibc = abs (ibcibf[ibface]);

    ++ibface;
  }
  while (ibface <= *nbface && ibc != TRANSP_INTRNL_UG3_GBC);

  if (ibc != TRANSP_INTRNL_UG3_GBC)
    return (0);

  if (ieliel != NULL)
  {
    ierr = 0;

    ielibf = (INT_1D *) ug_malloc (&ierr, (*nbface+1) * sizeof (INT_1D));

    if (ierr > 0)
      return (100112);

    ierr = ug3_ielibf2 (*nbface, nelem, ieliel, ielibf);

    for (jelem = 1; jelem <= nelem; ++jelem)
    {
      for (jeln = 0; jeln <= 3; ++jeln)
      {
        ibface = -ieliel[jelem][jeln];

        if (ibface > 0)
        {
          ibc = abs (ibcibf[ibface]);

          if (ibc == TRANSP_INTRNL_UG3_GBC)
          {
            ielem = ielibf[ibface];

            if (ielem > 0 && ielem != jelem)
            {
              ieln = (ieliel[ielem][0] == -ibface) ? 0:
                     (ieliel[ielem][1] == -ibface) ? 1:
                     (ieliel[ielem][2] == -ibface) ? 2: 3;

              ieliel[ielem][ieln] = jelem;
              ieliel[jelem][jeln] = ielem;
            }
          }
        }
      }
    }

    ug_free (ielibf);
  }

  if (ieliel != NULL)
  {
    ierr = 0;

    jbfibf = (INT_1D *) ug_malloc (&ierr, (*nbface+1) * sizeof (INT_1D));

    if (ierr > 0)
      return (100112);
  }

  jbface = 0;

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    ibc = abs (ibcibf[ibface]);

    if (ibc != TRANSP_INTRNL_UG3_GBC)
    {
      ++jbface;

      if (ieliel != NULL) jbfibf[ibface] = jbface;

      inibf[jbface][0] = inibf[ibface][0];
      inibf[jbface][1] = inibf[ibface][1];
      inibf[jbface][2] = inibf[ibface][2];

      ibcibf[jbface] = ibcibf[ibface];

      if (idibf != NULL) idibf[jbface] = idibf[ibface];

      if (irfibf != NULL) irfibf[jbface] = irfibf[ibface];
    }
  }

  *nbface = jbface;

  if (ieliel != NULL)
  {
    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      for (ieln = 0; ieln <= 3; ++ieln)
      {
        ibface = -ieliel[ielem][ieln];

        if (ibface > 0)
          ieliel[ielem][ieln] = -jbfibf[ibface];
      }
    }

    ug_free (jbfibf);
  }

  return (0);

}
