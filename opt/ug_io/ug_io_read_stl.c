#include "UG_IO_LIB.h"

/*
 * Global variable.
 *
 */

static double STL_Rel_Tol = 0.05;

double ug_io_get_stl_tolerance ()
{
  return (STL_Rel_Tol);
}


void ug_io_set_stl_tolerance (double tolerance)
{
  STL_Rel_Tol = tolerance;
  return;
}


INT_ ug_io_read_stl
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ Message_Flag,
  INT_ Read_Task_Flag,
  INT_ *Number_of_Nodes,
  INT_ *Number_of_Surf_Trias,
  INT_1D * Surf_ID_Flag,
  INT_3D * Surf_Tria_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Read grid data from a Stereolithography (Standard Tessellation Language)
 * surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_stl.c,v 1.9 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */


  CHAR_UG_MAX Text;
  CHAR_UG_MAX Text_Line;
  char *Read_Label;
  char *tok;

  INT_ array_size, byte_count, delta_size, i, id, j, inode1, inode2, inode3,
       maxlevel, Error_Flag, Node_Index, Read_Flag, Surf_ID_Index, Surf_Tria_Index;
  
  double dc0, len1, len2, len3, tol;

  static double xmax, xmin, ymax, ymin, zmax, zmin;
  
  UG3_OCTREE_POINTLIST *pointlist_array = NULL;

  //DOUBLE_3D nv;

  DOUBLE_3X3 vertex;

  float TMP_float[3];

  UG3_OCTREE *octree = NULL;

  dc0 = 0.0;

  maxlevel = 2;

  Node_Index = 0;
  Surf_ID_Index = 0;
  Surf_Tria_Index = 0;

  if (Read_Task_Flag == 1)
  {
    xmin = dc0;
    xmax = dc0;
    ymin = dc0;
    ymax = dc0;
    zmin = dc0;
    zmax = dc0;
  }
  else
  {
    array_size = 0;
    delta_size = *Number_of_Nodes / 3 + ((*Number_of_Nodes % 3) > 0 ? 1 : 0);

    maxlevel = (INT_)(*Number_of_Nodes / 100000.0) + 1;
    maxlevel = MAX(2, MIN (5, maxlevel));
    octree = ug3_octree_allocatetree (xmin, xmax, ymin, ymax, zmin, zmax,
                                      0, maxlevel);
    if (octree == NULL)
    {
      strcpy (Error_Message, "Unable to create octree");
      return (1);
    }
  }

  if (File_Format == UG_IO_FORMATTED)
  {
    do
    {
      strcpy (Text_Line, "");
      Read_Label = fgets (Text_Line, UG_MAX_CHAR_STRING_LENGTH, Grid_File);

      if (Read_Label != NULL)
      {
        if (strstr (Text_Line, "solid") != NULL)
        {
          Surf_ID_Index++;
        }
        else if (strstr (Text_Line, "endsolid") != NULL)
        {
          continue;
        }
        else if (strstr (Text_Line, "facet normal") != NULL)
        {
          Surf_Tria_Index++;
/*
          if (Read_Task_Flag == 2)
          {
            tok = strtok (Text_Line, " ");

            tok = strtok (NULL, " ");
            tok = strtok (NULL, " ");
            nv[0] = strtod (tok, (char **) NULL);

            tok = strtok (NULL, " ");
            nv[1] = strtod (tok, (char **) NULL);

            tok = strtok (NULL, " ");
            nv[2] = strtod (tok, (char **) NULL);
          }
*/
        }
        else if (strstr (Text_Line, "outer loop") != NULL)
        {
          i = 0;
        }
        else if (strstr (Text_Line, "vertex") != NULL)
        {
          if (i > 2)
          {
            sprintf (Error_Message, "More than 3 vertices listed for triangle %d", Surf_Tria_Index);
            return (1);
          }

          tok = strtok (Text_Line," ");        

          tok = strtok (NULL," ");
          vertex[i][0] = strtod (tok, (char **) NULL);

          tok = strtok(NULL," ");
          vertex[i][1] = strtod (tok, (char **) NULL);

          tok = strtok(NULL," ");
          vertex[i][2] = strtod (tok, (char **) NULL);

          if (Read_Task_Flag == 1)
          {
            if (vertex[i][0] < xmin) xmin = vertex[i][0];
            if (vertex[i][0] > xmax) xmax = vertex[i][0];
            if (vertex[i][1] < ymin) ymin = vertex[i][1];
            if (vertex[i][1] > ymax) ymax = vertex[i][1];
            if (vertex[i][2] < zmin) zmin = vertex[i][2];
            if (vertex[i][2] > zmax) zmax = vertex[i][2];
          }
          else
          {
            if (i == 2)
            {
	      if (Node_Index >= array_size)
	      {
		Error_Flag = 0;
		array_size += delta_size;
                pointlist_array = (UG3_OCTREE_POINTLIST *) ug_realloc (&Error_Flag,
								       pointlist_array,
								       array_size * 
								       sizeof (UG3_OCTREE_POINTLIST));
		if (Error_Flag > 0)
		{
		  strcpy(Error_Message,"Unable to allocate octree pointlist.");
		  return(1);
		}
	      }

              len1 = STL_DISTANCE(vertex[0], vertex[1]);
              len2 = STL_DISTANCE(vertex[1], vertex[2]);
              len3 = STL_DISTANCE(vertex[2], vertex[0]);

              tol = STL_Rel_Tol * MIN(len1, MIN(len2, len3));

              for (j = 0; j < 3; j++)
              {
                id = ug3_octree_findclosestpoint (octree, tol, vertex[j], Coordinates);
                if (id < 1)
                {
                  id = ug3_octree_addpoint (octree, Node_Index+1, vertex[j],
					    &(pointlist_array[Node_Index]));

                  if (id < 1)
                  {
                    strcpy (Error_Message, "Unable to add point to octree");
                    ug3_octree_destroytree (octree);
                    ug_free (pointlist_array);
                    return (1);
                  }

                  Coordinates[id][0] = vertex[j][0];
                  Coordinates[id][1] = vertex[j][1];
                  Coordinates[id][2] = vertex[j][2];
		  Node_Index++;
                }

                Surf_Tria_Connectivity[Surf_Tria_Index][j] = id;
                Surf_ID_Flag[Surf_Tria_Index] = Surf_ID_Index;
              }
            }
          }
          i++;
        }
        else if (strstr (Text_Line, "endfacet") != NULL)
        {
          continue;
        }
        else if (strstr (Text_Line, "endloop") != NULL)
        {
          if (Read_Task_Flag == 2)
          {
            inode1 = Surf_Tria_Connectivity[Surf_Tria_Index][0];
            inode2 = Surf_Tria_Connectivity[Surf_Tria_Index][1];
            inode3 = Surf_Tria_Connectivity[Surf_Tria_Index][2];

            if (inode1 == inode2 || inode1 == inode3 || inode2 == inode3)
            {
              if (Message_Flag >= 1)
              {
                sprintf (Text, "UG_IO    : Skipped tria-face =%10i Duplicate Connectivity", Surf_Tria_Index);
                ug_message (Text);
              }
              Surf_Tria_Index--;
              continue;
            }
          }
        }
      }
    }
    while (Read_Label != NULL);
  }
  else if (File_Format == UG_IO_BINARY_SINGLE)
  {
    Read_Flag = ug_fread (Text_Line, sizeof (char), 80, Grid_File);
    if (Read_Flag != 80)
    {
      strcpy (Error_Message, "error reading STL grid file");
      return (1);
    }

    Read_Flag = ug_fread (&Surf_Tria_Index, sizeof (unsigned int), 1, Grid_File);
    if (Read_Flag != 1)
    {
      strcpy (Error_Message, "error reading STL grid file");
      return (1);
    }

    for (i = 0; i < Surf_Tria_Index; i++)
    {
      Read_Flag = 0;

      Read_Flag = Read_Flag 
                + ug_fread (&TMP_float, sizeof (float), 3, Grid_File);
/*
      nv[0] = (double) TMP_float[0];
      nv[1] = (double) TMP_float[1];
      nv[2] = (double) TMP_float[2];
*/

      for (j = 0; j < 3; j++)
      {
        Read_Flag = Read_Flag 
                  + ug_fread (&TMP_float, sizeof (float), 3, Grid_File);

        vertex[j][0] = (double) TMP_float[0];
        vertex[j][1] = (double) TMP_float[1];
        vertex[j][2] = (double) TMP_float[2];
      }

      if (Read_Task_Flag == 1)
      {
        for (j = 0; j < 3; j++)
        {
          if (vertex[j][0] < xmin) xmin = vertex[j][0];
          if (vertex[j][0] > xmax) xmax = vertex[j][0];
          if (vertex[j][1] < ymin) ymin = vertex[j][1];
          if (vertex[j][1] > ymax) ymax = vertex[j][1];
          if (vertex[j][2] < zmin) zmin = vertex[j][2];
          if (vertex[j][2] > zmax) zmax = vertex[j][2];
        }
      }
      else
      {
        for (j = 0; j < 3; j++)
        {
          if (j == 0)
          {
            len1 = STL_DISTANCE(vertex[1], vertex[0]);
            len2 = STL_DISTANCE(vertex[2], vertex[0]);
          }
          else if (j == 1)
          {
            len1 = STL_DISTANCE(vertex[0], vertex[1]);
            len2 = STL_DISTANCE(vertex[2], vertex[1]);
          }
          else
          {
            len1 = STL_DISTANCE(vertex[0], vertex[2]);
            len2 = STL_DISTANCE(vertex[1], vertex[2]);
          }

          tol = STL_Rel_Tol * MIN(len1, len2);

          id = ug3_octree_findclosestpoint (octree, tol, vertex[j], Coordinates);
          if (id < 1)
          {
	    if (Node_Index >= array_size)
	    {
	      Error_Flag = 0;
	      array_size += delta_size;
	      pointlist_array = (UG3_OCTREE_POINTLIST *) ug_realloc (&Error_Flag,
								     pointlist_array,
								     array_size * 
								     sizeof (UG3_OCTREE_POINTLIST));
	      if (Error_Flag > 0)
	      {
		strcpy(Error_Message,"Unable to allocate octree pointlist.");
		return(1);
	      }
	    }

            id = ug3_octree_addpoint (octree, Node_Index+1, vertex[j],
                                      &(pointlist_array[Node_Index]));
            if (id < 1)
            {
              strcpy (Error_Message, "Unable to insert point into BSP tree");
              ug3_octree_destroytree (octree);
              ug_free (pointlist_array);
              return (1);
            }

            Coordinates[id][0] = vertex[j][0];
            Coordinates[id][1] = vertex[j][1];
            Coordinates[id][2] = vertex[j][2];
            Node_Index++;
          }

          Surf_Tria_Connectivity[i + 1][j] = id;
          Surf_ID_Flag[i + 1] = 1;
        }

        Read_Flag = Read_Flag 
                  + ug_fread (&byte_count, sizeof (unsigned short int), 1, Grid_File);

        if (Read_Flag != 13)
        {
          strcpy (Error_Message, "error reading STL grid file");
          ug3_octree_destroytree (octree);
          ug_free (pointlist_array);
          return (1);
        }

        inode1 = Surf_Tria_Connectivity[i + 1][0];
        inode2 = Surf_Tria_Connectivity[i + 1][1];
        inode3 = Surf_Tria_Connectivity[i + 1][2];

        if (inode1 == inode2 || inode1 == inode3 || inode2 == inode3)
        {
          sprintf (Error_Message, "Duplicate connectivity for Triangle %d", i + 1);
          ug3_octree_destroytree (octree);
          ug_free (pointlist_array);
          return (1);
        }
      }
    }
  }
  else
  {
    strcpy (Error_Message, "Only formatted ASCII or binary float formats are supported for STL files");
    *Number_of_Surf_Trias = 0;
    *Number_of_Nodes = 3 * 0;
    return (1);
  }

  if (Read_Task_Flag == 1)
  {
    *Number_of_Surf_Trias = Surf_Tria_Index;
    *Number_of_Nodes = 3 * Surf_Tria_Index;
  }
  else if (Read_Task_Flag == 2)
  {
    *Number_of_Surf_Trias = Surf_Tria_Index;
    *Number_of_Nodes = Node_Index;
    ug3_octree_destroytree (octree);
    ug_free (pointlist_array);
  }

  return (0);
}
