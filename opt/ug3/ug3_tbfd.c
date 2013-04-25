#include "UG3_LIB.h"

INT_ ug3_tbfd
 (INT_ mmsg,
  INT_ *mtbf,
  INT_ *nbface,
  INT_ *nnode,
  INT_1D ** ibcibf_ptr,
  INT_1D ** idibf_ptr,
  INT_1D ** ierribf_ptr,
  INT_1D ** iqibf_ptr,
  INT_1D ** irfibf_ptr,
  INT_3D ** inibf_ptr,
  DOUBLE_1D ** del_ptr,
  DOUBLE_1D ** ds_ptr,
  DOUBLE_3D ** x_ptr)

{

/*
 * Create duplicate faces and nodes for transparent boundary surface faces.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_tbfd.c,v 1.7 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  DOUBLE_1D *del, *ds;
  DOUBLE_3D *x;
  DOUBLE_1D *df=NULL;
  DOUBLE_4D *trv=NULL;
  DOUBLE_4D *trw=NULL;

  INT_1D *ibcibf, *idibf, *ierribf, *irfibf;
  INT_3D *inibf;

  INT_3D *ibfibf = NULL;
  INT_1D *ibfichk = NULL;
  INT_1D *ibfin = NULL;
  INT_1D *iqibf=NULL;
  INT_1D *jnin = NULL;
  INT_1D *jtbfitbf = NULL;
  INT_1D *libfin = NULL;
  INT_1D *mchkbf = NULL;

  INT_ ibc, ibface, ierr, mbc, merr, miqibf, mmultc, mreorder, nbfaced, nbfpnt,
       nbfpntd, nnodeb, nnoded, ntbface, ntbnode;

  ibcibf = *ibcibf_ptr;

  ntbface = 0;

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    ibc = ibcibf[ibface];
  
    if (ibc == -TRANSP_UG3_GBC || ibc == -TRANSP_INTRNL_UG3_GBC)
      ++ntbface;
  }

  if (ntbface == 0)
  {
    *mtbf = 0;

    return (0);
  }
  else
    *mtbf = 1;

  mbc = -1;

  merr = 1;
  mmultc = 1;
  mreorder = 0;

  miqibf = (*iqibf_ptr == NULL) ? 0: 1;

  nbfaced =2* *nbface;
  nnoded =2* *nnode;

  nbfpntd = nbfaced;

  idibf = *idibf_ptr;
  ierribf = *ierribf_ptr;
  inibf = *inibf_ptr;
  if (miqibf == 1) iqibf = *iqibf_ptr;
  irfibf = *irfibf_ptr;

  del = *del_ptr;
  ds = *ds_ptr;
  x = *x_ptr;

  ierr = 0;

  ibfin = (INT_1D *) ug_malloc (&ierr, (nbfpntd+1) * sizeof (INT_1D));

  ibfibf = (INT_3D *) ug_malloc (&ierr, (nbfaced+1) * sizeof (INT_3D));
  ibfichk = (INT_1D *) ug_malloc (&ierr, (nbfaced+1) * sizeof (INT_1D));
  jtbfitbf = (INT_1D *) ug_malloc (&ierr, (nbfaced+1) * sizeof (INT_1D));
  mchkbf = (INT_1D *) ug_malloc (&ierr, (nbfaced+1) * sizeof (INT_1D));

  libfin = (INT_1D *) ug_malloc (&ierr, (nnoded+2) * sizeof (INT_1D));
  jnin = (INT_1D *) ug_malloc (&ierr, (nnoded+1) * sizeof (INT_1D));

  if (ierr > 0)
  {
    ug_free (ibfin);
    ug_free (ibfibf);
    ug_free (ibfichk);
    ug_free (jtbfitbf);
    ug_free (mchkbf);
    ug_free (libfin);
    ug_free (jnin);
    return (100727);
  }

  ierr = ug3_ibfin (*nbface, &nbfpntd, *nnode, &nbfpnt, inibf, &ibfin, libfin);

  if (ierr > 0)
  {
    ug_free (ibfin);
    ug_free (ibfibf);
    ug_free (ibfichk);
    ug_free (jtbfitbf);
    ug_free (mchkbf);
    ug_free (libfin);
    ug_free (jnin);
    return (ierr);
  }

  ierr = ug3_ibfibf (merr, mmsg, mmultc, mreorder, *nbface,
                     ibcibf, ibfin, inibf, irfibf, libfin, ibfichk, mchkbf,
                     ibfibf, ierribf,
                     x);

  if (ierr > 0)
  {
    ug_free (ibfin);
    ug_free (ibfibf);
    ug_free (ibfichk);
    ug_free (jtbfitbf);
    ug_free (mchkbf);
    ug_free (libfin);
    ug_free (jnin);
    return (ierr);
  }

  ug3_nnodeb (*nbface, &nnodeb, inibf);

  ug3_tbfd0 (mbc, 1, nbface, nnode, &nnodeb, &ntbface, &ntbnode,
             ibcibf, ibfibf, ibfin, idibf, inibf, iqibf, irfibf, jtbfitbf, jnin,
             libfin, 
             del, df, ds, trv, trw,  x);

  nbfaced = nbfaced + ntbface;
  nnoded = nnoded + ntbnode;

  ierr = 0;

  *ibcibf_ptr = (INT_1D *) ug_realloc (&ierr, *ibcibf_ptr,
                                      (nbfaced+1) * sizeof (INT_1D));
  *idibf_ptr = (INT_1D *) ug_realloc (&ierr, *idibf_ptr, 
                                      (nbfaced+1) * sizeof (INT_1D));
  *ierribf_ptr = (INT_1D *) ug_realloc (&ierr, *ierribf_ptr, 
                                        (nbfaced+1) * sizeof (INT_1D));
  *inibf_ptr = (INT_3D *) ug_realloc (&ierr, *inibf_ptr, 
                                      (nbfaced+1) * sizeof (INT_3D));
  if (miqibf == 1)
    *iqibf_ptr = (INT_1D *) ug_realloc (&ierr, *iqibf_ptr, 
                                        (nbfaced+1) * sizeof (INT_1D));
  *irfibf_ptr = (INT_1D *) ug_realloc (&ierr, *irfibf_ptr, 
                                       (nbfaced+1) * sizeof (INT_1D));

  *del_ptr = (DOUBLE_1D *) ug_realloc (&ierr, *del_ptr, 
                                       (nnoded+1) * sizeof (DOUBLE_1D));
  *ds_ptr = (DOUBLE_1D *) ug_realloc (&ierr, *ds_ptr, 
                                      (nnoded+1) * sizeof (DOUBLE_1D));
  *x_ptr = (DOUBLE_3D *) ug_realloc (&ierr, *x_ptr, 
                                     (nnoded+1) * sizeof (DOUBLE_3D));

  ibfibf = (INT_3D *) ug_realloc (&ierr, ibfibf, 
                                  (nbfaced+1) * sizeof (INT_3D));
  jtbfitbf = (INT_1D *) ug_realloc (&ierr, jtbfitbf, 
                                    (nbfaced+1) * sizeof (INT_1D));

  if (ierr > 0)
  {
    ug_free (ibfin);
    ug_free (ibfibf);
    ug_free (ibfichk);
    ug_free (jtbfitbf);
    ug_free (mchkbf);
    ug_free (libfin);
    ug_free (jnin);
    return (100728);
  }

  ibcibf = *ibcibf_ptr;
  idibf = *idibf_ptr;
  ierribf = *ierribf_ptr;
  inibf = *inibf_ptr;
  if (miqibf == 1) iqibf = *iqibf_ptr;
  irfibf = *irfibf_ptr;

  del = *del_ptr;
  ds = *ds_ptr;
  x = *x_ptr;

  ug3_tbfd0 (mbc, 2, nbface, nnode, &nnodeb, &ntbface, &ntbnode,
             ibcibf, ibfibf, ibfin, idibf, inibf, iqibf, irfibf, jtbfitbf, jnin,
             libfin, 
             del, df, ds, trv, trw,  x);

  ug_free (ibfin);
  ug_free (ibfibf);
  ug_free (ibfichk);
  ug_free (jtbfitbf);
  ug_free (mchkbf);
  ug_free (libfin);
  ug_free (jnin);

  for (ibface = 1; ibface <= *nbface; ++ibface)
  {
    ibc = ibcibf[ibface];

    ibcibf[ibface] = (ibc == -TRANSP_UG3_GBC) ? -SOLID_TRANSP_UG3_GBC:
                     (ibc == -TRANSP_INTRNL_UG3_GBC) ? -SOLID_TRANSP_INTRNL_UG3_GBC:
                     (ibc == -TMP_TRANSP_UG3_GBC) ? -SOLID_TMP_TRANSP_UG3_GBC:
                     (ibc == -TMP_TRANSP_INTRNL_UG3_GBC) ? -SOLID_TMP_TRANSP_INTRNL_UG3_GBC: ibc;
  }

  return (0);

}
