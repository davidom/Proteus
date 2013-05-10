#include "UG_IO_LIB.h"

INT_ ug_io_write_2d_gdata
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Number_of_Bnd_Edges,
  INT_ Number_of_Nodes,
  INT_ Number_of_Quads,
  INT_ Number_of_Trias,
  INT_2D * Bnd_Edge_Connectivity,
  INT_1D * Bnd_Edge_Grid_BC_Flag,
  INT_1D * Bnd_Edge_ID_Flag,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

/*
 * Write boundary edge grid data to a 2D planar grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_2d_gdata.c,v 1.2 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ ID_Flag, Index, Number_of_Bytes, Number_of_Faces, Number_of_Write_Items,
       Special_Write_Items, Write_Flag;

  float TMP_float;

  Number_of_Faces = Number_of_Quads + Number_of_Trias;

  if (File_Format == UG_IO_FORMATTED)
  {
    Write_Flag = fprintf (Grid_File, "%i\n", Number_of_Bnd_Edges);

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing 2D grid file boundary data");
      return (1);
    }

    for (Index = 1; Index <= Number_of_Bnd_Edges; ++Index)
    {
      if (Bnd_Edge_ID_Flag != NULL)
        ID_Flag = Bnd_Edge_ID_Flag[Index];
      else
        ID_Flag = 1;

      Write_Flag = fprintf (Grid_File, "%i %i %i\n",
                            Bnd_Edge_Connectivity[Index][0],
                            Bnd_Edge_Connectivity[Index][1], ID_Flag);
    }

    if (Write_Flag < 0)
    {
      strcpy (Error_Message, "error writing 2D grid file boundary data");
      return (1);
    }

    if (Number_of_Faces == 0 && Bnd_Edge_Grid_BC_Flag != NULL)
    {
      for (Index = 1; Index <= Number_of_Bnd_Edges; ++Index)
      {
        Write_Flag = fprintf (Grid_File, "%i\n",
                              Bnd_Edge_Grid_BC_Flag[Index]);
      }

      if (Write_Flag < 0)
      {
        strcpy (Error_Message, "error writing 2D grid file boundary data");
        return (1);
      }

      if (Initial_Normal_Spacing != NULL)
      {
        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          Write_Flag = fprintf (Grid_File, "%.15g\n",
                                Initial_Normal_Spacing[Index]);
        }

        if (Write_Flag < 0)
        {
          strcpy (Error_Message, "error writing 2D grid file boundary data");
          return (1);
        }

        if (BL_Thickness != NULL)
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            Write_Flag = fprintf (Grid_File, "%.15g\n", BL_Thickness[Index]);
          }

          if (Write_Flag < 0)
          {
            strcpy (Error_Message, "error writing 2D grid file boundary data");
            return (1);
          }
        }
      }
    }
  }
  else
  {
    Special_Write_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                            File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

    Number_of_Bytes = ((INT_) (sizeof (INT_)));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                  sizeof (INT_), 1, Grid_File);
                

    Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bnd_Edges,
                                                sizeof (INT_), 1, Grid_File);

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                  sizeof (INT_), 1,  Grid_File);

    Number_of_Write_Items = 1 + Special_Write_Items + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing 2D grid file boundary data");
      return (1);
    }

    Number_of_Bytes = Number_of_Bnd_Edges * ((INT_) (sizeof (INT_2D)))
                    + Number_of_Bnd_Edges * ((INT_) (sizeof (INT_)));

    Write_Flag = 0;

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                  sizeof (INT_), 1, Grid_File);

    for (Index = 1; Index <= Number_of_Bnd_Edges; ++Index)
    {
      if (Bnd_Edge_ID_Flag != NULL)
        ID_Flag = Bnd_Edge_ID_Flag[Index];
      else
        ID_Flag = 1;

      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Bnd_Edge_Connectivity[Index][0],
                                                  sizeof (INT_), 2, Grid_File);
                                                 

      Write_Flag = Write_Flag + (INT_) ug_fwrite (&ID_Flag,
                                                  sizeof (INT_), 1, Grid_File);
    }

    if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
        File_Format == UG_IO_UNFORMATTED_SINGLE)
      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                  sizeof (INT_), 1, Grid_File);

    Number_of_Write_Items = Number_of_Bnd_Edges*3
                          + Special_Write_Items + Special_Write_Items;

    if (Write_Flag != Number_of_Write_Items)
    {
      strcpy (Error_Message, "error writing 2D grid file boundary data");
      return (1);
    }

    if (Number_of_Faces == 0 && Bnd_Edge_Grid_BC_Flag != NULL)
    {
      Number_of_Bytes = Number_of_Bnd_Edges * ((INT_) (sizeof (INT_)));

      Write_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                    sizeof (INT_), 1,
                                                    Grid_File);

      Write_Flag = Write_Flag + (INT_) ug_fwrite (&Bnd_Edge_Grid_BC_Flag[1],
                                                  sizeof (INT_1D), 
                                                  Number_of_Bnd_Edges,
                                                  Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                    sizeof (INT_), 1,
                                                    Grid_File);

      Number_of_Write_Items = Number_of_Bnd_Edges
                            + Special_Write_Items + Special_Write_Items;

      if (Write_Flag != Number_of_Write_Items)
      {
        strcpy (Error_Message, "error writing 2D grid file boundary data");
        return (1);
      }

      if (Initial_Normal_Spacing != NULL)
      {
        Number_of_Bytes = Number_of_Nodes
                        * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
                           ? ((INT_) (sizeof (DOUBLE_1D)))
                           : ((INT_) (sizeof (float))));

        Write_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                      sizeof (INT_), 1,
                                                      Grid_File);

        if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
            File_Format == UG_IO_BINARY_SINGLE)
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            TMP_float = (float) Initial_Normal_Spacing[Index];

            Write_Flag = Write_Flag + (INT_) ug_fwrite (&TMP_float,
                                                        sizeof (float), 1,
                                                        Grid_File);
          }
        }
        else
        {
          Write_Flag = Write_Flag + (INT_) ug_fwrite (&Initial_Normal_Spacing[1],
                                                      sizeof (DOUBLE_1D),
                                                      Number_of_Nodes,
                                                      Grid_File);
        }

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                      sizeof (INT_), 1,
                                                      Grid_File);

        Number_of_Write_Items = Number_of_Nodes
                              + Special_Write_Items + Special_Write_Items;

        if (Write_Flag != Number_of_Write_Items)
        {
          strcpy (Error_Message, "error writing 2D grid file boundary data");
          return (1);
        }

        if (BL_Thickness != NULL)
        {
          Number_of_Bytes = Number_of_Nodes
                          * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
                             ? ((INT_) (sizeof (DOUBLE_1D)))
                             : ((INT_) (sizeof (float))));

          Write_Flag = 0;

          if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
              File_Format == UG_IO_UNFORMATTED_SINGLE)
            Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                        sizeof (INT_), 1,
                                                        Grid_File);

          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            for (Index = 1; Index <= Number_of_Nodes; ++Index)
            {
              TMP_float = (float) BL_Thickness[Index];

              Write_Flag = Write_Flag + (INT_) ug_fwrite (&TMP_float,
                                                          sizeof (float), 1,
                                                          Grid_File);
            }
          }
          else
          {
            Write_Flag = Write_Flag + (INT_) ug_fwrite (&BL_Thickness[1],
                                                        sizeof (DOUBLE_1D),
                                                        Number_of_Nodes,
                                                        Grid_File);
          }

          if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
              File_Format == UG_IO_UNFORMATTED_SINGLE)
            Write_Flag = Write_Flag + (INT_) ug_fwrite (&Number_of_Bytes,
                                                        sizeof (INT_), 1,
                                                        Grid_File);

          Number_of_Write_Items = Number_of_Nodes
                                + Special_Write_Items + Special_Write_Items;

          if (Write_Flag != Number_of_Write_Items)
          {
            strcpy (Error_Message, "error writing 2D grid file boundary data");
            return (1);
          }
        }
      }
    }
  }

  return (0);

}
