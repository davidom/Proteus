#include "UG_LIB.h"

CHAR_UG_MAX Opt_Param_Strings;

INT_ Opt_Flag = 0;

INT_ ug_get_param_index
 (char Param_Name[],
  INT_ *Param_Type_Flag,
  UG_Param_Struct * UG_Param_Struct_Ptr);

INT_ ug_set_param_from_string
 (char Param_String[],
  char Next_Param_String[],
  INT_ Message_Flag,
  UG_Param_Struct * UG_Param_Struct_Ptr);

INT_ ug_set_params_from_arg_or_file
 (char *argv[],
  char UG_Param_File_Name[],
  INT_ argc,
  INT_ Message_Flag,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Search a UG parameter file and/or an array of input arguments for UG
 * parameters to read and re-set.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_set_params_from_arg_or_file.c,v 1.17 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  FILE *Param_File;

  char *Read_Label = NULL;
  char *String_ptr;

  CHAR_UG_MAX drive, dname, fname, ext, Param_String, Next_Param_String, String,
              Sub_String;
  CHAR_31 Name_Text;
  CHAR_133 Text;

  INT_ arg, EOF_Flag, Length, Opt_Param, Pass, Pass1, Pass2;
  INT_ Error_Flag = 0;
  INT_ Param_Error_Flag = 0;

  if (UG_Param_Struct_Ptr == NULL)
    return (0);

  Pass1 = (argc > 1) ? 1: 2;
  Pass2 = (UG_Param_File_Name && strcmp (UG_Param_File_Name, "") != 0) ? 2: 1;

  for (Pass = Pass2; Pass >= Pass1; --Pass)
  {
    if (Message_Flag)
    {
      ug_message (" ");
      if (Pass == 1)
        ug_message ("UG PARAM : SETTING INPUT PARAMETERS FROM ARGUMENT VECTOR");
      else
      {
        ug_splitpath (UG_Param_File_Name, drive, dname, fname, ext);

        strcat (fname, ext);
        strcpy (Name_Text, "");
        strncat (Name_Text, fname, 29);

        ug_message ("UG PARAM : SETTING INPUT PARAMETERS FROM FILE");
        sprintf (Text, "UG PARAM : %-s", Name_Text);
        ug_message (Text);
      }
      ug_message (" ");
    }

    if (Pass == 2)
    {
      Param_File = ug_fopen (UG_Param_File_Name, "r");

      if (Param_File == NULL)
        return (413);
    }

    arg = 1;

    do
    {
      strcpy (Opt_Param_Strings, "");

      Opt_Flag = 0;
      Opt_Param = 0;

      do
      {
        if (Opt_Flag == 0)
        {
          strcpy (Param_String, "");

          strcpy (Next_Param_String, "");

          if (Pass == 1)
          {
            strcpy (Param_String, argv[arg]);

            if (arg+1 < argc)
              strcpy (Next_Param_String, argv[arg+1]);
          }
          else
            Read_Label = fgets (Param_String, UG_MAX_CHAR_STRING_LENGTH, Param_File);
        }

        else
        {
          if (Opt_Param == 0)
            strcpy (String, Opt_Param_Strings);

          Length = (INT_) strcspn (String, " ");

          if (Length)
          {
            strcpy (Sub_String, "");

            strncat (Sub_String, String, Length);

            strcpy (Param_String, "");

            strcpy (Param_String, Sub_String);

            strcpy (Next_Param_String, "");

            ++Opt_Param;

            String_ptr = strstr (String, " ");

            if (String_ptr)
            {
              if (strlen (String_ptr) > 1)
              {
                ++String_ptr;

                strcpy (String, String_ptr);
              }
              else
                strcpy (String, "");
            }
            else
              strcpy (String, "");
          }
          else
            strcpy (Param_String, "");
        }

        if (strcmp (Param_String, ""))
        {
          Error_Flag = ug_set_param_from_string (Param_String, Next_Param_String, Message_Flag, UG_Param_Struct_Ptr);

          if (Pass == 1 && Error_Flag == -6 && (Opt_Flag == 0 || (Opt_Flag == 3 && Opt_Param == 0)))
          {
            ++arg;

            Error_Flag = 0;
          }

          if (Message_Flag >= 1)
          {
            strcpy (Name_Text, "");
            strncat (Name_Text, Param_String, 29);

            if (Error_Flag == -2)
            {
              sprintf (Text, "UG PARAM : %-24s    = UNREADABLE PARAMETER STRING", Name_Text);
              ug_message (Text);
            }

            else if (Error_Flag == -3)
            {
              sprintf (Text, "UG PARAM : %-24s    = UNKNOWN PARAMETER NAME", Name_Text);
              ug_message (Text);
            }

            else if (Error_Flag == -4 || Error_Flag == -5)
            {
              sprintf (Text, "UG PARAM : %-24s    = PARAMETER VALUE MISSING", Name_Text);
              ug_message (Text);
            }
          }

          if (Error_Flag < -1)
            ++Param_Error_Flag;

          Error_Flag = MAX (Error_Flag, 0);
        }
        else
          Opt_Flag = 0;
      }
      while (Error_Flag == 0 && Opt_Flag);

      if (Pass == 1)
        ++arg;
      else
        EOF_Flag = feof (Param_File);
    }
    while (((Pass == 1 && arg < argc) || (Pass == 2 && EOF_Flag == 0)) && Error_Flag == 0);

    if (Pass == 2)
      ug_fclose (Param_File);
  }

  if (Error_Flag == 0 && Param_Error_Flag)
    return (412);

  return (Error_Flag);

}

