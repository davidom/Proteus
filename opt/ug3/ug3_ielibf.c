#include "UG3_LIB.h"

INT_ ug3_ielibf
 (INT_ minl,
  INT_ mmsg,
  INT_ nbface,
  INT_ nelem,
  INT_ nnode,
  INT_ *nbfaceu,
  INT_1D * ibcibf,
  INT_4D * ieliel,
  INT_3D * inibf,
  INT_4D * iniel,
  INT_1D * ibfjbf,
  INT_1D * ibfjn,
  INT_1D * ielibf,
  INT_1D * ierribf)

{

/*
 * Determine for each boundary face the element that contains that face and set
 * the element neighbors at boundaries.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ielibf.c,v 1.26 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ibc, ibface, ielem, ielem1, ielem2, ielem3, ieln, inode1, inode2, inode3,
       jelem, jeln, jnode1, jnode2, jnode3, jnode4, mbfdir;

  ibc = STD_UG3_GBC;

  *nbfaceu = 0;

  mbfdir = 1;

  ug3_ielibf0 (mbfdir, nbface, nelem, nnode,
               inibf, iniel, ibfjbf, ibfjn, ielibf);

  if (minl == 1)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ielem = ielibf[ibface];

      if (ielem > 0)
      {
        jnode1 = iniel[ielem][0];
        jnode2 = iniel[ielem][1];
        jnode3 = iniel[ielem][2];
        jnode4 = iniel[ielem][3];

        if (jnode1 > nnode || jnode2 > nnode ||
            jnode3 > nnode || jnode4 > nnode)
          ielibf[ibface] = 0;
      }
    }
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    if (ielibf[ibface] == 0)
      ++(*nbfaceu);
  }

  if (*nbfaceu == 0)
  {
    if (mmsg == 2)
    {
      sprintf (Text, "Boundary Recovery  : Unrecovered Faces =%10i", *nbfaceu);
      ug_message (Text);
    }

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ielem = ielibf[ibface];

      jnode1 = iniel[ielem][0];
      jnode2 = iniel[ielem][1];
      jnode3 = iniel[ielem][2];

      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      ieln = (jnode1 != inode1 && jnode1 != inode2 && jnode1 != inode3) ? 0 :
             (jnode2 != inode1 && jnode2 != inode2 && jnode2 != inode3) ? 1 :
             (jnode3 != inode1 && jnode3 != inode2 && jnode3 != inode3) ? 2 : 3;

      jelem = ieliel[ielem][ieln];

      ieliel[ielem][ieln] = -ibface;

      if (jelem > 0)
      {
        ielem1 = ieliel[jelem][0];
        ielem2 = ieliel[jelem][1];
        ielem3 = ieliel[jelem][2];

        jeln = (ielem1 == ielem) ? 0 :
               (ielem2 == ielem) ? 1 :
               (ielem3 == ielem) ? 2 : 3;

        if (ibcibf != NULL) ibc = ibcibf[ibface];

        if (minl <= 0 || CHK_TRANSP_UG3_GBC (ibc))
          ieliel[jelem][jeln] = -ibface;
        else
          ieliel[jelem][jeln] = 0;
      }
    }
  }
  else
  {
    if (minl == 2)
    {
      for (ielem = 1; ielem <= nelem; ++ielem)
      {
        if (ieliel[ielem][0] < 0)
          ieliel[ielem][0] = 0;
        if (ieliel[ielem][1] < 0)
          ieliel[ielem][1] = 0;
        if (ieliel[ielem][2] < 0)
          ieliel[ielem][2] = 0;
        if (ieliel[ielem][3] < 0)
          ieliel[ielem][3] = 0;
      }
    }

    if (minl <= 0)
    {
      if (ierribf == NULL)
        return (300);

      else
      {
        for (ibface = 1; ibface <= nbface; ++ibface)
        {
          ierribf[ibface] = (ielibf[ibface] == 0) ? -1: 0;
        }

        return (200306);
      }
    }
    else
    {
      if (mmsg == 2)
      {
        sprintf (Text, "Boundary Recovery  : Unrecovered Faces =%10i",
                 *nbfaceu);
        ug_message (Text);
      }

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        ielem = ielibf[ibface];

        if (ielem > 0)
        {
          jnode1 = iniel[ielem][0];
          jnode2 = iniel[ielem][1];
          jnode3 = iniel[ielem][2];

          inode1 = inibf[ibface][0];
          inode2 = inibf[ibface][1];
          inode3 = inibf[ibface][2];

          ieln = (jnode1 != inode1 && jnode1 != inode2 && jnode1 != inode3) ? 0:
                 (jnode2 != inode1 && jnode2 != inode2 && jnode2 != inode3) ? 1:
                 (jnode3 != inode1 && jnode3 != inode2 && jnode3 != inode3) ? 2:
                                                                              3;

          jelem = ieliel[ielem][ieln];

          ieliel[ielem][ieln] = -ibface;

          if (jelem > 0)
          {
            ielem1 = ieliel[jelem][0];
            ielem2 = ieliel[jelem][1];
            ielem3 = ieliel[jelem][2];

            jeln = (ielem1 == ielem) ? 0 :
                   (ielem2 == ielem) ? 1 :
                   (ielem3 == ielem) ? 2 : 3;

            if (ibcibf != NULL) ibc = ibcibf[ibface];

            if (minl != 2 && CHK_TRANSP_UG3_GBC (ibc))
              ieliel[jelem][jeln] = -ibface;
            else
              ieliel[jelem][jeln] = 0;
          }
        }
      }
    }
  }

  return (0);

}
