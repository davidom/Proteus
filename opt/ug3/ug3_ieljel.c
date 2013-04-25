#include "UG3_LIB.h"

INT_ ug3_ieljel
 (INT_ nelem,
  INT_ *nelpeltd,
  INT_1D ** ieljel_ptr,
  INT_1D * ielin,
  INT_4D * iniel,
  INT_1D * lieljel,
  INT_1D * lielin,
  INT_1D * mchkel)

{

/*
 * Create list of nearby elements for each element.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ieljel.c,v 1.5 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *ieljel;

  INT_ ielem, jelem, jeln, jloc, jloc1, jloc2, jnode, kelem, loc, nrealloc;

  INT_ Error_Flag = 0;

  nrealloc = nelem/4;
  nrealloc = MAX (nrealloc, 100);

  // initialize element check flag

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    mchkel[ielem] = 0;
  }

  ieljel = *ieljel_ptr;

  // loop over elements

  loc = 0;

  for (jelem = 1; jelem <= nelem; ++jelem)
  {
    // set starting location of nearby element map

    lieljel[jelem] = loc+1;

    // loop over nodes of primary element

    for (jeln = 0; jeln <= 3; ++jeln)
    {
      // primary element node

      jnode = iniel[jelem][jeln];

      // loop over all elements attached to primary element node

      jloc1 = lielin[jnode];
      jloc2 = lielin[jnode+1] - 1;

      for (jloc = jloc1; jloc <= jloc2; ++jloc)
      {
        // attached element

        kelem = ielin[jloc];

        if (mchkel[kelem] != jelem)
        {
          // set element check flag

          mchkel[kelem] = jelem;

          // reallocate list of nearby elements

          if (loc == *nelpeltd)
          {
            *nelpeltd = *nelpeltd + nrealloc;

            *ieljel_ptr = (INT_1D *) ug_realloc (&Error_Flag,
                                                 *ieljel_ptr,
                                                 (*nelpeltd+1)
                                               * sizeof (INT_1D));

            ieljel = *ieljel_ptr;

            if (Error_Flag > 0)
            {
              ug_free (ieljel);
              return (100721);
            }
          }

          // set list of search BL nodes

          ++loc;

          ieljel[loc] = kelem;
        }
      }
    }
  }

  lieljel[nelem+1] = loc;

  return (0);

}
