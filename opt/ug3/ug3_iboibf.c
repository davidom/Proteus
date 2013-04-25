#include "UG3_LIB.h"

void ug3_iboibf
 (INT_ mmsg,
  INT_ nbface,
  INT_ *nbo,
  INT_3D * ibfibf,
  INT_1D * mchkbf,
  INT_1D * iboibf)

{

/*
 * Determine the number of closed boundary objects.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_iboibf.c,v 1.12 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ibface, ibfn, ibo, ichk, ichk1, ichk2, jbface, jchk, mfound;

  if (iboibf == NULL)
    return;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    iboibf[ibface] = 0;
  }

  ibface = 1;

  ibo = 0;

  do
  {
    ++ibo;

    ichk = 1;

    mchkbf[ichk] = ibface;

    iboibf[ibface] = ibo;

    ichk2 = 0;

    do
    {
      ichk1 = ichk2 + 1;
      ichk2 = ichk;

      for (jchk = ichk1; jchk <= ichk2; ++jchk)
      {
        jbface = mchkbf[jchk];

        for (ibfn = 0; ibfn <= 2; ++ibfn)
        {
          ibface = ibfibf[jbface][ibfn];

	  if (ibface > 0)
	  {
            if (iboibf[ibface] == 0)
            {
              ++ichk;

              mchkbf[ichk] = ibface;

              iboibf[ibface] = ibo;
            }
          }
        }
      }
    }
    while (ichk > ichk2);

    mfound = 0;

    ibface = 0;

    do
    {
      ++ibface;

      if (iboibf[ibface] == 0)
        mfound = 1;
    }
    while (ibface < nbface && mfound == 0);
  }
  while (mfound == 1);

  *nbo = ibo;

  if (mmsg == 2)
  {
    sprintf (Text, "Boundary Objects   : Objects           =%10i", *nbo);
    ug_message (Text);
  }

  return;

}
