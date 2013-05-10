#include "UG3_LIB.h"

void ug3_rmtbfd
 (INT_ *nbface,
  INT_ nelem,
  INT_ nelemc5,
  INT_ nelemc6,
  INT_ nelemc8,
  INT_ *nnode,
  INT_ *nnodeb,
  INT_ nquad,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_4D * iniel,
  INT_5D * inielc5,
  INT_6D * inielc6,
  INT_8D * inielc8,
  INT_4D * iniq,
  INT_1D * iqibf,
  INT_1D * irfibf,
  INT_1D * jnin,
  DOUBLE_1D * del,
  DOUBLE_1D * df,
  DOUBLE_1D * ds,
  DOUBLE_3D * x)

{

/*
 * Remove duplicate faces and nodes from transparent boundary surface faces.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_rmtbfd.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibc, ibc1, ibc2, ibc3, ibface, ibface1, ibface2, ibface3, ielem, inode,
       inode1, inode2, inode3, inode4, inode5, inode6, inode7, inode8, iquad,
       jbface, jnode, ntbface, ntbnode;

  ibface = 1;

  do
  {
    ibc = ibcibf[ibface];

    ++ibface;
  }
  while (ibface < *nbface && ! CHK_TMP_UG3_GBC (ibc));

  if (! CHK_TMP_UG3_GBC (ibc))
    return;

  for (inode = 1; inode <= *nnode; ++inode)
  {
    jnin[inode] = 0;
  }

  ntbface = 0;

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    ibc = ibcibf[ibface];

    if (CHK_TMP_UG3_GBC (ibc))
    {
      ++ntbface;

      ibface1 = ibfibf[ibface][0];
      ibface2 = ibfibf[ibface][1];
      ibface3 = ibfibf[ibface][2];

      ibc1 = ibcibf[ibface1];
      ibc2 = ibcibf[ibface2];
      ibc3 = ibcibf[ibface3];

      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      if (CHK_TRANSP_UG3_GBC (ibc1))
      {
        if (CHK_TMP_UG3_GBC (ibc1))
        {
          if (jnin[inode2] == 0) jnin[inode2] = 1;
          if (jnin[inode3] == 0) jnin[inode3] = 1;
        }
        else
        {
          jnin[inode2] = -1;
          jnin[inode3] = -1;
        }
      }

      if (CHK_TRANSP_UG3_GBC (ibc2))
      {
        if (CHK_TMP_UG3_GBC (ibc2))
        {
          if (jnin[inode1] == 0) jnin[inode1] = 1;
          if (jnin[inode3] == 0) jnin[inode3] = 1;
        }
        else
        {
          jnin[inode1] = -1;
          jnin[inode3] = -1;
        }
      }

      if (CHK_TRANSP_UG3_GBC (ibc3))
      {
        if (CHK_TMP_UG3_GBC (ibc3))
        {
          if (jnin[inode1] == 0) jnin[inode1] = 1;
          if (jnin[inode2] == 0) jnin[inode2] = 1;
        }
        else
        {
          jnin[inode1] = -1;
          jnin[inode2] = -1;
        }
      }
    }
  }

  ntbnode = 0;

  for (inode = 1; inode <= *nnodeb; ++inode)
  {
    if (jnin[inode] == 1)
    {
      ++ntbnode;

      jnin[inode] = -1;
    }
    else
      jnin[inode] = 0;
  }

  jnode = 0;

  for (inode = 1; inode <= *nnode; ++inode)
  {
    if (jnin[inode] == 0)
    {
      ++jnode;

      jnin[inode] = jnode;

      if (del != NULL) del[jnode] = del[inode];
      if (df != NULL) df[jnode] = df[inode];
      if (ds != NULL) ds[jnode] = ds[inode];

      x[jnode][0] = x[inode][0];
      x[jnode][1] = x[inode][1];
      x[jnode][2] = x[inode][2];
    }
  }

  *nnodeb = *nnodeb - ntbnode;

  *nnode = *nnode - ntbnode;

  jbface = 0;

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    ibc = ibcibf[ibface];

    if (! CHK_TMP_UG3_GBC (ibc))
    {
      ++jbface;

      ibcibf[jbface] = ibcibf[ibface];
      idibf[jbface] = idibf[ibface];
      irfibf[jbface] = irfibf[ibface];

      if (iqibf != NULL) iqibf[jbface] = iqibf[ibface];

      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      inibf[jbface][0] = jnin[inode1];
      inibf[jbface][1] = jnin[inode2];
      inibf[jbface][2] = jnin[inode3];
    }
  }

  *nbface = *nbface - ntbface;

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

  for (ielem = 1; ielem <= nelemc5; ++ielem)
  {
    inode1 = inielc5[ielem][0];
    inode2 = inielc5[ielem][1];
    inode3 = inielc5[ielem][2];
    inode4 = inielc5[ielem][3];
    inode5 = inielc5[ielem][4];

    inielc5[ielem][0] = jnin[inode1];
    inielc5[ielem][1] = jnin[inode2];
    inielc5[ielem][2] = jnin[inode3];
    inielc5[ielem][3] = jnin[inode4];
    inielc5[ielem][4] = jnin[inode5];
  }

  for (ielem = 1; ielem <= nelemc6; ++ielem)
  {
    inode1 = inielc6[ielem][0];
    inode2 = inielc6[ielem][1];
    inode3 = inielc6[ielem][2];
    inode4 = inielc6[ielem][3];
    inode5 = inielc6[ielem][4];
    inode6 = inielc6[ielem][5];

    inielc6[ielem][0] = jnin[inode1];
    inielc6[ielem][1] = jnin[inode2];
    inielc6[ielem][2] = jnin[inode3];
    inielc6[ielem][3] = jnin[inode4];
    inielc6[ielem][4] = jnin[inode5];
    inielc6[ielem][5] = jnin[inode6];
  }

  for (ielem = 1; ielem <= nelemc8; ++ielem)
  {
    inode1 = inielc8[ielem][0];
    inode2 = inielc8[ielem][1];
    inode3 = inielc8[ielem][2];
    inode4 = inielc8[ielem][3];
    inode5 = inielc8[ielem][4];
    inode6 = inielc8[ielem][5];
    inode7 = inielc8[ielem][6];
    inode8 = inielc8[ielem][7];

    inielc8[ielem][0] = jnin[inode1];
    inielc8[ielem][1] = jnin[inode2];
    inielc8[ielem][2] = jnin[inode3];
    inielc8[ielem][3] = jnin[inode4];
    inielc8[ielem][4] = jnin[inode5];
    inielc8[ielem][5] = jnin[inode6];
    inielc8[ielem][6] = jnin[inode7];
    inielc8[ielem][7] = jnin[inode8];
  }

  return;

}
