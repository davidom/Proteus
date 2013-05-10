#include "UG_IO_LIB.h"

INT_ ug_io_write_efunc
 (char Case_Name[],
  char Error_Message[],
  CHAR_21 * U_Scalar_Labels,
  CHAR_21 * U_Vector_Labels,
  INT_ Number_of_Nodes,
  INT_ Number_of_U_Scalars,
  INT_ Number_of_U_Vectors,
  INT_1D * U_Scalar_Flags,
  INT_1D * U_Vector_Flags,
  DOUBLE_1D * U_Scalars,
  DOUBLE_3D * U_Vectors)

{

/*
 * Write function data to Ensight Gold case and function files.
 * 
 * UG_IO LIB : Unstructured Grid - Input/Output Routine Library
 * $Id: ug_io_write_efunc.c,v 1.3 2012/08/23 04:01:49 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

#define NCHAR 80

  FILE **Scalar_Func_File;
  FILE **Vector_Func_File;

  FILE *Case_File = NULL;
  FILE *Func_File = NULL;
  FILE *Node_Map_File = NULL;

  char Text[NCHAR];

  INT_1D *Node_Map = NULL;

  INT_ Dim, Error_Flag, Face_Nodes, ID_Parts, Index, Offset, Node_Index,
       Number_of_ID_Nodes, Number_of_ID_Parts, Number_of_Parts,
       Number_of_Read_Items_NM, Number_of_Vol_Elems, Number_of_Write_Items,
       Read_Flag_NM, Surf_ID, U_Index, Write_Flag;

  int n;

  float ui;

  Error_Flag = 0;

  Scalar_Func_File = (FILE **) ug_malloc (&Error_Flag,
                                          Number_of_U_Scalars * sizeof (FILE));
  Vector_Func_File = (FILE **) ug_malloc (&Error_Flag,
                                          Number_of_U_Vectors * sizeof (FILE));

  if (Error_Flag > 0)
  {
    ug_free (Scalar_Func_File);
    ug_free (Vector_Func_File);
    strcpy (Error_Message, "unable to malloc Ensight function file work array");
    return (1);
  }
 
  for (U_Index = 0; U_Index < Number_of_U_Scalars; ++U_Index)
  {
    if (U_Scalar_Flags[U_Index])
    {
      sprintf (Text, "%s.%s.efunc", Case_Name, U_Scalar_Labels[U_Index]);
   
      Scalar_Func_File[U_Index] = ug_fopen (Text, "w");

      if (Scalar_Func_File[U_Index] == NULL)
      {
        ug_free (Scalar_Func_File);
        ug_free (Vector_Func_File);
        strcpy (Error_Message, "error opening Ensight scalar function file");
        return (1);
      }
    }
  }
 
  for (U_Index = 0; U_Index < Number_of_U_Vectors; ++U_Index)
  {
    if (U_Vector_Flags[U_Index])
    {
      sprintf (Text, "%s.%s.efunc", Case_Name, U_Vector_Labels[U_Index]);
   
      Vector_Func_File[U_Index] = ug_fopen (Text, "a");

      if (Vector_Func_File[U_Index] == NULL)
      {
        ug_free (Scalar_Func_File);
        ug_free (Vector_Func_File);
        strcpy (Error_Message, "error opening Ensight vector function file");
        return (1);
      }
    }
  }

  Number_of_Parts = 0;

  Number_of_Write_Items = 0;

  Write_Flag = 0;

  Error_Flag = 0;

  Node_Map = (INT_1D *) ug_malloc (&Error_Flag,
                                   (Number_of_Nodes+1) * sizeof (INT_1D));

  if (Error_Flag > 0)
  {
    ug_free (Node_Map);
    ug_free (Scalar_Func_File);
    ug_free (Vector_Func_File);
    strcpy (Error_Message, "unable to malloc Ensight function file work array");
    return (1);
  }

  Number_of_Read_Items_NM = 0;

  Read_Flag_NM = 0;

  sprintf (Text, "%s.ensight.nmap", Case_Name);
   
  Node_Map_File = ug_fopen (Text, "r");

  if (Node_Map_File == NULL)
  {
    ug_free (Node_Map);
    ug_free (Scalar_Func_File);
    ug_free (Vector_Func_File);
    strcpy (Error_Message, "error opening node map data file - try again after re-creating Ensight grid file");
    return (1);
  }

  Read_Flag_NM = Read_Flag_NM + ug_fread (&Number_of_ID_Parts, sizeof (INT_), 1, Node_Map_File);
  ++Number_of_Read_Items_NM;

  Read_Flag_NM = Read_Flag_NM + ug_fread (&Number_of_Vol_Elems, sizeof (INT_), 1, Node_Map_File);
  ++Number_of_Read_Items_NM;

  for (ID_Parts = 1; ID_Parts <= Number_of_ID_Parts; ++ID_Parts)
  {
    ++Number_of_Parts;

    Read_Flag_NM = Read_Flag_NM + ug_fread (&Face_Nodes, sizeof (INT_), 1, Node_Map_File);
    ++Number_of_Read_Items_NM;

    Read_Flag_NM = Read_Flag_NM + ug_fread (&Surf_ID, sizeof (INT_), 1, Node_Map_File);
    ++Number_of_Read_Items_NM;

    Read_Flag_NM = Read_Flag_NM + ug_fread (&Number_of_ID_Nodes, sizeof (INT_), 1, Node_Map_File);
    ++Number_of_Read_Items_NM;

    ug_set_int (1, Number_of_Nodes, 0, Node_Map);

    for (Index = 1; Index <= Number_of_ID_Nodes; ++Index)
    {
      Node_Index = 0;

      Read_Flag_NM = Read_Flag_NM + ug_fread (&Node_Index, sizeof (INT_), 1, Node_Map_File);
      ++Number_of_Read_Items_NM;

      Node_Map[Node_Index] = Index;
    }

    Offset = 0;

    for (U_Index = 0; U_Index < Number_of_U_Scalars; ++U_Index)
    {
      if (U_Scalar_Flags[U_Index])
      {
        Func_File = Scalar_Func_File[U_Index];

        strcpy (Text, "part");
        Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Func_File);
        Number_of_Write_Items = Number_of_Write_Items + NCHAR;

        n = (int) Number_of_Parts;

        Write_Flag = Write_Flag + ug_fwrite (&n, sizeof(int), 1, Func_File);
        ++Number_of_Write_Items;

        strcpy (Text, "coordinates");
        Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Func_File);
        Number_of_Write_Items = Number_of_Write_Items + NCHAR;

        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          if (Node_Map[Index])
          {
            ui = (float) (U_Scalars[Offset+Index]);

            Write_Flag = Write_Flag + ug_fwrite (&ui, sizeof (float), 1, Func_File);
            ++Number_of_Write_Items;
          }
        }

        Offset = Offset + Number_of_Nodes+1;
      }
    }

    Offset = 0;

    for (U_Index = 0; U_Index < Number_of_U_Vectors; ++U_Index)
    {
      if (U_Vector_Flags[U_Index])
      {
        Func_File = Vector_Func_File[U_Index];

        strcpy (Text, "part");
        Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Func_File);
        Number_of_Write_Items = Number_of_Write_Items + NCHAR;

        n = (int) Number_of_Parts;

        Write_Flag = Write_Flag + ug_fwrite (&n, sizeof(int), 1, Func_File);
        ++Number_of_Write_Items;

        strcpy (Text, "coordinates");
        Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Func_File);
        Number_of_Write_Items = Number_of_Write_Items + NCHAR;

        for (Dim = 0; Dim < 3; ++Dim)
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            if (Node_Map[Index])
            {
              ui = (float) (U_Vectors[Offset+Index][Dim]);

              Write_Flag = Write_Flag + ug_fwrite (&ui, sizeof (float), 1, Func_File);

              ++Number_of_Write_Items;
            }
          }
        }

        Offset = Offset + Number_of_Nodes+1;
      }
    }
  }
  
  ug_fclose (Node_Map_File);

  ug_free (Node_Map);

  if (Read_Flag_NM != Number_of_Read_Items_NM)
  {
    strcpy (Error_Message, "error reading node map data file");
    return (1);
  }

  if (Number_of_Vol_Elems)
  {
    ++Number_of_Parts;
 
    Offset = 0;

    for (U_Index = 0; U_Index < Number_of_U_Scalars; ++U_Index)
    {
      if (U_Scalar_Flags[U_Index])
      {
        Func_File = Scalar_Func_File[U_Index];

        strcpy (Text, U_Scalar_Labels[U_Index]);
        strcat (Text, " scalar file");
        Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Func_File);
        Number_of_Write_Items = Number_of_Write_Items + NCHAR;

        strcpy (Text, "part");
        Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Func_File);
        Number_of_Write_Items = Number_of_Write_Items + NCHAR;

        n = (int) Number_of_Parts;

        Write_Flag = Write_Flag + ug_fwrite (&n, sizeof(int), 1, Func_File);
        ++Number_of_Write_Items;

        strcpy (Text, "coordinates");
        Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Func_File);
        Number_of_Write_Items = Number_of_Write_Items + NCHAR;

        for (Index = 1; Index <= Number_of_Nodes; ++Index)
        {
          ui = (float) (U_Scalars[Offset+Index]);

          Write_Flag = Write_Flag + ug_fwrite (&ui, sizeof (float), 1, Func_File);

          ++Number_of_Write_Items;
        }

        Offset = Offset + Number_of_Nodes+1;
      }
    }
 
    Offset = 0;

    for (U_Index = 0; U_Index < Number_of_U_Vectors; ++U_Index)
    {
      if (U_Vector_Flags[U_Index])
      {
        Func_File = Vector_Func_File[U_Index];

        strcpy (Text, U_Vector_Labels[U_Index]);
        strcat (Text, " vector file");
        Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Func_File);
        Number_of_Write_Items = Number_of_Write_Items + NCHAR;

        strcpy (Text, "part");
        Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Func_File);
        Number_of_Write_Items = Number_of_Write_Items + NCHAR;

        n = (int) Number_of_Parts;

        Write_Flag = Write_Flag + ug_fwrite (&n, sizeof(int), 1, Func_File);
        ++Number_of_Write_Items;

        strcpy (Text, "coordinates");
        Write_Flag = Write_Flag + ug_fwrite (Text, sizeof (char), NCHAR, Func_File);
        Number_of_Write_Items = Number_of_Write_Items + NCHAR;

        for (Dim = 0; Dim < 3; ++Dim)
        {
          for (Index = 1; Index <= Number_of_Nodes; ++Index)
          {
            ui = (float) (U_Vectors[Offset+Index][Dim]);

            Write_Flag = Write_Flag + ug_fwrite (&ui, sizeof (float), 1, Func_File);

            ++Number_of_Write_Items;
          }
        }

        Offset = Offset + Number_of_Nodes+1;
      }
    }
  }

  for (U_Index = 0; U_Index < Number_of_U_Scalars; ++U_Index)
  {
    ug_fclose (Scalar_Func_File[U_Index]);
  }
 
  for (U_Index = 0; U_Index < Number_of_U_Vectors; ++U_Index)
  {
    ug_fclose (Vector_Func_File[U_Index]);
  }
  
  ug_free (Scalar_Func_File);
  ug_free (Vector_Func_File);

  if (Write_Flag != Number_of_Write_Items)
  {
    strcpy (Error_Message, "error writing Ensight function file");
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

  Write_Flag = fprintf (Case_File, "VARIABLE\n");

  for (U_Index = 0; U_Index < Number_of_U_Scalars; ++U_Index)
  {
    Write_Flag = fprintf (Case_File, "scalar per node: %s %s.%s.efunc\n",
                          U_Scalar_Labels[U_Index],
                          Case_Name, U_Scalar_Labels[U_Index]);
  }

  for (U_Index = 0; U_Index < Number_of_U_Vectors; ++U_Index)
  {
    Write_Flag = fprintf (Case_File, "vector per node: %s %s.%s.efunc\n",
                          U_Vector_Labels[U_Index],
                          Case_Name, U_Vector_Labels[U_Index]);
  }

  ug_fclose (Case_File);

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing Ensight case file");
    return (1);
  }

  return (0);

}

