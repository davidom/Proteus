#include "UG_LIB.h"

INT_ ug_get_param_from_arg_or_file
 (char *argv[],
  char Char_Param_Value[],
  char Param_Name[],
  char UG_Param_File_Name[],
  INT_1D **Int_Param_Vector,
  INT_ argc,
  INT_ *Int_Param_Value,
  INT_ Param_Type_Flag,
  INT_ *Param_Vector_Entries,
  DOUBLE_1D **Double_Param_Vector,
  double *Double_Param_Value)

{

/*
 * Get the values for a UG type double parameter vector from either an array of
 * arguments or if not found there from the UG parameter structure.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_param_from_arg_or_file.c,v 1.12 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  static FILE *Param_File = NULL;
  static INT_ Alloc_Flag = 0;
  static INT_ arg = 1;

  char *Read_Label;

  CHAR_UG_MAX Alt_Name, Name, Param_String, Next_Param_String, Value;

  INT_ Error_Flag, Found_Flag, Next_Param_Flag, Pass, Pass1, Pass2;
  INT_ EOF_Flag = 0;
  INT_ Get_Flag = 0;

  Pass1 = (argc > 1) ? 1: 2;
  Pass2 = (UG_Param_File_Name && strcmp (UG_Param_File_Name, "") != 0) ? 2: 1;

  if (Pass2 < Pass1)
    return (0);

  Pass = Pass1;

  do
  {
    if (Pass == 2)
    {
      Param_File = ug_fopen (UG_Param_File_Name, "r");

      if (Param_File == NULL)
        return (0);
    }

    do
    {
      Found_Flag = 0;

      if (arg < argc && Pass == 1)
      {
        do
        {
          strcpy (Param_String, argv[arg]);

          if (arg+1 < argc)
            strcpy (Next_Param_String, argv[arg+1]);
          else
            strcpy (Next_Param_String, "");

          if (strcmp (Next_Param_String, "") == 0 && Param_Type_Flag == CHAR_PARAM_TYPE_FLAG)
          {
            strcat (Param_String, " NULL");

            Next_Param_Flag = 1;
          }

          else if (strstr (Param_String, "=") || strcmp (Next_Param_String, "") == 0 || Param_Type_Flag == -DOUBLE_PARAM_VECTOR_TYPE_FLAG || Param_Type_Flag == -INT_PARAM_VECTOR_TYPE_FLAG)
            Next_Param_Flag = 0;
          else
          {
            strcat (Param_String, " ");
            strcat (Param_String, Next_Param_String);

            Next_Param_Flag = 1;
          }

          Error_Flag = ug_get_param_name_from_string (Param_String, Name, Value);

          if (Error_Flag == -4 && (Param_Type_Flag == -DOUBLE_PARAM_VECTOR_TYPE_FLAG || Param_Type_Flag == -INT_PARAM_VECTOR_TYPE_FLAG))
          {
            strcpy (Value, Name);

            Error_Flag = 0;
          }

          strcpy (Alt_Name, "-");
          strcat (Alt_Name, Param_Name);
 
          if (Error_Flag == 0)
            Found_Flag = (Param_Type_Flag < 0 || strcmp (Param_Name, Name) == 0 || strcmp (Alt_Name, Name) == 0) ? 1: 0;

          if (Next_Param_Flag == 1 && Found_Flag)
            ++arg;
    
          ++arg;
        }
        while (arg < argc && Found_Flag == 0);
      }

      else if (Pass == 2)
      {
        do
        {
          Read_Label = fgets (Param_String, UG_MAX_CHAR_STRING_LENGTH, Param_File);

          if (Read_Label)
          {
            Error_Flag = ug_get_param_name_from_string (Param_String, Name, Value);

            if (Error_Flag == -4 && Param_Type_Flag == CHAR_PARAM_TYPE_FLAG)
            {
              strcpy (Value, "NULL");

              Error_Flag = 0;
            }

            else if (Error_Flag == -4 && (Param_Type_Flag == -DOUBLE_PARAM_VECTOR_TYPE_FLAG || Param_Type_Flag == -INT_PARAM_VECTOR_TYPE_FLAG))
            {
              strcpy (Value, Name);

              Error_Flag = 0;
            }

            strcpy (Alt_Name, "-");
            strcat (Alt_Name, Param_Name);
 
            if (Error_Flag == 0)
              Found_Flag = (Param_Type_Flag < 0 || strcmp (Param_Name, Name) == 0 || strcmp (Alt_Name, Name) == 0) ? 1: 0;
          }

          EOF_Flag = feof (Param_File);

        }
        while (EOF_Flag == 0 && Found_Flag == 0);
      }

      if (Found_Flag == 1)
      {
        Error_Flag = ug_get_param_from_value (Char_Param_Value, Value,
                                              Param_Vector_Entries, Int_Param_Value, &Param_Type_Flag, Int_Param_Vector,
                                              Double_Param_Value, Double_Param_Vector);

        if (Param_Type_Flag == CHAR_PARAM_TYPE_FLAG && strcmp (Value, "NULL") == 0) strcpy (Char_Param_Value, "");

        if (Error_Flag)
          Get_Flag = 0;
        else if (Param_Type_Flag < 0)
        {
          Get_Flag = 1;

          Alloc_Flag = 1;
        }
        else
          Get_Flag = -1;
      }
      else if (Param_Type_Flag < 0)
      {
        Get_Flag = 0;

        if (Alloc_Flag == 1 && Param_Type_Flag == -DOUBLE_PARAM_VECTOR_TYPE_FLAG)
        {
          ug_free (*Double_Param_Vector);

          *Double_Param_Vector = NULL;
        }
        else if (Alloc_Flag == 1 && Param_Type_Flag == -INT_PARAM_VECTOR_TYPE_FLAG)
        {
          ug_free (*Int_Param_Vector);

          *Int_Param_Vector = NULL;
        }

        Param_Type_Flag = abs (Param_Type_Flag);
      }
    }
    while (Get_Flag == 1);

    if (Get_Flag != 0)
      Get_Flag = 1;

    if (Pass == 1)
    {
      Alloc_Flag = 0;

      arg = 1;
    }
    else
    {
      Alloc_Flag = 0;

      ug_fclose (Param_File);

      Param_File = NULL;
    }

    ++Pass;
  }
  while (Pass <= Pass2 && Get_Flag == 0);

  return (Get_Flag);

}
