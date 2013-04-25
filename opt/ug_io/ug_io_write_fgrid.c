#include "UG_IO_LIB.h"

INT_ ug_io_write_fgrid
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Tets,
  INT_1D * Surf_ID_Flag,
  INT_3D * Surf_Tria_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Write grid data to a FGRID volume or surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_fgrid.c,v 1.23 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Geom_Index, Index, Number_of_Bytes, Number_of_Write_Items, Print_Loop,
       Print_Loop_Limit, Special_Write_Items, Write_Flag;

  float TMP_float;

  double TMP_double;

  Print_Loop_Limit = 5;

  if (File_Format == UG_IO_FORMATTED)
  {
    Write_Flag = fprintf (Grid_File, "%i %i %i\n", Number_of_Nodes,
                          Number_of_Surf_Trias, Number_of_Vol_Tets);

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing FGRID grid file");
      return (1);
    }

    for (Geom_Index = 0; Geom_Index <= 2; ++Geom_Index)
    {
      Index = 1;

      do
      {
        if (Index < Number_of_Nodes)
        {
          Print_Loop = 1;

          do
          {
            Write_Flag = fprintf (Grid_File, "%.15g ",
                                  Coordinates[Index][Geom_Index]);

            ++Index;

            ++Print_Loop;
          }
          while (Print_Loop < Print_Loop_Limit && Index < Number_of_Nodes);
        }

        Write_Flag = fprintf (Grid_File, "%.15g\n",
                              Coordinates[Index][Geom_Index]);

        ++Index;
      }
      while (Index <= Number_of_Nodes);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing FGRID grid file");
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
      strcpy (Error_Message, "error writing FGRID grid file");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
    {
      Write_Flag = fprintf (Grid_File, "%i\n", Surf_ID_Flag[Index]);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing FGRID grid file");
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
      strcpy (Error_Message, "error writing FGRID grid file");
      return (1);
    }
  }
  else
  {
    Special_Write_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                            File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    Number_of_Bytes = 3 * ((INT_) (sizeof (INT_)));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);
                

    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Nodes,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Surf_Trias,
                                         sizeof (INT_), 1, Grid_File);
    Write_Flag = Write_Flag + ug_fwrite (&Number_of_Vol_Tets,
                                         sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = 3 + Special_Write_Items + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing FGRID grid file");
      return (1);
    }

    Number_of_Bytes = Number_of_Surf_Trias * ((INT_) (sizeof (INT_3D)))
                    + Number_of_Surf_Trias * ((INT_) (sizeof (INT_1D)))
                    + Number_of_Vol_Tets * ((INT_) (sizeof (INT_4D)))
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
      for (Geom_Index = 0; Geom_Index < 3; ++Geom_Index)
      {
        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          TMP_float = (float)Coordinates[Index][Geom_Index];

          Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
                                               sizeof (float), 1, Grid_File);
        }
      }
    }
    else
    {
      for (Geom_Index = 0; Geom_Index < 3; ++Geom_Index)
      {
        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          TMP_double = Coordinates[Index][Geom_Index];
 
          Write_Flag = Write_Flag + ug_fwrite (&TMP_double,
                                               sizeof (double), 1, Grid_File);
        }
      }
    }

    Number_of_Write_Items = Number_of_Nodes*3 + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing FGRID grid file");
      return (1);
    }

    Write_Flag = ug_fwrite (&Surf_Tria_Connectivity[1][0],
                            sizeof (INT_), Number_of_Surf_Trias*3, Grid_File);

    if (Write_Flag != Number_of_Surf_Trias*3)
    {
      strcpy (Error_Message, "error writing FGRID grid file");
      return (1);
    }

    Write_Flag = ug_fwrite (&Surf_ID_Flag[1],
                            sizeof (INT_1D), Number_of_Surf_Trias, Grid_File);

    if (Write_Flag != Number_of_Surf_Trias)
    {
      strcpy (Error_Message, "error writing FGRID grid file");
      return (1);
    }

    Write_Flag = ug_fwrite (&Vol_Tet_Connectivity[1][0],
                            sizeof (INT_), Number_of_Vol_Tets*4, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                           sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = Number_of_Vol_Tets*4 + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing FGRID grid file");
      return (1);
    }
  }

  return (0);

}
