#include "UG3_LIB.h"

INT_ ug3_ielinm
 (INT_ nelem,
  INT_ nelemc5,
  INT_ nelemc6,
  INT_ nelemc8,
  INT_ *nelpntd,
  INT_ nnode,
  INT_ *nelpnt,
  INT_4D * iniel,
  INT_5D * inielc5,
  INT_6D * inielc6,
  INT_8D * inielc8,
  INT_1D ** ielin_ptr,
  INT_1D * lielin)

{

/*
 * Create a list of mixed elements surrounding a node.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ielinm.c,v 1.3 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D * ielin;

  INT_ ielem, ielemc, ierr, inode, inode1, inode2, inode3, inode4, inode5,
       inode6, inode7, inode8, loc, nelpn;

  for (inode = 1; inode <= nnode; ++inode)
  {
    lielin[inode] = 0;
  }

  if (nelem > 0)
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

  if (nelemc5 > 0)
  {
    for (ielem = 1; ielem <= nelemc5; ++ielem)
    {
      inode1 = inielc5[ielem][0];
      inode2 = inielc5[ielem][1];
      inode3 = inielc5[ielem][2];
      inode4 = inielc5[ielem][3];
      inode5 = inielc5[ielem][4];

      ++lielin[inode1];
      ++lielin[inode2];
      ++lielin[inode3];
      ++lielin[inode4];
      ++lielin[inode5];
    }
  }

  if (nelemc6 > 0)
  {
    for (ielem = 1; ielem <= nelemc6; ++ielem)
    {
      inode1 = inielc6[ielem][0];
      inode2 = inielc6[ielem][1];
      inode3 = inielc6[ielem][2];
      inode4 = inielc6[ielem][3];
      inode5 = inielc6[ielem][4];
      inode6 = inielc6[ielem][5];

      ++lielin[inode1];
      ++lielin[inode2];
      ++lielin[inode3];
      ++lielin[inode4];
      ++lielin[inode5];
      ++lielin[inode6];
    }
  }

  if (nelemc8 > 0)
  {
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

      ++lielin[inode1];
      ++lielin[inode2];
      ++lielin[inode3];
      ++lielin[inode4];
      ++lielin[inode5];
      ++lielin[inode6];
      ++lielin[inode7];
      ++lielin[inode8];
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
      return (100303);
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

  if (nelem > 0)
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

  if (nelemc5 > 0)
  {
    ielemc = nelem;

    for (ielem = 1; ielem <= nelemc5; ++ielem)
    {
      inode1 = inielc5[ielem][0];
      inode2 = inielc5[ielem][1];
      inode3 = inielc5[ielem][2];
      inode4 = inielc5[ielem][3];
      inode5 = inielc5[ielem][4];

      loc = lielin[inode1];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode1] = loc;

      loc = lielin[inode2];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode2] = loc;

      loc = lielin[inode3];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode3] = loc;

      loc = lielin[inode4];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode4] = loc;

      loc = lielin[inode5];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode5] = loc;
    }
  }

  if (nelemc6 > 0)
  {
    ielemc = nelem + nelemc5;

    for (ielem = 1; ielem <= nelemc6; ++ielem)
    {
      inode1 = inielc6[ielem][0];
      inode2 = inielc6[ielem][1];
      inode3 = inielc6[ielem][2];
      inode4 = inielc6[ielem][3];
      inode5 = inielc6[ielem][4];
      inode6 = inielc6[ielem][5];

      loc = lielin[inode1];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode1] = loc;

      loc = lielin[inode2];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode2] = loc;

      loc = lielin[inode3];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode3] = loc;

      loc = lielin[inode4];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode4] = loc;

      loc = lielin[inode5];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode5] = loc;

      loc = lielin[inode6];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode6] = loc;
    }
  }

  if (nelemc8 > 0)
  {
    ielemc = nelem + nelemc5 + nelemc6;

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

      loc = lielin[inode1];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode1] = loc;

      loc = lielin[inode2];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode2] = loc;

      loc = lielin[inode3];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode3] = loc;

      loc = lielin[inode4];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode4] = loc;

      loc = lielin[inode5];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode5] = loc;

      loc = lielin[inode6];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode6] = loc;

      loc = lielin[inode7];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode7] = loc;

      loc = lielin[inode8];

      ielin[loc] = ielemc + ielem;

      ++loc;

      lielin[inode8] = loc;
    }
  }

  for (inode = nnode + 1; inode >= 2; --inode)
  {
    lielin[inode] = lielin[inode-1];
  }

  lielin[1] = 1;

  return (0);

}
