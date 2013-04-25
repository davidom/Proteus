#include "UG_IO_LIB.h"

INT_ ug_io_write_gdata
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Number_of_BL_Vol_Tets,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_ Number_of_Vol_Hexs,
  INT_ Number_of_Vol_Pents_5,
  INT_ Number_of_Vol_Pents_6, 
  INT_ Number_of_Vol_Tets,
  INT_1D * Surf_Grid_BC_Flag,
  INT_1D * Surf_Reconnection_Flag,
  INT_1D * Vol_ID_Flag,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

/*
 * Write volume ID and number of BL tets (volume grids only) and write boundary
 * surface grid BC, boundary surface reconection flag, BL initial spacing, and
 * BL thicknes.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_gdata.c,v 1.6 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ Index, Number_of_Bytes, Number_of_Surf_Faces, Number_of_Vol_Elems,
       Number_of_Write_Items, Special_Write_Items, TMP_INT_, Write_Flag;

  float fc0, TMP_float;

  double dc0, TMP_double;

  fc0 = 0.0;
  dc0 = 0.0;

  Number_of_Surf_Faces = Number_of_Surf_Trias + Number_of_Surf_Quads;

  Number_of_Vol_Elems = Number_of_Vol_Hexs + Number_of_Vol_Pents_5
                      + Number_of_Vol_Pents_6 + Number_of_Vol_Tets;

  if (Number_of_Vol_Elems > 0)
  {
    if (File_Format == UG_IO_FORMATTED)
    {
      Write_Flag = 0;

      Write_Flag = fprintf (Grid_File, "%i\n", Number_of_BL_Vol_Tets);

      if (Write_Flag < 0)
      {
        strcpy (Error_Message, "error writing data of grid file");
        return (1);
      }

      if (Vol_ID_Flag != NULL)
      {
        for (Index = 1; Index <= Number_of_Vol_Elems; ++Index)
        {
          Write_Flag = fprintf (Grid_File, "%i\n", Vol_ID_Flag[Index]);
        }
      }
      else
      {
        for (Index = 1; Index <= Number_of_Vol_Elems; ++Index)
        {
          Write_Flag = fprintf (Grid_File, "0\n");
        }
      }

      if (Write_Flag < 0)
      {
        strcpy (Error_Message, "error writing grid file data");
        return (1);
      }
    }
    else
    {
      Special_Write_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                              File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1 : 0);

      Number_of_Bytes = (INT_) (sizeof (INT_));

      Write_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                             sizeof (INT_), 1, Grid_File);

      Write_Flag = Write_Flag + ug_fwrite (&Number_of_BL_Vol_Tets,
                                           sizeof (INT_), 1, Grid_File);

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                             sizeof (INT_), 1, Grid_File);

      Number_of_Write_Items = 1 + Special_Write_Items + Special_Write_Items;

      if (Write_Flag != Number_of_Write_Items)
      {
        strcpy (Error_Message, "error writing data of grid file");
        return (1);
      }

      Number_of_Bytes = Number_of_Vol_Elems * ((INT_) (sizeof (INT_1D)));

      Write_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                             sizeof (INT_), 1, Grid_File);

      if (Vol_ID_Flag != NULL)
      {
        Write_Flag = Write_Flag + ug_fwrite (&Vol_ID_Flag[1],
                                             sizeof (INT_1D),
                                             Number_of_Vol_Elems, Grid_File);
      }
      else
      {
        TMP_INT_ = 0;

        for (Index = 1; Index <= Number_of_Vol_Elems; ++Index)
        {
          Write_Flag = Write_Flag + ug_fwrite (&TMP_INT_,
                                               sizeof (INT_), 1, Grid_File);
        }
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                             sizeof (INT_), 1, Grid_File);

      Number_of_Write_Items = Number_of_Vol_Elems
                            + Special_Write_Items + Special_Write_Items;

      if (Write_Flag < Number_of_Write_Items)
      {
        strcpy (Error_Message, "error writing grid file data");
        return (1);
      }
    }
  }

  if (File_Format == UG_IO_FORMATTED)
  {
    if (Surf_Reconnection_Flag != NULL || Surf_Grid_BC_Flag != NULL ||
        Initial_Normal_Spacing != NULL || BL_Thickness != NULL)
    {
      Write_Flag = 0;

      if (Surf_Reconnection_Flag != NULL)
      {
        for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
        {
          Write_Flag = fprintf (Grid_File, "%i\n",
                                Surf_Reconnection_Flag[Index]);
        }
      }
      else
      {
        for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
        {
          Write_Flag = fprintf (Grid_File, "0\n");
        }
      }

      if (Write_Flag < 0)
      {
        strcpy (Error_Message, "error writing grid file data");
        return (1);
      }

      if (Surf_Grid_BC_Flag != NULL)
      {
        for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
        {
          Write_Flag = fprintf (Grid_File, "%i\n", Surf_Grid_BC_Flag[Index]);
        }
      }
      else
      {
        for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
        {
          Write_Flag = fprintf (Grid_File, "1\n");
        }
      }

      if (Write_Flag < 0)
      {
        strcpy (Error_Message, "error writing grid file data");
        return (1);
      }

      if (Number_of_Vol_Elems == 0)
      {
        if (Initial_Normal_Spacing != NULL)
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            Write_Flag = fprintf (Grid_File, "%.15g\n",
                                  Initial_Normal_Spacing[Index]);
          }
        }
        else
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            Write_Flag = fprintf (Grid_File, "0.0\n");
          }
        }

        if (Write_Flag < 0)
        {
          strcpy (Error_Message, "error writing grid file data");
          return (1);
        }

        if (BL_Thickness != NULL)
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            Write_Flag = fprintf (Grid_File, "%.15g\n", BL_Thickness[Index]);
          }
        }
        else
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            Write_Flag = fprintf (Grid_File, "0.0\n");
          }
        }

        if (Write_Flag < 0)
        {
          strcpy (Error_Message, "error writing grid file data");
          return (1);
        }
      }
    }
  }
  else
  {
    if (Surf_Reconnection_Flag != NULL || Surf_Grid_BC_Flag != NULL ||
        Initial_Normal_Spacing != NULL || BL_Thickness != NULL)
    {
      Special_Write_Items = ((File_Format == UG_IO_UNFORMATTED_DOUBLE ||
                              File_Format == UG_IO_UNFORMATTED_SINGLE) ? 1:
                                                                         0);

      Number_of_Bytes = Number_of_Surf_Faces * ((INT_) (sizeof (INT_1D)));

      Write_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                             sizeof (INT_), 1, Grid_File);

      if (Surf_Reconnection_Flag != NULL)
      {
        Write_Flag = Write_Flag + ug_fwrite (&Surf_Reconnection_Flag[1],
                                             sizeof (INT_1D),
                                             Number_of_Surf_Faces, Grid_File);
      }
      else
      {
        TMP_INT_ = 0;

        for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
        {
          Write_Flag = Write_Flag + ug_fwrite (&TMP_INT_,
                                               sizeof (INT_), 1, Grid_File);
        }
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                             sizeof (INT_), 1, Grid_File);

      Number_of_Write_Items = Number_of_Surf_Faces
                            + Special_Write_Items + Special_Write_Items;

      if (Write_Flag < Number_of_Write_Items)
      {
        strcpy (Error_Message, "error writing grid file data");
        return (1);
      }

      Number_of_Bytes = Number_of_Surf_Faces * ((INT_) (sizeof (INT_1D)));

      Write_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                             sizeof (INT_), 1, Grid_File);

      if (Surf_Grid_BC_Flag != NULL)
      {
        Write_Flag = Write_Flag + ug_fwrite (&Surf_Grid_BC_Flag[1],
                                             sizeof (INT_1D), 
                                             Number_of_Surf_Faces, Grid_File);
      }
      else
      {
        TMP_INT_ = 1;

        for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
        {
          Write_Flag = Write_Flag + ug_fwrite (&TMP_INT_,
                                               sizeof (INT_), 1, Grid_File);
        }
      }

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                             sizeof (INT_), 1, Grid_File);

      Number_of_Write_Items = Number_of_Surf_Faces
                            + Special_Write_Items + Special_Write_Items;

      if (Write_Flag < Number_of_Write_Items)
      {
        strcpy (Error_Message, "error writing grid file data");
        return (1);
      }

      Number_of_Bytes = Number_of_Nodes
                      * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
                         ? ((INT_) (sizeof (DOUBLE_1D)))
                         : ((INT_) (sizeof (float))));

      Write_Flag = 0;

      if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
          File_Format == UG_IO_UNFORMATTED_SINGLE)
        Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                             sizeof (INT_), 1, Grid_File);

      if (Number_of_Vol_Elems == 0)
      {
        if (Initial_Normal_Spacing != NULL)
        {
          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            for (Index = 1; Index <= Number_of_Nodes; ++Index)
            {
              TMP_float = (float) Initial_Normal_Spacing[Index];

              Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
                                                   sizeof (float), 1,
                                                   Grid_File);
            }
          }
          else
          {
            Write_Flag = Write_Flag + ug_fwrite (&Initial_Normal_Spacing[1],
                                                 sizeof (DOUBLE_1D),
                                                 Number_of_Nodes, Grid_File);
          }
        }
        else
        {
          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            TMP_float = fc0;

            for (Index = 1; Index <= Number_of_Nodes; ++Index)
            {
              Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
                                                   sizeof (float), 1,
                                                   Grid_File);
                        
            }
          }
          else
          {
            TMP_double = dc0;

            for (Index = 1; Index <= Number_of_Nodes; ++Index)
            {
              Write_Flag = Write_Flag + ug_fwrite (&TMP_double,
                                                   sizeof (double), 1,
                                                   Grid_File);
            }
          }
        }

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Grid_File);

        Number_of_Write_Items = Number_of_Nodes
                              + Special_Write_Items + Special_Write_Items;

        if (Write_Flag < Number_of_Write_Items)
        {
          strcpy (Error_Message, "error writing grid file data");
          return (1);
        }

        Number_of_Bytes = Number_of_Nodes
                        * ((File_Format == UG_IO_UNFORMATTED_DOUBLE)
                           ? ((INT_) (sizeof (DOUBLE_1D)))
                           : ((INT_) (sizeof (float))));

        Write_Flag = 0;

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Grid_File);

        if (BL_Thickness != NULL)
        {
          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            for (Index = 1; Index <= Number_of_Nodes; ++Index)
            {
              TMP_float = (float) BL_Thickness[Index];

              Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
                                                   sizeof (float), 1,
                                                   Grid_File);
            }
          }
          else
          {
            Write_Flag = Write_Flag + ug_fwrite (&BL_Thickness[1],
                                                 sizeof (DOUBLE_1D),
                                                 Number_of_Nodes, Grid_File);
          }
        }
        else
        {
          if (File_Format == UG_IO_UNFORMATTED_SINGLE ||
              File_Format == UG_IO_BINARY_SINGLE)
          {
            TMP_float = fc0;

            for (Index = 1; Index <= Number_of_Nodes; ++Index)
            {
              Write_Flag = Write_Flag + ug_fwrite (&TMP_float,
                                                   sizeof (float), 1,
                                                   Grid_File);
                        
            }
          }
          else
          {
            TMP_double = dc0;

            for (Index = 1; Index <= Number_of_Nodes; ++Index)
            {
              Write_Flag = Write_Flag + ug_fwrite (&TMP_double,
                                                   sizeof (double), 1,
                                                   Grid_File);
            }
          }
        }

        if (File_Format == UG_IO_UNFORMATTED_DOUBLE ||
            File_Format == UG_IO_UNFORMATTED_SINGLE)
          Write_Flag = Write_Flag + ug_fwrite (&Number_of_Bytes,
                                               sizeof (INT_), 1, Grid_File);

        Number_of_Write_Items = Number_of_Nodes
                              + Special_Write_Items + Special_Write_Items;

        if (Write_Flag < Number_of_Write_Items)
        {
          strcpy (Error_Message, "error writing grid file data");
          return (1);
        }
      }
    }
  }

  return (0);

}
