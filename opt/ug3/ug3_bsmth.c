#include "UG3_LIB.h"

void ug3_bsmth
 (INT_ mmsg,
  INT_ nbface,
  INT_ nnode,
  INT_ nsmth,
  INT_3D * inibf,
  INT_1D * nnp,
  double dulim,
  double dutol,
  DOUBLE_1D * u,
  DOUBLE_1D * usum)

{

/*
 * Smooth a boundary function value using centroid averaging with limits.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_bsmth.c,v 1.9 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ibface, inode, inode1, inode2, inode3, ismth, nsmthm;

  double csmth, dc0, dc1, du, dum, dumaxm, duref, ratio, ui, umax, umin, usumi,
         w;

  dc0 = 0.0;
  dc1 = 1.0;

  csmth = 0.5;
  nsmthm = 3;

  if (nsmth <= 0)
    return;

  nsmthm = MIN (nsmth, nsmthm);

  ratio = dc1 + dulim;

  if (mmsg == 2)
  {
    sprintf (Text, "Smoothing          : Nodes, B-Faces    =%10i%10i",
             nnode, nbface);
    ug_message (Text);
  }

  for (inode = 1; inode <= nnode; ++inode)
  {
    nnp[inode] = 0;
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    nnp[inode1] = nnp[inode1] + 3;
    nnp[inode2] = nnp[inode2] + 3;
    nnp[inode3] = nnp[inode3] + 3;
  }

  ismth = 1;

  do
  {
    for (inode = 1; inode <= nnode; ++inode)
    {
      usum[inode] = dc0;
    }

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      usumi = u[inode1] + u[inode2] + u[inode3];

      usum[inode1] = usum[inode1] + usumi;
      usum[inode2] = usum[inode2] + usumi;
      usum[inode3] = usum[inode3] + usumi;
    }

    dumaxm = dc0;

    for (inode = 1; inode <= nnode; ++inode)
    {
      ui = u[inode];

      du = csmth * (usum[inode] / ((double) nnp[inode]) - ui);

      u[inode] = ui + du;

      dum = fabs (du);

      dumaxm = MAX (dum, dumaxm);
    }

    if (ismth == 1)
      duref = dumaxm;

    if (ismth > nsmthm)
    {
      w = dumaxm - dutol * duref;

      if (w > dc0 && ratio > dc1)
      {
        ibface = 1;

        do
        {
          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          umax = MAX (u[inode1], u[inode2]);
          umax = MAX (u[inode3], umax);
          umin = MIN (u[inode1], u[inode2]);
          umin = MIN (u[inode3], umin);

          w = umax - ratio * umin;

          ++ibface;
        }
        while (ibface <= nbface && w <= dc0);
      }
    }
    else
      w = dc1;

    ++ismth;
  }
  while (ismth <= nsmth && w > dc0);

  --ismth;

  if (mmsg == 2)
  {
    sprintf (Text, "Smoothing          : Iterations        =%10i", ismth);
    ug_message (Text);
  }

  return;

}
