#include "UG3_LIB.h"

INT_ ug3_ielin
 (INT_ ielem0,
  INT_ nelem,
  INT_ *nelpntd,
  INT_ nnode,
  INT_ *nelpnt,
  INT_4D * iniel,
  INT_1D * madd,
  INT_1D ** ielin_ptr,
  INT_1D * lielin)

{

/*
 * Create a list of elements surrounding a node.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ielin.c,v 1.8 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D * ielin;

  INT_ ielem, ierr, inode, inode1, inode2, inode3, inode4, loc, nelpn;

  for (inode = 1; inode <= nnode; ++inode)
  {
    lielin[inode] = 0;
  }

  if (ielem0 > 1)
  {
    for (ielem = ielem0; ielem <= nelem; ++ielem)
    {
      if (madd[ielem] >= 1)
      {
        inode1 = iniel[ielem][0];
        inode2 = iniel[ielem][1];
        inode3 = iniel[ielem][2];
        inode4 = iniel[ielem][3];

        ++lielin[inode1];
        ++lielin[inode2];
        ++lielin[inode3];
        ++lielin[inode4];
      }
    }
  }
  else
  {
    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      inode1 = iniel[ielem][0];
      inode2 = iniel[ielem][1];
      inode3 = iniel[ielem][2];
      inode4 = iniel[ielem][3];

      ++lielin[inode1];
      ++lielin[inode2];
      ++lielin[inode3];
      ++lielin[inode4];
    }
  }

  *nelpnt = 0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    *nelpnt = *nelpnt + lielin[inode];
  }

  if (*nelpnt > *nelpntd)
  {
    *nelpntd = *nelpnt;

    ierr = 0;

    *ielin_ptr = (INT_1D *) ug_realloc (&ierr, *ielin_ptr,
                                        (*nelpntd+1) * sizeof (INT_1D));

    if (ierr > 0)
      return (100301);
  }

  ielin = *ielin_ptr;

  loc = 1;

  nelpn = 0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    loc = loc + nelpn;

    nelpn = lielin[inode];

    lielin[inode] = loc;
  }

  if (ielem0 > 1)
  {
    for (ielem = ielem0; ielem <= nelem; ++ielem)
    {
      if (madd[ielem] >= 1)
      {
        inode1 = iniel[ielem][0];
        inode2 = iniel[ielem][1];
        inode3 = iniel[ielem][2];
        inode4 = iniel[ielem][3];

        loc = lielin[inode1];

        ielin[loc] = ielem;

        ++loc;

        lielin[inode1] = loc;

        loc = lielin[inode2];

        ielin[loc] = ielem;

        ++loc;

        lielin[inode2] = loc;

        loc = lielin[inode3];

        ielin[loc] = ielem;

        ++loc;

        lielin[inode3] = loc;

        loc = lielin[inode4];

        ielin[loc] = ielem;

        ++loc;

        lielin[inode4] = loc;
      }
    }
  }
  else
  {
    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      inode1 = iniel[ielem][0];
      inode2 = iniel[ielem][1];
      inode3 = iniel[ielem][2];
      inode4 = iniel[ielem][3];

      loc = lielin[inode1];

      ielin[loc] = ielem;

      ++loc;

      lielin[inode1] = loc;

      loc = lielin[inode2];

      ielin[loc] = ielem;

      ++loc;

      lielin[inode2] = loc;

      loc = lielin[inode3];

      ielin[loc] = ielem;

      ++loc;

      lielin[inode3] = loc;

      loc = lielin[inode4];

      ielin[loc] = ielem;

      ++loc;

      lielin[inode4] = loc;
    }
  }

  for (inode = nnode + 1; inode >= 2; --inode)
  {
    lielin[inode] = lielin[inode-1];
  }

  lielin[1] = 1;

  return (0);

}
