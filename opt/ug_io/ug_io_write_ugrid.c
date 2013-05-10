#include "UG_IO_LIB.h"

INT_ ug_io_write_ugrid
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Hexs,
  INT_ Number_of_Vol_Pents_5,
  INT_ Number_of_Vol_Pents_6,
  INT_ Number_of_Vol_Tets,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_3D * Surf_Tria_Connectivity,
  INT_8D * Vol_Hex_Connectivity,
  INT_5D * Vol_Pent_5_Connectivity,
  INT_6D * Vol_Pent_6_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Write grid data to a UGRID volume or surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_ugrid.c,v 1.18 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index, Number_of_Bytes, Number_of_Surf_Faces, Number_of_Write_Items,
       Special_Write_Items, Write_Flag;

  float TMP_float;

  Number_of_Surf_Faces = Number_of_Surf_Quads + Number_of_Surf_Trias;

  if (File_Format == UG_IO_FORMATTED)
  {
    Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i %i\n", Number_of_Nodes,
                          Number_of_Surf_Trias, Number_of_Surf_Quads,
                          Number_of_Vol_Tets, Number_of_Vol_Pents_5,
                          Number_of_Vol_Pents_6, Number_of_Vol_Hexs);

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Nodes; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%.15g %.15g %.15g\n",
                            Coordinates[Index][0],
                            Coordinates[Index][1],
                            Coordinates[Index][2]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i %i %i\n",
                            Surf_Tria_Connectivity[Index][0],
                            Surf_Tria_Connectivity[Index][1],
                            Surf_Tria_Connectivity[Index][2]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i %i %i %i\n",
                            Surf_Quad_Connectivity[Index][0],
                            Surf_Quad_Connectivity[Index][1],
                            Surf_Quad_Connectivity[Index][2],
                            Surf_Quad_Connectivity[Index][3]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i\n", Surf_ID_Flag[Index]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Vol_Tets; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i %i %i %i\n",
                            Vol_Tet_Connectivity[Index][0],
                            Vol_Tet_Connectivity[Index][1],
                            Vol_Tet_Connectivity[Index][2],
                            Vol_Tet_Connectivity[Index][3]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Vol_Pents_5; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i %i %i %i %i\n",
                            Vol_Pent_5_Connectivity[Index][0],
                            Vol_Pent_5_Connectivity[Index][1],
                            Vol_Pent_5_Connectivity[Index][2],
                            Vol_Pent_5_Connectivity[Index][3],
                            Vol_Pent_5_Connectivity[Index][4]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Vol_Pents_6; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i\n",
                            Vol_Pent_6_Connectivity[Index][0],
                            Vol_Pent_6_Connectivity[Index][1],
                            Vol_Pent_6_Connectivity[Index][2],
                            Vol_Pent_6_Connectivity[Index][3],
                            Vol_Pent_6_Connectivity[Index][4],
                            Vol_Pent_6_Connectivity[Index][5]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Vol_Hexs; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i %i %i %i %i %i %i %i\n",
                            Vol_Hex_Connectivity[Index][0],
                            Vol_Hex_Connectivity[Index][1],
                            Vol_Hex_Connectivity[Index][2],
                            Vol_Hex_Connectivity[Index][3],
                            Vol_Hex_Connectivity[Index][4],
                            Vol_Hex_Connectivity[Index][5],
                            Vol_Hex_Connectivity[Index][6],
                            Vol_Hex_Connectivity[Index][7]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
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
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Nodes,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Surf_Trias,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Surf_Quads,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Tets,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Pents_5,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Pents_6,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Hexs,
                                         sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = 7 + Special_Write_Items + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    Number_of_Bytes = Number_of_Surf_Trias * ((INT_) (sizeof (INT_3D)))
                    + Number_of_Surf_Quads * ((INT_) (sizeof (INT_4D)))
                    + Number_of_Surf_Faces * ((INT_) (sizeof (INT_1D)))
                    + Number_of_Vol_Tets * ((INT_) (sizeof (INT_4D)))
                    + Number_of_Vol_Pents_5 * ((INT_) (sizeof (INT_5D)))
                    + Number_of_Vol_Pents_6 * ((INT_) (sizeof (INT_6D)))
                    + Number_of_Vol_Hexs * ((INT_) (sizeof (INT_8D)))
                    + 3 * Number_of_Nodes
                    * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
                       ? ((INT_) (sizeof (double)))
                       : ((INT_) (sizeof (float))));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
        File_Format == UG_IO_BINARY_SINGLE)
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        TMP_float = (float) Coordinates[Index][0];

        Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
                                             sizeof (float), 1, Grid_File);

        TMP_float = (float) Coordinates[Index][1];

        Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
                                             sizeof (float), 1, Grid_File);

        TMP_float = (float) Coordinates[Index][2];

        Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
                                             sizeof (float), 1, Grid_File);
      }
    }
    else
      Write_Flag = Write_Flag + ug_fwrite (&Coordinates[1][0],
                                           sizeof (double),
                                           Number_of_Nodes*3, Grid_File);

    Number_of_Write_Items = 3*Number_of_Nodes + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    Write_Flag = ug_fwrite (&Surf_Tria_Connectivity[1][0],
                            sizeof (INT_), Number_of_Surf_Trias*3, Grid_File);

    if (Write_Flag != Number_of_Surf_Trias*3)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    Write_Flag = ug_fwrite (&Surf_Quad_Connectivity[1][0],
                            sizeof (INT_), Number_of_Surf_Quads*4, Grid_File);

    if (Write_Flag != Number_of_Surf_Quads*4)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    Write_Flag = ug_fwrite (&Surf_ID_Flag[1],
                            sizeof (INT_), Number_of_Surf_Faces, Grid_File);

    if (Write_Flag != Number_of_Surf_Faces)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    Write_Flag = ug_fwrite (&Vol_Tet_Connectivity[1][0],
                            sizeof (INT_1D), Number_of_Vol_Tets*4, Grid_File);

    if (Write_Flag != Number_of_Vol_Tets*4)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    Write_Flag = ug_fwrite (&Vol_Pent_5_Connectivity[1][0],
                            sizeof (INT_), Number_of_Vol_Pents_5*5, Grid_File); 

    if (Write_Flag != Number_of_Vol_Pents_5*5)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    Write_Flag = ug_fwrite (&Vol_Pent_6_Connectivity[1][0],
                            sizeof (INT_), Number_of_Vol_Pents_6*6, Grid_File); 

    if (Write_Flag != Number_of_Vol_Pents_6*6)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }

    Write_Flag = ug_fwrite (&Vol_Hex_Connectivity[1][0],
                            sizeof (INT_), Number_of_Vol_Hexs*8, Grid_File); 

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = Number_of_Vol_Hexs*8 + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing UGRID grid file");
      return (1);
    }
  }

  return (0);

}