INT_ ug_set_param_from_string
 (char Param_String[],
  char Next_Param_String[],
  INT_ Message_Flag,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Set a UG parameter from a given string.
 */

  CHAR_UG_MAX Char_Param_Value, Name, String, Value;

  static INT_1D *Int_Param_Vector = NULL;
  static DOUBLE_1D *Double_Param_Vector = NULL;
  static INT_ Entries = 0;
  static INT_ Index = 0;
  static INT_ Param_Type_Flag = 0;

  INT_ Error_Flag, Int_Param_Value, Next_Param_Flag;

  double Double_Param_Value;

  // reset vector flags and exit
  // if last vector parameter was not fully set then exit with error

  if (strcmp (Param_String, "RESET") == 0)
  {
    ug_free (Double_Param_Vector);
    ug_free (Int_Param_Vector);

    Double_Param_Vector = NULL;
    Int_Param_Vector = NULL;

    if (Param_Type_Flag)
      return (409);

    Entries = 0;

    Param_Type_Flag = 0;

    return (0);
  }

  if (Param_Type_Flag != -DOUBLE_PARAM_VECTOR_TYPE_FLAG && Param_Type_Flag != -INT_PARAM_VECTOR_TYPE_FLAG)
    Param_Type_Flag = 0;

  if (strstr (Param_String, "=") || strcmp (Next_Param_String, "") == 0 || Param_Type_Flag == -DOUBLE_PARAM_VECTOR_TYPE_FLAG || Param_Type_Flag == -INT_PARAM_VECTOR_TYPE_FLAG)
  {
    strcpy (String, Param_String);

    Next_Param_Flag = 0;
  }
  else
  {
    strcpy (String, Param_String);
    strcat (String, " ");
    strcat (String, Next_Param_String);

    Next_Param_Flag = 1;
  }

  Error_Flag = ug_get_param_name_from_string (String, Name, Value);

  if (Error_Flag == -4 && (Param_Type_Flag == -DOUBLE_PARAM_VECTOR_TYPE_FLAG || Param_Type_Flag == -INT_PARAM_VECTOR_TYPE_FLAG))
  {
    strcpy (Value, Name);

    Error_Flag = 0;
  }

  if (Error_Flag == -4 || Error_Flag == -5)
    strcpy (Param_String, Name);

  if (Error_Flag != 0 && Error_Flag != -4 && Error_Flag != -5)
    return (Error_Flag);

  if (! Param_Type_Flag)
    Index = ug_get_param_index (Name, &Param_Type_Flag, UG_Param_Struct_Ptr);

  if (! Param_Type_Flag)
  {
    strcpy (Param_String, Name);
    return (-3);
  }

  if (Param_Type_Flag && (Error_Flag == -4 || Error_Flag == -5) && Param_Type_Flag == OPT_PARAM_TYPE_FLAG)
    Error_Flag = 0;

  if (Error_Flag != 0)
    return (Error_Flag);

  Error_Flag = ug_get_param_from_value (Char_Param_Value, Value,
                                        &Entries, &Int_Param_Value, &Param_Type_Flag, &Int_Param_Vector,
                                        &Double_Param_Value, &Double_Param_Vector);

  if (Error_Flag == 0)
  {
    if (Param_Type_Flag == CHAR_PARAM_TYPE_FLAG)
      ug_set_char_param_ (Index, Message_Flag, Char_Param_Value, UG_Param_Struct_Ptr);

    else if (Param_Type_Flag == DOUBLE_PARAM_TYPE_FLAG)
      ug_set_double_param_ (Index, Message_Flag, Double_Param_Value, UG_Param_Struct_Ptr);

    else if (Param_Type_Flag == INT_PARAM_TYPE_FLAG)
      ug_set_int_param_ (Index, Message_Flag, Int_Param_Value, UG_Param_Struct_Ptr);

    else if (Param_Type_Flag == DOUBLE_PARAM_VECTOR_TYPE_FLAG)
      Error_Flag = ug_set_double_param_vector_ (Index, Message_Flag, Entries, Double_Param_Vector, UG_Param_Struct_Ptr);

    else if (Param_Type_Flag == INT_PARAM_VECTOR_TYPE_FLAG)
      Error_Flag = ug_set_int_param_vector_ (Index, Message_Flag, Entries, Int_Param_Vector, UG_Param_Struct_Ptr);

    else if (Param_Type_Flag == OPT_PARAM_TYPE_FLAG)
    {
      if (Opt_Flag == 0)
      {
        Opt_Flag = 2;

        strcpy (Opt_Param_Strings, "");
        strcpy (Opt_Param_Strings, UG_Param_Struct_Ptr->Def_Char_Param[Index]);
      }
      else
        Error_Flag = 411;
    }

    else if (Param_Type_Flag == OPT_VALUE_PARAM_TYPE_FLAG)
    {
      if (Opt_Flag == 0)
      {
        Opt_Flag = 3;

        strcpy (Opt_Param_Strings, "");
        strcpy (Opt_Param_Strings, UG_Param_Struct_Ptr->Def_Char_Param[Index]);
        strcat (Opt_Param_Strings, Char_Param_Value);
      }
      else
        Error_Flag = 411;
    }
  }

  if (Error_Flag != 0 || Param_Type_Flag != -DOUBLE_PARAM_VECTOR_TYPE_FLAG)
  {
    ug_free (Double_Param_Vector);

    Double_Param_Vector = NULL;
  }

  if (Error_Flag != 0 || Param_Type_Flag != -INT_PARAM_VECTOR_TYPE_FLAG)
  {
    ug_free (Int_Param_Vector);

    Int_Param_Vector = NULL;
  }

  if (Error_Flag != 0 || (Param_Type_Flag != -DOUBLE_PARAM_VECTOR_TYPE_FLAG && Param_Type_Flag != -INT_PARAM_VECTOR_TYPE_FLAG))
  {
    Entries = 0;
    Param_Type_Flag = 0;
  }

  if (Error_Flag == 0 && Next_Param_Flag == 1)
    Error_Flag = -6;

  return (Error_Flag);

}

