#include "UG3_LIB.h"

void ug3_reorder0bn
 (INT_ *inode0,
  INT_ nnode,
  INT_ nbface,
  INT_3D * inibf,
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
 * Re-orders the boundary nodes so that the boundary nodes with 
 * mflag[inode] == 0 are first in the ordering.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_reorder0bn.c,v 1.2 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, inode, inode1, inode2, inode3, jnode, jnode0, mflagi, mfound,
       nmflag;

  double deli, dfi, dsi, trv1i, trv2i, trv3i, trvmi, trw1i, trw2i, trw3i, trwmi,
         x1i, x2i, x3i;

  mfound = 0;

  inode = 1;

  do
  {
    if (mflag[inode] != 0)
      mfound = 1;

    ++inode;
  }
  while (inode <= nnode && mfound == 0);

  if (mfound == 0)
  {
    *inode0 = nnode + 1;

    return;
  }

  *inode0 = inode - 1;

  nmflag = 0;

  for (inode = *inode0; inode <= nnode; ++inode)
  {
    if (mflag[inode] != 0)
      ++nmflag;

    jnin[inode] = inode;
  }

  jnode0 = nnode - nmflag + 1;

  mfound = 1;

  inode2 = nnode;

  inode = *inode0;

  do
  {
    if (mflag[inode] != 0)
    {
      mfound = 0;

      if (inode2 > inode)
      {
        jnode = inode2;

        do
        {
          if (mflag[jnode] == 0)
            mfound = 1;

          --jnode;
        }
        while (jnode > inode && mfound == 0);

        if (mfound == 1)
        {
          ++jnode;

          jnin[inode] = jnode;
          jnin[jnode] = inode;

          inode2 = jnode - 1;
        }
      }
    }

    ++inode;
  }
  while (inode <= nnode && mfound == 1);

  for (inode = 1; inode < *inode0; ++inode)
  {
    jnin[inode] = inode;
  }

  if (inibf != NULL)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      inibf[ibface][0] = jnin[inode1];
      inibf[ibface][1] = jnin[inode2];
      inibf[ibface][2] = jnin[inode3];
    }
  }

  for (inode = *inode0; inode <= nnode; ++inode)
  {
    jnode = jnin[inode];

    if (jnode > inode)
    {
      if (del != NULL)
      {
        deli = del[inode];

        del[inode] = del[jnode];
        del[jnode] = deli;
      }

      if (df != NULL)
      {
        dfi = df[inode];

        df[inode] = df[jnode];
        df[jnode] = dfi;
      }

      if (ds != NULL)
      {
        dsi = ds[inode];

        ds[inode] = ds[jnode];
        ds[jnode] = dsi;
      }

      if (trv != NULL)
      {
        trv1i = trv[inode][0];
        trv2i = trv[inode][1];
        trv3i = trv[inode][2];
        trvmi = trv[inode][3];

        trv[inode][0] = trv[jnode][0];
        trv[inode][1] = trv[jnode][1];
        trv[inode][2] = trv[jnode][2];
        trv[inode][3] = trv[jnode][3];
        trv[jnode][0] = trv1i;
        trv[jnode][1] = trv2i;
        trv[jnode][2] = trv3i;
        trv[jnode][3] = trvmi;
      }

      if (trw != NULL)
      {
        trw1i = trw[inode][0];
        trw2i = trw[inode][1];
        trw3i = trw[inode][2];
        trwmi = trw[inode][3];

        trw[inode][0] = trw[jnode][0];
        trw[inode][1] = trw[jnode][1];
        trw[inode][2] = trw[jnode][2];
        trw[inode][3] = trw[jnode][3];
        trw[jnode][0] = trw1i;
        trw[jnode][1] = trw2i;
        trw[jnode][2] = trw3i;
        trw[jnode][3] = trwmi;
      }

      if (x != NULL)
      {
        x1i = x[inode][0];
        x2i = x[inode][1];
        x3i = x[inode][2];

        x[inode][0] = x[jnode][0];
        x[inode][1] = x[jnode][1];
        x[inode][2] = x[jnode][2];
        x[jnode][0] = x1i;
        x[jnode][1] = x2i;
        x[jnode][2] = x3i;
      }

      mflagi = mflag[inode];

      mflag[inode] = mflag[jnode];
      mflag[jnode] = mflagi;
    }
  }

  *inode0 = jnode0;

  return;

}
