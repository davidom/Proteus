#include "UG3_LIB.h"

void ug3_reorderb
 (INT_ nbface,
  INT_ nelem,
  INT_ nelemc5,
  INT_ nelemc6,
  INT_ nhex,
  INT_ nnode,
  INT_ *nnodeb,
  INT_ nquad,
  INT_3D * inibf,
  INT_4D * iniel,
  INT_5D * inielc5,
  INT_6D * inielc6,
  INT_8D * inihex,
  INT_4D * iniq,
  INT_1D * jnin,
  INT_1D * mflag,
  DOUBLE_1D * df,
  DOUBLE_4D * trv,
  DOUBLE_4D * trw,
  DOUBLE_3D * x)

{

/*
 * Re-orders nodes so that all boundary nodes are first in the ordering.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_reorderb.c,v 1.15 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  DOUBLE_1D *del=NULL;
  DOUBLE_1D *ds=NULL;

  INT_ ibface, inode, inode0, inode1, inode2, inode3, inode4, iquad, mflagi;

  for (inode = 1; inode <= nnode; ++inode)
  {
    mflag[inode] = 1;
  }

  *nnodeb = 0;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    *nnodeb = MAX (*nnodeb, inode1);
    *nnodeb = MAX (*nnodeb, inode2);
    *nnodeb = MAX (*nnodeb, inode3);

    mflag[inode1] = 0;
    mflag[inode2] = 0;
    mflag[inode3] = 0;
  }

  for (iquad = 1; iquad <= nquad; ++iquad)
  {
    inode1 = iniq[iquad][0];
    inode2 = iniq[iquad][1];
    inode3 = iniq[iquad][2];
    inode4 = iniq[iquad][3];

    *nnodeb = MAX (*nnodeb, inode1);
    *nnodeb = MAX (*nnodeb, inode2);
    *nnodeb = MAX (*nnodeb, inode3);
    *nnodeb = MAX (*nnodeb, inode4);

    mflag[inode1] = 0;
    mflag[inode2] = 0;
    mflag[inode3] = 0;
    mflag[inode4] = 0;
  }

  inode0 = 1;

  do
  {
    mflagi = mflag[inode0];

    ++inode0;
  }
  while (inode0 <= *nnodeb && mflagi == 0);

  --inode0;

  if (mflagi == 1)
  {
    ug3_reorder0n (&inode0, nbface, nelem, nelemc5, nelemc6, nhex, nnode, nquad,
                   inibf, iniel, inielc5, inielc6, inihex, iniq, jnin, mflag,
                   del, df, ds, trv, trw, x);

    *nnodeb = inode0-1;
  }

  return;

}
