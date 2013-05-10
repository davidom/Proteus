#include "UG_IO_LIB.h"

INT_ ug_io_write_2d_grid_file
 (char Grid_File_Name[],
  char Error_Message[],
  UG_IO_Param_Struct * UG_IO_2D_Param_Struct_Ptr,
  INT_ Message_Flag,
  INT_ Number_of_Bnd_Edges,
  INT_ Number_of_Nodes,
  INT_ Number_of_Quads,
  INT_ Number_of_Trias,
  INT_2D * Bnd_Edge_Connectivity,
  INT_1D * Bnd_Edge_Grid_BC_Flag,
  INT_1D * Bnd_Edge_ID_Flag,
  INT_4D * Quad_Connectivity,
  INT_3D * Tria_Connectivity,
  DOUBLE_2D * Coordinates,
  DOUBLE_1D * Initial_Normal_Spacing,
  DOUBLE_1D * BL_Thickness)

{

/*
 * Write 2D grid data to a standard UG 2D grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_2d_grid_file.c,v 1.26 2013/03/16 17:09:09 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *Grid_File;

  CHAR_UG_MAX Case_Name, File_Name, drive, dname, fname, ext;
  CHAR_133 Text;
  CHAR_31 Name_Text;
  CHAR_21 File_Type_Suffix_;
  CHAR_11 File_Compression_Suffix_, File_Format_Suffix_, File_Mode_Suffix_;

  INT_1D *Face_ID_Flag = NULL;

  INT_ Error_Flag, File_Data_Type_, File_Format_, File_Mode_, File_Name_Length,
       File_Type_Format_Flag_;

  INT_ Check_File_Flag = 0;

  Error_Flag = ug_io_file_info (Grid_File_Name, Case_Name, Error_Message,
                                File_Compression_Suffix_, File_Format_Suffix_,
                                File_Mode_Suffix_, File_Type_Suffix_,
                                UG_IO_2D_Param_Struct_Ptr,
                                Check_File_Flag, &File_Data_Type_,
                                &File_Format_, &File_Mode_,
                                &File_Type_Format_Flag_);

  if (Error_Flag != 0)
    return (Error_Flag);

  if (File_Data_Type_ != UG_IO_2D_EDGE_GRID && File_Data_Type_ != UG_IO_2D_GRID)
  {
    strcpy (Error_Message, "file is not a 2D grid file");
    return (-623);
  }

  File_Format_ = abs (File_Format_);

  if (Message_Flag >= 1)
  {
    ug_message (" ");
    ug_message ("UG_IO    : OUTPUT GRID");
    ug_message (" ");
    ug_message ("UG_IO    : Writing Data");
    ug_message (" ");

    ug_cpu_message ("");
  }

  Error_Flag = ug_backup_file (Grid_File_Name);

  File_Name_Length = (INT_) strlen (Grid_File_Name)
                   - (INT_) strlen (File_Compression_Suffix_);

  strcpy (File_Name, "");
  strncat (File_Name, Grid_File_Name, File_Name_Length);

  Grid_File = ug_fopen (File_Name, "w");

  if (Grid_File == NULL)
  {
    strcpy (Error_Message, "unable to open output grid file");
    return (623);
  }

  if (strcmp (File_Type_Suffix_, ".ugrid") == 0)
  {
    Error_Flag = ug_io_write_2d_ugrid (Grid_File, Error_Message, File_Format_,
                                       Number_of_Nodes,
                                       Number_of_Quads, Number_of_Trias,
                                       Quad_Connectivity, Tria_Connectivity,
                                       Coordinates);
 
    if (Error_Flag > 0)
      return (623);

    Error_Flag = ug_io_write_2d_gdata (Grid_File, Error_Message, File_Format_,
                                       Number_of_Bnd_Edges, Number_of_Nodes,
                                       Number_of_Quads, Number_of_Trias,
                                       Bnd_Edge_Connectivity,
                                       Bnd_Edge_Grid_BC_Flag, Bnd_Edge_ID_Flag,
                                       Initial_Normal_Spacing, BL_Thickness);
 
    if (Error_Flag > 0)
      return (623);

    ug_fclose (Grid_File);
  }

  else if (strcmp (File_Type_Suffix_, ".mesh") == 0 ||
           strcmp (File_Type_Suffix_, ".meshb") == 0)
  {
    ug_fclose (Grid_File);

    Error_Flag = ug_io_write_mesh_2d_grid (File_Name, Error_Message,
                                           Number_of_Nodes,
                                           Number_of_Bnd_Edges,
                                           Number_of_Quads,
                                           Number_of_Trias,
                                           Bnd_Edge_ID_Flag,
                                           Bnd_Edge_Connectivity,
                                           Face_ID_Flag,
                                           Quad_Connectivity,
                                           Tria_Connectivity,
                                           Coordinates);
 
    if (Error_Flag > 0)
      return (623);
  }

  else
  { 
    ug_fclose (Grid_File);

    strcpy (Error_Message, "unknown output grid file type");
    return (623);
  }

  if (Message_Flag >= 1)
  { 
    ug_splitpath (Grid_File_Name, drive, dname, fname, ext);

    strcat (fname, ext);
    strcpy (Name_Text, "");
    strncat (Name_Text, fname, 29);

    sprintf (Text, "UG_IO    : Grid File Name    = %s", Name_Text);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Boundary Edges    =%10i", Number_of_Bnd_Edges);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Nodes             =%10i", Number_of_Nodes);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Quad Faces        =%10i", Number_of_Quads);
    ug_message (Text);
    sprintf (Text, "UG_IO    : Tria Faces        =%10i", Number_of_Trias);
    ug_message (Text);

    ug_message (" ");
    ug_cpu_message ("UG_IO    :");
  }

  Error_Flag = ug_compress_file (1, File_Compression_Suffix_, File_Name);
  
  return (0);

}
