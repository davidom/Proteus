#include "UG3_LIB.h"

INT_ ug3_idiel
 (INT_ nbface,
  INT_ nelem,
  INT_ nnode,
  INT_ *nv,
  INT_1D * idiel,
  INT_3D * inibf,
  INT_4D * iniel)

{

/*
 * Set volume ID flag.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_idiel.c,v 1.9 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *ibcibf = NULL;
  INT_1D *ielis = NULL;
  INT_4D *ieliel = NULL;

  INT_ idmax, idmin, ierr;

  idmax = ug_max_int (1, nelem, idiel);
  idmin = ug_min_int (1, nelem, idiel);

  *nv = 0;

  if (idmin != idmax)
    return (0);

  ierr = 0;

  ieliel = (INT_4D *) ug_malloc (&ierr, (nelem+1) * sizeof (INT_4D));
  ielis = (INT_1D *) ug_malloc (&ierr, (nelem+1) * sizeof (INT_1D));

  if (ierr > 0)
  {
    ug_free (ieliel);
    ug_free (ielis);
    return (100311);
  }

  ierr = ug3_ieliel2 (nelem, nnode, iniel, ieliel);

  if (ierr > 0)
  {
    ug_free (ieliel);
    ug_free (ielis);
    return (ierr);
  }

  ierr = ug3_ieliel2b (nbface, nelem, nnode, ibcibf, inibf, iniel, ieliel);

  if (ierr > 0)
  {
    ug_free (ieliel);
    ug_free (ielis);
    return (ierr);
  }

  *nv = 0;

  ug3_iviel (nelem, nv, ieliel, ielis, idiel);

  ug_free (ieliel);
  ug_free (ielis);

  return (ierr);

}
