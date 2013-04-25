#include "UG3_LIB.h"

INT_ ug3_nstv
 (double *nv1,
  double *nv2,
  double *nv3,
  double nvtol,
  double *sv1,
  double *sv2,
  double *sv3,
  double *tv1,
  double *tv2,
  double *tv3)

{

/*
 * Normalize and check a given normal vector and then create a suitable set of
 * orthogonal tangential vectors.
 *
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_nstv.c,v 1.4 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */  

  double dc0, dc1, nv1m, nv2m, nv3m, nvminm, w;

  dc0 = 0.0;
  dc1 = 1.0;

  w = sqrt ((*nv1) * (*nv1) + (*nv2) * (*nv2) + (*nv3) * (*nv3));

  if (w < nvtol)
    return (329);

  w = dc1 / w;

  *nv1 = w * (*nv1);
  *nv2 = w * (*nv2);
  *nv3 = w * (*nv3);

  nv1m = fabs (*nv1);
  nv2m = fabs (*nv2);
  nv3m = fabs (*nv3);

  nvminm = MIN (nv1m, nv2m);
  nvminm = MIN (nvminm, nv3m);

  if (nv1m == nvminm)
  {
    *sv1 = dc1;
    *sv2 = dc0;
    *sv3 = dc0;
  }
  else if (nv2m == nvminm)
  {
    *sv1 = dc0;
    *sv2 = dc1;
    *sv3 = dc0;
  }
  else 
  {
    *sv1 = dc0;
    *sv2 = dc0;
    *sv3 = dc1;
  }

  *tv1 = (*nv2) * (*sv3) - (*nv3) * (*sv2);
  *tv2 = (*nv3) * (*sv1) - (*nv1) * (*sv3);
  *tv3 = (*nv1) * (*sv2) - (*nv2) * (*sv1);

  w = dc1 / sqrt ((*tv1) * (*tv1) + (*tv2) * (*tv2) + (*tv3) * (*tv3));

  *tv1 = w * (*tv1);
  *tv2 = w * (*tv2);
  *tv3 = w * (*tv3);

  *sv1 = (*tv2) * (*nv3) - (*tv3) * (*nv2);
  *sv2 = (*tv3) * (*nv1) - (*tv1) * (*nv3);
  *sv3 = (*tv1) * (*nv2) - (*tv2) * (*nv1);

  return (0);

}
