#include "UG_IO_LIB.h"

INT_ ug_io_write_egrid
 (FILE * Grid_File,
  char Case_Name[],
  char Error_Message[],
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
 * Write grid data to Ensight Gold case and grid files and write boundary
 * surface node map data file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_egrid.c,v 1.4 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

#define NCHAR 80

  FILE *Case_File = NULL;
  FILE *Node_Map_File = NULL;

  INT_1D *Node_Map = NULL;

  char Text[NCHAR];

  INT_ Dim, Element_Node, Error_Flag, Face_Node, Found_Flag, Index, Node_Index,
       Number_of_ID_Faces, Number_of_ID_Nodes, Number_of_ID_Parts,
       Number_of_Parts, Number_of_Vol_Elems, Number_of_Write_Items,
       Number_of_Write_Items_NM, Surf_ID, Surf_ID_Max, Surf_ID_Min, Write_Flag,
       Write_Flag_NM;

  int i, n;

  float xi;
 
  Number_of_Vol_Elems = Number_of_Vol_Tets
                      + Number_of_Vol_Pents_5 + Number_of_Vol_Pents_6
                      + Number_of_Vol_Hexs;

  Number_of_Parts = 0;

  Number_of_Write_Items = 0;

  Write_Flag = 0;
 
  strcpy (Text, "C Binary");
  Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
  Number_of_Write_Items = Number_of_Write_Items + NCHAR;

  strcpy (Text, "Ensight Unstructured Data"); 
  Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
  Number_of_Write_Items = Number_of_Write_Items + NCHAR;

  strcpy (Text, "Geometry File = ");
  n = NCHAR - strlen (Text) - 1;
  strncat (Text, Case_Name, n);
  Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
  Number_of_Write_Items = Number_of_Write_Items + NCHAR;

  strcpy (Text, "node id given");
  Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
  Number_of_Write_Items = Number_of_Write_Items + NCHAR;

  strcpy (Text, "element id assign");
  Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
  Number_of_Write_Items = Number_of_Write_Items + NCHAR;

  Index = 1;

  Surf_ID = Surf_ID_Flag[Index];

  Surf_ID_Min = Surf_ID;
  Surf_ID_Max = Surf_ID;

  for (Index = 2; Index <= Number_of_Surf_Trias+Number_of_Surf_Quads; ++Index)
  {
    Surf_ID = Surf_ID_Flag[Index];

    Surf_ID_Min = MIN (Surf_ID, Surf_ID_Min);
    Surf_ID_Max = MAX (Surf_ID, Surf_ID_Max);
  }

  Number_of_ID_Parts = 0;

  for (Surf_ID = Surf_ID_Min; Surf_ID <= Surf_ID_Max; ++Surf_ID)
  {
    if (Number_of_Surf_Trias)
    {
      Index = 1;

      do
      {
        Found_Flag = (Surf_ID_Flag[Index] == Surf_ID) ? 1: 0;

        ++Index;
      }
      while (Index <= Number_of_Surf_Trias && Found_Flag == 0);

      if (Found_Flag == 1)
        ++Number_of_ID_Parts;
    }

    if (Number_of_Surf_Quads)
    {
      Index = 1;

      do
      {
        Found_Flag = (Surf_ID_Flag[Number_of_Surf_Trias+Index] == Surf_ID) ? 1: 0;

        ++Index;
      }
      while (Index <= Number_of_Surf_Quads && Found_Flag == 0);

      if (Found_Flag == 1)
        ++Number_of_ID_Parts;
    }
  }

  Error_Flag = 0;

  Node_Map = (INT_1D *) ug_malloc (&Error_Flag,
                                   (Number_of_Nodes+1) * sizeof (INT_1D));

  if (Error_Flag > 0)
  {
    ug_free (Node_Map);
    strcpy (Error_Message, "unable to malloc Ensight grid file work array");
    return (1);
  }

  sprintf (Text, "%s.ensight.nmap", Case_Name);

  ug_backup_file (Text);   
   
  Node_Map_File = ug_fopen (Text, "w");

  if (Node_Map_File == NULL)
  {
    ug_free (Node_Map);
    strcpy (Error_Message, "error opening Ensight node map data file");
    return (1);
  }

  Number_of_Write_Items_NM = 0;

  Write_Flag_NM = 0;

  Write_Flag_NM = Write_Flag_NM + ug_fwrite (&Number_of_ID_Parts, sizeof (INT_), 1, Node_Map_File);
  ++Number_of_Write_Items_NM;

  Write_Flag_NM = Write_Flag_NM + ug_fwrite (&Number_of_Vol_Elems, sizeof (INT_), 1, Node_Map_File);
  ++Number_of_Write_Items_NM;

  for (Surf_ID = Surf_ID_Min; Surf_ID <= Surf_ID_Max; ++Surf_ID)
  {
    if (Number_of_Surf_Trias)
    {
      Index = 1;

      do
      {
        Found_Flag = (Surf_ID_Flag[Index] == Surf_ID) ? 1: 0;

        ++Index;
      }
      while (Index <= Number_of_Surf_Trias && Found_Flag == 0);
    }
    else
      Found_Flag = 0;

    if (Found_Flag == 1)
    {
      ++Number_of_Parts;

      strcpy (Text, "part");
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      n = (int) Number_of_Parts;

      Write_Flag = Write_Flag + ug_fwrite (&n, sizeof(int), 1, Grid_File);
      ++Number_of_Write_Items;

      sprintf(Text, "Surface ID %i Tria Faces", Surf_ID);
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      ug_set_int (1, Number_of_Nodes, 0, Node_Map);

      Number_of_ID_Faces = 0;

      for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
      {
        if (Surf_ID_Flag[Index] == Surf_ID)
        {
          ++Number_of_ID_Faces;

          for (Face_Node = 0; Face_Node < 3; ++Face_Node)
          {
            Node_Index = Surf_Tria_Connectivity[Index][Face_Node];

            Node_Map[Node_Index] = 1;
          }
        }
      }

      Number_of_ID_Nodes = 0;

      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        if (Node_Map[Index])
        {
          ++Number_of_ID_Nodes;

          Node_Map[Index] = Number_of_ID_Nodes;
        }
      }

      strcpy (Text, "coordinates");
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      n = (int) Number_of_ID_Nodes;

      Write_Flag = Write_Flag + ug_fwrite (&n, sizeof (int), 1, Grid_File);
      ++Number_of_Write_Items;

      for (Index = 1; Index <= Number_of_ID_Nodes; ++Index)
      {
        i = (int) Index;

        Write_Flag = Write_Flag + ug_fwrite (&i, sizeof (int), 1, Grid_File);

        ++Number_of_Write_Items;
      }

      for (Dim = 0; Dim < 3; ++Dim)
      {
        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          if (Node_Map[Index])
          {
            xi = (float) (Coordinates[Index][Dim]);

            Write_Flag = Write_Flag + ug_fwrite (&xi, sizeof (float), 1, Grid_File);

            ++Number_of_Write_Items;
          }
        }
      }

      strcpy (Text, "tria3");
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      n = (int) Number_of_ID_Faces;

      Write_Flag = Write_Flag + ug_fwrite (&n, sizeof (int), 1, Grid_File);
      ++Number_of_Write_Items;

      for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
      {
        if (Surf_ID_Flag[Index] == Surf_ID)
        {
          for (Face_Node = 0; Face_Node < 3; ++Face_Node)
          {
            Node_Index = Surf_Tria_Connectivity[Index][Face_Node];

            i = (int) (Node_Map[Node_Index]);

            Write_Flag = Write_Flag + ug_fwrite (&i, sizeof (int), 1, Grid_File);

            ++Number_of_Write_Items;
          }
        }
      }

      Index = 3;

      Write_Flag_NM = Write_Flag_NM + ug_fwrite (&Index, sizeof (INT_), 1, Node_Map_File);
      ++Number_of_Write_Items_NM;

      Write_Flag_NM = Write_Flag_NM + ug_fwrite (&Surf_ID, sizeof (INT_), 1, Node_Map_File);
      ++Number_of_Write_Items_NM;

      Write_Flag_NM = Write_Flag_NM + ug_fwrite (&Number_of_ID_Nodes, sizeof (INT_), 1, Node_Map_File);
      ++Number_of_Write_Items_NM;

      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        if (Node_Map[Index])
        {
          Write_Flag_NM = Write_Flag_NM + ug_fwrite (&Index, sizeof (INT_), 1, Node_Map_File);
          ++Number_of_Write_Items_NM;
        }
      }
    }

    if (Number_of_Surf_Quads)
    {
      Index = 1;

      do
      {
        Found_Flag = (Surf_ID_Flag[Number_of_Surf_Trias+Index] == Surf_ID) ? 1: 0;

        ++Index;
      }
      while (Index <= Number_of_Surf_Quads && Found_Flag == 0);
    }
    else
      Found_Flag = 0;

    if (Found_Flag == 1)
    {
      ++Number_of_Parts;

      strcpy (Text, "part");
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      n = (int) Number_of_Parts;

      Write_Flag = Write_Flag + ug_fwrite (&n, sizeof(int), 1, Grid_File);
      ++Number_of_Write_Items;

      sprintf(Text, "Surface ID %i Quad Faces", Surf_ID);
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      ug_set_int (1, Number_of_Nodes, 0, Node_Map);

      Number_of_ID_Faces = 0;

      for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
      {
        if (Surf_ID_Flag[Number_of_Surf_Trias+Index] == Surf_ID)
        {
          ++Number_of_ID_Faces;

          for (Face_Node = 0; Face_Node < 4; ++Face_Node)
          {
            Node_Index = Surf_Quad_Connectivity[Index][Face_Node];

            Node_Map[Node_Index] = 1;
          }
        }
      }

      Number_of_ID_Nodes = 0;

      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        if (Node_Map[Index])
        {
          ++Number_of_ID_Nodes;

          Node_Map[Index] = Number_of_ID_Nodes;
        }
      }

      Number_of_ID_Nodes = Number_of_ID_Nodes;

      strcpy (Text, "coordinates");
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      n = (int) Number_of_ID_Nodes;

      Write_Flag = Write_Flag + ug_fwrite (&n, sizeof (int), 1, Grid_File);
      ++Number_of_Write_Items;

      for (Index = 1; Index <= Number_of_ID_Nodes; ++Index)
      {
        i = (int) Index;

        Write_Flag = Write_Flag + ug_fwrite (&i, sizeof (int), 1, Grid_File);

        ++Number_of_Write_Items;
      }

      for (Dim = 0; Dim < 3; ++Dim)
      {
        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          if (Node_Map[Index])
          {
            xi = (float) (Coordinates[Index][Dim]);

            Write_Flag = Write_Flag + ug_fwrite (&xi, sizeof (float), 1, Grid_File);

            ++Number_of_Write_Items;
          }
        }
      }

      strcpy (Text, "quad4");
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      n = (int) Number_of_ID_Faces;

      Write_Flag = Write_Flag + ug_fwrite (&n, sizeof (int), 1, Grid_File);
      ++Number_of_Write_Items;

      for (Index = 1; Index <= Number_of_Surf_Quads; ++Index)
      {
        if (Surf_ID_Flag[Number_of_Surf_Trias+Index] == Surf_ID)
        {
          for (Face_Node = 0; Face_Node < 4; ++Face_Node)
          {
            Node_Index = Surf_Quad_Connectivity[Index][Face_Node];

            i = (int) (Node_Map[Node_Index]);

            Write_Flag = Write_Flag + ug_fwrite (&i, sizeof (int), 1, Grid_File);

            ++Number_of_Write_Items;
          }
        }
      }

      Index = 4;

      Write_Flag_NM = Write_Flag_NM + ug_fwrite (&Index, sizeof (INT_), 1, Node_Map_File);
      ++Number_of_Write_Items_NM;

      Write_Flag_NM = Write_Flag_NM + ug_fwrite (&Surf_ID, sizeof (INT_), 1, Node_Map_File);
      ++Number_of_Write_Items_NM;

      Write_Flag_NM = Write_Flag_NM + ug_fwrite (&Number_of_ID_Nodes, sizeof (INT_), 1, Node_Map_File);
      ++Number_of_Write_Items_NM;

      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        if (Node_Map[Index])
        {
          Write_Flag_NM = Write_Flag_NM + ug_fwrite (&Index, sizeof (INT_), 1, Node_Map_File);
          ++Number_of_Write_Items_NM;
        }
      }
    }
  }

  ug_free (Node_Map);

  ug_fclose (Node_Map_File);

  if (Write_Flag_NM != Number_of_Write_Items_NM)
  {
    strcpy (Error_Message, "error writing Ensight node map data file");
    return (1);
  }

  if (Number_of_Vol_Elems)
  {
    ++Number_of_Parts;

    strcpy (Text, "part");
    Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
    Number_of_Write_Items = Number_of_Write_Items + NCHAR;

    n = (int) Number_of_Parts;

    Write_Flag = Write_Flag + ug_fwrite (&n, sizeof(int), 1, Grid_File);
    ++Number_of_Write_Items;

    strcpy (Text, "Entire Mesh");
    Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
    Number_of_Write_Items = Number_of_Write_Items + NCHAR;

    strcpy (Text, "coordinates");
    Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
    Number_of_Write_Items = Number_of_Write_Items + NCHAR;

    n = (int) Number_of_Nodes;

    Write_Flag = Write_Flag + ug_fwrite (&n, sizeof (int), 1, Grid_File);
    ++Number_of_Write_Items;

    for (Index = 1; Index <= Number_of_Nodes; ++Index)
    {
      i = (int) Index;

      Write_Flag = Write_Flag + ug_fwrite (&i, sizeof (int), 1, Grid_File);

      ++Number_of_Write_Items;
    }

    for (Dim = 0; Dim < 3; ++Dim)
    {
      for (Index = 1; Index <= Number_of_Nodes; ++Index)
      {
        xi = (float) (Coordinates[Index][Dim]);

        Write_Flag = Write_Flag + ug_fwrite (&xi, sizeof (float), 1, Grid_File);

        ++Number_of_Write_Items;
      }
    }

    if (Number_of_Vol_Tets)
    {                       
      strcpy (Text, "tetra4");
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      n = (int) Number_of_Vol_Tets;

      Write_Flag = Write_Flag + ug_fwrite (&n, sizeof (int), 1, Grid_File);
      ++Number_of_Write_Items;

      for (Element_Node = 0; Element_Node < 4; ++Element_Node)
      {
        for (Index = 1; Index <= Number_of_Vol_Tets; ++Index)
        {
          i = (int) (Vol_Tet_Connectivity[Index][Element_Node]);

          Write_Flag = Write_Flag + ug_fwrite (&i, sizeof (int), 1, Grid_File);

          ++Number_of_Write_Items;
        }
      }
    }
    
    if (Number_of_Vol_Pents_5)
    {                       
      strcpy (Text, "pyramid5");
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      n = (int) Number_of_Vol_Pents_5;

      Write_Flag = Write_Flag + ug_fwrite (&n, sizeof (int), 1, Grid_File);
      ++Number_of_Write_Items;

      for (Element_Node = 0; Element_Node < 5; ++Element_Node)
      {
        for (Index = 1; Index <= Number_of_Vol_Pents_5; ++Index)
        {
          i = (int) (Vol_Pent_5_Connectivity[Index][Element_Node]);

          Write_Flag = Write_Flag + ug_fwrite (&i, sizeof (int), 1, Grid_File);

          ++Number_of_Write_Items;
        }
      }
    }

    if (Number_of_Vol_Pents_6)
    {                       
      strcpy (Text, "penta6");
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      n = (int) Number_of_Vol_Pents_6;

      Write_Flag = Write_Flag + ug_fwrite (&n, sizeof (int), 1, Grid_File);
      ++Number_of_Write_Items;

      for (Element_Node = 0; Element_Node < 6; ++Element_Node)
      {
        for (Index = 1; Index <= Number_of_Vol_Pents_6; ++Index)
        {
          i = (int) (Vol_Pent_6_Connectivity[Index][Element_Node]);

          Write_Flag = Write_Flag + ug_fwrite (&i, sizeof (int), 1, Grid_File);

          ++Number_of_Write_Items;
        }
      }
    }
    
    if (Number_of_Vol_Hexs)
    {                       
      strcpy (Text, "hexa8");
      Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Grid_File);
      Number_of_Write_Items = Number_of_Write_Items + NCHAR;

      n = (int) Number_of_Vol_Hexs;

      Write_Flag = Write_Flag + ug_fwrite (&n, sizeof (int), 1, Grid_File);
      ++Number_of_Write_Items;

      for (Element_Node = 0; Element_Node < 8; ++Element_Node)
      {
        for (Index = 1; Index <= Number_of_Vol_Hexs; ++Index)
        {
          i = (int) (Vol_Hex_Connectivity[Index][Element_Node]);

          Write_Flag = Write_Flag + ug_fwrite (&i, sizeof (int), 1, Grid_File);

          ++Number_of_Write_Items;
        }
      }
    }
  }

  if (Write_Flag != Number_of_Write_Items)
  {
    strcpy (Error_Message, "error writing Ensight grid file");
    return (1);
  }

  sprintf (Text, "%s.case", Case_Name);

  ug_backup_file (Text);   

  Case_File = ug_fopen (Text, "w");

  if (Case_File == NULL)
  {
    strcpy (Error_Message, "error opening Ensight case file");
    return (1);
  }

  Write_Flag = fprintf (Case_File, "FORMAT\n");
  Write_Flag = fprintf (Case_File, "type: ensight gold\n");
  Write_Flag = fprintf (Case_File, "GEOMETRY\n");
  Write_Flag = fprintf (Case_File, "model: %s.egrid\n", Case_Name);

  ug_fclose (Case_File);

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing Ensight case file");
    return (1);
  }

  return (0);

}

