#include "UG_CPP_LIB.h"
#include <vector>
#include <deque>
#include <map>


INT_ ug_io_write_OpenFOAM
 (char Error_Message[],
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
  INT_5D * Vol_Pent_5_Connectivity,
  INT_6D * Vol_Pent_6_Connectivity,
  INT_4D * Vol_Tet_Connectivity,
  DOUBLE_3D * Coordinates)
{
/*
 * Write grid data to an OpenFOAM volume grid file.
 * 
 * UG CPP LIB : Unstructured Grid - C++ Routine Library
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_OpenFOAM.cpp,v 1.7 2012/12/12 23:17:45 marcum Exp $
 */

  char Points_File_Name[] = "points";
  char Faces_File_Name[] = "faces";
  char Neighbor_File_Name[] = "neighbour";
  char Owner_File_Name[] = "owner";
  char Boundary_File_Name[] = "boundary";
  char Write_Mode[] = "w";

  char Foam_Header[] = "/*--------------------------------*- C++ -*----------------------------------*\\\n| =========                 |                                                 |\n| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |\n|  \\\\    /   O peration     | Version:  1.7.1                                 |\n|   \\\\  /    A nd           | Web:      www.OpenFOAM.com                      |\n|    \\\\/     M anipulation  |                                                 |\n\\*---------------------------------------------------------------------------*/\n";
  char Foam_Separator1[] = "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //\n\n\n";
  char Foam_Separator2[] = "// ************************************************************************* //\n";

  FILE *Grid_File, *Grid_File2;

  INT_ cellI, Index, Write_Flag, j, node0, node1, node2, owner_cell;
  INT_ number_of_omitted_faces = 0;
  INT_ num_faces_written = 0;

  double a,b,c,d,e,f,g,h,i, result;

  Grid_File = ug_fopen(Points_File_Name, Write_Mode);

  if(Grid_File == NULL) {
    strcpy (Error_Message, "unable to open OpenFOAM points file");
    return (1);
  }

  //Write Node Coordinates
  fprintf(Grid_File,"%s",Foam_Header);
  fprintf(Grid_File,"FoamFile\n{\n    version     2.0;\n    format      ascii;\n    class       vectorField;\n    location    \"constant/polyMesh\";\n    object      points;\n}\n");
  fprintf(Grid_File,"%s",Foam_Separator1);
  Write_Flag = fprintf(Grid_File, "%d\n(\n",Number_of_Nodes);

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing OpenFOAM cellShape grid file");
    return (1);
  }

  for(Index=1; Index<=Number_of_Nodes; ++Index) {
    fprintf(Grid_File, "(%.15g %.15g %.15g)\n",
	    Coordinates[Index][0],
	    Coordinates[Index][1],
	    Coordinates[Index][2]);
  }
  fprintf(Grid_File, "\n)\n\n\n");
  fprintf(Grid_File,"%s",Foam_Separator2);
  ug_fclose(Grid_File);

  INT_ Number_of_Tria_Faces = 0, Number_of_Quad_Faces = 0, tf2, qf2,
       Number_of_Surfaces;
  INT_1D *Groups = NULL;
  INT_5D *Tria_Faces = NULL;
  INT_6D *Quad_Faces = NULL;

  if(ug3_extract_faces_from_volume_elements
     (Number_of_Surf_Quads,
      Number_of_Surf_Trias,
      Number_of_Vol_Hexs,
      Number_of_Vol_Pents_5,
      Number_of_Vol_Pents_6,
      Number_of_Vol_Tets,
      Surf_ID_Flag,
      Surf_Quad_Connectivity,
      Surf_Tria_Connectivity,
      Vol_Hex_Connectivity,
      Vol_Pent_5_Connectivity,
      Vol_Pent_6_Connectivity,
      Vol_Tet_Connectivity,
      &Number_of_Tria_Faces,
      &Number_of_Quad_Faces,
      &Number_of_Surfaces,
      &Groups,
      &Tria_Faces,
      &Quad_Faces)) {

    strcpy (Error_Message, "unable to extract faces for OpenFOAM grid file");
    return (1);
  }

  //Calculate centroids of the cells
  std::vector< std::vector<double> > CellCentroids(Number_of_Vol_Hexs +
						   Number_of_Vol_Pents_5 +
						   Number_of_Vol_Pents_6 +
						   Number_of_Vol_Tets);
  INT_ offset=0;
  for(Index=0; Index < Number_of_Vol_Tets; ++Index) {
    CellCentroids[Index].clear();
    CellCentroids[Index].resize(3,0);

    for(j=0; j<3; ++j)
      CellCentroids[Index][j] = 
	(Coordinates[Vol_Tet_Connectivity[Index+1][0]][j] + 
	 Coordinates[Vol_Tet_Connectivity[Index+1][1]][j] +
	 Coordinates[Vol_Tet_Connectivity[Index+1][2]][j] +
	 Coordinates[Vol_Tet_Connectivity[Index+1][3]][j]) / 4.;

  }
  offset += Number_of_Vol_Tets;
  for(Index=0; Index < Number_of_Vol_Pents_5; ++Index) {
    CellCentroids[Index+offset].clear();
    CellCentroids[Index+offset].resize(3,0);

    for(j=0; j<3; ++j)
      CellCentroids[Index+offset][j] = 
	(Coordinates[Vol_Pent_5_Connectivity[Index+1][0]][j] + 
	 Coordinates[Vol_Pent_5_Connectivity[Index+1][1]][j] +
	 Coordinates[Vol_Pent_5_Connectivity[Index+1][2]][j] +
	 Coordinates[Vol_Pent_5_Connectivity[Index+1][3]][j] +
	 Coordinates[Vol_Pent_5_Connectivity[Index+1][4]][j]) / 5.;
  }

  offset += Number_of_Vol_Pents_5;
  for(Index=0; Index < Number_of_Vol_Pents_6; ++Index) {
    CellCentroids[Index+offset].clear();
    CellCentroids[Index+offset].resize(3,0);

    for(j=0; j<3; ++j)
      CellCentroids[Index+offset][j] = 
	(Coordinates[Vol_Pent_6_Connectivity[Index+1][0]][j] + 
	 Coordinates[Vol_Pent_6_Connectivity[Index+1][1]][j] +
	 Coordinates[Vol_Pent_6_Connectivity[Index+1][2]][j] +
	 Coordinates[Vol_Pent_6_Connectivity[Index+1][3]][j] +
	 Coordinates[Vol_Pent_6_Connectivity[Index+1][4]][j] + 
	 Coordinates[Vol_Pent_6_Connectivity[Index+1][5]][j]) / 6.;
  }

  offset += Number_of_Vol_Pents_6;
  for(Index=0; Index < Number_of_Vol_Hexs; ++Index) {
    CellCentroids[Index+offset].clear();
    CellCentroids[Index+offset].resize(3,0);

    for(j=0; j<3; ++j)
      CellCentroids[Index+offset][j] = 
	(Coordinates[Vol_Hex_Connectivity[Index+1][0]][j] + 
	 Coordinates[Vol_Hex_Connectivity[Index+1][1]][j] +
	 Coordinates[Vol_Hex_Connectivity[Index+1][2]][j] +
	 Coordinates[Vol_Hex_Connectivity[Index+1][3]][j] +
	 Coordinates[Vol_Hex_Connectivity[Index+1][4]][j] + 
	 Coordinates[Vol_Hex_Connectivity[Index+1][5]][j] +
	 Coordinates[Vol_Hex_Connectivity[Index+1][6]][j] +
	 Coordinates[Vol_Hex_Connectivity[Index+1][7]][j]) / 8.;
  }
  //Done calculating the centroids of the cells

  //Sort the Tria and Quad Arrays so that the faces that define the surface
  // grids are together
  //First Compress out the duplicate data
  INT_ Number_of_Boundary_Trias = 0, Number_of_Boundary_Quads = 0;
  for(Index=0; Index<Number_of_Tria_Faces; ++Index) {
    //Compressing Out Duplicate Tria Entries
    tf2 = Index * 2;

    Tria_Faces[Index][0] = Tria_Faces[tf2][0];
    Tria_Faces[Index][1] = Tria_Faces[tf2][1];
    Tria_Faces[Index][2] = Tria_Faces[tf2][2];
    //Index is place to write to, tf2 is place to read from
    if(Tria_Faces[tf2][3] < 0) {
      //Put what is here in the Index Location, boundary information is here
      Tria_Faces[Index][3] = Tria_Faces[tf2][3];
      Tria_Faces[Index][4] = Tria_Faces[tf2+1][3];
      ++Number_of_Boundary_Trias;
    }
    else if(Tria_Faces[tf2+1][3] < 0) {
      //Put what is here in the Index Location instead, boundary information is here
      Tria_Faces[Index][3] = Tria_Faces[tf2+1][3];      
      Tria_Faces[Index][4] = Tria_Faces[tf2][3];
      ++Number_of_Boundary_Trias;
    }
    else {
      //This tria is not on a boundary and therefore the neighbor information
      // from both entries is needed
      //compare the neighbor from both entries tf2 and tf2+1
      if(Tria_Faces[tf2][3] < Tria_Faces[tf2+1][3]) {
	Tria_Faces[Index][3] = Tria_Faces[tf2][3];      
	Tria_Faces[Index][4] = Tria_Faces[tf2+1][3];
      }
      else {
	Tria_Faces[Index][3] = Tria_Faces[tf2+1][3];      
	Tria_Faces[Index][4] = Tria_Faces[tf2][3];
      }
    }
  }

  for(Index=0; Index<Number_of_Quad_Faces; ++Index) {
    //Compressing Out Duplicate Quad Entries
    qf2 = 2*Index;

    Quad_Faces[Index][0] = Quad_Faces[qf2][0];
    Quad_Faces[Index][1] = Quad_Faces[qf2][1];
    Quad_Faces[Index][2] = Quad_Faces[qf2][2];
    Quad_Faces[Index][3] = Quad_Faces[qf2][3];
    //Index is place to write to, qf2 is place to read from
    if(Quad_Faces[qf2][4] < 0) {
      //Put what is here in the Index Location, boundary information is here
      Quad_Faces[Index][4] = Quad_Faces[qf2][4];
      Quad_Faces[Index][5] = Quad_Faces[qf2+1][4];
      ++Number_of_Boundary_Quads;
    }
    else if(Quad_Faces[qf2+1][4] < 0) {
      //Put what is here in the Index Location instead, boundary information is here
      Quad_Faces[Index][4] = Quad_Faces[qf2+1][4];
      Quad_Faces[Index][5] = Quad_Faces[qf2][4];
      ++Number_of_Boundary_Quads;
    }
    else {
      //This quad is not on a boundary and therefore the neighbor information from
      // both entries is needed
      //compare the neighbor from both entries qf2 and qf2+1
      if(Quad_Faces[qf2][4] < Quad_Faces[qf2+1][4]) {
	Quad_Faces[Index][4] = Quad_Faces[qf2][4];      
	Quad_Faces[Index][5] = Quad_Faces[qf2+1][4];
      }
      else {
	Quad_Faces[Index][4] = Quad_Faces[qf2+1][4];      
	Quad_Faces[Index][5] = Quad_Faces[qf2][4];
      }
    }
  }

  //Put Triangles and Quads in Upper Triangluar Order
  INT_ Total_Number_of_Cells = Number_of_Vol_Hexs +
                               Number_of_Vol_Pents_5 +
                               Number_of_Vol_Pents_6 +
                               Number_of_Vol_Tets;
  INT_ currentCell, cellInOrder = 0, faceInOrder, neighborCellI, faceI;
  std::deque<int> nextCell;
  std::vector<int>::iterator vec_itr, vec_itr_end;
  std::vector<int> newOrderCell(Total_Number_of_Cells);
  std::vector<int> CellNewOrder(Total_Number_of_Cells);
  std::vector<int> newOrderFace(Number_of_Tria_Faces + Number_of_Quad_Faces - Number_of_Boundary_Trias - Number_of_Boundary_Quads,-1);
  std::vector<int> visited(Total_Number_of_Cells,0);
  std::map<int,int> neighbors;
  std::map<int,int>::iterator map_itr, map_itr_end;
  std::vector< std::vector<int> > cell_list(Total_Number_of_Cells);
  
  //make the cell_list of the owners of the faces
  if(Number_of_Tria_Faces > 0) {
    for(Index=0; Index<Number_of_Tria_Faces; ++Index) {
      if(Tria_Faces[Index][3] > 0) {
	cell_list.at(Tria_Faces[Index][3]-1).push_back(Index);
      }

      if(Tria_Faces[Index][4] > 0) {
	cell_list.at(Tria_Faces[Index][4]-1).push_back(Index);		  
      }
    }
  }
  if(Number_of_Quad_Faces > 0) {
    for(Index=0; Index<Number_of_Quad_Faces; ++Index) {
      if(Quad_Faces[Index][4] > 0) {
	cell_list.at(Quad_Faces[Index][4]-1).push_back(Index+Number_of_Tria_Faces);
      }

      if(Quad_Faces[Index][5] > 0) {
	cell_list.at(Quad_Faces[Index][5]-1).push_back(Index+Number_of_Tria_Faces);
      }
    }
  }

  cellInOrder = 0;
  //Put cells in upperTriangular order
  for(cellI=0; cellI<Total_Number_of_Cells; ++cellI) {
    //fidn the first cell that has not been visited yet
    if(visited[cellI] == 0) {

      currentCell = cellI;

      //use this cell as a start
      nextCell.push_back(currentCell);

      //loop through the nextCell list. Add the first cell into the cell order if it
      // has not already been visited and ask for its neighbors.  If the neighbor in
      // question has not been visited, add it to the end of the nextCell list
      
      while(nextCell.empty() == false) {

	currentCell = nextCell.front();
	nextCell.pop_front();

	if(visited[currentCell] == 0) {

	  visited[currentCell] = 1;

	  //add into cellOrder
	  newOrderCell[currentCell] = cellInOrder;
	  CellNewOrder[cellInOrder] = currentCell;
	  ++cellInOrder;

	  //find if the neighbors have been visited
	  //get neighbors
	  neighbors.clear();

	  vec_itr = cell_list[currentCell].begin();
	  vec_itr_end = cell_list[currentCell].end();
	  while(vec_itr != vec_itr_end) {

	    faceI = *vec_itr;

	    if(faceI < Number_of_Tria_Faces) {
	      if(Tria_Faces[faceI][3] < 0) {
		++vec_itr;
		continue;
	      }
	      
	      neighborCellI = Tria_Faces[faceI][4]-1;
	      if(neighborCellI == currentCell)
		neighborCellI = Tria_Faces[faceI][3]-1;
	    }
	    else {
	      if(Quad_Faces[faceI-Number_of_Tria_Faces][4] < 0) {
		++vec_itr;
		continue;
	      }
	      
	      neighborCellI = Quad_Faces[faceI-Number_of_Tria_Faces][5]-1;
	      if(neighborCellI == currentCell)
		neighborCellI = Quad_Faces[faceI-Number_of_Tria_Faces][4]-1;
	    }
	    
	    if(visited[neighborCellI] == 0) {

	      //not visited, add to the list
	      nextCell.push_back(neighborCellI);
	    }

	    ++vec_itr;
	  }
	}
      }
    }
  }

  //Reset Visited array
  for(j=0; j<Total_Number_of_Cells; ++j) {
    //if(visited[j] != 1)
      //printf("Cell %d not visited\n",j);
    visited[j] = 0;
  }

  faceInOrder = 0;
  //Cells are renumbered, put Faces in upperTriangular order
  for(cellI=0; cellI<Total_Number_of_Cells; ++cellI) {
    //find the first cell that has not been visited yet
    if(visited[cellI] == 0) {

      currentCell = cellI;

      //use this cell as a start
      nextCell.push_back(currentCell);

      //loop through the nextCell list. Add the first cell into the cell order if it
      // has not already been visited and ask for its neighbors.  If the neighbor in
      // question has not been visited, add it to the end of the nextCell list
      
      while(nextCell.empty() == false) {

	currentCell = nextCell.front();
	nextCell.pop_front();

	if(visited[currentCell] == 0) {

	  visited[currentCell] = 1;

	  neighbors.clear();

	  vec_itr = cell_list[currentCell].begin();
	  vec_itr_end = cell_list[currentCell].end();
	  while(vec_itr != vec_itr_end) {
	    
	    faceI = *vec_itr;
	    
	    if(faceI < Number_of_Tria_Faces) {
	      if(Tria_Faces[faceI][3] <= 0) {
		++number_of_omitted_faces;
		++vec_itr;
		continue;
	      }
	      
	      neighborCellI = Tria_Faces[faceI][4]-1;
	      if(neighborCellI == currentCell)
		neighborCellI = Tria_Faces[faceI][3]-1;
	    }
	    else {
	      if(Quad_Faces[faceI-Number_of_Tria_Faces][4] <= 0) {
		++number_of_omitted_faces;
		++vec_itr;
		continue;
	      }
	      
	      neighborCellI = Quad_Faces[faceI-Number_of_Tria_Faces][5]-1;
	      if(neighborCellI == currentCell)
		neighborCellI = Quad_Faces[faceI-Number_of_Tria_Faces][4]-1;
	    }
	    
	    if(visited[neighborCellI] == 0) {
	      
	      neighbors[newOrderCell[neighborCellI]] = faceI;

	      nextCell.push_back(neighborCellI);
	    }

	    ++vec_itr;
	  }

	  map_itr = neighbors.begin(); map_itr_end = neighbors.end();
	  while(map_itr != map_itr_end) {
	    faceI = map_itr->second;
	    if(faceI < Number_of_Tria_Faces) {
	      //if(Tria_Faces[faceI][3] <= 0)
		//printf("Adding Boundary Tria to interior UT ordered faces!\n");
	    }
	    else {
	      //if(Quad_Faces[faceI-Number_of_Tria_Faces][4] <= 0)
		//printf("Adding Boundary Quad to interior UT ordered faces!\n");
	    }
	    newOrderFace.at(faceInOrder) = faceI;
	    //printf("newOrderFace[%d] = %d\n",faceInOrder,newOrderFace[faceInOrder]);

	    ++faceInOrder;

	    ++map_itr;
	  }
	}
      }
    }
  }
  //printf("Number of Omitted Faces = %d\n",number_of_omitted_faces);
  //printf("Done with UT ordering\n");
  //Done with Putting Triangles and Quads in Upper Triangular Order
  //for(j=0; j<((INT_) (newOrderFace.size())); ++j) {
    //if(newOrderFace[j] < Number_of_Tria_Faces) {
      //if(Tria_Faces[newOrderFace[j]][3] <= 0)
	//printf("Tria %d:{%d, %d, %d} is unset on surface %d:%d\n",j,Tria_Faces[newOrderFace[j]][0],Tria_Faces[newOrderFace[j]][1],Tria_Faces[newOrderFace[j]][2],Tria_Faces[newOrderFace[j]][3],Tria_Faces[newOrderFace[j]][4]);
    //}
    //else {
      //if(Quad_Faces[newOrderFace[j]-Number_of_Tria_Faces][4] <= 0)
	//printf("Quad %d is unset on surface %d:%d\n",j+1,Quad_Faces[newOrderFace[j]-Number_of_Tria_Faces][4],Quad_Faces[newOrderFace[j]-Number_of_Tria_Faces][5]);
    //}
  //}
  //Re-number the Cell labels
  //Re-number the Tria Cell labels
  //data in Tria_Faces is 1-based, data in newOrderCell is 0-based, hence the
  // playing with the indices and values below
  if(Number_of_Tria_Faces > 0) {
    for(Index=0; Index<Number_of_Tria_Faces; ++Index) {
      if(Tria_Faces[Index][3] > 0) {
	Tria_Faces[Index][3] = newOrderCell[Tria_Faces[Index][3]-1]+1;
	Tria_Faces[Index][4] = newOrderCell[Tria_Faces[Index][4]-1]+1;
	if(Tria_Faces[Index][3] > Tria_Faces[Index][4])
	  ug3_swap_int(&Tria_Faces[Index][3],&Tria_Faces[Index][4]);
      }
      else {
	Tria_Faces[Index][4] = newOrderCell[Tria_Faces[Index][4]-1]+1;
      }
    }
  }
  
  //Re-number Quad Cell Labels
  if(Number_of_Quad_Faces > 0) {
    for(Index=0; Index<Number_of_Quad_Faces; ++Index) {
      if(Quad_Faces[Index][4] > 0) {
	Quad_Faces[Index][4] = newOrderCell[Quad_Faces[Index][4]-1]+1;
	Quad_Faces[Index][5] = newOrderCell[Quad_Faces[Index][5]-1]+1;
	if(Quad_Faces[Index][4] > Quad_Faces[Index][5])
	  ug3_swap_int(&Quad_Faces[Index][4],&Quad_Faces[Index][5]);
      }
      else {
	Quad_Faces[Index][5] = newOrderCell[Quad_Faces[Index][5]-1]+1;
      }
    }
  }
  //printf("Done Re-numbering Cells\n");
  //Done re-numbering the cell labels

  //Ensure Correct Face Orientation
  if(Number_of_Tria_Faces > 0) {
    for(Index=0; Index<Number_of_Tria_Faces; ++Index) {
      if(Tria_Faces[Index][3] > 0)
	owner_cell = CellNewOrder[Tria_Faces[Index][3]-1];
      else
	owner_cell = CellNewOrder[Tria_Faces[Index][4]-1];

      node0 = Tria_Faces[Index][0];
      node1 = Tria_Faces[Index][1];
      node2 = Tria_Faces[Index][2];

      a = Coordinates[node0][0] - CellCentroids[owner_cell][0];
      b = Coordinates[node0][1] - CellCentroids[owner_cell][1];
      c = Coordinates[node0][2] - CellCentroids[owner_cell][2];
      d = Coordinates[node1][0] - CellCentroids[owner_cell][0];
      e = Coordinates[node1][1] - CellCentroids[owner_cell][1];
      f = Coordinates[node1][2] - CellCentroids[owner_cell][2];
      g = Coordinates[node2][0] - CellCentroids[owner_cell][0];
      h = Coordinates[node2][1] - CellCentroids[owner_cell][1];
      i = Coordinates[node2][2] - CellCentroids[owner_cell][2];

      result = (a*((e*i)-(f*h)) - b*((d*i)-(f*g)) + c*((d*h)-(e*g)));
      
      if(result < 0) {
	Tria_Faces[Index][1] = node2;
	Tria_Faces[Index][2] = node1;
      }

    }
  }

  if(Number_of_Quad_Faces > 0) {
    for(Index=0; Index<Number_of_Quad_Faces; ++Index) {
      if(Quad_Faces[Index][4] > 0)
	owner_cell = CellNewOrder[Quad_Faces[Index][4]-1];
      else
	owner_cell = CellNewOrder[Quad_Faces[Index][5]-1];

      node0 = Quad_Faces[Index][0];
      node1 = Quad_Faces[Index][1];
      node2 = Quad_Faces[Index][2];

      a = Coordinates[node0][0] - CellCentroids[owner_cell][0];
      b = Coordinates[node0][1] - CellCentroids[owner_cell][1];
      c = Coordinates[node0][2] - CellCentroids[owner_cell][2];
      d = Coordinates[node1][0] - CellCentroids[owner_cell][0];
      e = Coordinates[node1][1] - CellCentroids[owner_cell][1];
      f = Coordinates[node1][2] - CellCentroids[owner_cell][2];
      g = Coordinates[node2][0] - CellCentroids[owner_cell][0];
      h = Coordinates[node2][1] - CellCentroids[owner_cell][1];
      i = Coordinates[node2][2] - CellCentroids[owner_cell][2];

      result = (a*((e*i)-(f*h)) - b*((d*i)-(f*g)) + c*((d*h)-(e*g)));
      
      if(result < 0) {
	Quad_Faces[Index][0] = node2;
	Quad_Faces[Index][2] = node0;
      }
    }
  }
  //Done Ensuring Correct Face Orientation

  //Put boundary faces in the new space created by compression
  tf2 = Number_of_Tria_Faces;
  for(Index = 0; Index < Number_of_Tria_Faces; ++Index) {
    //Put Tria Boundary Faces in new space
    if(Tria_Faces[Index][3] < 0) {
      Tria_Faces[tf2][1] = Tria_Faces[Index][0];
      Tria_Faces[tf2][2] = Tria_Faces[Index][1];
      Tria_Faces[tf2][3] = Tria_Faces[Index][2];
      Tria_Faces[tf2][0] = Tria_Faces[Index][3];
      Tria_Faces[tf2][4] = Tria_Faces[Index][4];
      ++tf2;
    }
  }
  //Sort this new information based on surface id
  qsort(&(Tria_Faces[Number_of_Tria_Faces]), 
	(size_t) Number_of_Boundary_Trias, 
	sizeof(INT_5D), 
	ug3_tria_sort);

  qf2 = Number_of_Quad_Faces;
  for(Index = 0; Index < Number_of_Quad_Faces; ++Index) {
    //Put Quad Boundary Faces in new space
    if(Quad_Faces[Index][4] < 0) {
      Quad_Faces[qf2][1] = Quad_Faces[Index][0];
      Quad_Faces[qf2][2] = Quad_Faces[Index][1];
      Quad_Faces[qf2][3] = Quad_Faces[Index][2];
      Quad_Faces[qf2][4] = Quad_Faces[Index][3];
      Quad_Faces[qf2][0] = Quad_Faces[Index][4];
      Quad_Faces[qf2][5] = Quad_Faces[Index][5];
      ++qf2;
    }
  }

  //Sort this new information based on surface id
  qsort(&(Quad_Faces[Number_of_Quad_Faces]), 
	(size_t) Number_of_Boundary_Quads, 
	sizeof(INT_6D), 
	ug3_quad_sort);
  //printf("Done Sorting Boundary Faces\n");
  //Done with putting Faces in new space

  //Write Face Information to File
  Grid_File = ug_fopen(Faces_File_Name, Write_Mode);
  
  if(Grid_File == NULL) {
    strcpy (Error_Message, "unable to open OpenFOAM faces file");
    return (1);
  }

  fprintf(Grid_File,"%s",Foam_Header);
  fprintf(Grid_File,"FoamFile\n{\n    version     2.0;\n    format      ascii;\n    class       faceList;\n    location    \"constant/polyMesh\";\n    object      faces;\n}\n");
  fprintf(Grid_File,"%s",Foam_Separator1);
  fprintf(Grid_File,"%d\n(\n",Number_of_Tria_Faces + Number_of_Quad_Faces);
  
  vec_itr = newOrderFace.begin(); vec_itr_end = newOrderFace.end();
  //printf("Total Faces to be written: %d\n",newOrderFace.size()+Number_of_Boundary_Trias+Number_of_Boundary_Quads);
  
  while(vec_itr != vec_itr_end) {

    faceI = *vec_itr;

    if(faceI < Number_of_Tria_Faces) {
      if(Tria_Faces[faceI][3] > 0)
	fprintf(Grid_File, "3(%d %d %d)\n",
		Tria_Faces[faceI][0]-1,
		Tria_Faces[faceI][1]-1,
		Tria_Faces[faceI][2]-1);
      //else
	//printf("Boundary Tria Found in Interior Mesh! Surface ID = %d\n",Tria_Faces[faceI][3]);
    }
    else {
      if(Quad_Faces[faceI-Number_of_Tria_Faces][4] > 0)
	fprintf(Grid_File, "4(%d %d %d %d)\n",
		Quad_Faces[faceI-Number_of_Tria_Faces][0]-1,
		Quad_Faces[faceI-Number_of_Tria_Faces][1]-1,
		Quad_Faces[faceI-Number_of_Tria_Faces][2]-1,
		Quad_Faces[faceI-Number_of_Tria_Faces][3]-1);
      //else
	//printf("Boundary Quad Found in Interior Mesh! Surface ID = %d\n",Quad_Faces[faceI][4]);
    }

    ++vec_itr;
  }
  //printf("Done writing Non-Boundary Faces\n");

  //Write Boundary Faces
  INT_ Tria_Index, Quad_Index, bound_face_counter=0;
  tf2 = Number_of_Tria_Faces + Number_of_Boundary_Trias;
  qf2 = Number_of_Quad_Faces + Number_of_Boundary_Quads;

  //No Quads, write trias
  if(Number_of_Quad_Faces == 0) {
    //printf("Writing Tria Boundary Faces\n");
    for(Index = Number_of_Tria_Faces; Index < tf2; ++Index) {
      if(Tria_Faces[Index][0] < 0) {
	fprintf(Grid_File, "3(%d %d %d)\n",
		Tria_Faces[Index][1]-1,
		Tria_Faces[Index][2]-1,
		Tria_Faces[Index][3]-1);
	++bound_face_counter;
      }
    }
  }
  //No Trias, write quads
  else if(Number_of_Tria_Faces == 0) {
    //printf("Writing Quad Boundary Faces\n");
    for(Index = Number_of_Quad_Faces; Index < qf2; ++Index) {
      if(Quad_Faces[Index][0] < 0) {
	fprintf(Grid_File, "4(%d %d %d %d)\n",
		Quad_Faces[Index][1]-1,
		Quad_Faces[Index][2]-1,
		Quad_Faces[Index][3]-1,
		Quad_Faces[Index][4]-1);
	++bound_face_counter;
      }
    }
  }
  //Quads and Trias, write them in groups
  else {
    Tria_Index = Number_of_Tria_Faces;
    Quad_Index = Number_of_Quad_Faces;
    while(1) {
      num_faces_written = 0;
      //If we've run out of trias or quads, just exit and write the rest of them
      if(Tria_Index >= Number_of_Tria_Faces + Number_of_Boundary_Trias)
	break;
      if(Quad_Index >= Number_of_Quad_Faces + Number_of_Boundary_Quads)
	break;
      
      //Trias are on surface with lower (more negative) surface id, write them
      if(Tria_Faces[Tria_Index][0] < Quad_Faces[Quad_Index][0]) {
	//printf("Writing All Tria Surface %d\n",Tria_Faces[Tria_Index][0]);
	while(1) {
	  fprintf(Grid_File, "3(%d %d %d)\n",
		  Tria_Faces[Tria_Index][1]-1,
		  Tria_Faces[Tria_Index][2]-1,
		  Tria_Faces[Tria_Index][3]-1);
	  ++bound_face_counter;
	  ++Tria_Index;
	  ++num_faces_written;
	  if(Tria_Faces[Tria_Index][0] != Tria_Faces[Tria_Index-1][0]) {
	    //printf("Triangle Surface Written with %d faces\n",num_faces_written);
	    break;
	  }
	}
      }
      //Quads are on surface with lower (more negative) surface id, write them
      else if(Tria_Faces[Tria_Index][0] > Quad_Faces[Quad_Index][0]) {
	//printf("Writing All Quad Surface %d\n",Quad_Faces[Quad_Index][0]);
	while(1) {
	  fprintf(Grid_File, "4(%d %d %d %d)\n",
		  Quad_Faces[Quad_Index][1]-1,
		  Quad_Faces[Quad_Index][2]-1,
		  Quad_Faces[Quad_Index][3]-1,
		  Quad_Faces[Quad_Index][4]-1);
	  ++bound_face_counter;
	  ++Quad_Index;
	  ++num_faces_written;
	  if(Quad_Faces[Quad_Index][0] != Quad_Faces[Quad_Index-1][0]) {
	    //printf("Quad Surface Written with %d faces\n",num_faces_written);
	    break;
	  }
	}
      }
      //Both are on the same surface, write both with trias first
      else {
	//printf("Writing Mixed Surface %d:%d\n",Tria_Faces[Tria_Index][0],Quad_Faces[Quad_Index][0]);
	while(1) {
	  fprintf(Grid_File, "3(%d %d %d)\n",
		  Tria_Faces[Tria_Index][1]-1,
		  Tria_Faces[Tria_Index][2]-1,
		  Tria_Faces[Tria_Index][3]-1);
	  ++bound_face_counter;
	  ++Tria_Index;
	  ++num_faces_written;
	  if(Tria_Faces[Tria_Index][0] != Tria_Faces[Tria_Index-1][0]) {
	    //printf("Mixed Face Written with %d Trias and ",num_faces_written);
	    num_faces_written = 0;
	    break;
	  }
	}
	while(1) {
	  fprintf(Grid_File, "4(%d %d %d %d)\n",
		  Quad_Faces[Quad_Index][1]-1,
		  Quad_Faces[Quad_Index][2]-1,
		  Quad_Faces[Quad_Index][3]-1,
		  Quad_Faces[Quad_Index][4]-1);
	  ++bound_face_counter;
	  ++Quad_Index;
	  ++num_faces_written;
	  if(Quad_Faces[Quad_Index][0] != Quad_Faces[Quad_Index-1][0]) {
	    //printf("%d Quads\n",num_faces_written);
	    num_faces_written = 0;
	    break;
	  }
	}
      }
    }
    //printf("After groups, %d Faces written\n",bound_face_counter);
    //We've run out of boundary trias, write the rest of the quads if there are any
    if(Tria_Index >= Number_of_Tria_Faces + Number_of_Boundary_Trias) {
      if(Number_of_Boundary_Quads > 0 &&
	 Quad_Index - (Number_of_Boundary_Quads + Number_of_Quad_Faces) < 0) {
	num_faces_written = 0;
	while(1) {
	  fprintf(Grid_File, "4(%d %d %d %d)\n",
		  Quad_Faces[Quad_Index][1]-1,
		  Quad_Faces[Quad_Index][2]-1,
		  Quad_Faces[Quad_Index][3]-1,
		  Quad_Faces[Quad_Index][4]-1);
	  ++bound_face_counter;
	  ++Quad_Index;
	  ++num_faces_written;
	  if(Quad_Faces[Quad_Index][0] != Quad_Faces[Quad_Index-1][0]) {
	    //printf("Quad Surface Written with %d faces\n",num_faces_written);
	    num_faces_written = 0;
	  }
	    
	  if(Quad_Faces[Quad_Index][0] > 0)
	    break;
	}
      }
    }
    //We've run out of boundary quads, write the rest of the trias if there are any
    else if(Quad_Index >= Number_of_Quad_Faces + Number_of_Boundary_Quads) {
      if(Number_of_Boundary_Trias > 0 &&
	 Tria_Index - (Number_of_Boundary_Trias + Number_of_Tria_Faces) < 0) {
	num_faces_written = 0;
	while(1) {
	  fprintf(Grid_File, "3(%d %d %d)\n",
		  Tria_Faces[Tria_Index][1]-1,
		  Tria_Faces[Tria_Index][2]-1,
		  Tria_Faces[Tria_Index][3]-1);
	  ++bound_face_counter;
	  ++Tria_Index;
	  ++num_faces_written;
	  if(Tria_Faces[Tria_Index][0] != Tria_Faces[Tria_Index-1][0]) {
	    //printf("Tria Surface Written with %d faces\n",num_faces_written);
	    num_faces_written = 0;
	  }
	  if(Tria_Faces[Tria_Index][0] > 0)
	    break;
	}
      }
    }
  }
  fprintf(Grid_File, ")\n\n\n");
  fprintf(Grid_File,"%s",Foam_Separator2);
  //printf("Interior Faces Written\t%d\n",newOrderFace.size());
  //printf("Interior Faces\t\t%d\n",Number_of_Tria_Faces+Number_of_Quad_Faces-Number_of_Boundary_Trias - Number_of_Boundary_Quads);
  //printf("Boundary Faces Written\t%d\n",bound_face_counter);
  //printf("Boundary Faces\t\t%d\n",Number_of_Boundary_Trias+Number_of_Boundary_Quads);
  ug_fclose(Grid_File);
  //printf("Done Writing Faces\n");
  //Done Writing Face Information to File

  //Write the Owner and Neighbor Files
  Grid_File = ug_fopen(Owner_File_Name, Write_Mode);

  if(Grid_File == NULL) {
    strcpy (Error_Message, "unable to open OpenFOAM owner file");
    return (1);
  }

  fprintf(Grid_File,"%s",Foam_Header);
  fprintf(Grid_File,"FoamFile\n{\n    version     2.0;\n    format      ascii;\n    class       labelList;\n    note        \"nPoints:%d  nCells:%d  nFaces:%d  nInternalFaces:%d\";\n    location    \"constant/polyMesh\";\n    object      owner;\n}\n",Number_of_Nodes,Number_of_Vol_Hexs+Number_of_Vol_Pents_5+Number_of_Vol_Pents_6+Number_of_Vol_Tets,Number_of_Tria_Faces+Number_of_Quad_Faces,Number_of_Tria_Faces+Number_of_Quad_Faces-Number_of_Surf_Trias-Number_of_Surf_Quads);
  fprintf(Grid_File,"%s",Foam_Separator1);
  fprintf(Grid_File,"%d\n(\n",Number_of_Tria_Faces+Number_of_Quad_Faces);

  Grid_File2 = ug_fopen(Neighbor_File_Name, Write_Mode);
  if(Grid_File2 == NULL) {
    strcpy (Error_Message, "unable to open OpenFOAM neighbor file");
    return (1);
  }
  fprintf(Grid_File2,"%s",Foam_Header);
  fprintf(Grid_File2,"FoamFile\n{\n    version     2.0;\n    format      ascii;\n    class       labelList;\n    note        \"nPoints:%d  nCells:%d  nFaces:%d  nInternalFaces:%d\";\n    location    \"constant/polyMesh\";\n    object      neighbour;\n}\n",Number_of_Nodes,Number_of_Vol_Hexs+Number_of_Vol_Pents_5+Number_of_Vol_Pents_6+Number_of_Vol_Tets,Number_of_Tria_Faces+Number_of_Quad_Faces,Number_of_Tria_Faces+Number_of_Quad_Faces-Number_of_Surf_Trias-Number_of_Surf_Quads);
  fprintf(Grid_File2,"%s",Foam_Separator1);
  fprintf(Grid_File2,"%d\n(\n",Number_of_Tria_Faces+Number_of_Quad_Faces);
  //Store All Non-Boundary Faces first
  vec_itr = newOrderFace.begin(); vec_itr_end = newOrderFace.end();
  while(vec_itr != vec_itr_end) {
    
    faceI = *vec_itr;
    
    if(faceI < Number_of_Tria_Faces) {
      if(Tria_Faces[faceI][3] > 0) {
	//owner should be lower-numbered cell
	//neighbour should be higher-numbered cell
	fprintf(Grid_File,"%d\n",Tria_Faces[faceI][3]-1);
	fprintf(Grid_File2,"%d\n",Tria_Faces[faceI][4]-1);
      }
    }
    else {
      if(Quad_Faces[faceI-Number_of_Tria_Faces][4] > 0) {
	//owner should be lower-numbered cell
	//neighbour should be higher-numbered cell
	fprintf(Grid_File,"%d\n",Quad_Faces[faceI-Number_of_Tria_Faces][4]-1);
	fprintf(Grid_File2,"%d\n",Quad_Faces[faceI-Number_of_Tria_Faces][5]-1);
      }
    }

    ++vec_itr;
  }

  //Store All Boundary Faces next
  //Remember that the [Tria/Quad]_Faces in the boundary section are different
  INT_ Number_of_Boundary_Surfaces = 0;
  if(Number_of_Quad_Faces == 0) {
    tf2 = Number_of_Tria_Faces + Number_of_Boundary_Trias;
    for(Index=Number_of_Tria_Faces; Index < tf2; ++Index) {
      if(Tria_Faces[Index][0] < 0) {
	//Cell is a boundary Cell
	fprintf(Grid_File,"%d\n",Tria_Faces[Index][4]-1);
	//neighbour is boundary, or -1
	fprintf(Grid_File2,"%d\n",-1);
	
	//Update number of boundary surfaces
	if(Index + 1 < tf2) {
	  if(Tria_Faces[Index+1][0] != Tria_Faces[Index][0]) {
	    ++Number_of_Boundary_Surfaces;
	  }
	}
      }
    }
    ++Number_of_Boundary_Surfaces;
  }
  else if(Number_of_Tria_Faces == 0) {
    qf2 = Number_of_Quad_Faces + Number_of_Boundary_Quads;
    for(Index = Number_of_Quad_Faces; Index < qf2; ++Index) {
      if(Quad_Faces[Index][0] < 0) {
	//Cell is a boundary Cell
	fprintf(Grid_File,"%d\n",Quad_Faces[Index][5]-1);
	//neighbor is boundary, or -1
	fprintf(Grid_File2,"%d\n",-1);

	//Update number of boundary surfaces
	if(Index + 1 < qf2) {
	  if(Quad_Faces[Index+1][0] != Quad_Faces[Index][0]) {
	    ++Number_of_Boundary_Surfaces;
	  }
	}
      }
    }
    ++Number_of_Boundary_Surfaces;
  }
  else {
    Tria_Index = Number_of_Tria_Faces;
    Quad_Index = Number_of_Quad_Faces;
    while(1) {
      //If we've run out of trias or quads, just exit and write the rest of them
      if(Tria_Index >= Number_of_Tria_Faces + Number_of_Boundary_Trias) {
	break;
      }
      if(Quad_Index >= Number_of_Quad_Faces + Number_of_Boundary_Quads) {
	break;
      }
      
      //Trias are on surface with lower (more negative) surface id, write them
      if(Tria_Faces[Tria_Index][0] < Quad_Faces[Quad_Index][0]) {
	++Number_of_Boundary_Surfaces;
	while(1) {
	  //Cell is a boundary Cell
	  fprintf(Grid_File,"%d\n",Tria_Faces[Tria_Index][4]-1);
	  //neighbour is boundary, or -1
	  fprintf(Grid_File2,"%d\n",-1);

	  ++Tria_Index;
	  if(Tria_Faces[Tria_Index][0] != Tria_Faces[Tria_Index-1][0]) {
	    //printf("Through writing all tria surface %d\n",Tria_Faces[Tria_Index-1][0]);
	    break;
	  }
	}
      }
      //Quads are on surface with lower (more negative) surface id, write them
      else if(Tria_Faces[Tria_Index][0] > Quad_Faces[Quad_Index][0]) {
	++Number_of_Boundary_Surfaces;
	while(1) {
	  //Cell is a boundary Cell
	  fprintf(Grid_File,"%d\n",Quad_Faces[Quad_Index][5]-1);
	  //neighbor is boundary, or -1
	  fprintf(Grid_File2,"%d\n",-1);

	  ++Quad_Index;
	  if(Quad_Faces[Quad_Index][0] != Quad_Faces[Quad_Index-1][0]) {
	    //printf("Through writing  all quad surface %d\n",Quad_Faces[Quad_Index-1][0]);
	    break;
	  }
	}
      }
      //Both are on the same surface, write both with trias first
      else {
	++Number_of_Boundary_Surfaces;
	while(1) {
	  //Cell is a boundary Cell
	  fprintf(Grid_File,"%d\n",Tria_Faces[Tria_Index][4]-1);
	  //neighbour is boundary, or -1
	  fprintf(Grid_File2,"%d\n",-1);
	  
	  ++Tria_Index;
	  if(Tria_Faces[Tria_Index][0] != Tria_Faces[Tria_Index-1][0])
	    break;
	}
	while(1) {
	  //Cell is a boundary Cell
	  fprintf(Grid_File,"%d\n",Quad_Faces[Quad_Index][5]-1);
	  //neighbor is boundary, or -1
	  fprintf(Grid_File2,"%d\n",-1);
	  
	  ++Quad_Index;
	  if(Quad_Faces[Quad_Index][0] != Quad_Faces[Quad_Index-1][0])
	    break;
	}
	//printf("Through writing mixed surface %d:%d\n",Tria_Faces[Tria_Index-1][0],Quad_Faces[Quad_Index-1][0]);
      }
    }
    //We've run out of boundary trias, write the rest of the quads if there are any
    qf2 = Number_of_Quad_Faces + Number_of_Boundary_Quads;
    while(Quad_Index < qf2) {
      
      //Cell is a boundary Cell
      fprintf(Grid_File,"%d\n",Quad_Faces[Quad_Index][5]-1);
      //neighbor is boundary, or -1
      fprintf(Grid_File2,"%d\n",-1);
      
      ++Quad_Index;
      if(Quad_Faces[Quad_Index][0] != Quad_Faces[Quad_Index-1][0] &&
	 Quad_Faces[Quad_Index-1][0] < 0) {
	//printf("Through writing  all quad surface %d\n",Quad_Faces[Quad_Index-1][0]);
	++Number_of_Boundary_Surfaces;
      }
    }
    //We've run out of boundary quads, write the rest of the trias if there are any
    tf2 = Number_of_Tria_Faces + Number_of_Boundary_Trias;
    while(Tria_Index < tf2) {
      //Cell is a boundary Cell
      fprintf(Grid_File,"%d\n",Tria_Faces[Tria_Index][4]-1);
      //neighbour is boundary, or -1
      fprintf(Grid_File2,"%d\n",-1);
      
      ++Tria_Index;
      if(Tria_Faces[Tria_Index][0] != Tria_Faces[Tria_Index-1][0] &&
	 Tria_Faces[Tria_Index-1][0] < 0) {
	//printf("Through writing  all tria surface %d\n",Tria_Faces[Tria_Index-1][0]);
	++Number_of_Boundary_Surfaces;
      }
    }
  }
  //printf("Number_of_Boundary_Surfaces: %d\n",Number_of_Boundary_Surfaces);
  fprintf(Grid_File, ")\n\n\n");
  fprintf(Grid_File,"%s",Foam_Separator2);
  fprintf(Grid_File2, ")\n\n\n");
  fprintf(Grid_File2,"%s",Foam_Separator2);
  ug_fclose(Grid_File);
  ug_fclose(Grid_File2);
  //printf("Done writing owner and neighbor files\n");
  //Done Writing Owner and Neighbor Files

  //Write Boundary File
  Grid_File = ug_fopen(Boundary_File_Name, Write_Mode);

  if(Grid_File == NULL) {
    strcpy (Error_Message, "unable to open OpenFOAM boundary file");
    return (1);
  }
 
  fprintf(Grid_File,"%s",Foam_Header);
  fprintf(Grid_File,"FoamFile\n{\n    version     2.0;\n    format      ascii;\n    class       polyBoundaryMesh;\n    location    \"constant/polyMesh\";\n    object      boundary;\n}\n");
  fprintf(Grid_File,"%s",Foam_Separator1);
  fprintf(Grid_File,"%d\n(\n",Number_of_Boundary_Surfaces);

  //printf("Number of Surfaces = %d\n",Number_of_Boundary_Surfaces);
  INT_ Number_of_Cells = 0, Start_Face = 0;
  if(Number_of_Quad_Faces == 0) {
    //printf("Tria Faces: %d\tBoundary Trias: %d\n",Number_of_Tria_Faces,Number_of_Boundary_Trias);
    Start_Face = Number_of_Tria_Faces - Number_of_Boundary_Trias;
    tf2 = Number_of_Tria_Faces + Number_of_Boundary_Trias;
    for(Index=Number_of_Tria_Faces; Index < tf2; ++Index) {
      if(Tria_Faces[Index][0] < 0) {
	++Number_of_Cells;
	//Update number of boundary surfaces
	if(Index + 1 < tf2) {
	  if(Tria_Faces[Index+1][0] != Tria_Faces[Index][0]) {
	    fprintf(Grid_File,"    fixedWalls\n    {\n        type            wall;\n        nFaces          %d;\n        startFace       %d;\n    }\n",Number_of_Cells,Start_Face);
	    Start_Face += Number_of_Cells;
	    Number_of_Cells = 0;
	  }
	}
      }
    }
    fprintf(Grid_File,"    fixedWalls\n    {\n        type            wall;\n        nFaces          %d;\n        startFace       %d;\n    }\n",Number_of_Cells,Start_Face);
  }
  else if(Number_of_Tria_Faces == 0) {
    Start_Face = Number_of_Quad_Faces - Number_of_Boundary_Quads;
    qf2 = Number_of_Tria_Faces + Number_of_Boundary_Trias;
    for(Index = Number_of_Quad_Faces; Index < qf2; ++Index) {
      if(Quad_Faces[Index][0] < 0) {
	++Number_of_Cells;
	//Update number of boundary surfaces
	if(Index + 1 < qf2) {
	  if(Quad_Faces[Index+1][0] != Quad_Faces[Index][0]) {
	    fprintf(Grid_File,"    fixedWalls\n    {\n        type            wall;\n        nFaces          %d;\n        startFace       %d;\n    }\n",Number_of_Cells,Start_Face);
	    Start_Face += Number_of_Cells;
	    Number_of_Cells = 0;
	  }
	}
      }
    }
    fprintf(Grid_File,"    fixedWalls\n    {\n        type            wall;\n        nFaces          %d;\n        startFace       %d;\n    }\n",Number_of_Cells,Start_Face);
  }
  else {
    Tria_Index = Number_of_Tria_Faces;
    Quad_Index = Number_of_Quad_Faces;
    Start_Face = Number_of_Tria_Faces + Number_of_Quad_Faces;
    Start_Face -= (Number_of_Boundary_Trias + Number_of_Boundary_Quads);
    while(1) {
      //If we've run out of trias or quads, just exit and write the rest of them
      if(Tria_Index >= Number_of_Tria_Faces + Number_of_Boundary_Trias) {
	break;
      }
      if(Quad_Index >= Number_of_Quad_Faces + Number_of_Boundary_Quads) {
	break;
      }
      
      //Trias are on surface with lower (more negative) surface id, write them
      if(Tria_Faces[Tria_Index][0] < Quad_Faces[Quad_Index][0] &&
	 Tria_Index < Number_of_Tria_Faces + Number_of_Boundary_Trias) {
	//printf("Writing All Tria Surface\n");
	while(1) {
	  ++Number_of_Cells;
	  ++Tria_Index;
	  if(Tria_Faces[Tria_Index][0] != Tria_Faces[Tria_Index-1][0]) {
	    fprintf(Grid_File,"    fixedWalls\n    {\n        type            wall;\n        nFaces          %d;\n        startFace       %d;\n    }\n",Number_of_Cells,Start_Face);
	    Start_Face += Number_of_Cells;
	    Number_of_Cells = 0;
	    break;
	  }
	}
      }
      //Quads are on surface with lower (more negative) surface id, write them
      else if(Tria_Faces[Tria_Index][0] > Quad_Faces[Quad_Index][0] &&
	      Quad_Index < Number_of_Quad_Faces + Number_of_Boundary_Quads) {
	//printf("Writing All Quad Surface\n");
	while(1) {
	  ++Number_of_Cells;
	  ++Quad_Index;
	  if(Quad_Faces[Quad_Index][0] != Quad_Faces[Quad_Index-1][0]) {
	    fprintf(Grid_File,"    fixedWalls\n    {\n        type            wall;\n        nFaces          %d;\n        startFace       %d;\n    }\n",Number_of_Cells,Start_Face);
	    Start_Face += Number_of_Cells;
	    Number_of_Cells = 0;
	    break;
	  }
	}
      }
      //Both are on the same surface, write both with trias first
      else {
	//printf("Writing Mixed Surface\n");
	while(1) {
	  ++Number_of_Cells;
	  ++Tria_Index;
	  if(Tria_Faces[Tria_Index][0] != Tria_Faces[Tria_Index-1][0])
	    break;
	}
	while(1) {
	  ++Number_of_Cells;
	  ++Quad_Index;
	  if(Quad_Faces[Quad_Index][0] != Quad_Faces[Quad_Index-1][0])
	    break;
	}
	fprintf(Grid_File,"    fixedWalls\n    {\n        type            wall;\n        nFaces          %d;\n        startFace       %d;\n    }\n",Number_of_Cells,Start_Face);
	Start_Face += Number_of_Cells;
	Number_of_Cells = 0;
      }
    }
    Number_of_Cells = 0;
    //We've run out of boundary trias, write the rest of the quads if there are any
    //printf("Trias left: %d\n",Number_of_Boundary_Trias+Number_of_Tria_Faces-Tria_Index);
    //printf("Quads left: %d\n",Number_of_Boundary_Quads+Number_of_Quad_Faces-Quad_Index);
    tf2 = Number_of_Tria_Faces + Number_of_Boundary_Trias;
    while(Tria_Index < tf2) {
      ++Number_of_Cells;
      ++Tria_Index;
      
      if(Tria_Faces[Tria_Index][0] != Tria_Faces[Tria_Index-1][0] &&
	 Tria_Faces[Tria_Index-1][0] < 0) {
	//printf("Only Trias left\n");
	fprintf(Grid_File,"    fixedWalls\n    {\n        type            wall;\n        nFaces          %d;\n        startFace       %d;\n    }\n",Number_of_Cells,Start_Face);
	Start_Face += Number_of_Cells;
	Number_of_Cells = 0;
      }
    }
    qf2 = Number_of_Quad_Faces + Number_of_Boundary_Quads;
    while(Quad_Index < qf2) {
      ++Number_of_Cells;
      ++Quad_Index;
      
      if(Quad_Faces[Quad_Index][0] != Quad_Faces[Quad_Index-1][0] &&
	 Quad_Faces[Quad_Index-1][0] < 0) {
	//printf("Only Quads left\n");
	fprintf(Grid_File,"    fixedWalls\n    {\n        type            wall;\n        nFaces          %d;\n        startFace       %d;\n    }\n",Number_of_Cells,Start_Face);
	Start_Face += Number_of_Cells;
	Number_of_Cells = 0;
      }      
    }
  }
  fprintf(Grid_File,")\n\n\n");
  fprintf(Grid_File,"%s",Foam_Separator2);
  ug_fclose(Grid_File);
  //Done Writing Boundary File

  /* Free working memory */
  ug_free (Groups);
  ug_free (Tria_Faces);
  ug_free (Quad_Faces);

  return (0);
}


