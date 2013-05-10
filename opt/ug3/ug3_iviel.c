#include "UG3_LIB.h"

void ug3_iviel
 (INT_ nelem,
  INT_ *nv,
  INT_4D * ieliel,
  INT_1D * ielis,
  INT_1D * iviel)

{

/*
 * Label elements that are fully connected as a single volume.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_iviel.c,v 1.5 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ielem, ieln, iit, is, is1, is2, iv, jelem, js, jv, nit;

  if (*nv == 1)
  {
    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      if (iviel[ielem] >= 1)
        iviel[ielem] = 0;
      else
        iviel[ielem] = -1;
    }
  }
  else
  {
    for (ielem = 1; ielem <= nelem; ++ielem)
    {
      iviel[ielem] = 0;
    }
  }

  *nv = nelem;

  iv = 0;

  do
  {
    ielem = 1;

    do
    {
      jv = iviel[ielem];

      ++ielem;
    }
    while (ielem <= nelem && jv != 0);

    --ielem;

    if (jv == 0)
    {
      ++iv;

      is2 = -1;

      iviel[ielem] = iv;

      is = 0;

      ielis[is] = ielem;

      nit = nelem;

      iit = 1;

      do
      {
        is1 = is2+1;
        is2 = is;

        for (js = is1; js <= is2; ++js)
        {
          jelem = ielis[js];

          for (ieln = 0; ieln <= 3; ++ieln)
          {
            ielem = ieliel[jelem][ieln];

            if (ielem > 0)
            {
              if (iviel[ielem] == 0)
              {
                iviel[ielem] = iv;

                ++is;

                ielis[is] = ielem;
              }
            }
          }
        }

        ++iit;
      }
      while (iit <= nit && is > is2);
    }
  }
  while (iv <= *nv && jv == 0);

  *nv = iv;

  return;

}
