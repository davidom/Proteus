#include "UG3_LIB.h"

INT_ ug3_chkbq
 (INT_ merr,
  INT_ mmsg,
  INT_ nbface,
  INT_3D * inibf,
  INT_1D * ierribf,
  double angbq,
  DOUBLE_3D * x)

{

/*
 * Check planar face angles for each triangular face of surface grid.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkbq.c,v 1.11 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 * 
 */

  CHAR_133 Text;

  INT_ ibface, ierr, inode1, inode2, inode3;

  double cosbq, cosbqs, dc0, dc1, dc45, dx211, dx212, dx213, dx21s, dx311,
         dx312, dx313, dx31s, dx321, dx322, dx323, dx32s, w, w1, w2, w3, x11,
         x12, x13, x21, x22, x23, x31, x32, x33;

  dc0 = 0.0;
  dc1 = 1.0;
  dc45 = 45.0;

  if (angbq == dc0)
    return (0);

  if (mmsg == 2)
  {
    sprintf (Text, "Checking B-Quality : B-Faces           =%10i", nbface);
    ug_message (Text);
  }

  cosbq = cos (angbq * atan (dc1) / dc45);
  cosbqs = cosbq * fabs (cosbq);

  if (merr)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ierribf[ibface] = 0;
    }
  }

  ierr = 0;

  ibface = 1;

  do
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    x11 = x[inode1][0];
    x12 = x[inode1][1];
    x13 = x[inode1][2];
    x21 = x[inode2][0];
    x22 = x[inode2][1];
    x23 = x[inode2][2];
    x31 = x[inode3][0];
    x32 = x[inode3][1];
    x33 = x[inode3][2];

    dx211 = x21 - x11;
    dx212 = x22 - x12;
    dx213 = x23 - x13;
    dx311 = x31 - x11;
    dx312 = x32 - x12;
    dx313 = x33 - x13;
    dx321 = x31 - x21;
    dx322 = x32 - x22;
    dx323 = x33 - x23;

    dx21s = dx211 * dx211 + dx212 * dx212 + dx213 * dx213;
    dx31s = dx311 * dx311 + dx312 * dx312 + dx313 * dx313;
    dx32s = dx321 * dx321 + dx322 * dx322 + dx323 * dx323;

    if (dx21s > dc0 && dx31s > dc0 && dx32s > dc0)
    {
      w1 =   dx211 * dx311 + dx212 * dx312 + dx213 * dx313;
      w2 = - dx211 * dx321 - dx212 * dx322 - dx213 * dx323;
      w3 =   dx321 * dx311 + dx322 * dx312 + dx323 * dx313;

      w1 = cosbqs * dx21s * dx31s - w1 * fabs (w1);
      w2 = cosbqs * dx21s * dx32s - w2 * fabs (w2);
      w3 = cosbqs * dx32s * dx31s - w3 * fabs (w3);

      w = MAX (w1, w2);
      w = MAX (w3, w);
    }
    else
      w = dc1;

    if (w > dc0)
    {
      ierr = 1;

      if (merr)
        ierribf[ibface] = -1;
    }

    ++ibface;
  }
  while (ibface <= nbface && (! ierr || merr));

  if (ierr)
  {
    if (merr)
      return (200302);
    else
      return (-1);
  }

  if (mmsg == 2)
    ug_message ("Checking B-Quality : B-Quality OK");

  return (0);
}
