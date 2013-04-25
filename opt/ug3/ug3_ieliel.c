#include "UG3_LIB.h"

INT_ ug3_ieliel
 (INT_ nelem,
  INT_1D * ielin,
  INT_4D * iniel,
  INT_1D * lielin,
  INT_4D * ieliel)

{

/*
 * Determine the element to element connectivity.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ieliel.c,v 1.6 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ielem, ielf, ieln, ieln1, ieln2, ieln3, inode, jelem, jelf, jeln,
       jnode, jnode1, jnode2, jnode3, keln, knode, loc, loc1, loc2, mfound,
       mset;

  INT_ ienielf[4][3] =
  {
    {1, 2, 3},
    {2, 0, 3},
    {0, 1, 3},
    {2, 1, 0}};

  INT_ melf[4];

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    ieliel[ielem][0] = 0;
    ieliel[ielem][1] = 0;
    ieliel[ielem][2] = 0;
    ieliel[ielem][3] = 0;
  }

  for (jelem = 1; jelem <= nelem; ++jelem)
  {
    for (jelf = 0; jelf <= 3; ++jelf)
    {
      if (ieliel[jelem][jelf] == 0)
      {
        mfound = 0;

        melf[0] = 0;
        melf[1] = 0;
        melf[2] = 0;
        melf[3] = 0;

        ieln1 = ienielf[jelf][0];
        ieln2 = ienielf[jelf][1];
        ieln3 = ienielf[jelf][2];

        jnode1 = iniel[jelem][ieln1];
        jnode2 = iniel[jelem][ieln2];
        jnode3 = iniel[jelem][ieln3];

        loc1 = lielin[jnode1];
        loc2 = lielin[jnode1 + 1] - 1;

        loc = loc1;

        do
        {
          ielem = ielin[loc];

          if (ielem != jelem)
          {
            ieln = 0;

            do
            {
              inode = iniel[ielem][ieln];

              if (inode == jnode1)
              {
                jeln = 0;

                do
                {
                  jnode = iniel[ielem][jeln];

                  if (jnode == jnode2)
                  {
                    keln = 0;

                    do
                    {
                      knode = iniel[ielem][keln];

                      if (knode == jnode3)
                      {
                        melf[ieln] = 1;
                        melf[jeln] = 1;
                        melf[keln] = 1;

                        mfound = 1;
                      }

                      ++keln;
                    }
                    while (keln <= 3 && mfound == 0);
                  }

                  ++jeln;
                }
                while (jeln <= 3 && mfound == 0);
              }

              ++ieln;
            }
            while (ieln <= 3 && mfound == 0);
          }

          ++loc;
        }
        while (loc <= loc2 && mfound == 0);

        if (mfound == 1)
        {
          mset = 0;

          ielf = 0;

          do
          {
            if (melf[ielf] == 0)
            {
              ieliel[jelem][jelf] = ielem;
              ieliel[ielem][ielf] = jelem;

              mset = 1;
            }

            ++ielf;
          }
          while (ielf <= 3 && mset == 0);

          if (mset == 0)
            return (314);
        }
      }
    }
  }

  return (0);

}
