#include "UG3_LIB.h"

static void ug3_bvls_set_tol (void);

/*
 * Routines to initialize, sum and solve least-squares optimization equations
 * for determining boundary surface normal vectors.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_bvls.c,v 1.25 2013/04/05 04:04:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

static INT_ set_tol = 0;

static double bvdiffmin = 0.001; // minimum normal vector diff. - about 0.1 deg
static double coffset = 0.1; // offset coefficient
static double dettol = 0.001; // determinant tolerance
static double wddtols = 0.001; // normal sum tolerance squared - about 1.8 deg

static double small; // small number
static double tol; // round-off error tolerance
static double artr; // solution accuracy to round-off error tolerance ratio

void ug3_bvls_set_tol (void)

{

/*
 * Initialize tolerances.
 */

  double csmall, cstol, ctol, dc1d2, tol0;

  dc1d2 = 0.5;

  set_tol = 1;

  csmall = 0.5; // small number coefficient
  cstol = 0.5; // solution accuracy coefficient
  ctol = 0.94; // round-off error coefficient

  ug_round_off_error (&tol0);

  small = pow (tol0, csmall);
  tol = pow (tol0, ctol);
  artr = dc1d2 * pow (tol0, cstol-ctol);

  return;

}

INT_ ug3_bvls
 (INT_ ibface,
  INT_ mnorm,
  INT_ mtask,
  INT_3D * inibf,
  double *bv1,
  double *bv2,
  double *bv3,
  DOUBLE_3D * x)

{

/*
 * Determine a normal vector for a given set of boundary surface faces using a
 * least-squares optimization.
 */

  static INT_ jbface = 0;
  static double c11_, c12_, c13_, c22_, c23_, c33_, c1_, c2_, c3_;

  INT_ inode1, inode2, inode3, inode4;
  INT_ ierr = 0;

  double c11, c12, c13, c22, c23, c33, c1, c2, c3, dc0;

  dc0 = 0.0;

  if (mtask == 0)
  {
    jbface = 0;

    c11_ = dc0;
    c12_ = dc0;
    c13_ = dc0;
    c22_ = dc0;
    c23_ = dc0;
    c33_ = dc0;

    c1_ = dc0;
    c2_ = dc0;
    c3_ = dc0;
  }

  else if (mtask == 1 && ibface > 0)
  {
    jbface = ibface;

    inode1 = inibf[ibface][0];
    inode2 = inibf[ibface][1];
    inode3 = inibf[ibface][2];
    inode4 = 0;

    ug3_bvls_coeff (inode1, inode2, inode3, inode4, mnorm, 
                    &c11, &c12, &c13, &c22, &c23, &c33, &c1, &c2, &c3, x);

    c11_ = c11_ + c11;
    c12_ = c12_ + c12;
    c13_ = c13_ + c13;
    c22_ = c22_ + c22;
    c23_ = c23_ + c23;
    c33_ = c33_ + c33;

    c1_ = c1_ + c1;
    c2_ = c2_ + c2;
    c3_ = c3_ + c3;
  }

  else if (mtask == 2 && jbface)
  {
    inode1 = inibf[jbface][0];
    inode2 = inibf[jbface][1];
    inode3 = inibf[jbface][2];
    inode4 = 0;

    ierr = ug3_bvls_bv (inode1, inode2, inode3, inode4, 
                        bv1, bv2, bv3, 
                        c11_, c12_, c13_, c22_, c23_, c33_, c1_, c2_, c3_, x);
  }

  else
    ierr = 1;

  return (ierr);

}

void ug3_bvls_coeff
 (INT_ inode1,
  INT_ inode2,
  INT_ inode3,
  INT_ inode4,
  INT_ mnorm,
  double *c11,
  double *c12,
  double *c13,
  double *c22,
  double *c23,
  double *c33,
  double *c1,
  double *c2,
  double *c3,
  DOUBLE_3D * x)

