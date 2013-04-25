#include "UG_IO_LIB.h"

INT_ ug_io_read_cart3d
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Read_Task_Flag,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Trias,
  INT_1D * Surf_ID_Flag,
  INT_3D * Surf_Tria_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Read grid data from a CART3D surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_cart3d.c,v 1.8 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  int true_int = 0;

  INT_ Index, Number_of_Bytes, Number_of_Read_Items, Read_Flag,
       Special_Read_Items;

  float TMP_float[3];
  float fc0;

  fc0 = 0.0;

  if (File_Format == UG_IO_FORMATTED)
  {
    Read_Flag = fscanf (Grid_File, "%i %i", Number_of_Nodes,
                                            Number_of_Surf_Trias);

    if (Read_Flag == EOF)
    {
      strcpy (Error_Message, "error reading CART3D grid file");
      return (1);
    }

    if (Read_Task_Flag == 2)
    {
      for (Index = 1; Index <= *Number_of_Nodes; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%lf %lf %lf",
                            &Coordinates[Index][0],
                            &Coordinates[Index][1],
                            &Coordinates[Index][2]);
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading CART3D grid file");
        return (1);
      }

      for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
      {
        Read_Flag = fscanf (Grid_File, "%i %i %i",
                            &Surf_Tria_Connectivity[Index][0],
                            &Surf_Tria_Connectivity[Index][1],
                            &Surf_Tria_Connectivity[Index][2]);
      }

      if (Read_Flag == EOF)
      {
        strcpy (Error_Message, "error reading CART3D grid file");
        return (1);
      }


      if (Surf_ID_Flag != NULL)
      {
        for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
        {
          Read_Flag = fscanf (Grid_File, "%i", &true_int);

          Surf_ID_Flag[Index] = (INT_) true_int;
        }

        if (Read_Flag == EOF)
        {
          for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
          {
            Surf_ID_Flag[Index] = 1;
          }
        }
      }
    }
  }

  else
  {
    Special_Read_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                           File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    Read_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                        sizeof (INT_), 1, Grid_File);

    Read_Flag = Read_Flag + ug_fread (Number_of_Nodes,
                                      sizeof (INT_), 1, Grid_File);
    Read_Flag = Read_Flag + ug_fread (Number_of_Surf_Trias,
                                      sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                        sizeof (INT_), 1, Grid_File);

    Number_of_Read_Items = 2 + 2 * Special_Read_Items;

    if (Read_Flag != Number_of_Read_Items)
    {
      strcpy (Error_Message, "error reading CART3D grid file");
      return (1);
    }

    if (Read_Task_Flag == 2)
    {
      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
          File_Format == UG_IO_BINARY_SINGLE)
      {
        TMP_float[0] = fc0;
        TMP_float[1] = fc0;
        TMP_float[2] = fc0;

        for (Index = 1; Index <= *Number_of_Nodes; ++Index)
        {
          Read_Flag = Read_Flag
                    + ug_fread (&TMP_float, sizeof (float), 3, Grid_File);

          Coordinates[Index][0] = (double) TMP_float[0];
          Coordinates[Index][1] = (double) TMP_float[1];
          Coordinates[Index][2] = (double) TMP_float[2];
        }
      }
      else
      {
        Read_Flag = Read_Flag + ug_fread (&Coordinates[1][0],
                                          sizeof (double),
                                          *Number_of_Nodes*3, Grid_File);
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Number_of_Read_Items = *Number_of_Nodes*3 + 2 * Special_Read_Items;

      if (Read_Flag != Number_of_Read_Items )
      {
        strcpy (Error_Message, "error reading CART3D grid file");
        return (1);
      }

      Read_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Read_Flag = Read_Flag + ug_fread (&Surf_Tria_Connectivity[1][0],
                                        sizeof (INT_),
                                        *Number_of_Surf_Trias*3, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                          sizeof (INT_), 1, Grid_File);

      Number_of_Read_Items = *Number_of_Surf_Trias*3 + 2 * Special_Read_Items;

      if (Read_Flag != Number_of_Read_Items)
      {
        strcpy (Error_Message, "error reading CART3D grid file");
        return (1);
      }

      if (Surf_ID_Flag != NULL)
      {
        Read_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Grid_File);

        Read_Flag = Read_Flag + ug_fread (&Surf_ID_Flag[1],
                                          sizeof (INT_1D),
                                          *Number_of_Surf_Trias, Grid_File);

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Read_Flag = Read_Flag + ug_fread (&Number_of_Bytes,
                                            sizeof (INT_), 1, Grid_File);

        Number_of_Read_Items = *Number_of_Surf_Trias + 2 * Special_Read_Items;

        if (Read_Flag != Number_of_Read_Items)
        {
          for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
          {
            Surf_ID_Flag[Index] = 1;
          }
        }
      }
    }
  }

  return (0);

}
