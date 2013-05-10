#include "UG3_LIB.h"

INT_ ug3_chkb
 (INT_ mclosed,
  INT_ merr,
  INT_ mmsg,
  INT_ nbface,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_3D * inibf,
  INT_1D * ierribf)
{

/*
 * Check boundary face connectivity. Face neighbors (ibfibf) and face nodes
 * (inibf) are checked.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkb.c,v 1.16 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ibc, ibface, ibface1, ibface2, ibface3, ibfn1, ibfn2, ibfn3, ierr,
       inode2, inode3, jbface, jbfn1, jbfn2, jbfn3, jerr, jnode2, jnode3,
       kbface;

  if (mmsg == 2)
  {
    sprintf (Text, "Checking           : B-Faces           =%10i", nbface);
    ug_message (Text);
  }

  if (merr)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ierribf[ibface] = 0;
    }
  }

  ierr = 0;

  ibface = 1;

  do
  {
    ibfn1 = 0;

    do
    {
      jbface = ibfibf[ibface][ibfn1];

      if (jbface > 0)
      {
        ibfn2 = (ibfn1 < 2) ? ibfn1+1 : 0;
        ibfn3 = (ibfn2 < 2) ? ibfn2+1 : 0;

        inode2 = inibf[ibface][ibfn2];
        inode3 = inibf[ibface][ibfn3];

        jerr = -1;

        jbfn1 = 0;

        do
        {
          kbface = ibfibf[jbface][jbfn1];

          if (kbface == ibface)
          {
            jbfn2 = (jbfn1 < 2) ? jbfn1+1 : 0;
            jbfn3 = (jbfn2 < 2) ? jbfn2+1 : 0;

            jnode2 = inibf[jbface][jbfn2];
            jnode3 = inibf[jbface][jbfn3];

            jerr = (jnode3 == inode2 && jnode2 == inode3) ? 0 : 1;
          }

          ++jbfn1;
        }
        while (jbfn1 <= 2 && jerr == -1);

        if (jerr != 0)
        {
          ierr = 1;

          if (merr)
          {
            ierribf[ibface] = -1;
            ierribf[jbface] = -1;
          }
        }
      }

      ++ibfn1;
    }
    while (ibfn1 <= 2 && ! ierr);

    ++ibface;
  }
  while (ibface <= nbface && ! ierr);

  if (ierr)
  {
    if (merr)
      return (200301);
    else
      return (304);
  }

  if (mclosed)
  {
    ierr = 0;

    ibface = 1;

    do
    {
      ibface1 = ibfibf[ibface][0];
      ibface2 = ibfibf[ibface][1];
      ibface3 = ibfibf[ibface][2];

      if (! ibface1 || ! ibface2 || ! ibface3)
      {
        if (ibcibf == NULL)
          ibc = 0;
        else
          ibc = ibcibf[ibface];

        if (! CHK_TRANSP_UG3_GBC (ibc))
        {
          ierr = 1;

          if (merr)
            ierribf[ibface] = -1;
        }
      }

      ++ibface;
    }
    while (ibface <= nbface && (! ierr || merr));

    if (ierr)
    {
      if (merr)
        return (200308);
      else
        return (305);
    }
  }

  if (mmsg == 2)
    ug_message ("Checking           : Connectivity OK");

  return (0);

}
