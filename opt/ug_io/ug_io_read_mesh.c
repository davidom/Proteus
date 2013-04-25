#include "UG_IO_LIB.h"

INT_ ug_io_read_mesh
 (FILE * Grid_File,
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

/*
 * Read grid data from a MESH surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_read_mesh.c,v 1.18 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX Text_Line;

  CHAR_133 Text;

  char *Read_Label;

  INT_ i1, i2, i3, i4, i5, i6, i7, i8, ID, Max_File_Bnd_Node_Index, Index,
       Number_of_Total_Nodes, Read_Flag;

  double d, x, y, z;

  Max_File_Bnd_Node_Index = 0;

  do
  {
    strcpy (Text_Line, "");

    Read_Label = fgets (Text_Line, UG_MAX_CHAR_STRING_LENGTH, Grid_File);

    strcpy (Text, "");

    sscanf (Text_Line, "%s", Text);

    if (Read_Label != NULL)
    {
      if (strcmp (Text, "Vertices") == 0)
      {
        Read_Flag = fscanf (Grid_File, "%i", Number_of_Nodes);

        Number_of_Total_Nodes = *Number_of_Nodes;

        if (Read_Task_Flag == 1)
        {
          for (Index = 1; Index <= *Number_of_Nodes; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%lg %lg %lg %lg", &x, &y, &z, &d);
          }
        }
        else
        {
          if (*Number_of_Bnd_Nodes > 0)
            *Number_of_Nodes = *Number_of_Bnd_Nodes;

          for (Index = 1; Index <= *Number_of_Nodes; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%lg %lg %lg %lg", &x, &y, &z, &d);

            Coordinates[Index][0] = x;
            Coordinates[Index][1] = y;
            Coordinates[Index][2] = z;
          }

          for (Index = *Number_of_Nodes+1;
               Index <= Number_of_Total_Nodes; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%lg %lg %lg %lg", &x, &y, &z, &d);
          }
        }

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading MESH surface grid file");
          return (1);
        }
      }

      else if (strcmp (Text, "Triangles") == 0)
      {
        Read_Flag = fscanf (Grid_File, "%i", Number_of_Surf_Trias);

        if (Read_Task_Flag == 1)
        {
          for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i", &i1, &i2, &i3, &ID);

            Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index, i1);
            Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index, i2);
            Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index, i3);
          }
        }
        else
        {
          for (Index = 1; Index <= *Number_of_Surf_Trias; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i", &i1, &i2, &i3, &ID);

            Surf_Tria_Connectivity[Index][0] = i1;
            Surf_Tria_Connectivity[Index][1] = i2;
            Surf_Tria_Connectivity[Index][2] = i3;

            if (Surf_ID_Flag != NULL)
              Surf_ID_Flag[Index] = ID;
          }
        }

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading MESH surface grid file");
          return (1);
        }
      }

      else if (strcmp (Text, "Quadrilaterals") == 0)
      {
        Read_Flag = fscanf (Grid_File, "%i", Number_of_Surf_Quads);

        if (Read_Task_Flag == 1)
        {
          for (Index = 1; Index <= *Number_of_Surf_Quads; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i %i",
                                &i1, &i2, &i3, &i4, &ID);

            Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index, i1);
            Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index, i2);
            Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index, i3);
            Max_File_Bnd_Node_Index = MAX (Max_File_Bnd_Node_Index, i4);
          }
        }
        else
        {
          for (Index = 1; Index <= *Number_of_Surf_Quads; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i %i",
                                &i1, &i2, &i3, &i4, &ID);

            Surf_Quad_Connectivity[Index][0] = i1;
            Surf_Quad_Connectivity[Index][1] = i2;
            Surf_Quad_Connectivity[Index][2] = i3;
            Surf_Quad_Connectivity[Index][3] = i4;

            if (Surf_ID_Flag != NULL)
              Surf_ID_Flag[*Number_of_Surf_Trias+Index] = ID;
          }
        }

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading MESH surface grid file");
          return (1);
        }
      }

      else if (strcmp (Text, "Tetrahedra") == 0)
      {
        Read_Flag = fscanf (Grid_File, "%i", Number_of_Vol_Tets);

        if (Read_Task_Flag == 2 && *Number_of_Bnd_Nodes == 0)
        {
          for (Index = 1; Index <= *Number_of_Vol_Tets; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i %i",
                                &i1, &i2, &i3, &i4, &ID);

            Vol_Tet_Connectivity[Index][0] = i1;
            Vol_Tet_Connectivity[Index][1] = i2;
            Vol_Tet_Connectivity[Index][2] = i3;
            Vol_Tet_Connectivity[Index][3] = i4;

            if (Vol_ID_Flag != NULL)
              Vol_ID_Flag[Index] = ID;
          }
        }
        else
        {
          for (Index = 1; Index <= *Number_of_Vol_Tets; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i %i",
                                &i1, &i2, &i3, &i4, &ID);
          }
        }

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading MESH surface grid file");
          return (1);
        }
      }

      else if (strcmp (Text, "Hexahedra") == 0)
      {
        Read_Flag = fscanf (Grid_File, "%i", Number_of_Vol_Hexs);

        if (Read_Task_Flag == 2 && *Number_of_Bnd_Nodes == 0)
        {
          for (Index = 1; Index <= *Number_of_Vol_Hexs; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i %i %i %i %i %i",
                                &i1, &i2, &i3, &i4, &i5, &i6, &i7, &i8, &ID);

            Vol_Hex_Connectivity[Index][0] = i1;
            Vol_Hex_Connectivity[Index][1] = i2;
            Vol_Hex_Connectivity[Index][2] = i3;
            Vol_Hex_Connectivity[Index][3] = i4;
            Vol_Hex_Connectivity[Index][4] = i5;
            Vol_Hex_Connectivity[Index][5] = i6;
            Vol_Hex_Connectivity[Index][6] = i7;
            Vol_Hex_Connectivity[Index][7] = i8;

            if (Vol_ID_Flag != NULL)
              Vol_ID_Flag[(*Number_of_Vol_Tets)+(*Number_of_Vol_Pents_5)+(*Number_of_Vol_Pents_6)+Index] = ID;
          }
        }
        else
        {
          for (Index = 1; Index <= *Number_of_Vol_Hexs; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i %i %i %i %i %i",
                                &i1, &i2, &i3, &i4, &i5, &i6, &i7, &i8, &ID);
          }
        }

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading MESH surface grid file");
          return (1);
        }
      }

      else if (strcmp (Text, "Pyramids") == 0)
      {
        Read_Flag = fscanf (Grid_File, "%i", Number_of_Vol_Pents_5);

        if (Read_Task_Flag == 2 && *Number_of_Bnd_Nodes == 0)
        {
          for (Index = 1; Index <= *Number_of_Vol_Pents_5; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i %i %i",
                                &i1, &i2, &i3, &i4, &i5, &ID);

            Vol_Pent_5_Connectivity[Index][0] = i1;
            Vol_Pent_5_Connectivity[Index][1] = i2;
            Vol_Pent_5_Connectivity[Index][2] = i5;
            Vol_Pent_5_Connectivity[Index][3] = i3;
            Vol_Pent_5_Connectivity[Index][4] = i4; 

            if (Vol_ID_Flag != NULL)
              Vol_ID_Flag[(*Number_of_Vol_Tets)+Index] = ID;
          }
        }
        else
        {
          for (Index = 1; Index <= *Number_of_Vol_Pents_5; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i %i %i",
                                &i1, &i2, &i3, &i4, &i5, &ID);
          }
        }

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading MESH surface grid file");
          return (1);
        }
      }

      else if (strcmp (Text, "Prisms") == 0)
      {
        Read_Flag = fscanf (Grid_File, "%i", Number_of_Vol_Pents_6);

        if (Read_Task_Flag == 2 && *Number_of_Bnd_Nodes == 0)
        {
          for (Index = 1; Index <= *Number_of_Vol_Pents_6; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i %i %i %i",
                                &i1, &i2, &i3, &i4, &i5, &i6, &ID);

            Vol_Pent_6_Connectivity[Index][0] = i1;
            Vol_Pent_6_Connectivity[Index][1] = i2;
            Vol_Pent_6_Connectivity[Index][2] = i3;
            Vol_Pent_6_Connectivity[Index][3] = i4;
            Vol_Pent_6_Connectivity[Index][4] = i5;
            Vol_Pent_6_Connectivity[Index][5] = i6;

            if (Vol_ID_Flag != NULL)
              Vol_ID_Flag[(*Number_of_Vol_Tets)+(*Number_of_Vol_Pents_5)+Index] = ID;
          }
        }
        else
        {
          for (Index = 1; Index <= *Number_of_Vol_Pents_6; ++Index)
          {
            Read_Flag = fscanf (Grid_File, "%i %i %i %i %i %i %i",
                                &i1, &i2, &i3, &i4, &i5, &i6, &ID);
          }
        }

        if (Read_Flag == EOF)
        {
          strcpy (Error_Message, "error reading MESH surface grid file");
          return (1);
        }
      }
    }
  }
  while (Read_Label != NULL);

  if (Read_Task_Flag == 1 && *Number_of_Bnd_Nodes == -1)
    *Number_of_Bnd_Nodes = Max_File_Bnd_Node_Index;

  if (*Number_of_Bnd_Nodes > 0)
  {
    *Number_of_Nodes = *Number_of_Bnd_Nodes;
    *Number_of_Vol_Hexs = 0;
    *Number_of_Vol_Pents_5 = 0;
    *Number_of_Vol_Pents_6 = 0;
    *Number_of_Vol_Tets = 0;
  }

  return (0);

}
