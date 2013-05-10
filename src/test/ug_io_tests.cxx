#include "gtest/gtest.h"
#include "UG_IO_LIB.h"
#include <iostream>
#include <file_io/ug_io_utility.hxx>

TEST(UGIOTest, FileReadErrorNoFile)
{
  std::string file_name = "non-existent_file.false_extension"; 
  char *Grid_File_Name = const_cast<char*>(file_name.c_str());
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

  EXPECT_NE(error_flag, 0);
}

TEST(UGIOTest, ReadSurfFileSuccess)
{
  int stdout_fd = dup(STDOUT_FILENO);
  FILE *fp = freopen("/dev/null", "w", stdout);
  int error_flag;
  std::string file_name = "../test_data/horse.surf";
  char * Grid_File_Name = const_cast<char*>(file_name.c_str());
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

  // First Call
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

  EXPECT_EQ(error_flag,0);

  Surf_Grid_BC_Flag = new INT_1D[Number_of_Surf_Quads + Number_of_Surf_Trias + 1];
  Surf_ID_Flag = new INT_1D[Number_of_Surf_Quads + Number_of_Surf_Trias + 1];
  Surf_Reconnection_Flag = new INT_1D[Number_of_Surf_Quads + Number_of_Surf_Trias + 1];
  Vol_ID_Flag = new INT_1D[Number_of_Vol_Tets + Number_of_Vol_Pents_5 + Number_of_Vol_Pents_6 + Number_of_Vol_Hexes + 1];
  Surf_Tria_Connectivity = new INT_3D[Number_of_Surf_Trias + 1];
  Surf_Quad_Connectivity = new INT_4D[Number_of_Surf_Quads + 1];
  Vol_Tet_Connectivity = new INT_4D[Number_of_Vol_Tets + 1];
  Vol_Pent_5_Connectivity = new INT_5D[Number_of_Vol_Pents_5 + 1];
  Vol_Pent_6_Connectivity = new INT_6D[Number_of_Vol_Pents_6 + 1];
  Vol_Hex_Connectivity = new INT_8D[Number_of_Vol_Hexes + 1];
  Initial_Normal_Spacing = new DOUBLE_1D[Number_of_Nodes + 1];
  Coordinates = new DOUBLE_3D[Number_of_Nodes + 1];

  //Second Call
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
  fclose(stdout);
  dup2(stdout_fd, STDOUT_FILENO);
  stdout = fdopen(STDOUT_FILENO, "w");
  close(stdout_fd);

  EXPECT_EQ(error_flag,0);
}

TEST(UGIOTest, FileIOUtilityRead)
{
  Proteus::Geometry::node_list node_list;
  Proteus::Geometry::tria_list tria_list;
  Proteus::Geometry::quad_list quad_list;
  Proteus::Geometry::tet_list tet_list;
  Proteus::Geometry::pent5_list pent5_list;
  Proteus::Geometry::pent6_list pent6_list;
  Proteus::Geometry::hex_list hex_list;

  std::string file_name = "../test_data/horse.surf";
  EXPECT_NO_THROW(
  create_entity_lists_from_file_ug_io
  (file_name,
   node_list,
   tria_list,
   quad_list,
   tet_list,
   pent5_list,
   pent6_list,
   hex_list));
}

TEST(UGIOTest, FileIOUtilityWrite)
{
  Proteus::Geometry::node_list node_list;
  Proteus::Geometry::tria_list tria_list;
  Proteus::Geometry::quad_list quad_list;
  Proteus::Geometry::tet_list tet_list;
  Proteus::Geometry::pent5_list pent5_list;
  Proteus::Geometry::pent6_list pent6_list;
  Proteus::Geometry::hex_list hex_list;

  std::string file_name = "../test_data/horse.surf";
  EXPECT_NO_THROW(
    create_entity_lists_from_file_ug_io
    (file_name,
     node_list,
     tria_list,
     quad_list,
     tet_list,
     pent5_list,
     pent6_list,
     hex_list)
  );

  std::string new_file_name = "../test_data/horse_write.surf";
  EXPECT_NO_THROW(
    write_entity_lists_to_file_ug_io
    (new_file_name,
     node_list,
     tria_list,
     quad_list,
     tet_list,
     pent5_list,
     pent6_list,
     hex_list)
  );
}
