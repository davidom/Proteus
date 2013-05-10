#include "UG_IO_LIB.h"

INT_ ug_io_write_stl
 (FILE * Grid_File,
  char Error_Message[],
  INT_ File_Format,
  INT_ *Number_of_Bnd_Nodes,
  INT_ Number_of_Nodes,
  INT_ Number_of_Surf_Quads,
  INT_ Number_of_Surf_Trias,
  INT_1D * Surf_ID_Flag,
  INT_4D * Surf_Quad_Connectivity,
  INT_3D * Surf_Tria_Connectivity,
  DOUBLE_3D * Coordinates)

{

/*
 * Write grid data to an STL surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_stl.c,v 1.8 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_81 Header;

  INT_1D *Node_Index_Map = NULL;

  DOUBLE_1D *Initial_Normal_Spacing = NULL;
  DOUBLE_1D *BL_Thickness = NULL;

  INT_ Error_Flag, ID, Max_ID, Min_ID, Node_Index1, Node_Index2, Node_Index3,
       Node_Index4, Number_of_Surf_Faces, Surf_ID, Surf_Index, Surf_Quad_Index,
       Surf_Tria_Index, Write_Flag;
  INT_ Header_Flag = 1;

  unsigned int Number_of_Faces;
  unsigned short int Attribute = 0;

  float fc1, dX21, dX31, dX42, dY21, dY31, dY42, dZ21, dZ31, dZ42, NX, NY, NZ,
        w, X1, X2, X3, Y1, Y2, Y3, Z1, Z2, Z3, X4, Y4, Z4;

  fc1 = 1.0;

  Error_Flag = ug3_bnd_nodes (0, Number_of_Bnd_Nodes, Number_of_Nodes,
                              Number_of_Surf_Trias, Number_of_Surf_Quads,
                              Surf_Tria_Connectivity, Surf_Quad_Connectivity,
                              &Node_Index_Map,
                              BL_Thickness, Initial_Normal_Spacing,
                              Coordinates);

  if (Error_Flag > 0)
  {
    sprintf (Error_Message, "unable to allocate required memory %i",
             Error_Flag);
    return (1);
  }

  ug_free (Node_Index_Map);

  Number_of_Surf_Faces = Number_of_Surf_Trias + Number_of_Surf_Quads;

  Surf_Index = 1;

  Surf_ID = Surf_ID_Flag[Surf_Index];

  Max_ID = Surf_ID;
  Min_ID = Surf_ID;

  for (Surf_Index = 2; Surf_Index <= Number_of_Surf_Faces; ++Surf_Index)
  {
    Surf_ID = Surf_ID_Flag[Surf_Index];

    Max_ID = MAX (Max_ID, Surf_ID);
    Min_ID = MIN (Min_ID, Surf_ID);
  }

  if (File_Format == UG_IO_FORMATTED)
  {
    for (ID = Min_ID; ID <= Max_ID; ++ID)
    {
      Surf_Index = 1;

      do
      {
        Surf_ID = Surf_ID_Flag[Surf_Index];

        ++Surf_Index;
      }
      while (Surf_Index <= Number_of_Surf_Faces && Surf_ID != ID);

      if (Surf_ID == ID)
      {
        Write_Flag = fprintf (Grid_File, "solid ID%i\n", ID);

        for (Surf_Tria_Index = 1;
             Surf_Tria_Index <= Number_of_Surf_Trias; ++Surf_Tria_Index)
        {
          Surf_Index = Surf_Tria_Index;

          Surf_ID = Surf_ID_Flag[Surf_Index];

          if (Surf_ID == ID)
          {
            Node_Index1 = Surf_Tria_Connectivity[Surf_Tria_Index][0];
            Node_Index2 = Surf_Tria_Connectivity[Surf_Tria_Index][1];
            Node_Index3 = Surf_Tria_Connectivity[Surf_Tria_Index][2];

            X1 = (float) Coordinates[Node_Index1][0];
            Y1 = (float) Coordinates[Node_Index1][1];
            Z1 = (float) Coordinates[Node_Index1][2];
            X2 = (float) Coordinates[Node_Index2][0];
            Y2 = (float) Coordinates[Node_Index2][1];
            Z2 = (float) Coordinates[Node_Index2][2];
            X3 = (float) Coordinates[Node_Index3][0];
            Y3 = (float) Coordinates[Node_Index3][1];
            Z3 = (float) Coordinates[Node_Index3][2];

            dX21 = X2 - X1;
            dY21 = Y2 - Y1;
            dZ21 = Z2 - Z1;
            dX31 = X3 - X1;
            dY31 = Y3 - Y1;
            dZ31 = Z3 - Z1;

            NX = dY21 * dZ31 - dZ21 * dY31;
            NY = dZ21 * dX31 - dX21 * dZ31;
            NZ = dX21 * dY31 - dY21 * dX31;

            w = fc1 / sqrt (NX * NX + NY * NY + NZ * NZ);

            NX = w * NX;
            NY = w * NY;
            NZ = w * NZ;

            Write_Flag = fprintf (Grid_File, " facet normal %.15g %.15g %.15g\n",
                                  NX, NY, NZ);
            Write_Flag = fprintf (Grid_File, "  outer loop\n");
            Write_Flag = fprintf (Grid_File, "   vertex %.15g %.15g %.15g\n",
                                  X1, Y1, Z1);
            Write_Flag = fprintf (Grid_File, "   vertex %.15g %.15g %.15g\n",
                                  X2, Y2, Z2);
            Write_Flag = fprintf (Grid_File, "   vertex %.15g %.15g %.15g\n",
                                  X3, Y3, Z3);
            Write_Flag = fprintf (Grid_File, "  endloop\n");
            Write_Flag = fprintf (Grid_File, " endfacet\n");
          }
        }

        for (Surf_Quad_Index = 1;
             Surf_Quad_Index <= Number_of_Surf_Quads; ++Surf_Quad_Index)
        {
          Surf_Index = Number_of_Surf_Trias + Surf_Quad_Index;

          Surf_ID = Surf_ID_Flag[Surf_Index];

          if (Surf_ID == ID)
          {
            Node_Index1 = Surf_Quad_Connectivity[Surf_Quad_Index][0];
            Node_Index2 = Surf_Quad_Connectivity[Surf_Quad_Index][1];
            Node_Index3 = Surf_Quad_Connectivity[Surf_Quad_Index][2];
            Node_Index4 = Surf_Quad_Connectivity[Surf_Quad_Index][3];

            X1 = (float) Coordinates[Node_Index1][0];
            Y1 = (float) Coordinates[Node_Index1][1];
            Z1 = (float) Coordinates[Node_Index1][2];
            X2 = (float) Coordinates[Node_Index2][0];
            Y2 = (float) Coordinates[Node_Index2][1];
            Z2 = (float) Coordinates[Node_Index2][2];
            X3 = (float) Coordinates[Node_Index3][0];
            Y3 = (float) Coordinates[Node_Index3][1];
            Z3 = (float) Coordinates[Node_Index3][2];
            X4 = (float) Coordinates[Node_Index4][0];
            Y4 = (float) Coordinates[Node_Index4][1];
            Z4 = (float) Coordinates[Node_Index4][2];

            dX31 = X3 - X1;
            dY31 = Y3 - Y1;
            dZ31 = Z3 - Z1;
            dX42 = X4 - X2;
            dY42 = Y4 - Y2;
            dZ42 = Z4 - Z2;

            NX = dY31 * dZ42 - dZ31 * dY42;
            NY = dZ31 * dX42 - dX31 * dZ42;
            NZ = dX31 * dY42 - dY31 * dX42;

            w = fc1 / sqrt (NX * NX + NY * NY + NZ * NZ);

            Write_Flag = fprintf (Grid_File, " facet normal %.15g %.15g %.15g\n",
                                  NX, NY, NZ);
            Write_Flag = fprintf (Grid_File, "  outer loop\n");
            Write_Flag = fprintf (Grid_File, "   vertex %.15g %.15g %.15g\n",
                                  X1, Y1, Z1);
            Write_Flag = fprintf (Grid_File, "   vertex %.15g %.15g %.15g\n",
                                  X2, Y2, Z2);
            Write_Flag = fprintf (Grid_File, "   vertex %.15g %.15g %.15g\n",
                                  X3, Y3, Z3);
            Write_Flag = fprintf (Grid_File, "  endloop\n");
            Write_Flag = fprintf (Grid_File, " endfacet\n");

            Write_Flag = fprintf (Grid_File, " facet normal %.15g %.15g %.15g\n",
                                  NX, NY, NZ);
            Write_Flag = fprintf (Grid_File, "  outer loop\n");
            Write_Flag = fprintf (Grid_File, "   vertex %.15g %.15g %.15g\n",
                                  X1, Y1, Z1);
            Write_Flag = fprintf (Grid_File, "   vertex %.15g %.15g %.15g\n",
                                  X3, Y3, Z3);
            Write_Flag = fprintf (Grid_File, "   vertex %.15g %.15g %.15g\n",
                                  X4, Y4, Z4);
            Write_Flag = fprintf (Grid_File, "  endloop\n");
            Write_Flag = fprintf (Grid_File, " endfacet\n");
          }
        }

        Write_Flag = fprintf (Grid_File, "endsolid\n");

        if (Write_Flag < 0)
        {
          strcpy (Error_Message, "error writing STL surface grid file");
          return (1);
        }
      }
    }
  }

  else if (File_Format == UG_IO_BINARY_SINGLE)
  {
    for (ID = Min_ID; ID <= Max_ID; ++ID)
    {
      Surf_Index = 1;

      do
      {
        Surf_ID = Surf_ID_Flag[Surf_Index];

        ++Surf_Index;
      }
      while (Surf_Index <= Number_of_Surf_Faces && Surf_ID != ID);

      if (Surf_ID == ID)
      {
        Write_Flag = 0;

        if (Header_Flag == 1)
        {
          Header_Flag = 0;

          strcpy (Header, "UG_IO                                                                           ");

          Number_of_Faces = (unsigned int) Number_of_Surf_Faces;

          Write_Flag = Write_Flag
                     + ug_fwrite (&Header, sizeof (char), 80, Grid_File);
          Write_Flag = Write_Flag - 80;

          Write_Flag = Write_Flag
                     + ug_fwrite (&Number_of_Faces, sizeof (unsigned int), 1,
                                Grid_File);
          Write_Flag = Write_Flag - 1;
        }

        for (Surf_Tria_Index = 1;
             Surf_Tria_Index <= Number_of_Surf_Trias; ++Surf_Tria_Index)
        {
          Surf_Index = Surf_Tria_Index;

          Surf_ID = Surf_ID_Flag[Surf_Index];

          if (Surf_ID == ID)
          {
            Node_Index1 = Surf_Tria_Connectivity[Surf_Tria_Index][0];
            Node_Index2 = Surf_Tria_Connectivity[Surf_Tria_Index][1];
            Node_Index3 = Surf_Tria_Connectivity[Surf_Tria_Index][2];

            X1 = (float) Coordinates[Node_Index1][0];
            Y1 = (float) Coordinates[Node_Index1][1];
            Z1 = (float) Coordinates[Node_Index1][2];
            X2 = (float) Coordinates[Node_Index2][0];
            Y2 = (float) Coordinates[Node_Index2][1];
            Z2 = (float) Coordinates[Node_Index2][2];
            X3 = (float) Coordinates[Node_Index3][0];
            Y3 = (float) Coordinates[Node_Index3][1];
            Z3 = (float) Coordinates[Node_Index3][2];

            dX21 = X2 - X1;
            dY21 = Y2 - Y1;
            dZ21 = Z2 - Z1;
            dX31 = X3 - X1;
            dY31 = Y3 - Y1;
            dZ31 = Z3 - Z1;

            NX = dY21 * dZ31 - dZ21 * dY31;
            NY = dZ21 * dX31 - dX21 * dZ31;
            NZ = dX21 * dY31 - dY21 * dX31;

            w = fc1 / sqrt (NX * NX + NY * NY + NZ * NZ);

            NX = w * NX;
            NY = w * NY;
            NZ = w * NZ;

            Write_Flag = Write_Flag
                       + ug_fwrite (&NX, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&NY, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&NZ, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&X1, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Y1, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Z1, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&X2, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Y2, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Z2, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&X3, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Y3, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Z3, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag - 12;

            Write_Flag = Write_Flag
                       + ug_fwrite (&Attribute, sizeof (unsigned short int), 1,
                                    Grid_File);
            Write_Flag = Write_Flag - 1;
          }
        }

        for (Surf_Quad_Index = 1;
             Surf_Quad_Index <= Number_of_Surf_Quads; ++Surf_Quad_Index)
        {
          Surf_Index = Number_of_Surf_Trias + Surf_Quad_Index;

          Surf_ID = Surf_ID_Flag[Surf_Index];

          if (Surf_ID == ID)
          {
            Node_Index1 = Surf_Quad_Connectivity[Surf_Quad_Index][0];
            Node_Index2 = Surf_Quad_Connectivity[Surf_Quad_Index][1];
            Node_Index3 = Surf_Quad_Connectivity[Surf_Quad_Index][2];
            Node_Index4 = Surf_Quad_Connectivity[Surf_Quad_Index][3];

            X1 = (float) Coordinates[Node_Index1][0];
            Y1 = (float) Coordinates[Node_Index1][1];
            Z1 = (float) Coordinates[Node_Index1][2];
            X2 = (float) Coordinates[Node_Index2][0];
            Y2 = (float) Coordinates[Node_Index2][1];
            Z2 = (float) Coordinates[Node_Index2][2];
            X3 = (float) Coordinates[Node_Index3][0];
            Y3 = (float) Coordinates[Node_Index3][1];
            Z3 = (float) Coordinates[Node_Index3][2];
            X4 = (float) Coordinates[Node_Index4][0];
            Y4 = (float) Coordinates[Node_Index4][1];
            Z4 = (float) Coordinates[Node_Index4][2];

            dX31 = X3 - X1;
            dY31 = Y3 - Y1;
            dZ31 = Z3 - Z1;
            dX42 = X4 - X2;
            dY42 = Y4 - Y2;
            dZ42 = Z4 - Z2;

            NX = dY31 * dZ42 - dZ31 * dY42;
            NY = dZ31 * dX42 - dX31 * dZ42;
            NZ = dX31 * dY42 - dY31 * dX42;

            w = fc1 / sqrt (NX * NX + NY * NY + NZ * NZ);

            Write_Flag = Write_Flag
                       + ug_fwrite (&NX, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&NY, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&NZ, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&X1, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Y1, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Z1, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&X2, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Y2, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Z2, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&X3, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Y3, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Z3, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag - 12;

            Write_Flag = Write_Flag
                       + ug_fwrite (&Attribute, sizeof (unsigned short int), 1,
                                    Grid_File);
            Write_Flag = Write_Flag - 1;

            Write_Flag = Write_Flag
                       + ug_fwrite (&NX, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&NY, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&NZ, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&X1, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Y1, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Z1, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&X3, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Y3, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Z3, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&X4, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Y4, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag
                       + ug_fwrite (&Z4, sizeof (float), 1, Grid_File);
            Write_Flag = Write_Flag - 12;

            Write_Flag = Write_Flag
                       + ug_fwrite (&Attribute, sizeof (unsigned short int), 1,
                                    Grid_File);
            Write_Flag = Write_Flag - 1;
          }
        }

        if (Write_Flag < 0)
        {
          strcpy (Error_Message, "error writing STL surface grid file");
          return (1);
        }
      }
    }
  }

  return (0);

}
