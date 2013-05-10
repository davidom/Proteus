#include "UG3_LIB.h"

INT_ ug3_chk
 (INT_ mbnd,
  INT_ mvolchk,
  INT_ mmsg,
  INT_ nelem,
  INT_ nnode,
  INT_4D * ieliel,
  INT_4D * iniel,
  double tol,
  DOUBLE_3D * x)

{

/*
 * Check element connectivity and element volume. Element neighbors (ieliel) and
 * element nodes (iniel) are checked. Element volume (vol) is checked for zero
 * or negative volumes and the number of elements with volume less than
 * tolerance is reported. 
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chk.c,v 1.22 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ielem, ieln1, ieln2, ieln3, ieln4, ierr, inode2, inode3, inode4, jelem,
       jeln1, jeln2, jeln3, jeln4, jnode3, jnode4, kelem, mchk, mfound;

  INT_ ielnmap[4][4][4] =
  {
    {
      {0, 1, 2, 3},
      {1, 0, 3, 2},
      {2, 0, 1, 3},
      {3, 0, 2, 1}},
    {
      {0, 1, 2, 3},
      {1, 2, 0, 3},
      {2, 1, 3, 0},
      {3, 1, 0, 2}},
    {
      {0, 2, 3, 1},
      {1, 2, 0, 3},
      {2, 3, 0, 1},
      {3, 2, 1, 0}},
    {
      {0, 3, 1, 2},
      {1, 3, 2, 0},
      {2, 3, 0, 1},
      {3, 0, 2, 1}}};

  ierr = 0;

  if (mmsg == 2)
  {
    sprintf (Text, "Checking           : Nodes, Elements   =%10i%10i",
             nnode, nelem);
    ug_message (Text);
  }

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    for (ieln1 = 0; ieln1 <= 3; ++ieln1)
    {
      jelem = ieliel[ielem][ieln1];

      if (jelem > 0)
      {
        ieln2 = ielnmap[ieln1][ieln1][1];
        ieln3 = ielnmap[ieln1][ieln1][2];
        ieln4 = ielnmap[ieln1][ieln1][3];

        inode2 = iniel[ielem][ieln2];
        inode3 = iniel[ielem][ieln3];
        inode4 = iniel[ielem][ieln4];

        mchk = 0;

        jeln1 = 0;

        do
        {
          kelem = ieliel[jelem][jeln1];

          if (kelem == ielem)
          {
            mfound = 0;

            jeln2 = 0;

            do
            {
              if (iniel[jelem][jeln2] == inode2)
                mfound = 1;

              ++jeln2;
            }
            while (jeln2 <= 3 && ! mfound);

            --jeln2;

            if (! mfound)
              return (301);

            jeln3 = ielnmap[jeln2][jeln1][3];
            jeln4 = ielnmap[jeln2][jeln1][2];

            jnode3 = iniel[jelem][jeln3];
            jnode4 = iniel[jelem][jeln4];

            if (jnode3 != inode3 || jnode4 != inode4)
              return (302);

            mchk = 1;
          }

          ++jeln1;
        }
        while (jeln1 <= 3 && ! mchk);

        if (! mchk)
          return (303);
      }
      else if (mbnd && ! jelem)
        return (315);
    }
  }

  if (mvolchk)
  {
    ierr = ug3_chkvol (mmsg, nelem, nnode, iniel, tol, x);

    if (ierr > 0)
      return (ierr);
  }

  if (mmsg == 2)
    ug_message ("Checking           : Connectivity OK");

  return (ierr);

}
