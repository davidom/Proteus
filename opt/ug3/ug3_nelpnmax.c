#include "UG3_LIB.h"

INT_ ug3_nelpnmax
 (INT_ mmsg,
  INT_ nelem,
  INT_ nnode,
  INT_4D * iniel,
  INT_1D * nelpn)

{

/*
 * Check maximum elements per node.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ielem, inode, inode1, inode2, inode3, inode4, nelpni, nelpnmax;

  for (inode = 1; inode <= nnode; ++inode)
  {
    nelpn[inode] = 0;
  }

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    inode1 = iniel[ielem][0];
    inode2 = iniel[ielem][1];
    inode3 = iniel[ielem][2];
    inode4 = iniel[ielem][3];

    if (inode1 <= nnode) ++nelpn[inode1];
    if (inode2 <= nnode) ++nelpn[inode2];
    if (inode3 <= nnode) ++nelpn[inode3];
    if (inode4 <= nnode) ++nelpn[inode4];
  }

  nelpnmax = 0;

  for (inode = 1; inode <= nnode; ++inode)
  {
    nelpni = nelpn[inode], 

    nelpnmax = MAX (nelpni, nelpnmax);
  }

  if (mmsg == 2)
  {
    sprintf (Text, "Elements per Node  : Max Elements/Node =%10i", nelpnmax);
    ug_message (Text);
  }

  return (nelpnmax);
}
