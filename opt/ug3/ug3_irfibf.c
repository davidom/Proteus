#include "UG3_LIB.h"

void ug3_irfibf
 (INT_ nbface,
  INT_ nnode,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_1D * irfibf,
  INT_1D * jnin)

{

/*
 * Check the boundary face reconnection flag and reset it so that the flag
 * matches that of the adjacent faces, and set the boundary reconnection flag
 * between faces of different surface ID's. Also set the boundary reconnection
 * flag between faces that have transparent free surfaces attached.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_irfibf.c,v 1.28 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibc, ibc1, ibc2, ibc3, ibface, ibface1, ibface2, ibface3, ibface4, ibfn1,
       ibfn2, ibfn3, id, id1, id2, id3, inode, inode1, inode2, inode3, irf0,
       irf00, irf01, irf02, irf03, irf10, irf11, irf12, irf13, irf20, irf21,
       irf22, irf23, irf30, irf31, irf32, irf33, jbface, jbfn1, jbfn2, jbfn3,
       jn1, jn2, jn3, jrf0;

  INT_ irfi[3], irfj[3];

  if (irfibf == NULL) return;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    irf0 = irfibf[ibface];

    irfi[2] = irf0 / 4;
    irf0 = irf0 - 4 * irfi[2];
    irfi[1] = irf0 / 2;
    irfi[0] = irf0 - 2 * irfi[1];

    for (ibfn1 = 0; ibfn1 <= 2; ++ibfn1)
    {
      if (irfi[ibfn1] == 0)
      {
        jbface = ibfibf[ibface][ibfn1];

        if (jbface > 0)
        {
          ibfn2 = (ibfn1 < 2) ? ibfn1+1 : 0;
          ibfn3 = (ibfn2 < 2) ? ibfn2+1 : 0;

          ibface2 = ibfibf[ibface][ibfn2];
          ibface4 = ibfibf[ibface][ibfn3];

          jbfn1 = (ibfibf[jbface][0] == ibface) ? 0 :
                  (ibfibf[jbface][1] == ibface) ? 1 : 2;
          jbfn2 = (jbfn1 < 2) ? jbfn1+1 : 0;
          jbfn3 = (jbfn2 < 2) ? jbfn2+1 : 0;

          ibface1 = ibfibf[jbface][jbfn3];
          ibface3 = ibfibf[jbface][jbfn2];

          jrf0 = irfibf[jbface];

          irfj[2] = jrf0 / 4;
          jrf0 = jrf0 - 4 * irfj[2];
          irfj[1] = jrf0 / 2;
          irfj[0] = jrf0 - 2 * irfj[1];

          if ((ibface1 != ibface2 || ibface1 == 0) &&
              (ibface3 != ibface4 || ibface3 == 0))
          {
            irf0 = MAX (irfi[ibfn1], irfj[jbfn1]);

            irfi[ibfn1] = irf0;
            irfj[jbfn1] = irf0;
          }
          else
          {
            irfi[ibfn1] = 1;
            irfj[jbfn1] = 1;
          }

          irfibf[jbface] = irfj[0] + 2 * irfj[1] + 4 * irfj[2];
        }
      }
    }

    irfibf[ibface] = irfi[0] + 2 * irfi[1] + 4 * irfi[2];
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibface1 = ibfibf[ibface][0];
    ibface2 = ibfibf[ibface][1];
    ibface3 = ibfibf[ibface][2];

    ibc = (ibcibf != NULL) ? ibcibf[ibface] : 0;

    id = idibf[ibface];

    irf00 = irfibf[ibface];

    irf03 = irf00 / 4;
    irf00 = irf00 - 4 * irf03;
    irf02 = irf00 / 2;
    irf01 = irf00 - irf02 - irf02;

    if (ibface1 > 0)
    {
      ibc1 = (ibcibf != NULL) ? ibcibf[ibface1] : 0;

      id1 = idibf[ibface1];

      if (ibc == ibc1 && id == id1)
      {
        irf10 = irfibf[ibface1];

        irf13 = irf10 / 4;
        irf10 = irf10 - 4 * irf13;
        irf12 = irf10 / 2;
        irf11 = irf10 - irf12 - irf12;

        irf00 = (ibfibf[ibface1][0] == ibface) ? irf11 :
                (ibfibf[ibface1][1] == ibface) ? irf12 : irf13;

        irf01 = MAX (irf01, irf00);
      }
      else
        irf01 = 1;
    }
    else
      irf01 = 1;

    if (ibface2 > 0)
    {
      ibc2 = (ibcibf != NULL) ? ibcibf[ibface2] : 0;

      id2 = idibf[ibface2];

      if (ibc == ibc2 && id == id2)
      {
        irf20 = irfibf[ibface2];

        irf23 = irf20 / 4;
        irf20 = irf20 - 4 * irf23;
        irf22 = irf20 / 2;
        irf21 = irf20 - irf22 - irf22;

        irf00 = (ibfibf[ibface2][0] == ibface) ? irf21 :
                (ibfibf[ibface2][1] == ibface) ? irf22 : irf23;

        irf02 = MAX (irf02, irf00);
      }
      else
        irf02 = 1;
    }
    else
      irf02 = 1;

    if (ibface3 > 0)
    {
      ibc3 = (ibcibf != NULL) ? ibcibf[ibface3] : 0;

      id3 = idibf[ibface3];

      if (ibc == ibc3 && id == id3)
      {
        irf30 = irfibf[ibface3];

        irf33 = irf30 / 4;
        irf30 = irf30 - 4 * irf33;
        irf32 = irf30 / 2;
        irf31 = irf30 - irf32 - irf32;

        irf00 = (ibfibf[ibface3][0] == ibface) ? irf31 :
                (ibfibf[ibface3][1] == ibface) ? irf32 : irf33;

        irf03 = MAX (irf03, irf00);
      }
      else
        irf03 = 1;
    }
    else
      irf03 = 1;

    irfibf[ibface] = irf01 + irf02 + irf02 + 4 * irf03;
  }

  for (inode = 1; inode <= nnode; ++inode)
  {
    jnin[inode] = 0;
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibface1 = ibfibf[ibface][0];
    ibface2 = ibfibf[ibface][1];
    ibface3 = ibfibf[ibface][2];

    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    if (ibface1 <= 0)
    {
      jnin[inode2] = 1;
      jnin[inode3] = 1;
    }

    if (ibface2 <= 0)
    {
      jnin[inode1] = 1;
      jnin[inode3] = 1;
    }

    if (ibface3 <= 0)
    {
      jnin[inode1] = 1;
      jnin[inode2] = 1;
    }
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibface1 = ibfibf[ibface][0];
    ibface2 = ibfibf[ibface][1];
    ibface3 = ibfibf[ibface][2];

    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];

    jn1 = jnin[inode1];
    jn2 = jnin[inode2];
    jn3 = jnin[inode3];

    irf00 = irfibf[ibface];

    irf03 = irf00 / 4;
    irf00 = irf00 - 4 * irf03;
    irf02 = irf00 / 2;
    irf01 = irf00 - irf02 - irf02;

    if (jn1 == 0 && jn2 && jn3)
      irf01 = 1;

    else if (jn2 == 0 && jn1 && jn3)
      irf02 = 1;

    else if (jn3 == 0 && jn1 && jn2)
      irf03 = 1;

    irfibf[ibface] = irf01 + irf02 + irf02 + 4 * irf03;
  }

  if (ibcibf != NULL)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      if (CHK_FIXED_BL_INT_UG3_GBC (ibcibf[ibface]))
        irfibf[ibface] = 7;
    }
  }

  return;

}
