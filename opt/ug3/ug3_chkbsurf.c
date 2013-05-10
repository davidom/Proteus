#include "UG3_LIB.h"

INT_ ug3_chkbsurf
 (INT_ mclosed,
  INT_ merr,
  INT_ mmsg,
  INT_ mmultc,
  INT_ mopen,
  INT_ mrecbdw,
  INT_ mrecbf,
  INT_ mrecbm,
  INT_ msetbc,
  INT_ mtrv,
  INT_ mtrw,
  INT_ mwbcsurf,
  INT_ nbface,
  INT_ *nbfpntd,
  INT_ nnode,
  INT_ nnodetr,
  INT_ nsetbcp,
  INT_ *mchkbq,
  INT_1D * ibcibf,
  INT_1D * idibf,
  INT_3D * inibf,
  INT_1D * irfibf,
  INT_1D * ibfichk,
  INT_1D ** ibfin,
  INT_1D * iboibf,
  INT_1D * libfin,
  INT_1D * mchkbf,
  INT_3D * ibfibf,
  INT_1D * ierribf,
  double angblisimx,
  double angbcpmax,
  double angbd,
  double angqbf,
  double angqbfm,
  double angrbfdd,
  double angrbfdd2,
  double angrbfmxd,
  double angrbfmxp,
  double angrbfsd,
  double arrecbf,
  double bfdwrec,
  DOUBLE_4D * trv,
  DOUBLE_4D * trw,
  DOUBLE_3D * x,
  void (*rec_function) (INT_,
                        INT_,
                        INT_,
                        INT_,
                        INT_,
                        INT_,
                        INT_,
                        INT_,
                        INT_,
                        INT_3D *,
                        INT_1D *,
                        INT_3D *,
                        INT_1D *,
                        INT_1D *,
                        INT_1D *,
                        double,
                        double,
                        double,
                        double,
                        DOUBLE_4D *,
                        DOUBLE_4D *,
                        DOUBLE_3D *))

