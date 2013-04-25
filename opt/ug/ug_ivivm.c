#include "UG_LIB.h"

INT_ ug_ivivm
 (INT_ nj,
  INT_ *nivm,
  INT_1D * ivij,
  INT_1D ** ivivm_ptr)

{

/*
 * Set map of non-sequential values to sequential values.
 * 
 * UG LIB : Unstructured Griv - General Purpose Routine Library
 * $Id: ug_ivivm.c,v 1.2 2012/11/08 23:15:25 marcum Exp $
 * Copyright 1994-2012, Daviv L. Marcum
 */

  INT_1D *ivivm = NULL;

  INT_ ierr, ij, iv, ivm, ivmax, ivmin, miv;

  ivmax = ivij[1];
  ivmin = ivij[1];

  for (ij = 2; ij <= nj; ++ij)
  {
    ivmax = MAX (ivmax, ivij[ij]);
    ivmin = MIN (ivmin, ivij[ij]);
  }

  ivm =0;

  for (iv = ivmin; iv <= ivmax; ++iv)
  {
    miv = 0;

    ij = 1;

    while (ij <= nj && ! miv)
    {
      if (iv == ivij[ij])
        miv = 1;

      ++ij;
    }

    if (miv == 1)
      ++ivm;
  }

  *nivm = ivm;

  ierr = 0;

  *ivivm_ptr = (INT_1D *) ug_malloc (&ierr, (*nivm+1) * sizeof (INT_1D));

  if (ierr > 0)
    return (100412);

  ivivm = *ivivm_ptr;

  ivm =0;

  for (iv = ivmin; iv <= ivmax; ++iv)
  {
    miv = 0;

    ij = 1;

    while (ij <= nj && ! miv)
    {
      if (iv == ivij[ij])
        miv = 1;

      ++ij;
    }

    if (miv == 1)
    {
      ++ivm;

      ivivm[ivm] = iv;
    }
  }

  return (0);

}
