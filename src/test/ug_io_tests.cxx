#include "gtest/gtest.h"
#include "UG_IO_LIB.h"
#include <iostream>

TEST(UGIOTest, FileReadErrorNoFile)
{
  char *Grid_File_Name = "non-existent_file.fale_extension";
  int error_flag;
  char Error_Message[1024] = {0};
  int Message_Flag = 1, Number_of_BL_Vol_Tets, Number_of_Nodes, Number_of_Surf_Quads, Number_of_Surf_Trias, Number_of_Vol_Hexes, Number_of_Vol_Pents_5, Number_of_Vol_Pents_6, Number_of_Vol_Tets = 1;
  UG_IO_Param_Struct *UG_IO_Param_Struct_Ptr = NULL;
  INT_1D *Surf_Grid_BC_Flag = NULL, *Surf_ID_Flag = NULL, *Surf_Reconnection_Flag = NULL, *Vol_ID_Flag = NULL;
  INT_3D *Surf_Tria_Connectivity = NULL;
  INT_4D *Surf_Quad_Connectivity = NULL, *Vol_Tet_Connectivity = NULL;
  INT_5D *Vol_Pent_5_Connectivity = NULL;
  INT_6D *Vol_Pent_6_Connectivity = NULL;
  INT_8D *Vol_Hex_Connectivity = NULL;
  DOUBLE_1D *Initial_Normal_Spacing = NULL, *BL_Thickness = NULL;
  DOUBLE_3D *Coordinates = NULL;

  ug_io_malloc_param(Error_Message,
		     &UG_IO_Param_Struct_Ptr);
  ug_io_initialize_param(Error_Message,
			 0,
			 1,
			 1,
			 1,
			 1,
			 1,
			 UG_IO_Param_Struct_Ptr);
  error_flag = ug_io_read_grid_file
      (Grid_File_Name,
       Error_Message,
       UG_IO_Param_Struct_Ptr,
       Message_Flag,
       &Number_of_BL_Vol_Tets,
       &Number_of_Nodes,
       &Number_of_Surf_Quads,
       &Number_of_Surf_Trias,
       &Number_of_Vol_Hexes,
       &Number_of_Vol_Pents_5,
       &Number_of_Vol_Pents_6,
       &Number_of_Vol_Tets,
       Surf_Grid_BC_Flag,
       Surf_ID_Flag,
       Surf_Reconnection_Flag,
       Surf_Quad_Connectivity,
       Surf_Tria_Connectivity,
       Vol_Hex_Connectivity,
       Vol_ID_Flag,
       Vol_Pent_5_Connectivity,
       Vol_Pent_6_Connectivity,
       Vol_Tet_Connectivity,
       Coordinates,
       Initial_Normal_Spacing,
       BL_Thickness);

  std::cout <<Error_Message<<std::endl;

  EXPECT_NE(error_flag, 0);
}
