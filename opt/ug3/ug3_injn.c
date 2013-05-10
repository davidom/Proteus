#include "UG3_LIB.h"

INT_ ug3_injn
 (INT_ mmsg,
  INT_ mreorder,
  INT_ nnode,
  INT_1D * ielin,
  INT_4D * iniel,
  INT_1D * lielin,
  INT_1D * ipin,
  INT_1D * injn)

{

/*
 * Reorder nodes to be connected or un-connected.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_injn.c,v 1.8 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ielem, ieln, iit, inode, inode1, inode2, inode3, inode4, ipass, jnode,
       jnode0, kelem, keln, kloc, kloc1, kloc2, knode, kpass, loc, loc1, loc2,
       ngroup;

  if (mreorder == 0)
  {
    for (inode = 1; inode <= nnode; ++inode)
    {
      injn[inode] = inode;
    }
  }

  else if (mreorder == 1)
  {
    if (mmsg == 2)
      ug_message ("Creating Groups    : REORDERING NODES AS CONNECTED GROUPS");

    for (inode = 1; inode <= nnode; ++inode)
    {
      ipin[inode] = 0;
    }

    knode = 1;

    jnode = 0;

    ipass = 1;

    do
    {
      ++jnode;

      ipin[knode] = ipass;

      injn[jnode] = knode;

      iit = 1;

      do
      {
        jnode0 = jnode;

        loc1 = lielin[knode];
        loc2 = lielin[knode+1] - 1;

        loc = loc1;

        do
        {
          ielem = ielin[loc];

          ieln = 0;

          do
          {
            knode = iniel[ielem][ieln];

            if (ipin[knode] == 0)
            {
              ++jnode;

              ipin[knode] = ipass;

              injn[jnode] = knode;
            }

            ++ieln;
          }
          while (ieln <= 3 && jnode == jnode0);

          ++loc;
        }
        while (loc <= loc2 && jnode == jnode0);

        ++iit;
      }
      while (iit <= nnode && jnode > jnode0);

      if (jnode < nnode)
      {
        knode = 1;

        do
        {
          ++knode;

          kpass = ipin[knode];
        }
        while (knode <= nnode && kpass > 0);

        if (kpass > 0)
          return (320);
      }

      ++ipass;
    }
    while (ipass <= nnode && jnode < nnode);

    ngroup = ipass - 1;
  }

  else if (mreorder == 2)
  {
    if (mmsg == 2)
      ug_message ("Creating Groups    : REORDERING NODES AS UNCONNECTED GROUPS");

    for (inode = 1; inode <= nnode; ++inode)
    {
      ipin[inode] = 0;
    }

    knode = 1;

    jnode = 0;

    ipass = 1;

    do
    {
      ++jnode;

      ipin[knode] = -ipass;

      injn[jnode] = knode;

      iit = 1;

      do
      {
        jnode0 = jnode;

        inode = knode;

        loc1 = lielin[inode];
        loc2 = lielin[inode+1] - 1;

        for (loc = loc1; loc <= loc2; ++loc)
        {
          ielem = ielin[loc];

          inode1 = iniel[ielem][0];
          inode2 = iniel[ielem][1];
          inode3 = iniel[ielem][2];
          inode4 = iniel[ielem][3];

          if (ipin[inode1] >= 0) ipin[inode1] = ipass;
          if (ipin[inode2] >= 0) ipin[inode2] = ipass;
          if (ipin[inode3] >= 0) ipin[inode3] = ipass;
          if (ipin[inode4] >= 0) ipin[inode4] = ipass;
        }

        loc = loc1;

        do
        {
          ielem = ielin[loc];

          ieln = 0;

          do
          {
            knode = iniel[ielem][ieln];

            if (knode != inode)
            {
              kloc1 = lielin[knode];
              kloc2 = lielin[knode+1] - 1;

              kloc = kloc1;

              do
              {
                kelem = ielin[kloc];

                keln = 0;

                do
                {
                  knode = iniel[kelem][keln];

                  kpass = ipin[knode];

                  if (kpass >= 0 && kpass != ipass)
                  {
                    ++jnode;

                    ipin[knode] = -ipass;

                    injn[jnode] = knode;
                  }

                  ++keln;
                }
                while (keln <= 3 && jnode == jnode0);

                ++kloc;
              }
              while (kloc <= kloc2 && jnode == jnode0);
            }

            ++ieln;
          }
          while (ieln <= 3 && jnode == jnode0);

          ++loc;
        }
        while (loc <= loc2 && jnode == jnode0);

        ++iit;
      }
      while (iit <= nnode && jnode > jnode0);

      if (jnode < nnode)
      {
        knode = 1;

        do
        {
          ++knode;

          kpass = ipin[knode];
        }
        while (knode <= nnode && kpass < 0);

        if (kpass < 0)
          return (321);
      }

      ++ipass;
    }
    while (ipass <= nnode && jnode < nnode);

    ngroup = ipass - 1;
  }

  if (mreorder >= 1)
  {
    jnode = 1;

    do
    {
      inode = injn[jnode];

      ++jnode;
    }
    while (jnode <= nnode && inode > 0);

    if (inode <= 0)
      return (322);

    for (inode = 1; inode <= nnode; ++inode)
    {
      ipin[inode] = 0;
    }

    jnode = 1;

    do
    {
      inode = injn[jnode];

      ipass = ipin[inode];

      ipin[inode] = jnode;

      ++jnode;
    }
    while (jnode <= nnode && ipass == 0);

    if (ipass > 0)
      return (323);

    inode = 1;

    do
    {
      ipass = ipin[inode];

      ++inode;
    }
    while (inode <= nnode && ipass > 0);

    if (ipass == 0)
      return (324);

    if (mmsg == 2)
    {
      sprintf (Text, "Creating Groups    : Number of Groups  =%10i", ngroup);
      ug_message (Text);
    }
  }

  return (0);

}
