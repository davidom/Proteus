#include "UG3_LIB.h"

INT_ ug3_inix
 (INT_ nxsort,
  INT_ nnode,
  INT_ *nx1,
  INT_ *nx2,
  INT_ *nx3,
  INT_1D * inix,
  INT_1D * linix,
  INT_1D * ixin,
  double tol,
  double *didx1,
  double *didx2,
  double *didx3,
  double *x1min,
  double *x2min,
  double *x3min,
  DOUBLE_3D * x)

{

/*
 * Determine map based on physical location.
 * 
 * UG3 LIB : Unxtructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_inix.c,v 1.6 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ inode, ix, ix1, ix2, ix3, loc, n, nnpx, nx, nx12;

  double c, dc1, dc1d3, dc3, dx1, dx2, dx3, dxmax, dxtol, rx1, rx2, rx3, x1,
         x1max, x2, x2max, x3, x3max;

  dc1 = 1.0;
  dc3 = 3.0;
  dc1d3 = dc1 / dc3;

  if (nxsort < 8)
    return (311);

  inode = 1;

  x1 = x[inode][0];
  x2 = x[inode][1];
  x3 = x[inode][2];

  x1max = x1;
  x2max = x2;
  x3max = x3;

  *x1min = x1;
  *x2min = x2;
  *x3min = x3;

  for (inode = 2; inode <= nnode; ++inode)
  {
    x1 = x[inode][0];
    x2 = x[inode][1];
    x3 = x[inode][2];

    x1max = MAX (x1, x1max);
    x2max = MAX (x2, x2max);
    x3max = MAX (x3, x3max);

    *x1min = MIN (x1, *x1min);
    *x2min = MIN (x2, *x2min);
    *x3min = MIN (x3, *x3min);
  }

  dx1 = x1max - (*x1min);
  dx2 = x2max - (*x2min);
  dx3 = x3max - (*x3min);

  dxmax = MAX (dx1, dx2);
  dxmax = MAX (dx3, dxmax);

  dxtol = tol * dxmax;

  if (dx1 == dxmax)
  {
    rx2 = (dx2 > dxtol) ? dx1 / dx2 : dc1;
    rx3 = (dx3 > dxtol) ? dx1 / dx3 : dc1;

    c = pow (((double) nxsort) * rx2 * rx3, dc1d3);
    n = NINT (c);

    *nx1 = MAX (n, 1);

    c = ((double) (*nx1)) / rx2 - dc1;
    n = NINT (c);

    *nx2 = (dx2 > dxtol) ? MAX (n, 1) : 1;

    c = ((double) (*nx1)) / rx3 - dc1;
    n = NINT (c);

    *nx3 = (dx3 > dxtol) ? MAX (n, 1) : 1;

    do
    {
      nx = ((*nx1) + 1) * ((*nx2) + 1) * ((*nx3) + 1);

      if (nx > nxsort)
        --(*nx1);

      if (*nx1 == 0)
      {
        *nx1 = 1;

        if (*nx2 > 1)
          --(*nx2);
        else
          --(*nx3);
      }
    }
    while (nx > nxsort);
  }

  else if (dx2 == dxmax)
  {
    rx1 = (dx1 > dxtol) ? dx2 / dx1 : dc1;
    rx3 = (dx3 > dxtol) ? dx2 / dx3 : dc1;

    c = pow (((double) nxsort) * rx1 * rx3, dc1d3);
    n = NINT (c);

    *nx2 = MAX (n, 1);

    c = ((double) (*nx2)) / rx1 - dc1;
    n = NINT (c);

    *nx1 = (dx1 > dxtol) ? MAX (n, 1) : 1;

    c = ((double) (*nx2)) / rx3 - dc1;
    n = NINT (c);

    *nx3 = (dx3 > dxtol) ? MAX (n, 1) : 1;

    do
    {
      nx = ((*nx1) + 1) * ((*nx2) + 1) * ((*nx3) + 1);

      if (nx > nxsort)
        --(*nx2);

      if (*nx2 == 0)
      {
        *nx2 = 1;

        if (*nx1 > 1)
          --(*nx1);
        else
          --(*nx3);
      }
    }
    while (nx > nxsort);
  }

  else 
  {
    rx1 = (dx1 > dxtol) ? dx3 / dx1 : dc1;
    rx2 = (dx2 > dxtol) ? dx3 / dx2 : dc1;

    c = pow (((double) nxsort) * rx1 * rx2, dc1d3) - dc1;
    n = NINT (c);

    *nx3 = MAX (n, 1);

    c = ((double) (*nx3)) / rx1 - dc1;
    n = NINT (c);

    *nx1 = (dx1 > dxtol) ? MAX (n, 1) : 1;

    c = ((double) (*nx3)) / rx2 - dc1;
    n = NINT (c);

    *nx2 = (dx2 > dxtol) ? MAX (n, 1) : 1;

    do
    {
      nx = ((*nx1) + 1) * ((*nx2) + 1) * ((*nx3) + 1);

      if (nx > nxsort)
        --(*nx3);

      if (*nx3 == 0)
      {
        *nx3 = 1;

        if (*nx1 > 1)
          --(*nx1);
        else
          --(*nx2);
      }
    }
    while (nx > nxsort);
  }

  *didx1 = (dx1 > dxtol) ? ((double) (*nx1)) / dx1 : dc1;
  *didx2 = (dx2 > dxtol) ? ((double) (*nx2)) / dx2 : dc1;
  *didx3 = (dx3 > dxtol) ? ((double) (*nx3)) / dx3 : dc1;

  for (ix = 1; ix <= nx; ++ix)
  {
    linix[ix] = 0;
  }

  ++(*nx1);
  ++(*nx2);
  ++(*nx3);

  nx12 = (*nx1) * (*nx2);

  for (inode = 1; inode <= nnode; ++inode)
  {
    x1 = x[inode][0];
    x2 = x[inode][1];
    x3 = x[inode][2];

    c = (x1 - (*x1min)) * (*didx1);

    ix1 = NINT (c);

    c = (x2 - (*x2min)) * (*didx2);

    ix2 = NINT (c);

    c = (x3 - (*x3min)) * (*didx3);

    ix3 = NINT (c);

    ix = 1 + ix1 + ix2 * (*nx1) + ix3 * nx12;

    ixin[inode] = ix;

    ++linix[ix];
  }

  nnpx = 0;

  for (ix = 1; ix <= nx; ++ix)
  {
    nnpx = nnpx + linix[ix];
  }

  loc = 1;

  nnpx = 0;

  for (ix = 1; ix <= nx; ++ix)
  {
    loc = loc + nnpx;

    nnpx = linix[ix];

    linix[ix] = loc;
  }

  for (inode = 1; inode <= nnode; ++inode)
  {
    ix = ixin[inode];

    loc = linix[ix];

    inix[loc] = inode;

    ++loc;

    linix[ix] = loc;
  }

  for (ix = nx + 1; ix >= 2; --ix)
  {
    linix[ix] = linix[ix-1];
  }

  linix[1] = 1;

  return (0);

}
