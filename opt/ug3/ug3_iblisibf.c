#include "UG3_LIB.h"

void ug3_iblisibf
 (INT_ mtask,
  INT_ nbface,
  INT_ *nblis,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * idibf,
  INT_1D * ibfichk,
  INT_1D * iblisibf)

{

/*
 * Flag boundary faces from rebuild (task 1) or fixed (task 2) surfaces that
 * intersect the BL region.  
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_iblisibf.c,v 1.6 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, ibfn, iblis, ichk, ichk1, ichk2, id, iit, jbface, jchk, mfound;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    iblisibf[ibface] = 0;
  }

  // start of loop over surfaces that intersect the BL region

  iblis = 0;

  do
  {
    // start of loop over boundary faces

    mfound = 0;

    ibface = 0;

    do
    {
      ++ibface;

      // if boundary face has not been checked and it is on a surface that
      // intersects the BL region then exit loop

      if (iblisibf[ibface] == 0 && 
          ((mtask == 1 && CHK_BL_INT_UG3_GBC (ibcibf[ibface])) ||
           (mtask == 2 && CHK_FIXED_BL_INT_UG3_GBC (ibcibf[ibface]))))
      {
        mfound = 1;

        id = idibf[ibface];
      }
    }
    while (ibface < nbface && mfound == 0);

    // set index (for surface that intersects the BL region) at all connected
    // boundary faces of the same surface ID

    if (mfound)
    {
      ++iblis;

      iblisibf[ibface] = iblis;

      // initialize list of faces to be checked

      ichk = 1;

      ibfichk[ichk] = ibface;

      // start of tree search loop

      ichk2 = 0;

      iit = 1;

      do
      {
        // loop over unchecked boundary faces that were added during last tree
        // search iteration

        ichk1 = ichk2+1;
        ichk2 = ichk;

        for (jchk = ichk1; jchk <= ichk2; ++jchk)
        {
          // boundary face to check

          jbface = ibfichk[jchk];

          // loop over boundary face neighbors

          for (ibfn = 0; ibfn <= 2; ++ibfn)
          {
            ibface = ibfibf[jbface][ibfn];

            // set neigbor face index if it exists, has not been set, has the
            // same surface ID, and is on a surface that intersects the BL
            // region

            if (ibface > 0)
            {
              if (iblisibf[ibface] == 0 && idibf[ibface] == id &&
                  ((mtask == 1 && CHK_BL_INT_UG3_GBC (ibcibf[ibface])) ||
                   (mtask == 2 && CHK_FIXED_BL_INT_UG3_GBC (ibcibf[ibface]))))
              {
                iblisibf[ibface] = iblis;

                ++ichk;

                ibfichk[ichk] = ibface;
              }
            }
          }
        }

        // end of tree search loop

        ++iit;
      }
      while (iit <= nbface && ichk > ichk2);
    }

    // end of loop over surfaces that intersect the BL region

  }
  while (mfound);

  *nblis = iblis;

  if (mtask == 1)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      if (CHK_BL_UG3_GBC (ibcibf[ibface]))
        iblisibf[ibface] = -1;
    }
  }

  return;

}
