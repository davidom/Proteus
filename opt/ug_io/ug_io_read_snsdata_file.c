#include "UG_IO_LIB.h"

INT_ ug_io_read_snsdata_file
 (char File_Name[],
  INT_ Message_Flag,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Set specified normal spacing (SNS) data for generation of SNS region from an
 * SNS data file.
 *
 * ANBL : ADVANCING-NORMAL BOUNDARY-LAYER UNSTRUCTURED GRID GENERATOR
 * 3D Version : $Id: ug_io_read_snsdata_file.c,v 1.4 2013/03/16 17:09:09 marcum Exp $
 * Copyright 1996-2012, David L. Marcum
 */

  FILE *snsdata_File;

  char *Read_Label;

  CHAR_UG_MAX Line, drive, dname, fname, ext;
  CHAR_133 Text;
  CHAR_31 Name_Text;

  INT_1D *idig = NULL;

  DOUBLE_1D *sns = NULL;

  INT_ EOF_Flag, Error_Flag, i, id, idmax, ig, iset, isns, j, jd, jg, k, nid,
       nidi, nidig, nsns, nsnsi, Read_Flag;

  snsdata_File = ug_fopen (File_Name, "r");

  if (snsdata_File == NULL)
    return (-1);

  if (Message_Flag >= 1)
  {
    ug_message (" ");
    ug_message ("UG_IO    : READ SPECIFIED NORMAL SPACING DATA");
    ug_message (" ");
    ug_message ("UG_IO    : Reading Data");
    ug_message (" ");

    ug_cpu_message ("");

    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "UG_IO    : SNS Data File Name= %s", Name_Text);
    ug_message (Text);
  }

