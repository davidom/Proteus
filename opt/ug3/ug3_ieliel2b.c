#include "UG3_LIB.h"

INT_ ug3_ieliel2b
 (INT_ nbface,
  INT_ nelem,
  INT_ nnode,
  INT_1D * ibcibf,
  INT_3D * inibf,
  INT_4D * iniel,
  INT_4D * ieliel)

{

/*
 * Add boundary faces element to element connectivity.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ieliel2b.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *ibfjbf = NULL;
  INT_1D *ibfjn = NULL;
  INT_1D *ielibf = NULL;
  INT_1D *ierribf = NULL;

  INT_ ierr = 0;
  INT_ minl = 0;
  INT_ mmsg = 0;
  INT_ nbfaceu = 0;

  if (ibcibf == NULL) minl = 2;

  ibfjbf = (INT_1D *) ug_malloc (&ierr, (nbface+1) * sizeof (INT_1D));
  ibfjn = (INT_1D *) ug_malloc (&ierr, (nnode+1) * sizeof (INT_1D));
  ielibf = (INT_1D *) ug_malloc (&ierr, (nbface+1) * sizeof (INT_1D));

  if (ierr > 0)
  {
    ug_free (ibfjbf);
    ug_free (ibfjn);
    ug_free (ielibf);
    return (100309);
  }

  ierr = ug3_ielibf (minl, mmsg, nbface, nelem, nnode, &nbfaceu, 
                     ibcibf, ieliel, inibf, iniel, ibfjbf, ibfjn,
                     ielibf, ierribf);

  ug_free (ibfjbf);
  ug_free (ibfjn);
  ug_free (ielibf);

  if (ierr > 0)
    return (ierr);

  return (0);

}