{

/*
 * Sum least-squares optimization equation coefficients for determining boundary
 * surface normal vectors.
 */

  double bfv1, bfv2, bfv3, dc1, dx211, dx212, dx213, dx311, dx312, dx313, dx411,
         dx412, dx413, w, x11, x12, x13, x21, x22, x23, x31, x32, x33, x41, x42,
         x43;

  dc1 = 1.0;

  // if node 1 is set then determine boundary face normal vector 

  if (inode1)
  {
    x11 = x[inode1][0];
    x12 = x[inode1][1];
    x13 = x[inode1][2];
    x21 = x[inode2][0];
    x22 = x[inode2][1];
    x23 = x[inode2][2];
    x31 = x[inode3][0];
    x32 = x[inode3][1];
    x33 = x[inode3][2];

    dx211 = x21 - x11;
    dx212 = x22 - x12;
    dx213 = x23 - x13;
    dx311 = x31 - x11;
    dx312 = x32 - x12;
    dx313 = x33 - x13;

    // un-normalized boundary face normal vector

    bfv1 = dx212 * dx313 - dx213 * dx312;
    bfv2 = dx213 * dx311 - dx211 * dx313;
    bfv3 = dx211 * dx312 - dx212 * dx311;

    // add un-normalized boundary face normal vector for a quad face

    if (inode4 > 0)
    {
      x41 = x[inode4][0];
      x42 = x[inode4][1];
      x43 = x[inode4][2];

      dx411 = x41 - x11;
      dx412 = x42 - x12;
      dx413 = x43 - x13;

      bfv1 = bfv1 + dx312 * dx413 - dx313 * dx412;
      bfv2 = bfv2 + dx313 * dx411 - dx311 * dx413;
      bfv3 = bfv3 + dx311 * dx412 - dx312 * dx411;
    }

    // normalize boundary face normal vector

    if (mnorm)
    {
      w = dc1 / sqrt (bfv1 * bfv1 + bfv2 * bfv2 + bfv3 * bfv3);

      bfv1 = w * bfv1;
      bfv2 = w * bfv2;
      bfv3 = w * bfv3;
    }

    *c1 = bfv1;
    *c2 = bfv2;
    *c3 = bfv3;
  }
  else
  {
    bfv1 = *c1;
    bfv2 = *c2;
    bfv3 = *c3;
  }

  *c11 = bfv1 * bfv1;
  *c12 = bfv1 * bfv2;
  *c13 = bfv1 * bfv3;
  *c22 = bfv2 * bfv2;
  *c23 = bfv2 * bfv3;
  *c33 = bfv3 * bfv3;

  return;

}

void ug3_bvls_mdd
 (INT_ *mdd,
  INT_ nsum,
  double c1,
  double c2,
  double c3)

{

/*
 * Set discontinuity flag at boundary surface points from the sum of boundary
 * surface face normal vectors attached to that point. If the point is very
 * discontinuous the sum will be very small and the flag will be set to 1.
 * Otherwise it is set to 0.
 */

  double w;

  w = (c1 * c1 + c2 * c2 + c3 * c3) / ((double) (nsum * nsum));

  *mdd = (w <= wddtols) ? 1: 0;

  return;

}

void ug3_bvls_mdd_coeff
 (INT_ inode1,
  INT_ inode2,
  INT_ inode3,
  INT_ inode4,
  INT_ mdd1,
  INT_ mdd2,
  INT_ mdd3,
  INT_ mdd4,
  double *c11,
  double *c12,
  double *c13,
  double *c22,
  double *c23,
  double *c33,
  double *c1,
  double *c2,
  double *c3,
  DOUBLE_3D * x)