{

/*
 * Check and prepare a boundary surface grid for volume grid generation.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_chkbsurf.c,v 1.69 2013/03/14 03:53:11 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX File_Name;

  INT_1D *ibcin, *ibcpibf, *iblisibf, *jnin, *mchkbn, *mrec;
  INT_1D *mchkbfm = NULL;
  INT_4D *iniq = NULL;

  INT_ ierr, mrbfdd1, mrecbmm, mreorder, nbfpnt, nblis, nbo;
  INT_ nquad=0;

  ibcin = libfin;
  ibcpibf = mchkbf;
  iblisibf = iboibf;
  jnin = libfin;
  mchkbn = libfin;
  mrec = iboibf;

  mrbfdd1 = 1;
  mreorder = 1;
  nbo = 0;

  ierr = ug3_chkbn (merr, mmsg, nbface, nnode, inibf, mchkbn, ierribf);

  if (ierr > 0)
    return (ierr);

  ierr = ug3_chkbq (merr, mmsg, nbface, inibf, ierribf, angqbf, x);

  if (ierr > 0)
    return (ierr);

  ierr = ug3_ibfin (nbface, nbfpntd, nnode, &nbfpnt, inibf, ibfin, libfin);

  if (ierr > 0)
    return (ierr);

  if (msetbc)
  {
    ug3_ibfibf0 (msetbc, nbface,
                 ibcibf, *ibfin, inibf, libfin, ibfichk, mchkbf, ibfibf);

    ug3_idibf (mmsg, msetbc, nbface, ibcibf, ibfibf, idibf, inibf, mchkbf,
               angbd, x);
  }

  ierr = ug3_ibfibf (merr, mmsg, mmultc, mreorder, nbface,
                     ibcibf, *ibfin, inibf, irfibf, libfin, ibfichk, mchkbf,
                     ibfibf, ierribf,
                     x);

  if (ierr > 0)
    return (ierr);

  ug3_setbcp (msetbc, nbface, nsetbcp,
              ibcibf, ibfibf, idibf, inibf, ibcpibf, ibfichk,
              angbcpmax, x);

  ug3_chkbcp1 (msetbc, nbface, ibcibf, idibf, inibf, angbcpmax, x);

  ug3_chkbcp2 (msetbc, nbface, ibcibf, ibfibf, idibf, inibf, angbd, x);

  ierr = ug3_chkbc (1, nbface, nbo, nnode, ibcibf, ibfibf, iboibf, ierribf);

  if (ierr > 0)
    return (ierr);

  ug3_iblisibf (1, nbface, &nblis, ibcibf, ibfibf, idibf, ibfichk, iblisibf);

  ug3_chkblisbc (1, nbface, &nblis, nnode, ibcibf, ibcin, iblisibf, inibf,
                 irfibf);

  ug3_iblisibf (2, nbface, &nblis, ibcibf, ibfibf, idibf, ibfichk, iblisibf);

  ug3_chkblisbc (2, nbface, &nblis, nnode, ibcibf, ibcin, iblisibf, inibf,
                 irfibf);

  ierr = ug3_chkblis (msetbc, nbface, ibcibf, ibfibf, ierribf, inibf,
                      angblisimx, x);

  if (ierr > 0)
    return (ierr);

  ierr = ug3_chkb (mclosed, merr, mmsg,
                   nbface, ibcibf, ibfibf, inibf, ierribf);

  if (ierr > 0)
    return (ierr);

  ug3_set_irfibf (nbface, ibfibf, idibf, inibf, irfibf,
                  angrbfdd2, angrbfmxp, x);

  ug3_irfibf (nbface, nnode, ibcibf, ibfibf, idibf, inibf, irfibf, jnin);

  if (mrecbf && mrecbm && (*rec_function) != NULL)
  {
    if (mrecbm > 0)
    {
      *mchkbq = ug3_chkbq (0, mmsg, nbface, inibf, ierribf, angqbfm, x);

      mrecbmm = (*mchkbq) ? mrecbm: 0;
    }
    else
      mrecbmm = abs (mrecbm);
  }
  else
    mrecbmm = 0;

  if (mrecbmm)
  {
    (*rec_function) (mmsg, mrbfdd1, mrecbdw, mrecbmm, mtrv, mtrw, nbface, nnode,
                     nnodetr,
                     ibfibf, idibf, inibf, irfibf, mchkbfm, mrec,
                     angrbfdd, angrbfmxd, arrecbf, bfdwrec,
                     trv, trw, x);

    ug3_set_irfibf (nbface, ibfibf, idibf, inibf, irfibf,
                    angrbfdd2, angrbfmxp, x);

    ug3_irfibf (nbface, nnode, ibcibf, ibfibf, idibf, inibf, irfibf, jnin);
  }

  *mchkbq = ug3_chkbq (0, mmsg, nbface, inibf, ierribf, angqbfm, x);

  *mchkbq = abs (*mchkbq);

  ierr = ug3_ibfin (nbface, nbfpntd, nnode, &nbfpnt, inibf, ibfin, libfin);

  if (ierr > 0)
    return (ierr);

  ug3_iboibf (mmsg, nbface, &nbo, ibfibf, mchkbf, iboibf);

  ierr = ug3_ibor (mclosed, mmsg, mopen, nbface, nbo, nnode,
                   ibcibf, ibfibf, *ibfin, iboibf, ierribf, inibf, irfibf,
                   libfin, x);

  if (ierr > 0)
    return (ierr);

  ierr = ug3_chkbc (2, nbface, nbo, nnode, ibcibf, ibfibf, iboibf, ierribf);

  if (ierr > 0)
    return (ierr);

  ierr = ug3_chkbda (merr, mmsg, nbface, ibfibf, inibf, mchkbf, ierribf,
                     angrbfsd, x);

  if (ierr > 0)
    return (ierr);

  if (mwbcsurf == 1)
  {
    strcpy (File_Name, "");

    ug_case_name (File_Name);
    ug_case_name (File_Name);

    strcat (File_Name, ".BC");

    ierr = ug3_write_surf_grid_file (File_Name,
                                     mmsg, nnode, nquad, nbface,
                                     ibcibf, idibf, iniq, irfibf, inibf, x);

    if (ierr > 0)
      return (343);
  }

  return (0);

}
