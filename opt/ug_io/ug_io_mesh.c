#include "UG_IO_LIB.h"

/*
 * Read and write grid data from a MESH file using external routines.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_mesh.c,v 1.3 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

INT_ (*ext_mesh_ug_io_read_2d_grid) (char[],
                                  char[],
                                  INT_,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_1D *,
                                  INT_2D *,
                                  INT_1D *,
                                  INT_4D *,
                                  INT_3D *,
                                  DOUBLE_2D *) = 0;

INT_ (*ext_mesh_ug_io_write_2d_grid) (char[],
                                      char[],
                                      INT_,
                                      INT_,
                                      INT_,
                                      INT_,
                                      INT_1D *,
                                      INT_2D *,
                                      INT_1D *,
                                      INT_4D *,
                                      INT_3D *,
                                      DOUBLE_2D *) = 0;

INT_ (*ext_mesh_ug_io_read_grid) (char[],
                                  char[],
                                  INT_,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_ *,
                                  INT_1D *,
                                  INT_4D *,
                                  INT_3D *,
                                  INT_8D *,
                                  INT_1D *,
                                  INT_5D *,
                                  INT_6D *,
                                  INT_4D *,
                                  DOUBLE_3D *) = 0;

INT_ (*ext_mesh_ug_io_write_grid) (char[],
                                   char[],
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_,
                                   INT_1D *,
                                   INT_4D *,
                                   INT_3D *,
                                   INT_8D *,
                                   INT_1D *,
                                   INT_5D *,
                                   INT_6D *,
                                   INT_4D *,
                                   DOUBLE_3D *) = 0;

INT_ ug_io_read_mesh_2d_grid
 (char File_Name[],
  char Error_Message[],
  INT_ Read_Task_Flag,
  INT_ *Number_of_Bnd_Nodes,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Bnd_Edges,
  INT_ *Number_of_Quads,
  INT_ *Number_of_Trias,
  INT_1D * ID_Flag,
  INT_2D * Bnd_Edge_Connectivity,
  INT_1D * Face_ID_Flag,
  INT_4D * Quad_Connectivity,
  INT_3D * Tria_Connectivity,
  DOUBLE_2D * Coordinates)

{
  INT_ Error_Flag;

  if (ug_io_mesh_2d_grid_lib_check ())
    Error_Flag = (*ext_mesh_ug_io_read_2d_grid) (File_Name,
                                                 Error_Message,
                                                 Read_Task_Flag,
                                                 Number_of_Bnd_Nodes,
                                                 Number_of_Nodes,
                                                 Number_of_Bnd_Edges,
                                                 Number_of_Quads,
                                                 Number_of_Trias,
                                                 ID_Flag,
                                                 Bnd_Edge_Connectivity,
                                                 Face_ID_Flag,
                                                 Quad_Connectivity,
                                                 Tria_Connectivity,
                                                 Coordinates);
  else
  {
    strcpy (Error_Message, "2D MESH grid file type is not available");
    return (1);
  }

  return (Error_Flag);
}

INT_ ug_io_write_mesh_2d_grid
 (char File_Name[],
  char Error_Message[],
  INT_ Number_of_Nodes,
  INT_ Number_of_Bnd_Edges,
  INT_ Number_of_Quads,
  INT_ Number_of_Trias,
  INT_1D * ID_Flag,
  INT_2D * Bnd_Edge_Connectivity,
  INT_1D * Face_ID_Flag,
  INT_4D * Quad_Connectivity,
  INT_3D * Tria_Connectivity,
  DOUBLE_2D * Coordinates)

{
  INT_ Error_Flag;

  if (ug_io_mesh_2d_grid_lib_check ())
    Error_Flag = (*ext_mesh_ug_io_write_2d_grid) (File_Name,
                                                  Error_Message,
                                                  Number_of_Nodes,
                                                  Number_of_Bnd_Edges,
                                                  Number_of_Quads,
                                                  Number_of_Trias,
                                                  ID_Flag,
                                                  Bnd_Edge_Connectivity,
                                                  Face_ID_Flag,
                                                  Quad_Connectivity,
                                                  Tria_Connectivity,
                                                  Coordinates);

  else
  {
    strcpy (Error_Message, "2D MESH grid file type is not available");
    return (1);
  }

  return (Error_Flag);
}

INT_ ug_io_read_mesh_grid
 (char File_Name[],
  char Error_Message[],
  INT_ Read_Task_Flag,
  INT_ *Number_of_Bnd_Nodes,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Quads,
  INT_ *Number_of_Surf_Trias,
  INT_ *Number_of_Vol_Hexs,
  INT_ *Number_of_Vol_Pents_5,
  INT_ *Number_of_Vol_Pents_6,
  INT_ *Number_of_Vol_Tets,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_3D * Surf_Tria_Connectivity,
  INT_8D * Vol_Hex_Connectivity,
  INT_1D * Vol_ID_Flag,
  INT_5D * Vol_Pent_5_Connectivity,
  INT_6D * Vol_Pent_6_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)

{
  FILE *Grid_File = NULL;

  INT_ Close_Flag, Error_Flag;

  if (ug_io_mesh_grid_lib_check ())
    Error_Flag = (*ext_mesh_ug_io_read_grid) (File_Name,
                                              Error_Message,
                                              Read_Task_Flag,
                                              Number_of_Bnd_Nodes,
                                              Number_of_Nodes,
                                              Number_of_Surf_Quads,
                                              Number_of_Surf_Trias,
                                              Number_of_Vol_Hexs,
                                              Number_of_Vol_Pents_5,
                                              Number_of_Vol_Pents_6,
                                              Number_of_Vol_Tets,
                                              Surf_ID_Flag,
                                              Surf_Quad_Connectivity,
                                              Surf_Tria_Connectivity,
                                              Vol_Hex_Connectivity,
                                              Vol_ID_Flag,
                                              Vol_Pent_5_Connectivity,
                                              Vol_Pent_6_Connectivity,
                                              Vol_Tet_Connectivity,
                                              Coordinates);
  else
  {
    Grid_File = ug_fopen (File_Name, "r");

    if (Grid_File == NULL)
    {
      strcpy (Error_Message, "unable to open grid file");
      return (1);
    }

    Error_Flag = ug_io_read_mesh (Grid_File,
                                  Error_Message,
                                  Read_Task_Flag,
                                  Number_of_Bnd_Nodes,
                                  Number_of_Nodes,
                                  Number_of_Surf_Quads,
                                  Number_of_Surf_Trias,
                                  Number_of_Vol_Hexs,
                                  Number_of_Vol_Pents_5,
                                  Number_of_Vol_Pents_6,
                                  Number_of_Vol_Tets,
                                  Surf_ID_Flag,
                                  Surf_Quad_Connectivity,
                                  Surf_Tria_Connectivity,
                                  Vol_Hex_Connectivity,
                                  Vol_ID_Flag,
                                  Vol_Pent_5_Connectivity,
                                  Vol_Pent_6_Connectivity,
                                  Vol_Tet_Connectivity,
                                  Coordinates);

    Close_Flag = ug_fclose (Grid_File);
  }

  return (Error_Flag);
}

INT_ ug_io_write_mesh_grid
 (char File_Name[],
  char Error_Message[],
  INT_ Number_of_BL_Vol_Tets,
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
  INT_1D * Vol_ID_Flag,
  INT_5D * Vol_Pent_5_Connectivity,
  INT_6D * Vol_Pent_6_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)

{
  FILE *Grid_File = NULL;

  INT_ Close_Flag, Error_Flag;

  if (ug_io_mesh_grid_lib_check ())
    Error_Flag = (*ext_mesh_ug_io_write_grid) (File_Name,
                                               Error_Message,
                                               Number_of_BL_Vol_Tets,
                                               Number_of_Nodes,
                                               Number_of_Surf_Quads,
                                               Number_of_Surf_Trias,
                                               Number_of_Vol_Hexs,
                                               Number_of_Vol_Pents_5,
                                               Number_of_Vol_Pents_6,
                                               Number_of_Vol_Tets,
                                               Surf_ID_Flag,
                                               Surf_Quad_Connectivity,
                                               Surf_Tria_Connectivity,
                                               Vol_Hex_Connectivity,
                                               Vol_ID_Flag,
                                               Vol_Pent_5_Connectivity,
                                               Vol_Pent_6_Connectivity,
                                               Vol_Tet_Connectivity,
                                               Coordinates);
  else
  {
    Grid_File = ug_fopen (File_Name, "w");

    if (Grid_File == NULL)
    {
      strcpy (Error_Message, "unable to open grid file");
      return (1);
    }

    Error_Flag = ug_io_write_mesh (Grid_File,
                                   Error_Message,
                                   Number_of_BL_Vol_Tets,
                                   Number_of_Nodes,
                                   Number_of_Surf_Quads,
                                   Number_of_Surf_Trias,
                                   Number_of_Vol_Hexs,
                                   Number_of_Vol_Pents_5,
                                   Number_of_Vol_Pents_6,
                                   Number_of_Vol_Tets,
                                   Surf_ID_Flag,
                                   Surf_Quad_Connectivity,
                                   Surf_Tria_Connectivity,
                                   Vol_Hex_Connectivity,
                                   Vol_ID_Flag,
                                   Vol_Pent_5_Connectivity,
                                   Vol_Pent_6_Connectivity,
                                   Vol_Tet_Connectivity,
                                   Coordinates);

    Close_Flag = ug_fclose (Grid_File);
  }

  return (Error_Flag);
}

INT_ ug_io_mesh_2d_grid_lib_check (void)
{
  // return a positive value if the 2D mesh library routines are available

  if (ext_mesh_ug_io_read_2d_grid && ext_mesh_ug_io_write_2d_grid)
    return (1);

  // 2D mesh library routines are not available

  else
    return (0);
}

INT_ ug_io_mesh_grid_lib_check (void)
{
  // return a positive value if the mesh library routines are available

  if (ext_mesh_ug_io_read_grid && ext_mesh_ug_io_write_grid)
    return (1);

  // mesh library routines are not available

  else
    return (0);
}

// register names of external mesh library routines

void ug_io_mesh_register_read_2d_grid
 (INT_ (*ext_routine) (char[],
                       char[],
                       INT_,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_1D *,
                       INT_2D *,
                       INT_1D *,
                       INT_4D *,
                       INT_3D *,
                       DOUBLE_2D *))
{
  ext_mesh_ug_io_read_2d_grid = ext_routine;
  return;
}

void ug_io_mesh_register_write_2d_grid
 (INT_ (*ext_routine) (char[],
                       char[],
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_1D *,
                       INT_2D *,
                       INT_1D *,
                       INT_4D *,
                       INT_3D *,
                       DOUBLE_2D *))
{
  ext_mesh_ug_io_write_2d_grid = ext_routine;
  return;
}

void ug_io_mesh_register_read_grid
 (INT_ (*ext_routine) (char[],
                       char[],
                       INT_,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_ *,
                       INT_1D *,
                       INT_4D *,
                       INT_3D *,
                       INT_8D *,
                       INT_1D *,
                       INT_5D *,
                       INT_6D *,
                       INT_4D *,
                       DOUBLE_3D *))
{
  ext_mesh_ug_io_read_grid = ext_routine;
  return;
}

void ug_io_mesh_register_write_grid
 (INT_ (*ext_routine) (char[],
                       char[],
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_,
                       INT_1D *,
                       INT_4D *,
                       INT_3D *,
                       INT_8D *,
                       INT_1D *,
                       INT_5D *,
                       INT_6D *,
                       INT_4D *,
                       DOUBLE_3D *))
{
  ext_mesh_ug_io_write_grid = ext_routine;
  return;
}


