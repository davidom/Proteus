#include "UG_IO_LIB.h"

INT_ ug_io_write_2d_ugrid
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Number_of_Nodes,
  INT_ Number_of_Quads,
  INT_ Number_of_Trias,
  INT_4D * Quad_Connectivity,
  INT_3D * Tria_Connectivity,
  DOUBLE_2D * Coordinates)

{

/*
 * Write grid data to a 2D UGRID planar or boundary edge grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_2d_ugrid.c,v 1.10 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Dummy_Index, Index, Number_of_Bytes, Number_of_Faces,
       Number_of_Write_Items, Special_Write_Items, Write_Flag;

  float fc0, TMP_float;

  double dc0;

  fc0 = 0.0;
  dc0 = 0.0;

  Number_of_Faces = Number_of_Quads + Number_of_Trias;

  if (File_Format == UG_IO_FORMATTED)
  {
    Write_Flag = fprintf (Grid_File, "%i %i %i 0 0 0 0\n", Number_of_Nodes,
                          Number_of_Trias, Number_of_Quads);

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Nodes; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%.15g %.15g 0.0\n",
                            Coordinates[Index][0],
                            Coordinates[Index][1]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Trias; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i %i %i\n",
                            Tria_Connectivity[Index][0],
                            Tria_Connectivity[Index][1],
                            Tria_Connectivity[Index][2]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Quads; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i %i %i %i\n",
                            Quad_Connectivity[Index][0],
                            Quad_Connectivity[Index][1],
                            Quad_Connectivity[Index][2],
                            Quad_Connectivity[Index][3]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Faces; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "1\n");
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }
  }
  else
  {
    Special_Write_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                            File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    Number_of_Bytes = 7 * ((INT_) (sizeof (INT_)));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                  sizeof (INT_), 1, Grid_File);

    Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Nodes,
                                                sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Trias,
                                                sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Quads,
                                                sizeof (INT_), 1, Grid_File);

    Index = 0;

    Write_Flag = Write_Flag + (INT_) ug_fwrite (&Index,
                                                sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + (INT_) ug_fwrite (&Index,
                                                sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + (INT_) ug_fwrite (&Index,
                                                sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + (INT_) ug_fwrite (&Index,
                                                sizeof (INT_), 1, Grid_File);
                                                  

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                  sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = 7 + Special_Write_Items + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }

    Number_of_Bytes = Number_of_Trias * ((INT_) (sizeof (INT_3D)))
                    + Number_of_Quads * ((INT_) (sizeof (INT_4D)))
                    + Number_of_Faces * ((INT_) (sizeof (INT_1D)))
                    + 3 * Number_of_Nodes
                    * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
                       ? ((INT_) (sizeof (double)))
                       : ((INT_) (sizeof (float))));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                  sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
        File_Format == UG_IO_BINARY_SINGLE)
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        TMP_float = (float) Coordinates[Index][0];

        Write_Flag = Write_Flag + (INT_) ug_fwrite (&TMP_float,
                                                    sizeof (float), 1,
                                                    Grid_File);

        TMP_float = (float) Coordinates[Index][1];

        Write_Flag = Write_Flag + (INT_) ug_fwrite (&TMP_float,
                                                    sizeof (float), 1,
                                                    Grid_File);

        Write_Flag = Write_Flag + (INT_) ug_fwrite (&fc0,
                                                    sizeof (float), 1,
                                                    Grid_File);
      }
    }
    else
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        Write_Flag = Write_Flag + (INT_) ug_fwrite (&Coordinates[Index][0],
                                                    sizeof (double), 1,
                                                    Grid_File);
        Write_Flag = Write_Flag + (INT_) ug_fwrite (&Coordinates[Index][1],
                                                    sizeof (double), 1,
                                                    Grid_File);
        Write_Flag = Write_Flag + (INT_) ug_fwrite (&dc0,
                                                    sizeof (double), 1,
                                                    Grid_File);
      }
    }

    Number_of_Write_Items = 3 * Number_of_Nodes + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }

    Write_Flag = (INT_) ug_fwrite (&Tria_Connectivity[1][0],
                                   sizeof (INT_), Number_of_Trias*3, Grid_File);

    if (Write_Flag != Number_of_Trias*3)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }

    Write_Flag = (INT_) ug_fwrite (&Quad_Connectivity[1][0],
                                   sizeof (INT_), Number_of_Quads*4, Grid_File);

    if (Write_Flag != Number_of_Quads*4)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }

    Dummy_Index = 1;

    Write_Flag = 0;

    for (Index = 1; Index <= Number_of_Faces; ++Index)
    {
      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Dummy_Index,
                                                  sizeof (INT_), 1, Grid_File);
    }

    if (Write_Flag != Number_of_Faces)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                  sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing 2D UGRID grid file");
      return (1);
    }
  }

  return (0);

}
