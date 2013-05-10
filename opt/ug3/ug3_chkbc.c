#include "UG3_LIB.h"

INT_ ug3_chkbc
 (INT_ mtask,
  INT_ nbface,
  INT_ nbo,
  INT_ nnodeb,
  INT_1D * ibcibf,
  INT_3D * ibfibf,
  INT_1D * iboibf,
  INT_1D * ierribf)

{

/*
 * Check grid boundary condition flag values.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkbc.c,v 1.6 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibc, ibface, ibfn, ibo, ierr, jbc, jbface, nbc;

  if (ibcibf == NULL)
    return (0);

  // initialize error flag

  ierr = 0;

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ierribf[ibface] = 0;
  }

  // check for invalid grid BC flag

  for (ibface = 1; ibface <= nbface; ++ibface)
  {
    ibc = ibcibf[ibface];

    if (ibc == 0)
      ibcibf[ibface] = 1;

    if (! CHK_VALID_UG3_GBC (ibc))
    {
      ierr = 1;

      ierribf[ibface] = -1;
    }
  }

  // exit if an error was found

  if (ierr)
    return (200307);

  // continue only for task #2

  if (mtask == 2)
  {
    // check for fixed BL intersecting surface face that has a neighbor that is
    // not a fixed BL intersecting surface, BL generating surface or standard
    // solid surface

    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ibc = ibcibf[ibface];

      if (CHK_FIXED_BL_INT_UG3_GBC (ibc))
      {
        for (ibfn = 0; ibfn <= 2; ++ibfn)
        {
          jbface = ibfibf[ibface][ibfn];

          jbc = ibcibf[jbface];

          if (! CHK_FIXED_BL_INT_UG3_GBC (jbc) &&
              ! CHK_STD_BL_UG3_GBC (jbc) &&
              ! CHK_STD_UG3_GBC (jbc))
          {
            ierr = 1;

            ierribf[ibface] = -1;
            ierribf[jbface] = -1;
          }
        }
      }
    }

    // exit if an error was found

    if (ierr == 1)
      return (200305);

    // check that their are no closed bodies that are entirely set to a grid BC
    // of an embedded/transparent BL generating surface

    for (ibo = 1; ibo <= nbo; ++ibo)
    {
      nbc = 0;

      for (ibface = 1; ibface <= nbface; ++ibface)
      {
        if (iboibf[ibface] == ibo)
        {
          ibc = ibcibf[ibface];

          if (CHK_BL_UG3_GBC (ibc) && CHK_TRANSP_UG3_GBC (ibc))
            ++nbc;
        }
      }

      if (nbc == nnodeb)
      {
        ierr = 1;

        for (ibface = 1; ibface <= nbface; ++ibface)
        {
          if (iboibf[ibface] == ibo)
          {
            ibc = ibcibf[ibface];

            if (CHK_BL_UG3_GBC (ibc) && CHK_TRANSP_UG3_GBC (ibc))
              ierribf[ibface] = 1;
          }
        }
      }
    }

    // exit if an error was found

    if (ierr == 1)
      return (200316);
  }

  return (0);

}
