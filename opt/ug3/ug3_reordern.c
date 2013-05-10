#include "UG3_LIB.h"

void ug3_reordern
 (INT_ *inode0,
  INT_ ielem0,
  INT_ nbface,
  INT_ nelem,
  INT_ nnode,
  INT_3D * inibf,
  INT_4D * iniel,
  INT_1D * jnin,
  INT_1D * mflag,
  DOUBLE_1D * del,
  DOUBLE_1D * df,
  DOUBLE_1D * ds,
  DOUBLE_4D * trv,
  DOUBLE_4D * trw,
  DOUBLE_3D * x)

{

/*
 * Re-orders the nodes so that the nodes of elements less than ielem0 are first
 * in the ordering.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_reordern.c,v 1.10 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */
 
  INT_5D *inielc5 = NULL;
  INT_6D *inielc6 = NULL;
  INT_8D *inihex = NULL;
  INT_4D *iniq = NULL;

  INT_ ielem, inode, inode1, inode2, inode3, inode4;
  INT_ nelemc5 = 0;
  INT_ nelemc6 = 0;
  INT_ nhex = 0;
  INT_ nquad = 0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    mflag[inode] = 1;
  }

  for (ielem = 1; ielem < ielem0; ++ielem)
  {
    inode1 = iniel[ielem][0];
    inode2 = iniel[ielem][1];
    inode3 = iniel[ielem][2];
    inode4 = iniel[ielem][3];

    mflag[inode1] = 0;
    mflag[inode2] = 0;
    mflag[inode3] = 0;
    mflag[inode4] = 0;
  }

  ug3_reorder0n (inode0, nbface, nelem, nelemc5, nelemc6, nhex, nnode, nquad,
                 inibf, iniel, inielc5, inielc6, inihex, iniq, jnin, mflag,
                 del, df, ds, trv, trw, x);

  return;

}
