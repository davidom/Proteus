#include "UG_LIB.h"

INT_ ug_write_param_file
 (char UG_Param_File_Name[],
  char Error_Message[],
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Write a UG parameter file with UG parameters that differ from default.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_write_param_file.c,v 1.15 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX *Char_Param, *Def_Char_Param;

  CHAR_11 *Char_Param_Flag, *Double_Param_Flag, *Int_Param_Flag;

  CHAR_25 *Char_Param_Name, *Double_Param_Name, *Int_Param_Name;

  INT_1D *Double_Param_Vector_Entries, *Double_Param_Vector_Loc, *Int_Param,
         *Int_Param_Vector, *Int_Param_Vector_Entries, *Int_Param_Vector_Loc, 
         *Def_Int_Param;

  DOUBLE_1D *Double_Param_Vector, *Double_Param, *Def_Double_Param;

  FILE *Param_File;

  INT_ Index, Loc, Number_of_Char_Params, Number_of_Double_Params,
       Number_of_Int_Params, Vector_Index, Write_Flag;

  if (strcmp (UG_Param_File_Name, "") == 0)
    return (0);

  Number_of_Char_Params = UG_Param_Struct_Ptr->Number_of_Char_Params;
  Char_Param_Name = UG_Param_Struct_Ptr->Char_Param_Name;
  Char_Param_Flag = UG_Param_Struct_Ptr->Char_Param_Flag;
  Char_Param = UG_Param_Struct_Ptr->Char_Param;
  Def_Char_Param = UG_Param_Struct_Ptr->Def_Char_Param;

  Number_of_Double_Params = UG_Param_Struct_Ptr->Number_of_Double_Params;
  Double_Param_Name = UG_Param_Struct_Ptr->Double_Param_Name;
  Double_Param_Flag = UG_Param_Struct_Ptr->Double_Param_Flag;
  Double_Param_Vector_Entries = UG_Param_Struct_Ptr->Double_Param_Vector_Entries;
  Double_Param_Vector_Loc = UG_Param_Struct_Ptr->Double_Param_Vector_Loc;
  Double_Param = UG_Param_Struct_Ptr->Double_Param;
  Double_Param_Vector = UG_Param_Struct_Ptr->Double_Param_Vector;
  Def_Double_Param = UG_Param_Struct_Ptr->Def_Double_Param;

  Number_of_Int_Params = UG_Param_Struct_Ptr->Number_of_Int_Params;
  Int_Param_Name = UG_Param_Struct_Ptr->Int_Param_Name;
  Int_Param_Flag = UG_Param_Struct_Ptr->Int_Param_Flag;
  Int_Param_Vector_Entries = UG_Param_Struct_Ptr->Int_Param_Vector_Entries;
  Int_Param_Vector_Loc = UG_Param_Struct_Ptr->Int_Param_Vector_Loc;
  Int_Param = UG_Param_Struct_Ptr->Int_Param;
  Int_Param_Vector = UG_Param_Struct_Ptr->Int_Param_Vector;
  Def_Int_Param = UG_Param_Struct_Ptr->Def_Int_Param;

  Param_File = ug_fopen (UG_Param_File_Name, "w");

  if (Param_File == NULL)
  {
    strcpy (Error_Message, "unable to open parameter file");
    return (457);
  }

  Write_Flag = fprintf (Param_File, " \n");

  for (Index = 0; Index < Number_of_Int_Params; ++Index)
  {
    if (strcmp (Int_Param_Flag[Index], "yes") == 0 ||
        strcmp (Int_Param_Flag[Index], "no") == 0)
    {
      if (Int_Param_Vector_Entries[Index] == -1)
      {
        if (Int_Param[Index] != Def_Int_Param[Index])
        {
          Write_Flag = fprintf (Param_File, "%s %i\n",
                                Int_Param_Name[Index],
                                (int) Int_Param[Index]);
        }
      }

      else if (Int_Param_Vector_Entries[Index] > 0)
      {
        Write_Flag = fprintf (Param_File, "%s %i\n",
                              Int_Param_Name[Index],
                              (int) Int_Param_Vector_Entries[Index]);

        Loc = Int_Param_Vector_Loc[Index];

        for (Vector_Index = 0;
             Vector_Index < Int_Param_Vector_Entries[Index]; ++Vector_Index)
        {
          Write_Flag = fprintf (Param_File, "%i\n", (int) Int_Param_Vector[Loc]);

          ++Loc;
        }
      }
    }
  }

  for (Index = 0; Index < Number_of_Double_Params; ++Index)
  {
    if (strcmp (Double_Param_Flag[Index], "yes") == 0 ||
        strcmp (Double_Param_Flag[Index], "no") == 0)
    {
      if (Double_Param_Vector_Entries[Index] == -1)
      {
        if (Double_Param[Index] != Def_Double_Param[Index])
        {
          Write_Flag = fprintf (Param_File, "%s %g\n",
                                Double_Param_Name[Index],
                                Double_Param[Index]);
        }
      }

      else if (Double_Param_Vector_Entries[Index] > 0)
      {
        Write_Flag = fprintf (Param_File, "%s %i\n",
                              Double_Param_Name[Index],
                              (int) Double_Param_Vector_Entries[Index]);

        Loc = Double_Param_Vector_Loc[Index];

        for (Vector_Index = 0;
             Vector_Index < Double_Param_Vector_Entries[Index]; ++Vector_Index)
        {
          Write_Flag = fprintf (Param_File, "%g\n", Double_Param_Vector[Loc]);

          ++Loc;
        }
      }
    }
  }

  for (Index = 0; Index < Number_of_Char_Params; ++Index)
  {
    if (strcmp (Char_Param_Flag[Index], "yes") == 0 ||
        strcmp (Char_Param_Flag[Index], "no") == 0)
    {
      if (strcmp (Char_Param[Index], Def_Char_Param[Index]) != 0)
      {
        Write_Flag = fprintf (Param_File, "%s %s\n",
                              Char_Param_Name[Index], Char_Param[Index]);
      }
    }
  }

  if (Write_Flag < 0)
  {
    strcpy (Error_Message, "error writing parameter file");
    return (458);
  }

  ug_fclose (Param_File);

  return (0);

}
