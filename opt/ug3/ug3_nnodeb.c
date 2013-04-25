#include "UG3_LIB.h"

void ug3_nnodeb
 (INT_ nbface,
  INT_ *nnodeb,
  INT_3D * inibf)

{

/*
 * Determine the maximum node index on the boundary surface.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_nnodeb.c,v 1.5 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibface, inode1, inode2, inode3;

  *nnodeb = 0;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    *nnodeb = MAX (inode1, *nnodeb);
    *nnodeb = MAX (inode2, *nnodeb);
    *nnodeb = MAX (inode3, *nnodeb);
  }

  return;

}
