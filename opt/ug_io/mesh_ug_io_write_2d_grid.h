INT_ mesh_ug_io_write_2d_grid
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
  DOUBLE_2D * Coordinates);
