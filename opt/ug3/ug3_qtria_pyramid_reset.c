#include "UG3_LIB.h"

INT_ ug3_qtria_pyramid_reset
 (INT_ mmsg,
  INT_ *nbface,
  INT_ nbfaced,
  INT_ nquad,
  INT_1D ** ibcibf_ptr,
  INT_1D ** idibf_ptr,
  INT_3D * inibf,
  INT_1D * iqibf,
  INT_1D ** irfibf_ptr)

{

/*
 * Reset quad-faces after mesh generation if the quad-faces were replaced with
 * pyramid elements and tria-faces.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_qtria_pyramid_reset.c,v 1.5 2013/03/16 18:26:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  INT_1D *ibcibf;
  INT_1D *idibf;
  INT_1D *irfibf;

  INT_ ierr, ibface, iquad, jbface, nbfacei, nbfacem;

  ibcibf = *ibcibf_ptr;
  idibf = *idibf_ptr;
  irfibf = *irfibf_ptr;

  if (mmsg == 2)
  {
    sprintf (Text, "Reset Pyramid-Quads: Quad-, Tria-BFaces=%10i%10i", nquad, *nbface);
    ug_message (Text);
  }

  nbfacei = *nbface;
  nbfacem = *nbface + nquad;

  // allocate space for new faces, nodes and pyramids

  if (nbfacem > nbfaced)
  {
    ierr = 0;

    *ibcibf_ptr = (INT_1D *) ug_realloc (&ierr, *ibcibf_ptr, (nbfacem+1) * sizeof (INT_1D));
    *idibf_ptr = (INT_1D *) ug_realloc (&ierr, *idibf_ptr, (nbfacem+1) * sizeof (INT_1D));
    *irfibf_ptr = (INT_1D *) ug_realloc (&ierr, *irfibf_ptr, (nbfacem+1) * sizeof (INT_1D));

    if (ierr > 0)
      return (100318);

    ibcibf = *ibcibf_ptr;
    idibf = *idibf_ptr;
    irfibf = *irfibf_ptr;
  }

  // save BC and ID flags for quad-faces

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    iquad = -iqibf[ibface];

    if (iquad > 0)
    {
      ibcibf[*nbface+iquad] = ibcibf[ibface];
      idibf[*nbface+iquad] = idibf[ibface];
      irfibf[*nbface+iquad] = irfibf[ibface];
    }
  }

  // remove tria-faces created for pyramids

  jbface = 0;

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    if (iqibf[ibface] >= 0)
    {
      ++jbface;

      inibf[jbface][0] = inibf[ibface][0];
      inibf[jbface][1] = inibf[ibface][1];
      inibf[jbface][2] = inibf[ibface][2];

      ibcibf[jbface] = ibcibf[ibface];
      idibf[jbface] = idibf[ibface];
      iqibf[jbface] = 0;
      irfibf[jbface] = irfibf[ibface];
    }
  }

  *nbface = jbface;

  // reorder BC and ID flags for quad-faces

  for (iquad = 1; iquad <= nquad; ++iquad)
  {
    ibcibf[*nbface+iquad] = ibcibf[nbfacei+iquad];
    idibf[*nbface+iquad] = idibf[nbfacei+iquad];
    irfibf[*nbface+iquad] = 0;
  }

  if (mmsg == 2)
  {
    sprintf (Text, "Reset Pyramid-Quads: Quad-, Tria-BFaces=%10i%10i", nquad, *nbface);
    ug_message (Text);
  }

  return (0);

}
