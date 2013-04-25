#include "UG_CPP_LIB.h"
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <exception>
#include <stdexcept>
#include <iostream>

INT_
ug3_extract_faces_from_volume_elements
(INT_ Number_of_Surf_Quads,
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
 INT_ *Number_of_Tria_Faces_,
 INT_ *Number_of_Quad_Faces_,
 INT_ *Number_of_Surfaces_,
 INT_1D **Groups_,
 INT_5D **Tria_Faces_,
 INT_6D **Quad_Faces_)
{
/*
 * Extract faces from volume elements.
 * 
 * UG CPP LIB : Unstructured Grid - C++ Routine Library
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_extract_faces_from_volume_elements.cpp,v 1.3 2012/12/12 23:17:45 marcum Exp $
 */

  bool match_found;

  INT_ i, tria_Index, quad_Index, matches_found, new_trias, deleted_quads;
  INT_ Reading_Index, Writing_Index;
  INT_ j, cellnum, npatch, nv, qf, reverse, surfid, tf, vs;
  INT_ Error_Flag, Index, Max_SurfID, Number_of_Cells, 
       Number_of_Faces, Number_of_Quad_Faces, Number_of_Tria_Faces, 
       Quad_Total, Tria_Total, Write_Flag;
  INT_ tmp_face[4];

  INT_1D *Groups = NULL;
  INT_5D *Tria_Faces = NULL;
  INT_6D *Quad_Faces = NULL;

  tf = 0;
  qf = 0;
  cellnum = 1;
  Max_SurfID = 0;
  Error_Flag = 0;
  Write_Flag = 0;

  Number_of_Cells = Number_of_Vol_Tets
                  + Number_of_Vol_Pents_5 + Number_of_Vol_Pents_6
                  + Number_of_Vol_Hexs;

  if (Number_of_Cells == 0)
    return (347);

  Number_of_Tria_Faces = Number_of_Surf_Trias + Number_of_Vol_Tets*4 
                       + Number_of_Vol_Pents_5*4 + Number_of_Vol_Pents_6*2 ;
  Number_of_Quad_Faces = Number_of_Surf_Quads + Number_of_Vol_Pents_5
                       + Number_of_Vol_Pents_6*3 + Number_of_Vol_Hexs*6;

  Tria_Total = Number_of_Tria_Faces;
  Quad_Total = Number_of_Quad_Faces;

  /* We counted face pairs, this is twice the actual number */
  *Number_of_Tria_Faces_ = Number_of_Tria_Faces = Number_of_Tria_Faces >> 1;
  *Number_of_Quad_Faces_ = Number_of_Quad_Faces = Number_of_Quad_Faces >> 1;

  Number_of_Faces = Number_of_Tria_Faces + Number_of_Quad_Faces;

  /* Allocate all memory we will need */
  if(Number_of_Vol_Hexs == 0)
    Tria_Faces = (INT_5D *)ug_malloc(&Error_Flag, Tria_Total * sizeof(INT_5D));
  else
    Tria_Faces = (INT_5D *)ug_malloc(&Error_Flag, (Tria_Total + 2*Number_of_Vol_Hexs) * sizeof(INT_5D));
  Quad_Faces = (INT_6D *) ug_malloc (&Error_Flag, Quad_Total * sizeof (INT_6D));
  
  if (Error_Flag > 0)
  {
    ug_free (Tria_Faces);
    ug_free (Quad_Faces);
    return (100317);
  }

  for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
  {
    surfid = Surf_ID_Flag[Index];
    Tria_Faces[tf][0] = Surf_Tria_Connectivity[Index][0];
    Tria_Faces[tf][1] = Surf_Tria_Connectivity[Index][1];
    Tria_Faces[tf][2] = Surf_Tria_Connectivity[Index][2];
    Tria_Faces[tf][3] = -surfid;
    if (surfid > Max_SurfID) Max_SurfID = surfid;
    tf++;
  }

  for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
  {
    surfid = Surf_ID_Flag[Number_of_Surf_Trias + Index];
    Quad_Faces[qf][0] = Surf_Quad_Connectivity[Index][0];
    Quad_Faces[qf][1] = Surf_Quad_Connectivity[Index][1];
    Quad_Faces[qf][2] = Surf_Quad_Connectivity[Index][2];
    Quad_Faces[qf][3] = Surf_Quad_Connectivity[Index][3];
    Quad_Faces[qf][4] = -surfid;
    if (surfid > Max_SurfID) Max_SurfID = surfid;
    qf++;
  }

  /* Add tet faces to list */
  for (Index = 1; Index <= Number_of_Vol_Tets; ++Index)
  {
    /* Face 1 */
    Tria_Faces[tf][0] = Vol_Tet_Connectivity[Index][0];
    Tria_Faces[tf][1] = Vol_Tet_Connectivity[Index][1];
    Tria_Faces[tf][2] = Vol_Tet_Connectivity[Index][3];
    Tria_Faces[tf][3] = cellnum;
    tf++;

    /* Face 2 */
    Tria_Faces[tf][0] = Vol_Tet_Connectivity[Index][1];
    Tria_Faces[tf][1] = Vol_Tet_Connectivity[Index][2];
    Tria_Faces[tf][2] = Vol_Tet_Connectivity[Index][3];
    Tria_Faces[tf][3] = cellnum;
    tf++;

    /* Face 3 */
    Tria_Faces[tf][0] = Vol_Tet_Connectivity[Index][3];
    Tria_Faces[tf][1] = Vol_Tet_Connectivity[Index][2];
    Tria_Faces[tf][2] = Vol_Tet_Connectivity[Index][0];
    Tria_Faces[tf][3] = cellnum;
    tf++;

    /* Face 4 */
    Tria_Faces[tf][0] = Vol_Tet_Connectivity[Index][0];
    Tria_Faces[tf][1] = Vol_Tet_Connectivity[Index][2];
    Tria_Faces[tf][2] = Vol_Tet_Connectivity[Index][1];
    Tria_Faces[tf][3] = cellnum;
    tf++;

    cellnum++;
  }

  /* Add pent5 faces to list */
  for (Index = 1; Index <= Number_of_Vol_Pents_5; ++Index)
  {
    /* Face 1 */
    Tria_Faces[tf][0] = Vol_Pent_5_Connectivity[Index][4];
    Tria_Faces[tf][1] = Vol_Pent_5_Connectivity[Index][1];
    Tria_Faces[tf][2] = Vol_Pent_5_Connectivity[Index][2];
    Tria_Faces[tf][3] = cellnum;
    tf++;

    /* Face 2 */
    Tria_Faces[tf][0] = Vol_Pent_5_Connectivity[Index][4];
    Tria_Faces[tf][1] = Vol_Pent_5_Connectivity[Index][2];
    Tria_Faces[tf][2] = Vol_Pent_5_Connectivity[Index][3];
    Tria_Faces[tf][3] = cellnum;
    tf++;

    /* Face 3 */
    Tria_Faces[tf][0] = Vol_Pent_5_Connectivity[Index][3];
    Tria_Faces[tf][1] = Vol_Pent_5_Connectivity[Index][2];
    Tria_Faces[tf][2] = Vol_Pent_5_Connectivity[Index][0];
    Tria_Faces[tf][3] = cellnum;
    tf++;

    /* Face 4 */
    Tria_Faces[tf][0] = Vol_Pent_5_Connectivity[Index][0];
    Tria_Faces[tf][1] = Vol_Pent_5_Connectivity[Index][2];
    Tria_Faces[tf][2] = Vol_Pent_5_Connectivity[Index][1];
    Tria_Faces[tf][3] = cellnum;
    tf++;

    /* Face 5 */
    Quad_Faces[qf][0] = Vol_Pent_5_Connectivity[Index][0];
    Quad_Faces[qf][1] = Vol_Pent_5_Connectivity[Index][1];
    Quad_Faces[qf][2] = Vol_Pent_5_Connectivity[Index][4];
    Quad_Faces[qf][3] = Vol_Pent_5_Connectivity[Index][3];
    Quad_Faces[qf][4] = cellnum;
    qf++;

    cellnum++;
  }

  /* Add pent6 faces to list */
  for (Index = 1; Index <= Number_of_Vol_Pents_6; ++Index)
  {
    /* Face 1 */
    Tria_Faces[tf][0] = Vol_Pent_6_Connectivity[Index][3];
    Tria_Faces[tf][1] = Vol_Pent_6_Connectivity[Index][4];
    Tria_Faces[tf][2] = Vol_Pent_6_Connectivity[Index][5];
    Tria_Faces[tf][3] = cellnum;
    tf++;

    /* Face 2 */
    Tria_Faces[tf][0] = Vol_Pent_6_Connectivity[Index][0];
    Tria_Faces[tf][1] = Vol_Pent_6_Connectivity[Index][2];
    Tria_Faces[tf][2] = Vol_Pent_6_Connectivity[Index][1];
    Tria_Faces[tf][3] = cellnum;
    tf++;

    /* Face 3 */
    Quad_Faces[qf][0] = Vol_Pent_6_Connectivity[Index][0];
    Quad_Faces[qf][1] = Vol_Pent_6_Connectivity[Index][1];
    Quad_Faces[qf][2] = Vol_Pent_6_Connectivity[Index][4];
    Quad_Faces[qf][3] = Vol_Pent_6_Connectivity[Index][3];
    Quad_Faces[qf][4] = cellnum;
    qf++;

    /* Face 4 */
    Quad_Faces[qf][0] = Vol_Pent_6_Connectivity[Index][1];
    Quad_Faces[qf][1] = Vol_Pent_6_Connectivity[Index][2];
    Quad_Faces[qf][2] = Vol_Pent_6_Connectivity[Index][5];
    Quad_Faces[qf][3] = Vol_Pent_6_Connectivity[Index][4];
    Quad_Faces[qf][4] = cellnum;
    qf++;

    /* Face 5 */
    Quad_Faces[qf][0] = Vol_Pent_6_Connectivity[Index][3];
    Quad_Faces[qf][1] = Vol_Pent_6_Connectivity[Index][5];
    Quad_Faces[qf][2] = Vol_Pent_6_Connectivity[Index][2];
    Quad_Faces[qf][3] = Vol_Pent_6_Connectivity[Index][0];
    Quad_Faces[qf][4] = cellnum;
    qf++;

    cellnum++;
  }

  /* Add hex faces to list */
  for (Index = 1; Index <= Number_of_Vol_Hexs; ++Index)
  {
    /* Face 1 */
    Quad_Faces[qf][0] = Vol_Hex_Connectivity[Index][0];
    Quad_Faces[qf][1] = Vol_Hex_Connectivity[Index][1];
    Quad_Faces[qf][2] = Vol_Hex_Connectivity[Index][5];
    Quad_Faces[qf][3] = Vol_Hex_Connectivity[Index][4];
    Quad_Faces[qf][4] = cellnum;
    qf++;

    /* Face 2 */
    Quad_Faces[qf][0] = Vol_Hex_Connectivity[Index][1];
    Quad_Faces[qf][1] = Vol_Hex_Connectivity[Index][2];
    Quad_Faces[qf][2] = Vol_Hex_Connectivity[Index][6];
    Quad_Faces[qf][3] = Vol_Hex_Connectivity[Index][5];
    Quad_Faces[qf][4] = cellnum;
    qf++;

    /* Face 3 */
    Quad_Faces[qf][0] = Vol_Hex_Connectivity[Index][2];
    Quad_Faces[qf][1] = Vol_Hex_Connectivity[Index][3];
    Quad_Faces[qf][2] = Vol_Hex_Connectivity[Index][7];
    Quad_Faces[qf][3] = Vol_Hex_Connectivity[Index][6];
    Quad_Faces[qf][4] = cellnum;
    qf++;

    /* Face 4 */
    Quad_Faces[qf][0] = Vol_Hex_Connectivity[Index][4];
    Quad_Faces[qf][1] = Vol_Hex_Connectivity[Index][7];
    Quad_Faces[qf][2] = Vol_Hex_Connectivity[Index][3];
    Quad_Faces[qf][3] = Vol_Hex_Connectivity[Index][0];
    Quad_Faces[qf][4] = cellnum;
    qf++;

    /* Face 5 */
    Quad_Faces[qf][0] = Vol_Hex_Connectivity[Index][4];
    Quad_Faces[qf][1] = Vol_Hex_Connectivity[Index][5];
    Quad_Faces[qf][2] = Vol_Hex_Connectivity[Index][6];
    Quad_Faces[qf][3] = Vol_Hex_Connectivity[Index][7];
    Quad_Faces[qf][4] = cellnum;
    qf++;

    /* Face 6 */
    Quad_Faces[qf][0] = Vol_Hex_Connectivity[Index][3];
    Quad_Faces[qf][1] = Vol_Hex_Connectivity[Index][2];
    Quad_Faces[qf][2] = Vol_Hex_Connectivity[Index][1];
    Quad_Faces[qf][3] = Vol_Hex_Connectivity[Index][0];
    Quad_Faces[qf][4] = cellnum;
    qf++;

    cellnum++;
  }

  if (tf != Tria_Total || qf != Quad_Total)
  {
    ug_free (Tria_Faces);
    ug_free (Quad_Faces);
    return (348);
  }

  /* Prepare tria faces, sort face connectivity */
  for (Index = 0; Index < Tria_Total; ++Index)
  {
    reverse = 0;
    if(Tria_Faces[Index][0] > Tria_Faces[Index][1])
    {
      ug3_swap_int(&Tria_Faces[Index][0],&Tria_Faces[Index][1]);
      reverse++;
    }
    if(Tria_Faces[Index][0] > Tria_Faces[Index][2])
    {
      ug3_swap_int(&Tria_Faces[Index][0],&Tria_Faces[Index][2]);
      reverse++;
    }
    if(Tria_Faces[Index][1] > Tria_Faces[Index][2])
    {
      ug3_swap_int(&Tria_Faces[Index][1],&Tria_Faces[Index][2]);
      reverse++;
    }

    if (reverse == 1 || reverse == 3)
      Tria_Faces[Index][4] = 1;
    else
      Tria_Faces[Index][4] = 0;
  }

  /* Prepare quad faces, sort face connectivity */
  for (Index = 0; Index < Quad_Total; ++Index)
  {
    vs = Quad_Faces[Index][0];
    nv = 0;
    for (j = 0; j < 4; ++j)
    {
      if (vs > Quad_Faces[Index][j])
      {
        vs = Quad_Faces[Index][j];
        nv = j;
      }
    }

    for (j = 0; j < 4; ++j)
    {
      tmp_face[j] = Quad_Faces[Index][(j+nv)&0x3];
    }

    /* Make orientation so that it will match other face */
    if(tmp_face[1] < tmp_face[3])
    {
      for(j = 0; j < 4; ++j)
        Quad_Faces[Index][j] = tmp_face[j];
      Quad_Faces[Index][5] = 0;
    }
    else
    {
      for(j = 0; j < 4; ++j)
        Quad_Faces[Index][j] = tmp_face[(4 - j) &0x3];
      Quad_Faces[Index][5] = 1;
    }
  }

  /* Sort the face information */
  qsort(Tria_Faces, (size_t) Tria_Total, sizeof(INT_5D), ug3_tria_sort);
  qsort(Quad_Faces, (size_t) Quad_Total, sizeof(INT_6D), ug3_quad_sort);

  //Split Mis-Matched Quad-Faces into two Trias
  std::vector<int> mismatched_quads;
  std::list<int> mismatched_trias;
  for(Index=0; Index < Tria_Total; ++Index) {
    if((Tria_Faces[Index][0] != Tria_Faces[Index+1][0] ||
	Tria_Faces[Index][1] != Tria_Faces[Index+1][1] ||
	Tria_Faces[Index][2] != Tria_Faces[Index+1][2]) &&
       Tria_Faces[Index][3] > 0) {

      mismatched_trias.push_back(Index);
    }
    else {
      ++Index;
    }
  }

  for(Index=0; Index < Quad_Total; ++Index) {
    if((Quad_Faces[Index][0] != Quad_Faces[Index+1][0] ||
	Quad_Faces[Index][1] != Quad_Faces[Index+1][1] ||
	Quad_Faces[Index][2] != Quad_Faces[Index+1][2] ||
	Quad_Faces[Index][3] != Quad_Faces[Index+1][3]) &&
       Quad_Faces[Index][4] > 0) {

      mismatched_quads.push_back(Index);
    }
    else {
      ++Index;
    }
  }

  //Resize the Tria_Faces Array
  INT_5D *temp_Tria_Faces = NULL;
  temp_Tria_Faces = (INT_5D *)ug_malloc(&Error_Flag, (Tria_Total + 2 * mismatched_quads.size()) * sizeof(INT_5D));
  //Copy contents to new, larger array
  for(Index=0; Index < Tria_Total; ++Index) {
    temp_Tria_Faces[Index][0] = Tria_Faces[Index][0];
    temp_Tria_Faces[Index][1] = Tria_Faces[Index][1];
    temp_Tria_Faces[Index][2] = Tria_Faces[Index][2];
    temp_Tria_Faces[Index][3] = Tria_Faces[Index][3];
    temp_Tria_Faces[Index][4] = Tria_Faces[Index][4];
  }
  //Free old array
  ug_free(Tria_Faces);
  //Assign pointer to new memory
  Tria_Faces = temp_Tria_Faces;

  std::vector<int>::iterator quad_itr, quad_itr_end;
  std::list<int>::iterator tria_itr, tria_itr_end;
  quad_itr = mismatched_quads.begin();
  quad_itr_end = mismatched_quads.end();
  new_trias = 0;
  deleted_quads = 0;
  while(quad_itr != quad_itr_end) {
    quad_Index = *quad_itr;

    tria_itr = mismatched_trias.begin();
    tria_itr_end = mismatched_trias.end();
    while(tria_itr != tria_itr_end) {
      matches_found = 0;
      tria_Index = *tria_itr;
      //loop through tria connectivity
      for(i=0; i<3; ++i) {
	match_found = false;
	//loop through the quad connectivity
	for(j=0; j<4; ++j) {
	  //if index is found, break and look for next index
	  if(Tria_Faces[tria_Index][i] == Quad_Faces[quad_Index][j]) {
	    match_found = true;
	    ++match_found;
	    break;
	  }
	}
	//finished looping over quad, if !found, break
	if(!match_found) {
	  break;
	}
      }
      //finished looping over tria, if found, then tria needs to be duplicated
      // remove tria from mismatched trias so it won't be searched again
      // create new tria at the end of Tria_Faces with appropriate neighbor
      // and boundary information
      if(match_found) {

	tria_itr = mismatched_trias.erase(tria_itr);
	Tria_Faces[Tria_Total][0] = Tria_Faces[tria_Index][0];
	Tria_Faces[Tria_Total][1] = Tria_Faces[tria_Index][1];
	Tria_Faces[Tria_Total][2] = Tria_Faces[tria_Index][2];
	//cellnum
	Tria_Faces[Tria_Total][3] = Quad_Faces[quad_Index][4];
	//orient flag
	Tria_Faces[Tria_Total][4] = Tria_Faces[tria_Index][4];
	++Tria_Total;
	++new_trias;

	if(matches_found == 2) //both matches found, go to next quad
	  break;
      }
      //tria not found, move on
      else
	++tria_itr;
    }

    ++quad_itr;
  }

  //if there are mismatched quads found and after the above loop there are
  // still entries in mismatched trias, then we have an error and cannot
  // continue with the data conversion
  if(mismatched_trias.empty() == false && 
     mismatched_quads.empty() == false) {
    ug_free (Tria_Faces);
    ug_free (Quad_Faces);
    return (349);
  }

  //Remove split quads from Quad_Faces Array by compression
  if(mismatched_quads.empty() == false) {
    quad_itr = mismatched_quads.begin();
    quad_itr_end = mismatched_quads.end();
    //Staring at the first quad to compress out
    Reading_Index = Writing_Index = *quad_itr;
    i = Reading_Index;
    while(i < Quad_Total) {
      if(quad_itr != quad_itr_end) {
	if(Reading_Index == (INT_)(*quad_itr)) {

	  ++quad_itr;
	  ++Reading_Index;
	  ++i;
	  ++deleted_quads;
	  continue;
	}
      }
      Quad_Faces[Writing_Index][0] = Quad_Faces[Reading_Index][0];
      Quad_Faces[Writing_Index][1] = Quad_Faces[Reading_Index][1];
      Quad_Faces[Writing_Index][2] = Quad_Faces[Reading_Index][2];
      Quad_Faces[Writing_Index][3] = Quad_Faces[Reading_Index][3];
      Quad_Faces[Writing_Index][4] = Quad_Faces[Reading_Index][4];
      Quad_Faces[Writing_Index][5] = Quad_Faces[Reading_Index][5];

      ++i;
      ++Reading_Index;
      ++Writing_Index;
    }
    Quad_Total -= mismatched_quads.size();
    mismatched_quads.clear();

    /* We counted face pairs, this is twice the actual number */
    *Number_of_Tria_Faces_ = Number_of_Tria_Faces = Tria_Total >> 1;
    *Number_of_Quad_Faces_ = Number_of_Quad_Faces = Quad_Total >> 1;  
    
    /* Sort the face information again */
    qsort(Tria_Faces, (size_t) Tria_Total, sizeof(INT_5D), ug3_tria_sort);
    qsort(Quad_Faces, (size_t) Quad_Total, sizeof(INT_6D), ug3_quad_sort);
    //printf("Number_of_Tria_Faces=%d\tNumber_of_Quad_Faces=%d\n",Number_of_Tria_Faces,Number_of_Quad_Faces);
    //printf("Tria_Total=%d\tQuad_Total=%d\n",Tria_Total,Quad_Total);
    //printf("%d New Trias added and\n",new_trias);
    //printf("%d Quads deleted \n",deleted_quads);
    //printf("due to Quad-Tria face mis-matches\n");
  }

  Groups = (INT_1D *) ug_malloc (&Error_Flag, (Max_SurfID+1) * sizeof (INT_1D));

  if (Error_Flag > 0)
  {
    ug_free (Groups);
    ug_free (Tria_Faces);
    ug_free (Quad_Faces);
    return (100318);
  }

  for (surfid = 1; surfid <= Max_SurfID; ++surfid)
  {
    Groups[surfid] = 0;
  }

  /* Count the number of faces in each group */
  for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
  {
    surfid = Surf_ID_Flag[Index];
    Groups[surfid]++;
  }

  for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
  {
    surfid = Surf_ID_Flag[Number_of_Surf_Trias + Index];
    Groups[surfid]++;
  }

  /* Count the number of groups */
  npatch = 0;
  for (surfid = 1; surfid <= Max_SurfID; ++surfid)
  {
    if (Groups[surfid]) npatch++;
  }

  *Number_of_Surfaces_ = npatch;

  *Groups_ = Groups;
  *Tria_Faces_ = Tria_Faces;
  *Quad_Faces_ = Quad_Faces;

  return(0);
}
