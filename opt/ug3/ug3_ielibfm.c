#include "UG3_LIB.h"

INT_ ug3_ielibfm
 (INT_ nbface,
  INT_ nelem,
  INT_ nelemc5,
  INT_ nelemc6,
  INT_ nelemc8,
  INT_ nquad,
  INT_3D * inibf,
  INT_4D * iniq,
  INT_4D * iniel,
  INT_5D * inielc5,
  INT_6D * inielc6,
  INT_8D * inielc8,
  INT_1D ** idibf_ptr,
  INT_1D ** ielibf_ptr)

{

/*
 *
 * Determine for each boundary face the element that contains the face and an
 * ID flag that identifies the element face orientation.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_ielibfm.c,v 1.3 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 *
 */

  INT_1D *idibf = NULL;
  INT_1D *ielibf = NULL;
  INT_1D *ifjf = NULL;
  INT_1D *ifjn = NULL;

  INT_ Error_Flag, ibface, ibface1, ibface2, ibface3, ibface4, ibface5, ibface6,
       iface, ifg, ifjn1, ifjn2, ifjn3, ifjn4, ifjnm, ielem, inode, inode1,
       inode2, inode3, inode4, inode5, inode6, inode7, inode8, inodem, iquad,
       iquad1, iquad2, iquad3, iquad4, iquad5, iquad6, iquad7, iquad8, jbface,
       jbface1, jbface2, jquad, jquad1, jquad2, mface, nface, nnodeb; 

  mface = MAX (nbface, nquad);
  nface = nbface + nquad;

  nnodeb = 0;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      nnodeb = MAX (inode1, nnodeb);
      nnodeb = MAX (inode2, nnodeb);
      nnodeb = MAX (inode3, nnodeb);
  }

  for (iquad = 1; iquad <= nquad; ++iquad)
  {
      inode1 = iniq[iquad][0];
      inode2 = iniq[iquad][1];
      inode3 = iniq[iquad][2];
      inode4 = iniq[iquad][3];

      nnodeb = MAX (inode1, nnodeb);
      nnodeb = MAX (inode2, nnodeb);
      nnodeb = MAX (inode3, nnodeb);
      nnodeb = MAX (inode4, nnodeb);
  }

  Error_Flag = 0;

  *idibf_ptr = (INT_1D *) ug_malloc (&Error_Flag, (nface+1) * sizeof (INT_1D));
  idibf = *idibf_ptr;
  *ielibf_ptr = (INT_1D *) ug_malloc (&Error_Flag, (nface+1) * sizeof (INT_1D));
  ielibf = *ielibf_ptr;
  ifjf = (INT_1D *) ug_malloc (&Error_Flag, (mface+1) * sizeof (INT_1D));
  ifjn = (INT_1D *) ug_malloc (&Error_Flag, (nnodeb+1) * sizeof (INT_1D));

  if (Error_Flag > 0)
  {
    ug_free (idibf);
    ug_free (ielibf);
    ug_free (ifjf);
    ug_free (ifjn);
    return (100316);
  }

  for (iface = 1; iface <= nface; ++iface)
  {
    ielibf[iface] = 0;
  }

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ifjf[ibface] = ibface;
  }

  jbface2 = 0;

  ifg = 1;

  do
  {
    jbface1 = jbface2 + 1;

    for (inode = 1; inode <= nnodeb; ++inode)
    {
      ifjn[inode] = 1;
    }

    for (jbface = jbface1; jbface <= nbface; ++jbface)
    {
      ibface = ifjf[jbface];

      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      ifjn1 = ifjn[inode1];
      ifjn2 = ifjn[inode2];
      ifjn3 = ifjn[inode3];

      ifjnm = MIN (ifjn1, ifjn2);
      ifjnm = MIN (ifjn3, ifjnm);

      if (ifjnm == 1)
      {
        ++jbface2;

        ibface2 = ifjf[jbface2];

        ifjf[jbface2] = ibface;
        ifjf[jbface] = ibface2;

        ifjn[inode1] = 0;
        ifjn[inode2] = 0;
        ifjn[inode3] = 0;
      }
    }

    for (inode = 1; inode <= nnodeb; ++inode)
    {
      ifjn[inode] = nbface + inode;
    }

    for (jbface = jbface1; jbface <= jbface2; ++jbface)
    {
      ibface = ifjf[jbface];

      inode1 = inibf[ibface][0];
      inode2 = inibf[ibface][1];
      inode3 = inibf[ibface][2];

      ifjn[inode1] = ibface;
      ifjn[inode2] = ibface;
      ifjn[inode3] = ibface;
    }

    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      inode1 = iniel[ielem][0];
      inode2 = iniel[ielem][1];
      inode3 = iniel[ielem][2];
      inode4 = iniel[ielem][3];

      inodem = MIN (inode1, inode2);
      inodem = MIN (inode3, inodem);
      inodem = MIN (inode4, inodem);

      if (inodem <= nnodeb)
      {
        ibface1 = (inode1 <= nnodeb) ? ifjn[inode1] : nbface+inode1;
        ibface2 = (inode2 <= nnodeb) ? ifjn[inode2] : nbface+inode2;
        ibface3 = (inode3 <= nnodeb) ? ifjn[inode3] : nbface+inode3;
        ibface4 = (inode4 <= nnodeb) ? ifjn[inode4] : nbface+inode4;

        if (ibface1 == ibface2 && ibface2 == ibface3)
        {
          ielibf[ibface1] = ielem;

          idibf[ibface1] = 1;
        }

        else if (ibface4 == ibface1 && ibface1 == ibface2)
        {
          ielibf[ibface4] = ielem;

          idibf[ibface4] = 2;
        }

        else if (ibface2 == ibface3 && ibface3 == ibface4)
        {
          ielibf[ibface2] = ielem;

          idibf[ibface2] = 3;
        }

        else if (ibface3 == ibface4 && ibface4 == ibface1)
        {
          ielibf[ibface3] = ielem;

          idibf[ibface3] = 4;
        }
      }
    }

    for (ielem = 1; ielem <= nelemc5; ++ielem)
    {
      inode1 = inielc5[ielem][0];
      inode2 = inielc5[ielem][1];
      inode3 = inielc5[ielem][2];
      inode4 = inielc5[ielem][3];
      inode5 = inielc5[ielem][4];

      inodem = MIN (inode1, inode2);
      inodem = MIN (inode3, inodem);
      inodem = MIN (inode4, inodem);
      inodem = MIN (inode5, inodem);

      if (inodem <= nnodeb)
      {
        ibface1 = (inode1 <= nnodeb) ? ifjn[inode1] : nbface+inode1;
        ibface2 = (inode2 <= nnodeb) ? ifjn[inode2] : nbface+inode2;
        ibface3 = (inode3 <= nnodeb) ? ifjn[inode3] : nbface+inode3;
        ibface4 = (inode4 <= nnodeb) ? ifjn[inode4] : nbface+inode4;
        ibface5 = (inode5 <= nnodeb) ? ifjn[inode5] : nbface+inode5;

        if (ibface1 == ibface2 && ibface2 == ibface3)
        {
          ielibf[ibface1] = nelem+ielem;

          idibf[ibface1] = 2;
        }

        else if (ibface4 == ibface1 && ibface1 == ibface3)
        {
          ielibf[ibface4] = nelem+ielem;

          idibf[ibface4] = 3;
        }

        else if (ibface3 == ibface4 && ibface4 == ibface5)
        {
          ielibf[ibface3] = nelem+ielem;

          idibf[ibface3] = 4;
        }

        else if (ibface2 == ibface3 && ibface3 == ibface5)
        {
          ielibf[ibface2] = nelem+ielem;

          idibf[ibface2] = 5;
        }
      }
    }

    for (ielem = 1; ielem <= nelemc6; ++ielem)
    {
      inode1 = inielc6[ielem][0];
      inode2 = inielc6[ielem][1];
      inode3 = inielc6[ielem][2];
      inode4 = inielc6[ielem][3];
      inode5 = inielc6[ielem][4];
      inode6 = inielc6[ielem][5];

      inodem = MIN (inode1, inode2);
      inodem = MIN (inode3, inodem);
      inodem = MIN (inode4, inodem);
      inodem = MIN (inode5, inodem);
      inodem = MIN (inode6, inodem);

      if (inodem <= nnodeb)
      {
        ibface1 = (inode1 <= nnodeb) ? ifjn[inode1] : nbface+inode1;
        ibface2 = (inode2 <= nnodeb) ? ifjn[inode2] : nbface+inode2;
        ibface3 = (inode3 <= nnodeb) ? ifjn[inode3] : nbface+inode3;
        ibface4 = (inode4 <= nnodeb) ? ifjn[inode4] : nbface+inode4;
        ibface5 = (inode5 <= nnodeb) ? ifjn[inode5] : nbface+inode5;
        ibface6 = (inode6 <= nnodeb) ? ifjn[inode6] : nbface+inode6;

        if (ibface1 == ibface2 && ibface2 == ibface3)
        {
          ielibf[ibface1] = nelem+nelemc5+ielem;

          idibf[ibface1] = 4;
        }

        else if (ibface4 == ibface5 && ibface5 == ibface6)
        {
          ielibf[ibface4] = nelem+nelemc5+ielem;

          idibf[ibface4] = 5;
        }
      }
    }

    ++ifg;
  }
  while (ifg <= nbface && jbface2 < nbface);

  if (nquad > 0)
  {
    for (iquad = 1; iquad <= nquad; ++iquad)
    {
      ifjf[iquad] = iquad;
    }

    jquad2 = 0;

    ifg = 1;

    do
    {
      for (inode = 1; inode <= nnodeb; ++inode)
      {
        ifjn[inode] = 1;
      }

      jquad1 = jquad2 + 1;

      for (jquad = jquad1; jquad <= nquad; ++jquad)
      {
        iquad = ifjf[jquad];

        inode1 = iniq[iquad][0];
        inode2 = iniq[iquad][1];
        inode3 = iniq[iquad][2];
        inode4 = iniq[iquad][3];

        ifjn1 = ifjn[inode1];
        ifjn2 = ifjn[inode2];
        ifjn3 = ifjn[inode3];
        ifjn4 = ifjn[inode4];

        ifjnm = MIN (ifjn1, ifjn2);
        ifjnm = MIN (ifjn3, ifjnm);
        ifjnm = MIN (ifjn4, ifjnm);

        if (ifjnm == 1)
        {
          ++jquad2;

          iquad2 = ifjf[jquad2];

          ifjf[jquad2] = iquad;
          ifjf[jquad] = iquad2;

          ifjn[inode1] = 0;
          ifjn[inode2] = 0;
          ifjn[inode3] = 0;
          ifjn[inode4] = 0;
        }
      }

      for (inode = 1; inode <= nnodeb; ++inode)
      {
        ifjn[inode] = nquad + inode;
      }

      for (jquad = jquad1; jquad <= jquad2; ++jquad)
      {
        iquad = ifjf[jquad];

        inode1 = iniq[iquad][0];
        inode2 = iniq[iquad][1];
        inode3 = iniq[iquad][2];
        inode4 = iniq[iquad][3];

        ifjn[inode1] = iquad;
        ifjn[inode2] = iquad;
        ifjn[inode3] = iquad;
        ifjn[inode4] = iquad;
      }

      for (ielem = 1; ielem <= nelemc5; ++ielem)
      {
        inode1 = inielc5[ielem][0];
        inode2 = inielc5[ielem][1];
        inode3 = inielc5[ielem][2];
        inode4 = inielc5[ielem][3];
        inode5 = inielc5[ielem][4];

        inodem = MIN (inode1, inode2);
        inodem = MIN (inode3, inodem);
        inodem = MIN (inode4, inodem);
        inodem = MIN (inode5, inodem);

        if (inodem <= nnodeb)
        {
          iquad1 = (inode1 <= nnodeb) ? ifjn[inode1] : nquad+inode1;
          iquad2 = (inode2 <= nnodeb) ? ifjn[inode2] : nquad+inode2;
          iquad3 = (inode3 <= nnodeb) ? ifjn[inode3] : nquad+inode3;
          iquad4 = (inode4 <= nnodeb) ? ifjn[inode4] : nquad+inode4;
          iquad5 = (inode5 <= nnodeb) ? ifjn[inode5] : nquad+inode5;

          if (iquad1 == iquad2 && iquad2 == iquad4 && iquad4 == iquad5)
          {
            ielibf[nbface+iquad1] = nelem+ielem;

              idibf[nbface+iquad1] = 1;
          }
        }
      }

      for (ielem = 1; ielem <= nelemc6; ++ielem)
      {
        inode1 = inielc6[ielem][0];
        inode2 = inielc6[ielem][1];
        inode3 = inielc6[ielem][2];
        inode4 = inielc6[ielem][3];
        inode5 = inielc6[ielem][4];
        inode6 = inielc6[ielem][5];

        inodem = MIN (inode1, inode2);
        inodem = MIN (inode3, inodem);
        inodem = MIN (inode4, inodem);
        inodem = MIN (inode5, inodem);
        inodem = MIN (inode6, inodem);

        if (inodem <= nnodeb)
        {
          iquad1 = (inode1 <= nnodeb) ? ifjn[inode1] : nquad+inode1;
          iquad2 = (inode2 <= nnodeb) ? ifjn[inode2] : nquad+inode2;
          iquad3 = (inode3 <= nnodeb) ? ifjn[inode3] : nquad+inode3;
          iquad4 = (inode4 <= nnodeb) ? ifjn[inode4] : nquad+inode4;
          iquad5 = (inode5 <= nnodeb) ? ifjn[inode5] : nquad+inode5;
          iquad6 = (inode6 <= nnodeb) ? ifjn[inode6] : nquad+inode6;

          if (iquad1 == iquad2 && iquad2 == iquad4 && iquad4 == iquad5)
          {
            ielibf[nbface+iquad1] = nelem+nelemc5+ielem;

              idibf[nbface+iquad1] = 1;
          }

          else if (iquad2 == iquad3 && iquad3 == iquad5 && iquad5 == iquad6)
          {
            ielibf[nbface+iquad2] = nelem+nelemc5+ielem;

              idibf[nbface+iquad2] = 2;
          }

          else if (iquad3 == iquad1 && iquad1 == iquad4 && iquad4 == iquad6)
          {
            ielibf[nbface+iquad3] = nelem+nelemc5+ielem;

              idibf[nbface+iquad3] = 3;
          }
        }
      }

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

        inodem = MIN (inode1, inode2);
        inodem = MIN (inode3, inodem);
        inodem = MIN (inode4, inodem);
        inodem = MIN (inode5, inodem);
        inodem = MIN (inode6, inodem);
        inodem = MIN (inode7, inodem);
        inodem = MIN (inode8, inodem);

        if (inodem <= nnodeb)
        {
          iquad1 = (inode1 <= nnodeb) ? ifjn[inode1] : nquad+inode1;
          iquad2 = (inode2 <= nnodeb) ? ifjn[inode2] : nquad+inode2;
          iquad3 = (inode3 <= nnodeb) ? ifjn[inode3] : nquad+inode3;
          iquad4 = (inode4 <= nnodeb) ? ifjn[inode4] : nquad+inode4;
          iquad5 = (inode5 <= nnodeb) ? ifjn[inode5] : nquad+inode5;
          iquad6 = (inode6 <= nnodeb) ? ifjn[inode6] : nquad+inode6;
          iquad7 = (inode7 <= nnodeb) ? ifjn[inode7] : nquad+inode7;
          iquad8 = (inode8 <= nnodeb) ? ifjn[inode8] : nquad+inode8;

          if (iquad1 == iquad5 && iquad5 == iquad2 && iquad2 == iquad6)
          {
            ielibf[nbface+iquad1] = nelem+nelemc5+ielem;

              idibf[nbface+iquad1] = 1;
          }

          else if (iquad2 == iquad3 && iquad3 == iquad6 && iquad6 == iquad7)
          {
            ielibf[nbface+iquad2] = nelem+nelemc5+ielem;

              idibf[nbface+iquad2] = 2;
          }

          else if (iquad3 == iquad4 && iquad4 == iquad7 && iquad7 == iquad8)
          {
            ielibf[nbface+iquad3] = nelem+nelemc5+ielem;

              idibf[nbface+iquad3] = 3;
          }

          else if (iquad4 == iquad1 && iquad1 == iquad5 && iquad5 == iquad8)
          {
            ielibf[nbface+iquad4] = nelem+nelemc5+ielem;

              idibf[nbface+iquad4] = 4;
          }

          else if (iquad1 == iquad2 && iquad2 == iquad3 && iquad3 == iquad4)
          {
            ielibf[nbface+iquad1] = nelem+nelemc5+ielem;

              idibf[nbface+iquad1] = 5;
          }

          else if (iquad6 == iquad5 && iquad5 == iquad7 && iquad7 == iquad8)
          {
            ielibf[nbface+iquad6] = nelem+nelemc5+ielem;

              idibf[nbface+iquad6] = 6;
          }
        }
      }

      ++ifg;
    }
    while (ifg <= nquad && jquad2 < nquad);
  }

  iface = 1;

  do
  {
    ielem = ielibf[iface];

    ++iface;
  }
  while (iface <= nface && ielem > 0);

  ug_free (ifjf);
  ug_free (ifjn);

  if (ielem == 0)
  {
    ug_free (idibf);
    ug_free (ielibf);

    *idibf_ptr = NULL;
    *ielibf_ptr = NULL;

    return (-1);
  }

  return (0);

}
