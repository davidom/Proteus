#include "UG3_LIB.h"

void ug3_reorder0bf
 (INT_ *ibface0,
  INT_ nbface,
  INT_ nelem,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * idibf,
  INT_4D * ieliel,
  INT_3D * inibf,
  INT_1D * iqibf,
  INT_1D * irfibf,
  INT_1D * jbfibf,
  INT_1D * mflag)

{

/*
 * Re-orders the boundary faces so that the boundary faces with 
 * mflag[ibface] == 0 are first in the ordering.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_reorder0bf.c,v 1.7 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibc, ibface, ibface1, ibface2, ibface3, ibface4, id, ielem, inode1,
       inode2, inode3, iq, irf, jbface, jbface0, mflagi, mfound, nmflag;

  mfound = 0;

  ibface = 1;

  do
  {
    if (mflag[ibface] != 0)
      mfound = 1;

    ++ibface;
  }
  while (ibface <= nbface && mfound == 0);

  if (mfound == 0)
  {
    *ibface0 = nbface + 1;

    return;
  }

  *ibface0 = ibface - 1;

  nmflag = 0;

  for (ibface = *ibface0; ibface <= nbface; ++ibface)
  {
    if (mflag[ibface] != 0)
      ++nmflag;

    jbfibf[ibface] = ibface;
  }

  jbface0 = nbface - nmflag + 1;

  mfound = 1;

  ibface2 = nbface;

  ibface = *ibface0;

  do
  {
    if (mflag[ibface] != 0)
    {
      mfound = 0;

      if (ibface2 > ibface)
      {
        jbface = ibface2;

        do
        {
          if (mflag[jbface] == 0)
            mfound = 1;

          --jbface;
        }
        while (jbface > ibface && mfound == 0);

        if (mfound == 1)
        {
          ++jbface;

          jbfibf[ibface] = jbface;
          jbfibf[jbface] = ibface;

          ibface2 = jbface - 1;
        }
      }
    }

    ++ibface;
  }
  while (ibface <= nbface && mfound == 1);

  for (ibface = 1; ibface < *ibface0; ++ibface)
  {
    jbfibf[ibface] = ibface;
  }

  if (ieliel != NULL)
  {
    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      ibface1 = - ieliel[ielem][0];
      ibface2 = - ieliel[ielem][1];
      ibface3 = - ieliel[ielem][2];
      ibface4 = - ieliel[ielem][3];

      if (ibface1 > 0) ieliel[ielem][0] = - jbfibf[ibface1];
      if (ibface2 > 0) ieliel[ielem][1] = - jbfibf[ibface2];
      if (ibface3 > 0) ieliel[ielem][2] = - jbfibf[ibface3];
      if (ibface4 > 0) ieliel[ielem][3] = - jbfibf[ibface4];
    }
  }

  if (ibfibf != NULL)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ibface1 = ibfibf[ibface][0];
      ibface2 = ibfibf[ibface][1];
      ibface3 = ibfibf[ibface][2];

      if (ibface1 > 0) ibfibf[ibface][0] = jbfibf[ibface1];
      if (ibface2 > 0) ibfibf[ibface][1] = jbfibf[ibface2];
      if (ibface3 > 0) ibfibf[ibface][2] = jbfibf[ibface3];
    }
  }

  for (ibface = *ibface0; ibface <= nbface; ++ibface)
  {
    jbface = jbfibf[ibface];

    if (jbface > ibface)
    {
      if (ibcibf != NULL)
      {
        ibc = ibcibf[ibface];

        ibcibf[ibface] = ibcibf[jbface];
        ibcibf[jbface] = ibc;
      }

      if (idibf != NULL)
      {
        id = idibf[ibface];

        idibf[ibface] = idibf[jbface];
        idibf[jbface] = id;
      }

      if (iqibf != NULL)
      {
        iq = iqibf[ibface];

        iqibf[ibface] = iqibf[jbface];
        iqibf[jbface] = iq;
      }

      if (irfibf != NULL)
      {
        irf = irfibf[ibface];

        irfibf[ibface] = irfibf[jbface];
        irfibf[jbface] = irf;
      }

      if (ibfibf != NULL)
      {
        ibface1 = ibfibf[ibface][0];
        ibface2 = ibfibf[ibface][1];
        ibface3 = ibfibf[ibface][2];

        ibfibf[ibface][0] = ibfibf[jbface][0];
        ibfibf[ibface][1] = ibfibf[jbface][1];
        ibfibf[ibface][2] = ibfibf[jbface][2];
        ibfibf[jbface][0] = ibface1;
        ibfibf[jbface][1] = ibface2;
        ibfibf[jbface][2] = ibface3;
      }

      if (inibf != NULL)
      {
        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        inibf[ibface][0] = inibf[jbface][0];
        inibf[ibface][1] = inibf[jbface][1];
        inibf[ibface][2] = inibf[jbface][2];
        inibf[jbface][0] = inode1;
        inibf[jbface][1] = inode2;
        inibf[jbface][2] = inode3;
      }

      mflagi = mflag[ibface];

      mflag[ibface] = mflag[jbface];
      mflag[jbface] = mflagi;
    }
  }

  *ibface0 = jbface0;

  return;

}
