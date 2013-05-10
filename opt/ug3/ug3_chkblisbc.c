#include "UG3_LIB.h"

void ug3_chkblisbc
 (INT_ mtask,
  INT_ nbface,
  INT_ *nblis,
  INT_ nnode,
  INT_1D * ibcibf,
  INT_1D * ibcin,
  INT_1D * iblisibf,
  INT_3D * inibf,
  INT_1D * irfibf)

{

/*
 * Check if boundary faces with a grid BC for a surface that intersect the BL
 * region are also adjacent to a surface that generates a BL region. For task 1
 * check boundary faces from rebuild surfaces or for task 2 check boundary faces
 * from fixed surfaces.
 *  
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkblisbc.c,v 1.6 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, iblis, inode, inode1, inode2, inode3, mfound;

  if (*nblis == 0)
    return;

  // set reconnection flag for fixed surfaces

  if (mtask == 2)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      if (CHK_FIXED_BL_INT_UG3_GBC (ibcibf[ibface]))
        irfibf[ibface] = 7;
    }
  }

  // set node BC flag for nodes on a surface that generates a BL region

  for (inode = 1; inode <= nnode; ++inode)
  {
    ibcin[inode] = 0;
  }

  mfound = 0;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    if (CHK_BL_UG3_GBC (ibcibf[ibface]))
    {
      mfound = 1;

      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      ibcin[inode1] = 1;
      ibcin[inode2] = 1;
      ibcin[inode3] = 1;
    }
  }

  if (mfound == 0)
    return;

  // start of loop over surfaces that intersect the BL region

  for (iblis = 1; iblis <= *nblis; ++iblis)
  {
    mfound = 0;

    ibface = 0;

    // start of loop over boundary faces

    do
    {
      ++ibface;

      // if the boundary face is on a surface that intersects the BL region and
      // is adjacent to a surface that generates a BL region then exit loop

      if (iblisibf[ibface] == iblis)
      {
        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        if (ibcin[inode1] == 1 || ibcin[inode2] == 1 || ibcin[inode3] == 1)
          mfound = 1;
      }
    }
    while (ibface < nbface && mfound == 0);

    // if the current surface that intersects the BL region is not adjacent to
    // a surface with a grid BC for generating a BL region then set the grid BC
    // for the current surface to that of a standard external or internal
    // surface

    if (mfound == 0)
    {
      --(*nblis);

      if (mtask == 1)
      {
        for (ibface = 1; ibface <= nbface; ++ibface)
        {
          if (iblisibf[ibface] == iblis)
          {
            if (ibcibf[ibface] == BL_INT_UG3_GBC)
              ibcibf[ibface] = STD_UG3_GBC;
            else
              ibcibf[ibface] = TRANSP_UG3_GBC;
          }
        }
      }

      else //if (mtask == 2)
      {
        for (ibface = 1; ibface <= nbface; ++ibface)
        {
          if (iblisibf[ibface] == iblis)
            ibcibf[ibface] = STD_UG3_GBC;
        }
      }
    }

    // end of loop over surfaces that intersect the BL region

  }

  return;

}
