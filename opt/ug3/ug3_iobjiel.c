#include "UG3_LIB.h"

void ug3_iobjiel
 (INT_ mmsg,
  INT_ nelem,
  INT_ *nobj,
  INT_4D * ieliel,
  INT_1D * mchkel,
  INT_1D * iobjiel)

{

/*
 * Determine the number of solid objects.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_iobjiel.c,v 1.3 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_ ichk, ichk1, ichk2, ielem, ieln, iobj, jchk, jelem, mfound;

  if (iobjiel == NULL)
    return;

  for (ielem = 1; ielem <= nelem; ++ielem)
  {
    iobjiel[ielem] = 0;
  }

  ielem = 1;

  iobj = 0;

  do
  {
    ++iobj;

    ichk = 1;

    mchkel[ichk] = ielem;

    iobjiel[ielem] = iobj;

    ichk2 = 0;

    do
    {
      ichk1 = ichk2 + 1;
      ichk2 = ichk;

      for (jchk = ichk1; jchk <= ichk2; ++jchk)
      {
        jelem = mchkel[jchk];

        for (ieln = 0; ieln <= 3; ++ieln)
        {
          ielem = ieliel[jelem][ieln];

	  if (ielem > 0)
	  {
            if (iobjiel[ielem] == 0)
            {
              ++ichk;

              mchkel[ichk] = ielem;

              iobjiel[ielem] = iobj;
            }
          }
        }
      }
    }
    while (ichk > ichk2);

    mfound = 0;

    ielem = 0;

    do
    {
      ++ielem;

      if (iobjiel[ielem] == 0)
        mfound = 1;
    }
    while (ielem < nelem && mfound == 0);
  }
  while (mfound == 1);

  *nobj = iobj;

  if (mmsg == 2)
  {
    sprintf (Text, "Solid Objects      : Objects           =%10i", *nobj);
    ug_message (Text);
  }

  return;

}