{

/*
 * Modify least-squares optimization equation coefficients for determining
 * boundary surface normal vectors at all very discontinuous boundary surface
 * points (as defined by the mdd discontinuity flag).
 */

  INT_ mdd0;

  double a1, a2, a3, bfv1, bfv2, bfv3, d1, d2, d3, dc0, dc1, dc3, dc1d2, dc1d3,
         dx211, dx212, dx213, dx311, dx312, dx313, dx411, dx412, dx413, w, x11,
         x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc3 = 3.0;
  dc1d2 = 0.5;

  dc1d3 = dc1 / dc3;

  if (inode1)
  {
    if (! set_tol) ug3_bvls_set_tol ();

    x11 = x[inode1][0];
    x12 = x[inode1][1];
    x13 = x[inode1][2];
    x21 = x[inode2][0];
    x22 = x[inode2][1];
    x23 = x[inode2][2];
    x31 = x[inode3][0];
    x32 = x[inode3][1];
    x33 = x[inode3][2];

    dx211 = x21 - x11;
    dx212 = x22 - x12;
    dx213 = x23 - x13;
    dx311 = x31 - x11;
    dx312 = x32 - x12;
    dx313 = x33 - x13;

    a1 = dx212 * dx313 - dx213 * dx312;
    a2 = dx213 * dx311 - dx211 * dx313;
    a3 = dx211 * dx312 - dx212 * dx311;

    if (inode4)
    {
      x41 = x[inode4][0];
      x42 = x[inode4][1];
      x43 = x[inode4][2];

      dx411 = x41 - x11;
      dx412 = x42 - x12;
      dx413 = x43 - x13;

      a1 = a1 + dx312 * dx413 - dx313 * dx412;
      a2 = a2 + dx313 * dx411 - dx311 * dx413;
      a3 = a3 + dx311 * dx412 - dx312 * dx411;
    }

    w = dc1 / sqrt (a1 * a1 + a2 * a2 + a3 * a3);

    bfv1 = w * a1;
    bfv2 = w * a2;
    bfv3 = w * a3;

    mdd0 = mdd1 + mdd2 + mdd3 + mdd4;

    if (inode4 == 0 && mdd0 <= 2)
    {
      if (mdd1 == 0)
      {
        d1 = dc0;
        d2 = dc0;
        d3 = dc0;

        if (mdd2 == 1)
        {
          d1 = d1 + dx211;
          d2 = d2 + dx212;
          d3 = d3 + dx213;
        }

        if (mdd3 == 1)
        {
          d1 = d1 + dx311;
          d2 = d2 + dx312;
          d3 = d3 + dx313;
        }

        if (mdd2 && mdd3)
        {
          d1 = dc1d2 * d1;
          d2 = dc1d2 * d2;
          d3 = dc1d2 * d3;
        }

        // offset coordinate above surface

        w = coffset * sqrt (d1 * d1 + d2 * d2 + d3 * d3);

        x11 = x11 + w * bfv1;
        x12 = x12 + w * bfv2;
        x13 = x13 + w * bfv3;
      }

      if (mdd2 == 0)
      {
        d1 = dc0;
        d2 = dc0;
        d3 = dc0;

        if (mdd1 == 1)
        {
          d1 = d1 - dx211;
          d2 = d2 - dx212;
          d3 = d3 - dx213;
        }

        if (mdd3 == 1)
        {
          d1 = d1 + dx311 - dx211;
          d2 = d2 + dx312 - dx212;
          d3 = d3 + dx313 - dx213;
        }

        if (mdd1 && mdd3)
        {
          d1 = dc1d2 * d1;
          d2 = dc1d2 * d2;
          d3 = dc1d2 * d3;
        }

        // offset coordinate above surface

        w = coffset * sqrt (d1 * d1 + d2 * d2 + d3 * d3);

        x21 = x21 + w * bfv1;
        x22 = x22 + w * bfv2;
        x23 = x23 + w * bfv3;
      }

      if (mdd3 == 0)
      {
        d1 = dc0;
        d2 = dc0;
        d3 = dc0;

        if (mdd1 == 1)
        {
          d1 = d1 - dx311;
          d2 = d2 - dx312;
          d3 = d3 - dx313;
        }

        if (mdd2 == 1)
        {
          d1 = d1 + dx211 - dx311;
          d2 = d2 + dx212 - dx312;
          d3 = d3 + dx213 - dx313;
        }

        if (mdd1 && mdd2)
        {
          d1 = dc1d2 * d1;
          d2 = dc1d2 * d2;
          d3 = dc1d2 * d3;
        }

        // offset coordinate above surface

        w = coffset * sqrt (d1 * d1 + d2 * d2 + d3 * d3);

        x31 = x31 + w * bfv1;
        x32 = x32 + w * bfv2;
        x33 = x33 + w * bfv3;
      }
    }

    else if (mdd0 <= 3)
    {
      if (mdd1 == 0)
      {
        d1 = dc0;
        d2 = dc0;
        d3 = dc0;

        if (mdd2 == 1)
        {
          d1 = d1 + dx211;
          d2 = d2 + dx212;
          d3 = d3 + dx213;
        }

        if (mdd3 == 1 && mdd0 != 2)
        {
          d1 = d1 + dx311;
          d2 = d2 + dx312;
          d3 = d3 + dx313;
        }

        if (mdd4 == 1)
        {
          d1 = d1 + dx411;
          d2 = d2 + dx412;
          d3 = d3 + dx413;
        }

        if (mdd2 && mdd3 && mdd4)
        {
          d1 = dc1d3 * d1;
          d2 = dc1d3 * d2;
          d3 = dc1d3 * d3;
        }

        // offset coordinate above surface

        w = coffset * sqrt (d1 * d1 + d2 * d2 + d3 * d3);

        x11 = x11 + w * bfv1;
        x12 = x12 + w * bfv2;
        x13 = x13 + w * bfv3;
      }

      if (mdd2 == 0)
      {
        d1 = dc0;
        d2 = dc0;
        d3 = dc0;

        if (mdd1 == 1)
        {
          d1 = d1 - dx211;
          d2 = d2 - dx212;
          d3 = d3 - dx213;
        }

        if (mdd3 == 1)
        {
          d1 = d1 + dx311 - dx211;
          d2 = d2 + dx312 - dx212;
          d3 = d3 + dx313 - dx213;
        }

        if (mdd4 == 1 && mdd0 != 2)
        {
          d1 = d1 + dx411 - dx211;
          d2 = d2 + dx412 - dx212;
          d3 = d3 + dx413 - dx213;
        }

        if (mdd1 && mdd3 && mdd4)
        {
          d1 = dc1d3 * d1;
          d2 = dc1d3 * d2;
          d3 = dc1d3 * d3;
        }

        // offset coordinate above surface

        w = coffset * sqrt (d1 * d1 + d2 * d2 + d3 * d3);

        x21 = x21 + w * bfv1;
        x22 = x22 + w * bfv2;
        x23 = x23 + w * bfv3;
      }

      if (mdd3 == 0)
      {
        d1 = dc0;
        d2 = dc0;
        d3 = dc0;

        if (mdd1 == 1 && mdd0 != 2)
        {
          d1 = d1 - dx311;
          d2 = d2 - dx312;
          d3 = d3 - dx313;
        }

        if (mdd2 == 1)
        {
          d1 = d1 + dx211 - dx311;
          d2 = d2 + dx212 - dx312;
          d3 = d3 + dx213 - dx313;
        }

        if (mdd4 == 1)
        {
          d1 = d1 - dx411;
          d2 = d2 - dx412;
          d3 = d3 - dx413;
        }

        if (mdd1 && mdd2 && mdd4)
        {
          d1 = dc1d3 * d1;
          d2 = dc1d3 * d2;
          d3 = dc1d3 * d3;
        }

        // offset coordinate above surface

        w = coffset * sqrt (d1 * d1 + d2 * d2 + d3 * d3);

        x31 = x31 + w * bfv1;
        x32 = x32 + w * bfv2;
        x33 = x33 + w * bfv3;
      }

      if (mdd4 == 0)
      {
        d1 = dc0;
        d2 = dc0;
        d3 = dc0;

        if (mdd1 == 1)
        {
          d1 = d1 - dx411;
          d2 = d2 - dx412;
          d3 = d3 - dx413;
        }

        if (mdd2 == 1 && mdd0 != 2)
        {
          d1 = d1 + dx211 - dx411;
          d2 = d2 + dx212 - dx412;
          d3 = d3 + dx213 - dx413;
        }

        if (mdd3 == 1)
        {
          d1 = d1 + dx311 - dx411;
          d2 = d2 + dx312 - dx412;
          d3 = d3 + dx313 - dx413;
        }

        if (mdd1 && mdd2 && mdd3)
        {
          d1 = dc1d3 * d1;
          d2 = dc1d3 * d2;
          d3 = dc1d3 * d3;
        }

        // offset coordinate above surface

        w = coffset * sqrt (d1 * d1 + d2 * d2 + d3 * d3);

        x41 = x41 + w * bfv1;
        x42 = x42 + w * bfv2;
        x43 = x43 + w * bfv3;
      }
    }

    dx211 = x21 - x11;
    dx212 = x22 - x12;
    dx213 = x23 - x13;
    dx311 = x31 - x11;
    dx312 = x32 - x12;
    dx313 = x33 - x13;

    a1 = dx212 * dx313 - dx213 * dx312;
    a2 = dx213 * dx311 - dx211 * dx313;
    a3 = dx211 * dx312 - dx212 * dx311;

    if (inode4)
    {
      dx411 = x41 - x11;
      dx412 = x42 - x12;
      dx413 = x43 - x13;

      a1 = a1 + dx312 * dx413 - dx313 * dx412;
      a2 = a2 + dx313 * dx411 - dx311 * dx413;
      a3 = a3 + dx311 * dx412 - dx312 * dx411;
    }

    w = dc1 / sqrt (a1 * a1 + a2 * a2 + a3 * a3);

    bfv1 = w * a1;
    bfv2 = w * a2;
    bfv3 = w * a3;

    *c1 = bfv1;
    *c2 = bfv2;
    *c3 = bfv3;
  }
  else
  {
    bfv1 = *c1;
    bfv2 = *c2;
    bfv3 = *c3;
  }

  *c11 = bfv1 * bfv1;
  *c12 = bfv1 * bfv2;
  *c13 = bfv1 * bfv3;
  *c22 = bfv2 * bfv2;
  *c23 = bfv2 * bfv3;
  *c33 = bfv3 * bfv3;

  return;

}

