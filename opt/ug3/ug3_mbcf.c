#include "UG3_LIB.h"

void ug3_mbcf
 (INT_ nbface,
  INT_ nnode,
  INT_ *mmbcf,
  INT_1D * ibcibf,
  INT_3D * inibf,
  INT_1D * mbcf)

{

/*
 * Set grid boundary condition flag at frozen partition boundary surface face
 * nodes.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_mbcf.c,v 1.9 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibcf, ibface, inode, inode1, inode2, inode3;

  ibcf = 0;

  if (mbcf != NULL)
  {
    for (inode = 1; inode <= nnode; ++inode)
    {
      mbcf[inode] = 0;
    }

    if (*mmbcf == 2)
    {
      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        if (CHK_PART_UG3_GBC (ibcibf[ibface]))
        {
          ++ibcf;

          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          mbcf[inode1] = 1;
          mbcf[inode2] = 1;
          mbcf[inode3] = 1;
        }
      }
    }
    else
    {
      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        if (ibcibf[ibface] == PART_UG3_GBC)
        {
          ++ibcf;

          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          mbcf[inode1] = 1;
          mbcf[inode2] = 1;
          mbcf[inode3] = 1;
        }
      }
    }
  }
  else
  {
    if (*mmbcf == 2)
    {
      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        if (CHK_PART_UG3_GBC (ibcibf[ibface]))
          ++ibcf;
      }
    }
    else
    {
      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        if (ibcibf[ibface] == PART_UG3_GBC)
          ++ibcf;
      }
    }
  }

  if (ibcf == nbface)
    *mmbcf = 2;
  else if (ibcf > 0)
    *mmbcf = 1;
  else
    *mmbcf = 0;

  return;

}
