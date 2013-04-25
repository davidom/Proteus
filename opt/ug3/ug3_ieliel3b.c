#include "UG3_LIB.h"

INT_ ug3_ieliel3b
 (INT_ nbface,
  INT_ nelem,
  INT_ nnode,
  INT_1D * ibcibf,
  INT_3D * inibf,
  INT_4D * iniel,
  INT_1D * ielibf,
  INT_4D * ieliel)

{

/*
 * Add boundary faces element to element connectivity and determine for each
 * boundary face the element that contains that face.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ieliel3b.c,v 1.3 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *ibfjbf = NULL;
  INT_1D *ibfjn = NULL;
  INT_1D *ierribf = NULL;

  INT_ ierr = 0;
  INT_ minl = 0;
  INT_ mmsg = 0;
  INT_ nbfaceu = 0;

  ibfjbf = (INT_1D *) ug_malloc (&ierr, (nbface+1) * sizeof (INT_1D));
  ibfjn = (INT_1D *) ug_malloc (&ierr, (nnode+1) * sizeof (INT_1D));

  if (ierr > 0)
  {
    ug_free (ibfjbf);
    ug_free (ibfjn);
    return (100312);
  }

  ierr = ug3_ielibf (minl, mmsg, nbface, nelem, nnode, &nbfaceu, 
                     ibcibf, ieliel, inibf, iniel, ibfjbf, ibfjn,
                     ielibf, ierribf);

  ug_free (ibfjbf);
  ug_free (ibfjn);

  if (ierr > 0)
    return (ierr);

  return (0);

}