INT_ ug3_bvls_bv
 (INT_ inode1,
  INT_ inode2,
  INT_ inode3,
  INT_ inode4,
  double *bv1,
  double *bv2,
  double *bv3,
  double c11,
  double c12,
  double c13,
  double c22,
  double c23,
  double c33,
  double c1,
  double c2,
  double c3,
  DOUBLE_3D * x)

{

/*
 * Determine a normal vector for a given set of boundary surface faces using
 * previously computed sumations of the least-squares optimization equation
 * coefficients.
 */

  INT_ ierr;

  double a1, a11, a12, a13, a2, a22, a23, a3, a33,
         b1, b2, b3, bm, bvdiff, cm, det, dc0, dc1, dc2,
         dx211, dx212, dx213, dx311, dx312, dx313, dx411, dx412, dx413, 
         rsnorm, rsnormc, rsnormi, sum1, sum2, sum3, w,
         x11, x12, x13, x21, x22, x23, x31, x32, x33, x41, x42, x43;

  dc0 = 0.0;
  dc1 = 1.0;
  dc2 = 2.0;

  ierr = 0;

  if (! set_tol) ug3_bvls_set_tol ();

  if (inode1)
  {
    x11 = x[inode1][0];
    x12 = x[inode1][1];
    x13 = x[inode1][2];
    x21 = x[inode2][0];
    x22 = x[inode2][1];
    x23 = x[inode2][2];
    x31 = x[inode3][0];
    x32 = x[inode3][1];
    x33 = x[inode3][2];

    dx211 = x21 - x11;
    dx212 = x22 - x12;
    dx213 = x23 - x13;
    dx311 = x31 - x11;
    dx312 = x32 - x12;
    dx313 = x33 - x13;

    b1 = dx212 * dx313 - dx213 * dx312;
    b2 = dx213 * dx311 - dx211 * dx313;
    b3 = dx211 * dx312 - dx212 * dx311;

    if (inode4)
    {
      x41 = x[inode4][0];
      x42 = x[inode4][1];
      x43 = x[inode4][2];

      dx411 = x41 - x11;
      dx412 = x42 - x12;
      dx413 = x43 - x13;

      b1 = b1 + dx312 * dx413 - dx313 * dx412;
      b2 = b2 + dx313 * dx411 - dx311 * dx413;
      b3 = b3 + dx311 * dx412 - dx312 * dx411;
    }

    bm = sqrt (b1 * b1 + b2 * b2 + b3 * b3);
    cm = sqrt (c1 * c1 + c2 * c2 + c3 * c3);

    w = cm - small * bm;

    if (w > dc0)
    {
      w = dc1 / cm;

      a1 = w * c1;
      a2 = w * c2;
      a3 = w * c3;

      w = dc1 / bm;

      b1 = w * b1;
      b2 = w * b2;
      b3 = w * b3;

      bvdiff = dc1 - b1 * a1 - b2 * a2 - b3 * a3;
      bvdiff = fabs (bvdiff);

      if (bvdiff < bvdiffmin)
        ierr = -1;
    }
    else
      ierr = -2;
  }

  if (ierr == 0)
  {
    sum1 = fabs (c1);
    sum2 = fabs (c2);
    sum3 = fabs (c3);

    rsnormc = MAX (sum1, sum2);
    rsnormc = MAX (rsnormc, sum3);

    a11 = c22 * c33 - c23 * c23;
    a22 = c11 * c33 - c13 * c13;
    a33 = c11 * c22 - c12 * c12;
    a12 = c13 * c23 - c33 * c12;
    a13 = c12 * c23 - c22 * c13;
    a23 = c12 * c13 - c11 * c23;

    det = fabs (c11 * a11 + c12 * a12 + c13 * a13);

    w = det - rsnormc * dettol;

    if (w > dc0)
    {
      sum1 = fabs (c11) + fabs (c12) + fabs (c13);
      sum2 = fabs (c12) + fabs (c22) + fabs (c23);
      sum3 = fabs (c13) + fabs (c23) + fabs (c33);

      rsnorm = MAX (sum1, sum2);
      rsnorm = MAX (rsnorm, sum3);

      sum1 = fabs (a11) + fabs (a12) + fabs (a13);
      sum2 = fabs (a12) + fabs (a22) + fabs (a23);
      sum3 = fabs (a13) + fabs (a23) + fabs (a33);

      rsnormi = MAX (sum1, sum2);
      rsnormi = MAX (rsnormi, sum3);
      //rsnormi = rsnormi / det;

      w = det * artr - rsnorm * rsnormi * rsnormc;

      if (w > dc0)
      {
        *bv1 = c1 * a11 + c2 * a12 + c3 * a13;
        *bv2 = c1 * a12 + c2 * a22 + c3 * a23;
        *bv3 = c1 * a13 + c2 * a23 + c3 * a33;
      }
      else
        ierr = -3;
    }
    else
      ierr = -4;
  }

  if (ierr)
  {
    *bv1 = c1;
    *bv2 = c2;
    *bv3 = c3;
  }

  w = dc1 / sqrt ((*bv1) * (*bv1) + (*bv2) * (*bv2) + (*bv3) * (*bv3));

  *bv1 = w * (*bv1);
  *bv2 = w * (*bv2);
  *bv3 = w * (*bv3);

  return (ierr);

}