//       x       x       x       x       x       x       x       x       x
//       ig      nids    id_1    id_2    ....    id_nids
//       nsns_ig sns_1   sns_2   ...     sns_nsns_ig

  ig = 0;

  nsns = 0;

  nidig = 0;

  do
  {
    nidi = 0;

    Read_Flag = fscanf (snsdata_File, "%i %i", &jg, &nidi);

    if (Read_Flag <= 0)
    {
      Error_Flag = -1;

      Read_Label = fgets (Line, UG_MAX_CHAR_STRING_LENGTH, snsdata_File);
    }
    else if (Read_Flag != 2)
      Error_Flag = 2;
    else
    {
      ++ig;

      Error_Flag = 0;
    }

    if (nidi > 0)
    {
      if (Error_Flag == 0)
      {
        idig = (INT_1D *) ug_realloc (&Error_Flag, idig,
                                       (nidig+1+nidi) * sizeof (INT_1D));

        if (Error_Flag > 0)
          Error_Flag = 1;
      }

      if (Error_Flag == 0)
      {
        idig[nidig] = nidi;

        Read_Flag = 0;

        for (i = nidig+1; i < nidig+1+nidi; ++i)
        {
          Read_Flag = Read_Flag + fscanf (snsdata_File, "%i", &(idig[i]));
        }

        if (Read_Flag != nidi)
          Error_Flag = 2;

        nidig = nidig + nidi+ 1;
      }
    }

    if (Error_Flag == 0)
    {
      Read_Flag = 0;

      Read_Flag = Read_Flag + fscanf (snsdata_File, "%i", &nsnsi);

      if (Read_Flag != 1)
        Error_Flag = 2;
    }

    if (Error_Flag == 0)
    {
      sns = (DOUBLE_1D *) ug_realloc (&Error_Flag, sns,
                                      (nsns+nsnsi) * sizeof (DOUBLE_1D));

      if (Error_Flag > 0)
        Error_Flag = 1;
    }

    if (Error_Flag == 0)
    {
      Read_Flag = 0;

      for (isns = nsns; isns < nsns+nsnsi; ++isns)
      {
        Read_Flag = Read_Flag + fscanf (snsdata_File, "%lg", &(sns[isns]));
      }

      if (Read_Flag != nsnsi)
        Error_Flag = 2;

      nsns = nsns + nsnsi;
    }

    if (Error_Flag == 0 && Message_Flag >= 1)
    {
      if (nidi > 0)
      {
        sprintf (Text, "UG_IO    : SNS_IDs#%-10i= %-10i", ig, idig[nidig-nidi]);
        ug_message (Text);

        for (i = nidig-nidi+1; i < nidig; ++i)
        {
          sprintf (Text, "UG_IO    :                   = %-10i", idig[i]);
          ug_message (Text);
        }
      }

      sprintf (Text, "UG_IO    : SNS#%-10i    = %g", ig, sns[nsns-nsnsi]);
      ug_message (Text);

      for (isns = nsns-nsnsi+1; isns < nsns; ++isns)
      {
        sprintf (Text, "UG_IO    :                   = %g", sns[isns]);
        ug_message (Text);
      }
    }

    EOF_Flag = feof (snsdata_File);
  }
  while (EOF_Flag == 0 && Error_Flag <= 0);

  fclose (snsdata_File);

  if (Error_Flag <= 0 && EOF_Flag != 0)
    Error_Flag = 0;

  if (ig == 0)
    Error_Flag = 3;

  if (Error_Flag == 0 && nidig > 0)
  {
    iset = ug_set_int_param_vector ("SNS_IDs", nidig, idig, UG_Param_Struct_Ptr);

    if (iset == 0)
      Error_Flag = 4;
    else if (iset > 1)
      Error_Flag = iset;
  }

  if (Error_Flag == 0 && nidig > 0)
  {
    k = 0;

    j = 0;

    nidi = 0;

    for (i = 0; i < nidig; ++i)
    {
      if (k == nidi)
      {
        nidi = idig[i];

        k = 0;

        nid = nid + nidi;
      }
      else
      {
        idig[j] = idig[i];

        ++j;

        ++k;
      }
    }

    nid = j;

    idmax = idig[0];

    for (i = 1; i < nid; ++i)
    {
      id = idig[i];

      idmax = MAX (idmax, id);
    }

    for (j = 0; j < nid; ++j)
    {
      jd = idig[j];

      for (i = j+1; i < nid; ++i)
      {
        id = idig[i];

        if (id == jd)
          idig[i] = idmax+1;
      }
    }

    j = 0;

    for (i = 0; i < nid; ++i)
    {
      id = idig[i];

      if (id <= idmax)
      {
        idig[j] = id;

        ++j;
      }
    }

    nid = j;

    if (nid > 0)
    {
      if (Error_Flag == 0 && Message_Flag >= 1)
      {
        sprintf (Text, "UG_IO    : BL_IDs            = %-10i", idig[0]);
        ug_message (Text);

        for (i = 1; i < nid; ++i)
        {
          sprintf (Text, "UG_IO    :                   = %-10i", idig[i]);
          ug_message (Text);
        }
      }

      iset = ug_set_int_param_vector ("BL_IDs", nid, idig, UG_Param_Struct_Ptr);

      if (iset == 0)
        Error_Flag = 4;
      else if (iset > 1)
        Error_Flag = iset;
    }
  }

  if (Error_Flag == 0)
  {
    iset = ug_set_double_param_vector ("SNS", nsns, sns, UG_Param_Struct_Ptr);

    if (iset == 0)
      Error_Flag = 4;
    else if (iset > 1)
      Error_Flag = iset;
  }

  ug_free (idig);
  ug_free (sns);

  if (Error_Flag >= 1)
  {
    if (Error_Flag == 1)
      return (100601);
    else if (Error_Flag == 1)
      return (643);
    else if (Error_Flag == 3)
      return (644);
    else if (Error_Flag == 4)
      return (645);
    else
      return (Error_Flag);
  }

  if (Message_Flag >= 1)
  {
    ug_message (" ");
    ug_cpu_message ("UG_IO    :");
  }

  return (0);

}