INT_ ug_get_param_index
 (char Param_Name[],
  INT_ *Param_Type_Flag,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Get UG parameter index and type from name and UG parameter structure.
 */

  INT_ Index;

  Index = 0;

  *Param_Type_Flag = 0;

  if (! *Param_Type_Flag)
  {
    Index = 0;

    do
    {
      if (ug_get_param_name_from_alt_names (UG_Param_Struct_Ptr->Char_Param_Name[Index], UG_Param_Struct_Ptr->Char_Param_Alt_Name[Index], Param_Name))
        *Param_Type_Flag = CHAR_PARAM_TYPE_FLAG;

      ++Index;
    }
    while (Index < UG_Param_Struct_Ptr->Number_of_Char_Params && ! *Param_Type_Flag);

    --Index;

    if (*Param_Type_Flag && UG_Param_Struct_Ptr->Char_Param_Type[Index] == 2)
      *Param_Type_Flag = OPT_PARAM_TYPE_FLAG;
    else if (*Param_Type_Flag && UG_Param_Struct_Ptr->Char_Param_Type[Index] == 3)
      *Param_Type_Flag = OPT_VALUE_PARAM_TYPE_FLAG;
  }

  if (! *Param_Type_Flag)
  {
    Index = 0;

    do
    {
      if (ug_get_param_name_from_alt_names (UG_Param_Struct_Ptr->Double_Param_Name[Index], UG_Param_Struct_Ptr->Double_Param_Alt_Name[Index], Param_Name))
        *Param_Type_Flag = DOUBLE_PARAM_TYPE_FLAG;

      ++Index;
    }
    while (Index < UG_Param_Struct_Ptr->Number_of_Double_Params && ! *Param_Type_Flag);

    --Index;

    if (*Param_Type_Flag && UG_Param_Struct_Ptr->Double_Param_Vector_Entries[Index] >= 0)
      *Param_Type_Flag = DOUBLE_PARAM_VECTOR_TYPE_FLAG;
  }

  if (! *Param_Type_Flag)
  {
    Index = 0;

    do
    {
      if (ug_get_param_name_from_alt_names (UG_Param_Struct_Ptr->Int_Param_Name[Index], UG_Param_Struct_Ptr->Int_Param_Alt_Name[Index], Param_Name))
        *Param_Type_Flag = INT_PARAM_TYPE_FLAG;

      ++Index;
    }
    while (Index < UG_Param_Struct_Ptr->Number_of_Int_Params && ! *Param_Type_Flag);

    --Index;

    if (*Param_Type_Flag && UG_Param_Struct_Ptr->Int_Param_Vector_Entries[Index] >= 0)
      *Param_Type_Flag = INT_PARAM_VECTOR_TYPE_FLAG;
  }

  return (Index);

}
