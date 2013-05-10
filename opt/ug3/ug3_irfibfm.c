#include "UG3_LIB.h"

void ug3_irfibfm
 (INT_ mtransp,
  INT_ nbface,
  INT_1D * ibcibf,
  INT_1D * irfibf)

{

/*
 * Reset the boundary face reconnection flag at partition boundary surface faces
 * and at transparent boundary surface faces.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_irfibfm.c,v 1.2 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ibc, ibface;

  if (mtransp)
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ibc = ibcibf[ibface];

      if (CHK_PART_UG3_GBC (ibc))
        irfibf[ibface] = 7;

      else if (CHK_TRANSP_UG3_GBC (ibc))
        irfibf[ibface] = 0;
    }
  }
  else
  {
    for (ibface = 1; ibface <= nbface; ++ibface)
    {
      ibc = ibcibf[ibface];

      if (CHK_PART_UG3_GBC (ibc) || CHK_TRANSP_UG3_GBC (ibc))
        irfibf[ibface] = 7;
    }
  }
  
  return;

}
