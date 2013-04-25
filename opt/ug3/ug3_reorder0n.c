#include "UG3_LIB.h"

void ug3_reorder0n
 (INT_ *inode0,
  INT_ nbface,
  INT_ nelem,
  INT_ nelemc5,
  INT_ nelemc6,
  INT_ nhex,
  INT_ nnode,
  INT_ nquad,
  INT_3D * inibf,
  INT_4D * iniel,
  INT_5D * inielc5,
  INT_6D * inielc6,
  INT_8D * inihex,
  INT_4D * iniq,
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
 * Re-orders the nodes so that the nodes with mflag[inode] == 0 are first
 * in the ordering.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_reorder0n.c,v 1.10 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, ielem, ielemc, ihex, inode, inode1, inode2, inode3, inode4,
       inode5, inode6, inode7, inode8, iquad, jnode, jnode0, mflagi, mfound,
       nmflag;

  double deli, dfi, dsi, trv1, trv2, trv3, trvm, trw1, trw2, trw3, trwm, x1, x2,
         x3;

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

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    inibf[ibface][0] = jnin[inode1];
    inibf[ibface][1] = jnin[inode2];
    inibf[ibface][2] = jnin[inode3];
  }

  for (iquad = 1; iquad <= nquad; ++iquad)
  {
    inode1 = iniq[iquad][0];
    inode2 = iniq[iquad][1];
    inode3 = iniq[iquad][2];
    inode4 = iniq[iquad][3];

    iniq[iquad][0] = jnin[inode1];
    iniq[iquad][1] = jnin[inode2];
    iniq[iquad][2] = jnin[inode3];
    iniq[iquad][3] = jnin[inode4];
  }

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    inode1 = iniel[ielem][0];
    inode2 = iniel[ielem][1];
    inode3 = iniel[ielem][2];
    inode4 = iniel[ielem][3];

    iniel[ielem][0] = jnin[inode1];
    iniel[ielem][1] = jnin[inode2];
    iniel[ielem][2] = jnin[inode3];
    iniel[ielem][3] = jnin[inode4];
  }

  for (ielemc = 1; ielemc <= nelemc5; ++ielemc)
  {
    inode1 = inielc5[ielemc][0];
    inode2 = inielc5[ielemc][1];
    inode3 = inielc5[ielemc][2];
    inode4 = inielc5[ielemc][3];
    inode5 = inielc5[ielemc][4];

    inielc5[ielemc][0] = jnin[inode1];
    inielc5[ielemc][1] = jnin[inode2];
    inielc5[ielemc][2] = jnin[inode3];
    inielc5[ielemc][3] = jnin[inode4];
    inielc5[ielemc][4] = jnin[inode5];
  }

  for (ielemc = 1; ielemc <= nelemc6; ++ielemc)
  {
    inode1 = inielc6[ielemc][0];
    inode2 = inielc6[ielemc][1];
    inode3 = inielc6[ielemc][2];
    inode4 = inielc6[ielemc][3];
    inode5 = inielc6[ielemc][4];
    inode6 = inielc6[ielemc][5];

    inielc6[ielemc][0] = jnin[inode1];
    inielc6[ielemc][1] = jnin[inode2];
    inielc6[ielemc][2] = jnin[inode3];
    inielc6[ielemc][3] = jnin[inode4];
    inielc6[ielemc][4] = jnin[inode5];
    inielc6[ielemc][5] = jnin[inode6];
  }

  for (ihex = 1; ihex <= nhex; ++ihex)
  {
    inode1 = inihex[ihex][0];
    inode2 = inihex[ihex][1];
    inode3 = inihex[ihex][2];
    inode4 = inihex[ihex][3];
    inode5 = inihex[ihex][4];
    inode6 = inihex[ihex][5];
    inode7 = inihex[ihex][6];
    inode8 = inihex[ihex][7];

    inihex[ihex][0] = jnin[inode1];
    inihex[ihex][1] = jnin[inode2];
    inihex[ihex][2] = jnin[inode3];
    inihex[ihex][3] = jnin[inode4];
    inihex[ihex][4] = jnin[inode5];
    inihex[ihex][5] = jnin[inode6];
    inihex[ihex][6] = jnin[inode7];
    inihex[ihex][7] = jnin[inode8];
  }

  for (inode = *inode0; inode <= nnode; ++inode)
  {
    jnode = jnin[inode];

    if (jnode > inode)
    {
      x1 = x[inode][0];
      x2 = x[inode][1];
      x3 = x[inode][2];

      x[inode][0] = x[jnode][0];
      x[inode][1] = x[jnode][1];
      x[inode][2] = x[jnode][2];

      x[jnode][0] = x1;
      x[jnode][1] = x2;
      x[jnode][2] = x3;

      mflagi = mflag[inode];

      mflag[inode] = mflag[jnode];
      mflag[jnode] = mflagi;
    }
  }

  if (del != NULL)
  {
    for (inode = *inode0; inode <= nnode; ++inode)
    {
      jnode = jnin[inode];

      if (jnode > inode)
      {
        deli = del[inode];

        del[inode] = del[jnode];

        del[jnode] = deli;
      }
    }
  }

  if (df != NULL)
  {
    for (inode = *inode0; inode <= nnode; ++inode)
    {
      jnode = jnin[inode];

      if (jnode > inode)
      {
        dfi = df[inode];

        df[inode] = df[jnode];

        df[jnode] = dfi;
      }
    }
  }

  if (ds != NULL)
  {
    for (inode = *inode0; inode <= nnode; ++inode)
    {
      jnode = jnin[inode];

      if (jnode > inode)
      {
        dsi = ds[inode];

        ds[inode] = ds[jnode];

        ds[jnode] = dsi;
      }
    }
  }

  if (trv != NULL)
  {
    for (inode = *inode0; inode <= nnode; ++inode)
    {
      jnode = jnin[inode];

      if (jnode > inode)
      {
        trv1 = trv[inode][0];
        trv2 = trv[inode][1];
        trv3 = trv[inode][2];
        trvm = trv[inode][3];

        trv[inode][0] = trv[jnode][0];
        trv[inode][1] = trv[jnode][1];
        trv[inode][2] = trv[jnode][2];
        trv[inode][3] = trv[jnode][3];

        trv[jnode][0] = trv1;
        trv[jnode][1] = trv2;
        trv[jnode][2] = trv3;
        trv[jnode][3] = trvm;
      }
    }
  }

  if (trw != NULL)
  {
    for (inode = *inode0; inode <= nnode; ++inode)
    {
      jnode = jnin[inode];

      if (jnode > inode)
      {
        trw1 = trw[inode][0];
        trw2 = trw[inode][1];
        trw3 = trw[inode][2];
        trwm = trw[inode][3];

        trw[inode][0] = trw[jnode][0];
        trw[inode][1] = trw[jnode][1];
        trw[inode][2] = trw[jnode][2];
        trw[inode][3] = trw[jnode][3];

        trw[jnode][0] = trw1;
        trw[jnode][1] = trw2;
        trw[jnode][2] = trw3;
        trw[jnode][3] = trwm;
      }
    }
  }

  *inode0 = jnode0;

  return;

}
