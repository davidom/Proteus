#include "UG3_LIB.h"

INT_ ug3_chkbn
 (INT_ merr,
  INT_ mmsg,
  INT_ nbface,
  INT_ nnode,
  INT_3D * inibf,
  INT_1D * mchkbn,
  INT_1D * ierribf)
{

/*
 * Check that all nodes are on a boundary surface, all faces contain unique
 * nodes, and that all faces reference included nodes.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkbn.c,v 1.17 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ i, ibface, ierr, inode, inode1, inode2, inode3, n;

  if (mmsg == 2)
  {
    sprintf (Text, "Checking B-Nodes   : Nodes             =%10i", nnode);
    ug_message (Text);
  }

  if (merr)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ierribf[ibface] = 0;
    }
  }

  for (inode = 1; inode <= nnode; ++inode)
  {
    mchkbn[inode] = 1;
  }

  ierr = 0;

  ibface = 1;

  do
  {
    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    if (inode1 < 1 || inode1 > nnode)
    {
      ierr = 1;

      if (merr)
        ierribf[ibface] = -1;
    }
    else
      mchkbn[inode1] = 0;

    if (inode2 < 1 || inode2 > nnode)
    {
      ierr = 1;

      if (merr)
        ierribf[ibface] = -1;
    }
    else
      mchkbn[inode2] = 0;

    if (inode3 < 1 || inode3 > nnode)
    {
      ierr = 1;

      if (merr)
        ierribf[ibface] = -1;
    }
    else
      mchkbn[inode3] = 0;

    if (inode1 == inode2 || inode1 == inode3 || inode2 == inode3)
    {
      ierr = 2;

      if (merr)
        ierribf[ibface] = -1;
    }

    ++ibface;
  }
  while (ibface <= nbface && (! ierr || merr));

  if (ierr == 1)
  {
    if (merr)
      return (200314);
    else
      return (331);
  }
  else if (ierr == 2)
  {
    if (merr)
    {
      for (inode = 1; inode <= nnode; ++inode)
      {
        mchkbn[inode] = 0;
      }

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        if (ierribf[ibface] == -1)
        {
          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          if (inode1 == inode2 || inode1 == inode3)
            mchkbn[inode1] = 1;
          else if (inode2 == inode3)
            mchkbn[inode2] = 1;
        }
      }

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        inode1 = inibf[ibface][0];
        inode2 = inibf[ibface][1];
        inode3 = inibf[ibface][2];

        if (mchkbn[inode1] || mchkbn[inode2] || mchkbn[inode3])
          ierribf[ibface] = -1;
      }

      return (200319);
    }
    else
      return (326);
  }

  if (mmsg == 2)
    ug_message ("Checking B-Nodes   : All faces reference included nodes");

  inode = 1;

  do
  {
    if (mchkbn[inode])
      ierr = 1;

    ++inode;
  }
  while (inode <= nnode && (! ierr || merr));

  if (ierr)
  {
    if (merr)
    {
      n = MIN (nnode, nbface);

      i = 0;

      for (inode = 1; inode <= nnode; ++inode)
      {
        if (mchkbn[inode])
	{
          ++i;

	  if (i <= n)
            ierribf[i] = inode;
	}
      }

      return (200312);
    }
    else
      return (318);
  }

  if (mmsg == 2)
    ug_message ("Checking B-Nodes   : All nodes on boundary");

  return (0);

}
