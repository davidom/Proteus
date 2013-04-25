#include "UG3_LIB.h"

void ug3_idsmth
 (INT_ midsmth,
  INT_ nbface,
  INT_ nidsmthl,
  INT_ nnodeb,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * ibfin,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_1D * ilin,
  INT_1D * libfin,
  DOUBLE_1D * del,
  DOUBLE_1D * ds)

{

/*
 * Distribute normal spacing and BL thickness on surface faces set by individual
 * surface face ID.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_idsmth.c,v 1.3 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, ibface1, ibface2, ibface3, iidsmth, id, idmax, idmin, il, ifound,
       ilmax, ilmin, ilp1, inode, inode1, inode2, inode3, isum, loc, loc1, loc2,
       mid, mset, nidsmth;

  double cidsmth, cidsmthm, dc0, dc1, delsum, dssum, w;

  dc0 = 0.0;
  dc1 = 1.0;

  cidsmth = 0.5;

  if (midsmth == 0 || cidsmth == dc0) return;

  cidsmthm = dc1 - cidsmth;

  nidsmth = 2 * nidsmthl;

  idmax = idibf[1];
  idmin = idibf[1];

  for (ibface = 2; ibface <= nbface; ++ibface)
  {     
    id = idibf[ibface];

    idmax = MAX (id, idmax);
    idmin = MIN (id, idmin);
  }

  ifound = 0;

  for (id = idmin; id <= idmax; ++id)
  {
    mid = 0;

    ibface = 1;

    do
    {
      if (idibf[ibface] == id && CHK_BL_UG3_GBC (ibcibf[ibface])) mid = 1;

      ++ibface;
    }
    while (ibface <= nbface && mid == 0);

    if (mid == 1) ++ifound;
  }

  if (ifound > 1)
  {
    for (inode = 1; inode <= nnodeb; ++inode)
    {
      ilin[inode] = 0;
    }

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      if (CHK_BL_UG3_GBC (ibcibf[ibface]))
      {
        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        ilin[inode1] = -1;
        ilin[inode2] = -1;
        ilin[inode3] = -1;
      }
    }

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      if (CHK_BL_UG3_GBC (ibcibf[ibface]))
      {
        id = idibf[ibface];

        ibface1 = ibfibf[ibface][0];
        ibface2 = ibfibf[ibface][1];
        ibface3 = ibfibf[ibface][2];

        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        if (id != idibf[ibface1] && CHK_BL_UG3_GBC (ibcibf[ibface1]))
        {
          ilin[inode2] = 1;
          ilin[inode3] = 1;
        }

        if (id != idibf[ibface2] && CHK_BL_UG3_GBC (ibcibf[ibface2]))
        {
          ilin[inode1] = 1;
          ilin[inode3] = 1;
        }

        if (id != idibf[ibface3] && CHK_BL_UG3_GBC (ibcibf[ibface3]))
        {
          ilin[inode1] = 1;
          ilin[inode2] = 1;
        }
      }
    }

    il = 1;

    do
    {
      ilp1 = il+1;

      mset = 0;

      for (inode = 1; inode <= nnodeb; ++inode)
      {
        if (ilin[inode] == il)
        {
          loc1 = libfin[inode];
          loc2 = libfin[inode+1] - 1;

          for (loc = loc1; loc <= loc2; ++loc)
          {
            ibface = ibfin[loc];

            inode1 = inibf[ibface][0];
            inode2 = inibf[ibface][1];
            inode3 = inibf[ibface][2];

            if (ilin[inode1] == -1)
            {
              mset = 1;

              ilin[inode1] = ilp1;
            }

            if (ilin[inode2] == -1)
            {
              mset = 1;

              ilin[inode2] = ilp1;
            }

            if (ilin[inode3] == -1)
            {
              mset = 1;

              ilin[inode3] = ilp1;
            }
          }
        }
      }

      ++il;
    }
    while (il <= nnodeb && mset == 1);

    --il;

    for (id = idmin; id <= idmax; ++id)
    {
      ilmax = 0;

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        if (idibf[ibface] == id && CHK_BL_UG3_GBC (ibcibf[ibface]))
        {
          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          ilmax = MAX (ilmax, ilin[inode1]);
          ilmax = MAX (ilmax, ilin[inode2]);
          ilmax = MAX (ilmax, ilin[inode3]);
        }
      }

      if (ilmax > 0)
      {
        ilmin = ilmax-2;
        ilmin = MIN (ilmin, nidsmthl);

        for (ibface = 1; ibface <= nbface; ++ibface)
        {
          if (idibf[ibface] == id && CHK_BL_UG3_GBC (ibcibf[ibface]))
          {
            inode1 = inibf[ibface][0];
            inode2 = inibf[ibface][1];
            inode3 = inibf[ibface][2];

            if (ilin[inode1] > ilmin) ilin[inode1] = -1;
            if (ilin[inode2] > ilmin) ilin[inode2] = -1;
            if (ilin[inode3] > ilmin) ilin[inode3] = -1;
          }
        }
      }
    }

    for (iidsmth = 1; iidsmth <= nidsmth; ++iidsmth)
    {
      for (inode = 1; inode <= nnodeb; ++inode)
      {
        if (ilin[inode] > 0)
        {
          isum = 0;

          delsum = dc0;

          dssum = dc0;

          loc1 = libfin[inode];
          loc2 = libfin[inode+1] - 1;

          for (loc = loc1; loc <= loc2; ++loc)
          {
            ibface = ibfin[loc];

            inode1 = inibf[ibface][0];
            inode2 = inibf[ibface][1];
            inode3 = inibf[ibface][2];

            if (ilin[inode1] != 0)
            {
              ++isum;

              delsum = delsum + del[inode1];

              dssum = dssum + ds[inode1];
            }

            if (ilin[inode2] != 0)
            {
              ++isum;

              delsum = delsum + del[inode2];

              dssum = dssum + ds[inode2];
            }

            if (ilin[inode3] != 0)
            {
              ++isum;

              delsum = delsum + del[inode3];

              dssum = dssum + ds[inode3];
            }
          }

          w = cidsmth / ((double) isum);

          del[inode] = cidsmthm * del[inode] + w * delsum;

          ds[inode] = cidsmthm * ds[inode] + w * dssum;
        }
      }
    }
  }

  return;
}
