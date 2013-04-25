#include "UG3_LIB.h"

void ug3_extract_surf
 (INT_ mextract_i,
  INT_ mtask,
  INT_ nbface,
  INT_ nnode,
  INT_ *nbfacej,
  INT_ *nnodej,
  INT_1D * ibcibf,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_1D * iqibf,
  INT_1D * irfibf,
  INT_1D * jnin,
  INT_1D * jbcjbf,
  INT_1D * jdjbf,
  INT_3D * jnjbf,
  INT_1D * jqjbf,
  INT_1D * jrfjbf,
  INT_1D * mextract,
  DOUBLE_1D * del,
  DOUBLE_1D * ds,
  DOUBLE_2D * u,
  DOUBLE_3D * x,
  DOUBLE_1D * delj,
  DOUBLE_1D * dsj,
  DOUBLE_2D * uj,
  DOUBLE_3D * xj)

{

/*
 * Extract a partial surface grid corresponding to a given surface flag.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_extract_surf.c,v 1.3 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, inode, inode1, inode2, inode3, jbface, jnode;

  for (inode = 1; inode <= nnode; ++inode)
  {
    jnin[inode] = 0;
  }

  jbface = 0;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    if (mextract[ibface] == mextract_i)
    {
      ++jbface;

      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      jnin[inode1] = 1;
      jnin[inode2] = 1;
      jnin[inode3] = 1;

      if (mtask)
      {
        jnjbf[jbface][0] = inode1;
        jnjbf[jbface][1] = inode2;
        jnjbf[jbface][2] = inode3;

        if (jbcjbf && ibcibf)
          jbcjbf[jbface] = ibcibf[ibface];

        if (jdjbf && idibf)
          jdjbf[jbface] = idibf[ibface];

        if (jqjbf && iqibf)
          jqjbf[jbface] = iqibf[ibface];

        if (jrfjbf && irfibf)
          jrfjbf[jbface] = irfibf[ibface];
      }
    }
  }

  *nbfacej = jbface;

  jnode = 0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    if (jnin[inode])
    {
      ++jnode;

      if (mtask)
      {
        jnin[inode] = jnode;

        xj[jnode][0] = x[inode][0];
        xj[jnode][1] = x[inode][1];
        xj[jnode][2] = x[inode][2];

        if (uj && u)
        {
          uj[jnode][0] = u[inode][0];
          uj[jnode][1] = u[inode][1];
        }

        if (delj && del)
          delj[jnode] = del[inode];

        if (dsj && ds)
          dsj[jnode] = ds[inode];
      }
    }
  }

  *nnodej = jnode;

  if (mtask)
  {
    for (jbface = 1; jbface <= *nbfacej; ++jbface)
    {
      inode1 = jnjbf[jbface][0];
      inode2 = jnjbf[jbface][1];
      inode3 = jnjbf[jbface][2];

      jnjbf[jbface][0] = jnin[inode1];
      jnjbf[jbface][1] = jnin[inode2];
      jnjbf[jbface][2] = jnin[inode3];
    }
  }

  return;

}
