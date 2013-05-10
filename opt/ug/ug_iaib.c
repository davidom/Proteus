#include "UG_LIB.h"

void ug_iaib
 (INT_ na,
  INT_ nb,
  INT_1D * ibia,
  INT_1D * iaib,
  INT_1D * liaib)

{

/*
 * Create an inverse list.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_iaib.c,v 1.3 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ia, ib, ib0, loc, nadpb;

  for (ib = 1; ib <= nb; ++ib)
  {
    liaib[ib] = 0;
  }

  ib0 = nb;

  for (ia = 1; ia <= na; ++ia)
  {
    ib = ibia[ia];

    if (ib > 0)
    {
      ib0 = MIN (ib0, ib);

      liaib[ib] = liaib[ib] + 1;
    }
  }

  loc = 1;

  nadpb = 0;

  for (ib = ib0; ib <= nb; ++ib)
  {
    loc = loc + nadpb;

    nadpb = liaib[ib];

    liaib[ib] = loc;
  }

  for (ia = 1; ia <= na; ++ia)
  {
    ib = ibia[ia];

    if (ib > 0)
    {
      loc = liaib[ib];

      iaib[loc] = ia;

      ++loc;

      liaib[ib] = loc;
    }
  }

  for (ib = nb + 1; ib > ib0; --ib)
  {
    liaib[ib] = liaib[ib-1];
  }

  for (ib = 1; ib <= ib0; ++ib)
  {
    liaib[ib] = 1;
  }

  return;

}
