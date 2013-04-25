#include "UG_IO_LIB.h"

INT_ ug_io_read_psdata_file
 (char File_Name[],
  INT_ Message_Flag,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Set surface IDs and transformation data for periodic surfaces that intersect
 * the BL region from specified periodic surface data file.
 *
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_psdata_file.c,v 1.5 2013/03/16 17:09:09 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *psdata_File;

  char *Read_Label;

  CHAR_UG_MAX Line, drive, dname, fname, ext;
  CHAR_133 Text;
  CHAR_31 Name_Text;

  INT_1D *int_ptr = NULL;
  INT_2D *idips = NULL;

  DOUBLE_1D *double_ptr = NULL;
  DOUBLE_3X3 *tm = NULL;
  DOUBLE_3D *xps0 = NULL;

  INT_ EOF_Flag, ipsurf, iread, iset, jpsurf, n, npsurf, Number_of_Items,
       Read_Flag;
  INT_ Error_Flag = 0;
  INT_ nalloc = 10;
  INT_ npsurfd = 0;

  psdata_File = ug_fopen (File_Name, "r");

  if (psdata_File == NULL)
    return (-1);

  if (Message_Flag >= 1)
  {
    ug_message (" ");
    ug_message ("UG_IO    : READ PERIODIC SURFACE DATA");
    ug_message (" ");
    ug_message ("UG_IO    : Reading Data");
    ug_message (" ");

    ug_cpu_message ("");

    ug_splitpath (File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "UG_IO    : PS Data File Name = %s", Name_Text);
    ug_message (Text);
  }

//	x	x	x	x	x	x
//ipsurf	id1	id2	xps01	xps02	xps03
//				tm11	tm12	tm13
//				tm21	tm22	tm23
//				tm31	tm32	tm33

  ipsurf = -1;

  iread = 0;

  do
  {
    Error_Flag = 0;
    
    ++ipsurf;

    npsurf = ipsurf+1;

    if (npsurf > npsurfd)
    {
      npsurfd = npsurfd + nalloc;

      idips = (INT_2D *) ug_realloc (&Error_Flag, idips,
                                     npsurfd * sizeof (INT_2D));
      tm = (DOUBLE_3X3 *) ug_realloc (&Error_Flag, tm,
                                      npsurfd * sizeof (DOUBLE_3X3));
      xps0 = (DOUBLE_3D *) ug_realloc (&Error_Flag, xps0,
                                      npsurfd * sizeof (DOUBLE_3D));

      if (Error_Flag > 0)
        Error_Flag = 2;
    }

    if (Error_Flag == 0)
    {
      Number_of_Items = 15;

      Read_Flag = fscanf (psdata_File,
                          "%i %i %i %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg",
                          &jpsurf,
                          &(idips[ipsurf][0]), &(idips[ipsurf][1]),
                          &(xps0[ipsurf][0]), &(xps0[ipsurf][1]),
                          &(xps0[ipsurf][2]),
                          &(tm[ipsurf][0][0]), &(tm[ipsurf][0][1]),
                          &(tm[ipsurf][0][2]),
                          &(tm[ipsurf][1][0]), &(tm[ipsurf][1][1]),
                          &(tm[ipsurf][1][2]),
                          &(tm[ipsurf][2][0]), &(tm[ipsurf][2][1]),
                          &(tm[ipsurf][2][2]));

      if (Read_Flag <= 0)
      {
        Error_Flag = -1;

        --ipsurf;

        npsurf = ipsurf+1;

        Read_Label = fgets (Line, UG_MAX_CHAR_STRING_LENGTH, psdata_File);
      }
      else if (Read_Flag != Number_of_Items)
        Error_Flag = 3;
    }

    EOF_Flag = feof (psdata_File);
  }
  while (EOF_Flag == 0 && Error_Flag <= 0);

  fclose (psdata_File);

  if (Error_Flag <= 1 && EOF_Flag != 0)
    Error_Flag = 0;
  if (npsurf == 0)
    Error_Flag = 0;

  if (npsurf > 0)
  {
    if (Error_Flag == 0)
    {
      iset = ug_set_int_param ("npsurf", npsurf, UG_Param_Struct_Ptr);

      if (iset == 0)
        Error_Flag = 3;
    }

    if (Error_Flag == 0)
    {
      n = 2 * npsurf;

      int_ptr = (INT_1D *) &(idips[0][0]);

      iset = ug_set_int_param_vector ("PS_IDs", n, int_ptr, UG_Param_Struct_Ptr);

      if (iset == 0)
        Error_Flag = 3;
      else if (iset > 1)
        Error_Flag = iset;
    }

    if (Error_Flag == 0)
    {
      n = 9 * npsurf;

      double_ptr = (DOUBLE_1D *) &(tm[0][0][0]);

      iset = ug_set_double_param_vector ("PS_TMs", n, double_ptr, UG_Param_Struct_Ptr);

      if (iset == 0)
        Error_Flag = 3;
      else if (iset > 1)
        Error_Flag = iset;
    }

    if (Error_Flag == 0)
    {
      n = 3 * npsurf;

      double_ptr = (DOUBLE_1D *) &(xps0[0][0]);

      iset = ug_set_double_param_vector ("PS_XPS0s", n, double_ptr, UG_Param_Struct_Ptr);

      if (iset == 0)
        Error_Flag = 3;
      else if (iset > 1)
        Error_Flag = iset;
    }
  }

  if (Error_Flag == 0 && Message_Flag >= 1)
  {
    sprintf (Text, "UG_IO    : npsurf            = %-10i", npsurf);
    ug_message (Text);

    for (ipsurf = 0; ipsurf < npsurf; ++ipsurf)
    {
      sprintf (Text, "UG_IO    : PS_IDs#%-10i = %-10i",
               ipsurf+1, idips[ipsurf][0]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %-10i",
               idips[ipsurf][1]);
      ug_message (Text);

      sprintf (Text, "UG_IO    : PS_TMs#%-10i = %g",
               ipsurf+1, tm[ipsurf][0][0]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %g",
               tm[ipsurf][0][1]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %g",
               tm[ipsurf][0][2]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %g",
               tm[ipsurf][1][0]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %g",
               tm[ipsurf][1][1]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %g",
               tm[ipsurf][1][2]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %g",
               tm[ipsurf][2][0]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %g",
               tm[ipsurf][2][1]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %g",
               tm[ipsurf][2][2]);
      ug_message (Text);

      sprintf (Text, "UG_IO    : PS_XPS0s#%-9i= %g",
               ipsurf+1, xps0[ipsurf][0]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %g",
               xps0[ipsurf][1]);
      ug_message (Text);

      sprintf (Text, "UG_IO    :                   = %g",
               xps0[ipsurf][2]);
      ug_message (Text);
    }
  }

  ug_free (idips);
  ug_free (tm);
  ug_free (xps0);

  if (Error_Flag >= 1)
  {
    if (Error_Flag == 1)
      return (641);
    else if (Error_Flag == 2)
      return (100600);
    else if (Error_Flag == 3)
      return (642);
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
