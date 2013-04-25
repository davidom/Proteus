#include "UG_IO_LIB.h"

INT_ ug_io_write_cart3d
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ *Number_of_Bnd_Nodes,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Trias,
  INT_1D * Surf_ID_Flag,
  INT_3D * Surf_Tria_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Write grid data to a CART3D surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_cart3d.c,v 1.6 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_4D *Surf_Quad_Connectivity = NULL;
  INT_1D *Node_Index_Map = NULL;

  DOUBLE_1D *Initial_Normal_Spacing = NULL;
  DOUBLE_1D *BL_Thickness = NULL;

  INT_ Error_Flag, Index, Node_Index, Node_Index1, Node_Index2, Node_Index3,
       Number_of_Bytes, Number_of_Write_Items, Surf_Node_Index,
       Special_Write_Items, Write_Flag;
  INT_ Number_of_Surf_Quads = 0;

  float TMP_float[3];

  Error_Flag = ug3_bnd_nodes (0, Number_of_Bnd_Nodes, Number_of_Nodes,
                              Number_of_Surf_Trias, Number_of_Surf_Quads,
                              Surf_Tria_Connectivity, Surf_Quad_Connectivity,
                              &Node_Index_Map,
                              BL_Thickness, Initial_Normal_Spacing,
                              Coordinates);

  if (Error_Flag > 0)
  {
    sprintf (Error_Message, "unable to allocate required memory %i",
             Error_Flag);
    return (1);
  }

  if (File_Format == UG_IO_FORMATTED)
  {
    Write_Flag = fprintf (Grid_File, "%i %i\n",
                          Number_of_Nodes, Number_of_Surf_Trias);

    if (Write_Flag < 0)
    {
      ug_free (Node_Index_Map);
      strcpy (Error_Message, "error writing CART3D grid file");
      return (1);
    }

    for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
    {
      Surf_Node_Index = Node_Index_Map[Node_Index];

      if (Surf_Node_Index > 0)
        Write_Flag = fprintf (Grid_File, "%.15g %.15g %.15g\n",
                              Coordinates[Node_Index][0],
                              Coordinates[Node_Index][1],
                              Coordinates[Node_Index][2]);
    }

    if (Write_Flag < 0)
    {
      ug_free (Node_Index_Map);
      strcpy (Error_Message, "error writing CART3D grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
    {
      Node_Index1 = Surf_Tria_Connectivity[Index][0];
      Node_Index2 = Surf_Tria_Connectivity[Index][1];
      Node_Index3 = Surf_Tria_Connectivity[Index][2];

      Write_Flag = fprintf (Grid_File, "%i %i %i\n",
                            Node_Index1, Node_Index2, Node_Index3);
    }

    if (Write_Flag < 0)
    {
      ug_free (Node_Index_Map);
      strcpy (Error_Message, "error writing CART3D grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i\n", Surf_ID_Flag[Index]);
    }

    if (Write_Flag < 0)
    {
      ug_free (Node_Index_Map);
      strcpy (Error_Message, "error writing CART3D grid file");
      return (1);
    }
  }

  else
  {
    Special_Write_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                            File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    Number_of_Bytes = 2 * ((INT_) (sizeof (INT_)));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Nodes,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Surf_Trias,
                                         sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = 2 + 2 * Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      ug_free (Node_Index_Map);
      strcpy (Error_Message, "error writing CART3D grid file");
      return (1);
    }

    Number_of_Bytes = 3 * (*Number_of_Bnd_Nodes)
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
      for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
      {
        Surf_Node_Index = Node_Index_Map[Node_Index];

        if (Surf_Node_Index > 0)
        {
          TMP_float[0] = (float) Coordinates[Node_Index][0];
          TMP_float[1] = (float) Coordinates[Node_Index][1];
          TMP_float[2] = (float) Coordinates[Node_Index][2];

          Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
                                               sizeof (float), 3, Grid_File);
        }
      }
    }
    else
    {
      for (Node_Index = 1; Node_Index <= Number_of_Nodes; ++Node_Index)
      {
        Surf_Node_Index = Node_Index_Map[Node_Index];

        if (Surf_Node_Index > 0)
          Write_Flag = Write_Flag + ug_fwrite (&Coordinates[Node_Index][0],
                                               sizeof (double), 3, Grid_File);
      }
    }

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = (*Number_of_Bnd_Nodes)*3 + 2 * Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items )
    {
      ug_free (Node_Index_Map);
      strcpy (Error_Message, "error writing CART3D grid file");
      return (1);
    }

    Number_of_Bytes = Number_of_Surf_Trias * ((INT_) (sizeof (INT_3D)));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Write_Flag = Write_Flag + ug_fwrite (&Surf_Tria_Connectivity[1][0],
                                         sizeof (INT_), Number_of_Surf_Trias*3,
                                         Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = Number_of_Surf_Trias*3 + 2 * Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items )
    {
      ug_free (Node_Index_Map);
      strcpy (Error_Message, "error writing CART3D grid file");
      return (1);
    }

    Number_of_Bytes = Number_of_Surf_Trias * ((INT_) (sizeof (INT_1D)));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Write_Flag = Write_Flag + ug_fwrite (&Surf_ID_Flag[1],
                                         sizeof (INT_), Number_of_Surf_Trias,
                                         Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = Number_of_Surf_Trias + 2 * Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items )
    {
      ug_free (Node_Index_Map);
      strcpy (Error_Message, "error writing CART3D grid file");
      return (1);
    }
  }

  ug_free (Node_Index_Map);

  return (0);

}
