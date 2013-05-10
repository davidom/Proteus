#include "UG3_LIB.h"

void ug3_vsmth
 (INT_ mmsg,
  INT_ nelem,
  INT_ nnode,
  INT_ nsmth,
  INT_4D * iniel,
  INT_1D * nnp,
  DOUBLE_1D * u,
  DOUBLE_1D * usum)

{

/*
 * Smooth a function value using centroid averaging.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_vsmth.c,v 1.4 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ielem, inode, inode1, inode2, inode3, inode4, ismth;

  double csmth, dc0, ui, usumi;

  dc0 = 0.0;

  csmth = 0.5;

  if (nsmth == 0)
    return;

  if (mmsg == 2)
  {
    sprintf (Text, "Smoothing          : Nodes, Elements   =%10i%10i",
             nnode, nelem);
    ug_message (Text);
  }

  for (inode = 1; inode <= nnode; ++inode)
  {
    nnp[inode] = 0;
  }

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    inode1 = iniel[ielem][0];
    inode2 = iniel[ielem][1];
    inode3 = iniel[ielem][2];
    inode4 = iniel[ielem][3];

    nnp[inode1] = nnp[inode1] + 4;
    nnp[inode2] = nnp[inode2] + 4;
    nnp[inode3] = nnp[inode3] + 4;
    nnp[inode4] = nnp[inode4] + 4;
  }

  for (ismth = 1; ismth <= nsmth; ++ismth)
  {
    for (inode = 1; inode <= nnode; ++inode)
    {
      usum[inode] = dc0;
    }

    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      inode1 = iniel[ielem][0];
      inode2 = iniel[ielem][1];
      inode3 = iniel[ielem][2];
      inode4 = iniel[ielem][3];

      usumi = u[inode1] + u[inode2] + u[inode3] + u[inode4];

      usum[inode1] = usum[inode1] + usumi;
      usum[inode2] = usum[inode2] + usumi;
      usum[inode3] = usum[inode3] + usumi;
      usum[inode4] = usum[inode4] + usumi;
    }

    for (inode = 1; inode <= nnode; ++inode)
    {
      ui = u[inode];

      u[inode] = ui + csmth * (usum[inode] / ((double) nnp[inode]) - ui);
    }
  }

  return;

}
