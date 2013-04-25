#include "UG3_LIB.h"

void ug3_reorder0
 (INT_ *ielem0,
  INT_ ielem00,
  INT_ nelem,
  INT_4D * ieliel,
  INT_4D * iniel,
  INT_1D * jeliel,
  INT_1D * mflag,
  double relem0)

{

/*
 * Re-orders the elements so that the elements with mflag[ielem] == 0 are first
 * in the ordering if there are at least relem0*nelem elements to be re-ordered.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_reorder0.c,v 1.9 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ielem, ielem1, ielem2, ielem3, ielem4, inode1, inode2, inode3, inode4,
       jelem, jelem0, mflagi, mfound, mieliel, nmflag;

  double r;

  mfound = 0;

  ielem = ielem00;

  do
  {
    if (mflag[ielem] != 0)
      mfound = 1;

    ++ielem;
  }
  while (ielem <= nelem && mfound == 0);

  if (mfound == 0)
  {
    *ielem0 = nelem + 1;

    return;
  }

  *ielem0 = ielem - 1;

  nmflag = 0;

  for (ielem = *ielem0; ielem <= nelem; ++ielem)
  {
    if (mflag[ielem] != 0)
      ++nmflag;

    jeliel[ielem] = ielem;
  }

  jelem0 = nelem - nmflag + 1;

  r = ((double) (jelem0-(*ielem0))) / ((double) (nelem-(*ielem0)+1));

  if (r >= relem0)
  {
    mfound = 1;

    ielem2 = nelem;

    ielem = *ielem0;

    do
    {
      if (mflag[ielem] != 0)
      {
        mfound = 0;

        if (ielem2 > ielem)
        {
          jelem = ielem2;

          do
          {
            if (mflag[jelem] == 0)
              mfound = 1;

            --jelem;
          }
          while (jelem > ielem && mfound == 0);

          if (mfound == 1)
          {
            ++jelem;

            jeliel[ielem] = jelem;
            jeliel[jelem] = ielem;

            ielem2 = jelem - 1;
          }
        }
      }

      ++ielem;
    }
    while (ielem <= nelem && mfound == 1);

    mieliel = (ieliel == NULL) ? 0 : 1;

    if (mieliel == 1)
    {
      for (ielem = *ielem0; ielem <= nelem; ++ielem)
      {
        jelem = jeliel[ielem];

        if (jelem > ielem)
        {
          ielem1 = ieliel[ielem][0];
          ielem2 = ieliel[ielem][1];
          ielem3 = ieliel[ielem][2];
          ielem4 = ieliel[ielem][3];

          ieliel[ielem][0] = ieliel[jelem][0];
          ieliel[ielem][1] = ieliel[jelem][1];
          ieliel[ielem][2] = ieliel[jelem][2];
          ieliel[ielem][3] = ieliel[jelem][3];

          ieliel[jelem][0] = ielem1;
          ieliel[jelem][1] = ielem2;
          ieliel[jelem][2] = ielem3;
          ieliel[jelem][3] = ielem4;
        }
      }

      for (ielem = 1; ielem < *ielem0; ++ielem)
      {
        jeliel[ielem] = ielem;
      }

      for (ielem = 1; ielem <= nelem; ++ielem)
      {
        ielem1 = ieliel[ielem][0];
        ielem2 = ieliel[ielem][1];
        ielem3 = ieliel[ielem][2];
        ielem4 = ieliel[ielem][3];

        if (ielem1 > 0)
          ieliel[ielem][0] = jeliel[ielem1];
        if (ielem2 > 0)
          ieliel[ielem][1] = jeliel[ielem2];
        if (ielem3 > 0)
          ieliel[ielem][2] = jeliel[ielem3];
        if (ielem4 > 0)
          ieliel[ielem][3] = jeliel[ielem4];
      }
    }

    for (ielem = *ielem0; ielem <= nelem; ++ielem)
    {
      jelem = jeliel[ielem];

      if (jelem > ielem)
      {
        inode1 = iniel[ielem][0];
        inode2 = iniel[ielem][1];
        inode3 = iniel[ielem][2];
        inode4 = iniel[ielem][3];

        iniel[ielem][0] = iniel[jelem][0];
        iniel[ielem][1] = iniel[jelem][1];
        iniel[ielem][2] = iniel[jelem][2];
        iniel[ielem][3] = iniel[jelem][3];

        iniel[jelem][0] = inode1;
        iniel[jelem][1] = inode2;
        iniel[jelem][2] = inode3;
        iniel[jelem][3] = inode4;

        mflagi = mflag[ielem];

        mflag[ielem] = mflag[jelem];
        mflag[jelem] = mflagi;
      }
    }

    *ielem0 = jelem0;
  }

  return;

}
