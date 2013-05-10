#include "UG3_LIB.h"

INT_ ug3_ieliel2
 (INT_ nelem,
  INT_ nnode,
  INT_4D * iniel,
  INT_4D * ieliel)

{

/*
 * Determine the element to element connectivity from element node connectivity.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ieliel2.c,v 1.10 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *ielin = NULL;
  INT_1D *lielin = NULL;
  INT_1D *madd = NULL;

  INT_ ielem0 = 1;
  INT_ ierr = 0;
  INT_ nelpnt = 0;
  INT_ nelpntd = 0;

  ielin = (INT_1D *) ug_malloc (&ierr, (nelpntd+1) * sizeof (INT_1D));
  lielin = (INT_1D *) ug_malloc (&ierr, (nnode+2) * sizeof (INT_1D));

  if (ierr > 0)
  {
    ug_free (ielin);
    ug_free (lielin);
    return (100304);
  }

  ierr = ug3_ielin (ielem0, nelem, &nelpntd, nnode, &nelpnt,
                    iniel, madd, &ielin, lielin);

  if (ierr > 0)
  {
    ug_free (ielin);
    ug_free (lielin);
    return (ierr);
  }

  ierr = ug3_ieliel (nelem, ielin, iniel, lielin, ieliel);

  ug_free (ielin);
  ug_free (lielin);

  if (ierr > 0)
    return (ierr);

  return (0);

}
