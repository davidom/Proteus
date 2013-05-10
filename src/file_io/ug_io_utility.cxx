#include <geometry/entity.hxx>
#include <geometry/entity_list.hxx>
#include <geometry/geometry.hxx>
#include "UG_IO_LIB.h"

#include <string>
#include <iostream>

void
create_entity_lists_from_file_ug_io
(std::string file_name,
 Proteus::Geometry::node_list &node_list,
 Proteus::Geometry::tria_list &tria_list,
 Proteus::Geometry::quad_list &quad_list,
 Proteus::Geometry::tet_list &tet_list,
 Proteus::Geometry::pent5_list &pent5_list,
 Proteus::Geometry::pent6_list &pent6_list,
 Proteus::Geometry::hex_list &hex_list)
{
  // Supress output from UG_IO
  int stdout_fd = dup(STDOUT_FILENO);
  FILE *fp = freopen("/dev/null", "w", stdout);

  int error_flag;
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

  // Return stdout to it's normal state (no longer supressed)  
  fclose(stdout);
  dup2(stdout_fd, STDOUT_FILENO);
  stdout = fdopen(STDOUT_FILENO, "w");
  close(stdout_fd);

  //std::cout <<"Read "<<Number_of_Nodes<<" nodes from file.\n";
  //std::cout <<"Read "<<Number_of_Surf_Trias<<" trias from file.\n";
  //std::cout <<"Read "<<Number_of_Surf_Quads<<" quads from file.\n";
  //std::cout <<"Read "<<Number_of_BL_Vol_Tets<<" BL tets from file.\n";
  //std::cout <<"Read "<<Number_of_Vol_Tets<<" tets from file.\n";
  //std::cout <<"Read "<<Number_of_Vol_Pents_5<<" pents5 from file.\n";
  //std::cout <<"Read "<<Number_of_Vol_Pents_6<<" pents6 from file.\n";
  //std::cout <<"Read "<<Number_of_Vol_Hexes<<" hexes from file.\n";

  // Create Nodes
  for(int i=1; i<=Number_of_Nodes; ++i) {
	node_list.push(Coordinates[i][0],Coordinates[i][1],Coordinates[i][2]);
  }
  delete [] Coordinates;

  // Create Trias
  // Must remember to shift to 0-based numbers
  for(int i=1; i<=Number_of_Surf_Trias; ++i) {
	tria_list.push
	(Surf_Tria_Connectivity[i][0]-1,
	 Surf_Tria_Connectivity[i][1]-1,
	 Surf_Tria_Connectivity[i][2]-1);
  }
  delete [] Surf_Tria_Connectivity;

  // Create Quads
  // Must remember to shift to 0-based numbers
  for(int i=1; i<=Number_of_Surf_Quads; ++i) {
	quad_list.push
	(Surf_Quad_Connectivity[i][0]-1,
	 Surf_Quad_Connectivity[i][1]-1,
	 Surf_Quad_Connectivity[i][2]-1);
  }
  delete [] Surf_Quad_Connectivity;

  delete [] Surf_Grid_BC_Flag;
  delete [] Surf_ID_Flag;
  delete [] Surf_Reconnection_Flag;
  delete [] Vol_Hex_Connectivity;
  delete [] Vol_ID_Flag;
  delete [] Vol_Pent_5_Connectivity;
  delete [] Vol_Pent_6_Connectivity;
  delete [] Vol_Tet_Connectivity;
  delete [] Initial_Normal_Spacing;
}

void
write_entity_lists_to_file_ug_io
(std::string file_name,
 Proteus::Geometry::node_list &node_list,
 Proteus::Geometry::tria_list &tria_list,
 Proteus::Geometry::quad_list &quad_list,
 Proteus::Geometry::tet_list &tet_list,
 Proteus::Geometry::pent5_list &pent5_list,
 Proteus::Geometry::pent6_list &pent6_list,
 Proteus::Geometry::hex_list &hex_list)
{
  // Supress output from UG_IO
  int stdout_fd = dup(STDOUT_FILENO);
  FILE *fp = freopen("/dev/null", "w", stdout);

  int error_flag;
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

  // Record the List Sizes
  Number_of_BL_Vol_Tets = 0;
  Number_of_Nodes = node_list.size();
  Number_of_Surf_Quads = quad_list.size();
  Number_of_Surf_Trias = tria_list.size();
  Number_of_Vol_Hexes = hex_list.size();
  Number_of_Vol_Pents_5 = pent5_list.size();
  Number_of_Vol_Pents_6 = pent6_list.size();
  Number_of_Vol_Tets = tet_list.size();

  // Allocate Arrays
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

  // Counters
  std::size_t i, j;

  // Copy Over Coordinates
  for(i=0, j=1; i<Number_of_Nodes; ++i, ++j) {
	Coordinates[j][0] = node_list[i][0];
	Coordinates[j][1] = node_list[i][1];
	Coordinates[j][2] = node_list[i][2];

	Initial_Normal_Spacing[j] = 0.;
  }

  // Copy Over Trias
  for(i=0, j=1; i<Number_of_Surf_Trias; ++i, ++j) {
	Surf_Tria_Connectivity[j][0] = tria_list[i][0] + 1;
	Surf_Tria_Connectivity[j][1] = tria_list[i][1] + 1;
	Surf_Tria_Connectivity[j][2] = tria_list[i][2] + 1;

	Surf_Grid_BC_Flag[j] = 1;
	Surf_Reconnection_Flag[j] = 0;
  }

  ug_io_write_grid_file
  (Grid_File_Name,
   Error_Message,
   UG_IO_Param_Struct_Ptr,
   Message_Flag,
   Number_of_BL_Vol_Tets,
   Number_of_Nodes,
   Number_of_Surf_Quads,
   Number_of_Surf_Trias,
   Number_of_Vol_Hexes,
   Number_of_Vol_Pents_5,
   Number_of_Vol_Pents_6,
   Number_of_Vol_Tets,
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

  // Return stdout to it's normal state (no longer supressed)  
  fclose(stdout);
  dup2(stdout_fd, STDOUT_FILENO);
  stdout = fdopen(STDOUT_FILENO, "w");
  close(stdout_fd);

}
