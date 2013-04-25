#include "UG_IO_LIB.h"

INT_ ug_io_write_gambit
 (FILE * Grid_File,
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
 * Write grid data to a GAMBIT volume or surface grid file.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_gambit.c,v 1.9 2013/03/16 18:02:35 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *Surf_Face_Elem = NULL;
  INT_1D *Surf_Face_Number = NULL;

  CHAR_81 Compile_Date;
  CHAR_81 Compile_OS;
  CHAR_21 Current_Date;
  CHAR_21 Current_Time;
  CHAR_81 Version_Date;
  CHAR_81 Version_Number;

  INT_ Elem_Index, Elem_Type, Error_Flag, Face_Number, Found_Flag, Global_Index,
       Index, Number_of_Elems, Number_of_Elem_Types, Number_of_Faces,
       Number_of_Surf_Faces, Number_of_Surf_IDs, Surf_ID, Surf_ID_Max,
       Surf_ID_Min, Write_Flag;

  Number_of_Elems = Number_of_Vol_Hexs + Number_of_Vol_Pents_5
                  + Number_of_Vol_Pents_6 + Number_of_Vol_Tets;

  Number_of_Elem_Types = 0;
  
  if (Number_of_Vol_Hexs > 0) ++Number_of_Elem_Types;
  if (Number_of_Vol_Pents_5 > 0) ++Number_of_Elem_Types;
  if (Number_of_Vol_Pents_6 > 0) ++Number_of_Elem_Types;
  if (Number_of_Vol_Tets > 0) ++Number_of_Elem_Types;

  Number_of_Surf_Faces = Number_of_Surf_Trias + Number_of_Surf_Quads;

  Surf_ID_Max = Surf_ID_Flag[1];
  Surf_ID_Min = Surf_ID_Flag[1];

  for (Index = 1; Index <= Number_of_Surf_Faces; ++Index)
  {
    Surf_ID = Surf_ID_Flag[Index];

    Surf_ID_Max = MAX (Surf_ID, Surf_ID_Max);
    Surf_ID_Min = MIN (Surf_ID, Surf_ID_Min);
  }

  Number_of_Surf_IDs = 0;

  for (Surf_ID = Surf_ID_Min; Surf_ID <= Surf_ID_Max; ++Surf_ID)
  {
    Found_Flag = 0;

    Index = 1;

    do
    {
      if (Surf_ID == Surf_ID_Flag[Index]) Found_Flag = 1;

      ++Index;
    }
    while (Index <= Number_of_Surf_Trias && Found_Flag == 0);

    if (Found_Flag == 1)
      ++Number_of_Surf_IDs;

    Found_Flag = 0;

    Index = Number_of_Surf_Trias+1;

    do
    {
      if (Surf_ID == Surf_ID_Flag[Index]) Found_Flag = 1;

      ++Index;
    }
    while (Index <= Number_of_Surf_Faces && Found_Flag == 0);

    if (Found_Flag == 1)
      ++Number_of_Surf_IDs;
  }

  ug_io_version (Compile_Date, Compile_OS, Version_Date, Version_Number);

  ug_date_time (Current_Date, Current_Time);

  Write_Flag = fprintf (Grid_File, "        CONTROL INFO 1.2.1\n");
  Write_Flag = fprintf (Grid_File, "** GAMBIT NEUTRAL FILE\n");
  Write_Flag = fprintf (Grid_File, "UG_IO : %s %s\n", Version_Number, Version_Date);
  Write_Flag = fprintf (Grid_File, "PROGRAM:                           UG_IO     VERSION:   1.00\n");
  Write_Flag = fprintf (Grid_File, "%s %s\n", Current_Date, Current_Time);
  Write_Flag = fprintf (Grid_File, "     NUMNP     NELEM     NGRPS    NBSETS     NDCFD     NDFVL\n");
  Write_Flag = fprintf (Grid_File, " %9i %9i %9i %9i         3         3\n",
                        Number_of_Nodes, Number_of_Elems, Number_of_Elem_Types,
                        Number_of_Surf_IDs);
  Write_Flag = fprintf (Grid_File, "ENDOFSECTION\n");

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing GAMBIT grid file");
    return (1);
  }

  Write_Flag = fprintf (Grid_File, "   NODAL COORDINATES 1.2.1\n");

  for (Index = 1; Index <= Number_of_Nodes; ++Index)
  {
    Write_Flag = fprintf (Grid_File,
                          "%10i%20.12e%20.12e%20.12e\n",
                          Index,
                          Coordinates[Index][0],
                          Coordinates[Index][1],
                          Coordinates[Index][2]);
  }

  Write_Flag = fprintf (Grid_File, "ENDOFSECTION\n");

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing GAMBIT grid file");
    return (1);
  }

  Write_Flag = fprintf (Grid_File, "      ELEMENTS/CELLS 1.2.1\n");

  Global_Index = 0;

  for (Index = 1; Index <= Number_of_Vol_Tets; ++Index)
  {
    ++Global_Index;

    Write_Flag = fprintf (Grid_File,
                          "%8i  6  4 %8i%8i%8i%8i\n",
                          Global_Index,
                          Vol_Tet_Connectivity[Index][0],
                          Vol_Tet_Connectivity[Index][1],
                          Vol_Tet_Connectivity[Index][2],
                          Vol_Tet_Connectivity[Index][3]);
  }

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing GAMBIT grid file");
    return (1);
  }

  for (Index = 1; Index <= Number_of_Vol_Pents_5; ++Index)
  {
    ++Global_Index;

    Write_Flag = fprintf (Grid_File,
                          "%8i  7  5 %8i%8i%8i%8i%8i\n",
                          Global_Index,
                          Vol_Pent_5_Connectivity[Index][1],
                          Vol_Pent_5_Connectivity[Index][0],
                          Vol_Pent_5_Connectivity[Index][4],
                          Vol_Pent_5_Connectivity[Index][3],
                          Vol_Pent_5_Connectivity[Index][2]);
  }

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing GAMBIT grid file");
    return (1);
  }

  for (Index = 1; Index <= Number_of_Vol_Pents_6; ++Index)
  {
    ++Global_Index;

    Write_Flag = fprintf (Grid_File,
                          "%8i  5  6 %8i%8i%8i%8i%8i%8i\n",
                          Global_Index,
                          Vol_Pent_6_Connectivity[Index][0],
                          Vol_Pent_6_Connectivity[Index][1],
                          Vol_Pent_6_Connectivity[Index][2],
                          Vol_Pent_6_Connectivity[Index][3],
                          Vol_Pent_6_Connectivity[Index][4],
                          Vol_Pent_6_Connectivity[Index][5]);
  }

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing GAMBIT grid file");
    return (1);
  }

  for (Index = 1; Index <= Number_of_Vol_Hexs; ++Index)
  {
    ++Global_Index;

    Write_Flag = fprintf (Grid_File,
                          "%8i  5  6 %8i%8i%8i%8i%8i%8i%8i\n",
                          Global_Index,
                          Vol_Hex_Connectivity[Index][0],
                          Vol_Hex_Connectivity[Index][1],
                          Vol_Hex_Connectivity[Index][3],
                          Vol_Hex_Connectivity[Index][2],
                          Vol_Hex_Connectivity[Index][4],
                          Vol_Hex_Connectivity[Index][5],
                          Vol_Hex_Connectivity[Index][7]);

    Write_Flag = fprintf (Grid_File,
                          "               %8i\n",
                          Vol_Hex_Connectivity[Index][6]);
  }

  Write_Flag = fprintf (Grid_File, "ENDOFSECTION\n");

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing GAMBIT grid file");
    return (1);
  }

  Error_Flag = ug3_ielibfm (Number_of_Surf_Trias,
                            Number_of_Vol_Tets,
                            Number_of_Vol_Pents_5,
                            Number_of_Vol_Pents_6,
                            Number_of_Vol_Hexs,
                            Number_of_Surf_Quads,
                            Surf_Tria_Connectivity,
                            Surf_Quad_Connectivity,
                            Vol_Tet_Connectivity,
                            Vol_Pent_5_Connectivity,
                            Vol_Pent_6_Connectivity,
                            Vol_Hex_Connectivity,
                            &Surf_Face_Number,
                            &Surf_Face_Elem);

  if (Error_Flag > 0)
  {
    sprintf (Error_Message, "unable to malloc GAMBIT I/O work arrays %i",
             Error_Flag);
    ug_free (Surf_Face_Number);
    ug_free (Surf_Face_Elem);
    return (1);
  }

  if (Error_Flag == -1)
  {
    strcpy (Error_Message, "unable to find GAMBIT boundary faces");
    return (1);
  }

  for (Surf_ID = Surf_ID_Min; Surf_ID <= Surf_ID_Max; ++Surf_ID)
  {
    Number_of_Faces = 0;

    for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
    {
      if (Surf_ID == Surf_ID_Flag[Index]) ++Number_of_Faces;
    }

    if (Number_of_Faces > 0)
    {
      Write_Flag = fprintf (Grid_File, " BOUNDARY CONDITIONS 1.2.1\n");
      Write_Flag = fprintf (Grid_File,
       "BOUNDARY SURFACE %9i TRIAS         1%10i         0         0\n",
       Surf_ID, Number_of_Faces);

      for (Index = 1; Index <= Number_of_Surf_Trias; ++Index)
      {
        if (Surf_ID == Surf_ID_Flag[Index])
        {
          Elem_Index = Surf_Face_Elem[Index];

	  Face_Number = Surf_Face_Number[Index];

	  Elem_Type = (Elem_Index <= Number_of_Vol_Tets) ? 5 :
	              (Elem_Index <= Number_of_Vol_Tets
                                   + Number_of_Vol_Pents_5) ? 7 :
	              (Elem_Index <= Number_of_Vol_Tets
                                   + Number_of_Vol_Pents_5
                                   + Number_of_Vol_Pents_6) ? 5 : 4;

          Write_Flag = fprintf (Grid_File, "%10i%5i%5i\n",
                                           Elem_Index, Elem_Type, Face_Number);
        }
      }

      Write_Flag = fprintf (Grid_File, "ENDOFSECTION\n");

      if (Write_Flag < 0)
      {
        strcpy (Error_Message, "error writing GAMBIT grid file");
        return (1);
      }
    }

    Number_of_Faces = 0;

    for (Index = Number_of_Surf_Trias+1; Index <= Number_of_Surf_Faces; ++Index)
    {
      if (Surf_ID == Surf_ID_Flag[Index]) ++Number_of_Faces;
    }

    if (Number_of_Faces > 0)
    {
      Write_Flag = fprintf (Grid_File, " BOUNDARY CONDITIONS 1.2.1\n");
      Write_Flag = fprintf (Grid_File,
       "BOUNDARY SURFACE %9i QUADS         1%10i         0         0\n",
       Surf_ID, Number_of_Faces);

      for (Index = Number_of_Surf_Trias+1;
           Index <= Number_of_Surf_Faces; ++Index)
      {
        if (Surf_ID == Surf_ID_Flag[Index])
        {
          Elem_Index = Surf_Face_Elem[Index];

	  Face_Number = Surf_Face_Number[Index];

	  Elem_Type = (Elem_Index <= Number_of_Vol_Tets
                                   + Number_of_Vol_Pents_5) ? 7 :
	              (Elem_Index <= Number_of_Vol_Tets
                                   + Number_of_Vol_Pents_5
                                   + Number_of_Vol_Pents_6) ? 5 : 4;

          Write_Flag = fprintf (Grid_File, "%10i%5i%5i\n",
                                           Elem_Index, Elem_Type, Face_Number);
        }
      }

      Write_Flag = fprintf (Grid_File, "ENDOFSECTION\n");

      if (Write_Flag < 0)
      {
        strcpy (Error_Message, "error writing GAMBIT grid file");
        ug_free (Surf_Face_Number);
        ug_free (Surf_Face_Elem);
        return (1);
      }
    }
  }

  ug_free (Surf_Face_Number);
  ug_free (Surf_Face_Elem);

  return (0);

}
