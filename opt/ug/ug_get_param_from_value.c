#include "UG_LIB.h"
INT_ ug_get_int_range_from_string
 (char Sub_String[],
  INT_ *Int_Param_Value_1,
  INT_ *Int_Param_Value_2);

INT_ ug_get_param_from_value
 (char Char_Param_Value[],
  char Value[],
  INT_ *Entries,
  INT_ *Int_Param_Value,
  INT_ *Param_Type_Flag,
  INT_1D **_Int_Param_Vector,
  double *Double_Param_Value,
  DOUBLE_1D **_Double_Param_Vector)

{

/*
 * Get a UG parameter from a given value sub-string.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_param_from_value.c,v 1.15 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  char *String_ptr;

  CHAR_UG_MAX String, Sub_String;

  static INT_ Vector_Index = 0;

  INT_ Entry_Type, Error_Flag,
       Int_Param_Value_, Int_Param_Value_1, Int_Param_Value_2,
       Length, Max_Entries, Pass, Read_Flag;

  double Double_Param_Value_;

  // set character parameter

  if (*Param_Type_Flag == CHAR_PARAM_TYPE_FLAG || *Param_Type_Flag == OPT_PARAM_TYPE_FLAG || *Param_Type_Flag == OPT_VALUE_PARAM_TYPE_FLAG)
    strcpy (Char_Param_Value, Value);

  // set double parameter

  else if (*Param_Type_Flag == DOUBLE_PARAM_TYPE_FLAG)
  {
    Read_Flag = sscanf (Value, "%lf", Double_Param_Value);

    if (Read_Flag != 1)
      return (410);
  }

  // set int parameter

  else if (*Param_Type_Flag == INT_PARAM_TYPE_FLAG)
  {
    Read_Flag = sscanf (Value, "%i", (int *) Int_Param_Value);

    if (Read_Flag != 1)
      return (410);
  }

  // set double or int vector parameter of type-1 name entry1,entry2,...
  // or
  // set number of double or int vector parameters of type-2
  // name number_of_entries
  //      entry1
  //      entry2
  //      .
  //      .
  //      .

  else if (*Param_Type_Flag == DOUBLE_PARAM_VECTOR_TYPE_FLAG || *Param_Type_Flag == INT_PARAM_VECTOR_TYPE_FLAG)
  {
    Entry_Type = (strstr (Value, ",") || (*Param_Type_Flag == INT_PARAM_VECTOR_TYPE_FLAG && strstr (Value, "-"))) ? 1: 2;

    Pass = 1;

    do
    {
      if (Entry_Type == 1)
      {
        strcpy (String, Value);

        Vector_Index = 0;

        do
        {
          Length = (INT_) strcspn (String, ",");

          if (Length)
          {
            strcpy (Sub_String, "");

            strncat (Sub_String, String, Length);

            if (*Param_Type_Flag == DOUBLE_PARAM_VECTOR_TYPE_FLAG)
            {
              Read_Flag = sscanf (Sub_String, "%lg", &Double_Param_Value_);

              if (Read_Flag != 1)
                return (410);

              if (Pass == 2)
                (*_Double_Param_Vector)[Vector_Index] = Double_Param_Value_;

              ++Vector_Index;
            }
            else
            {
              Error_Flag = ug_get_int_range_from_string (Sub_String, &Int_Param_Value_1, &Int_Param_Value_2);

              if (Error_Flag)
                return (Error_Flag);

              for (Int_Param_Value_ = Int_Param_Value_1; Int_Param_Value_ <= Int_Param_Value_2; ++Int_Param_Value_)
              {
                if (Pass == 2)
                  (*_Int_Param_Vector)[Vector_Index] = Int_Param_Value_;

                ++Vector_Index;
              }
            }

            String_ptr = strstr (String, ",");

            if (String_ptr)
            {
              if (strlen (String_ptr) > 1)
              {
                String_ptr = String_ptr+1;

                strcpy (String, String_ptr);
              }
              else
                String_ptr = NULL;
            }
          }
          else
            String_ptr = NULL;
        }
        while (String_ptr);

        *Entries = Vector_Index;

        Vector_Index = 0;
      }
      else
      {
        *Entries = 0;

        Read_Flag = sscanf (Value, "%i", (int *) Entries);

        if (*Entries < 0 || Read_Flag != 1)
          return (410);
      }

      if (*Entries == 0)
        return (-4);

      if (Pass == 1)
      {
        Error_Flag = 0;

        if (*Param_Type_Flag == DOUBLE_PARAM_VECTOR_TYPE_FLAG)
          *_Double_Param_Vector = (DOUBLE_1D *) ug_malloc (&Error_Flag, (*Entries) * sizeof (DOUBLE_1D));
        else
          *_Int_Param_Vector = (INT_1D *) ug_malloc (&Error_Flag, (*Entries) * sizeof (INT_1D));

        if (Error_Flag > 0)
          return (100409);

        if (Entry_Type == 2)
        {
          *Param_Type_Flag = -(*Param_Type_Flag);

          Vector_Index = 0;

          ++Pass;
        }
      }

      ++Pass;
    }
    while (*Entries && Pass <= 2);
  }

  // continue setting double vector parameter of type-2
  // name number_of_entries
  //      entry1
  //      entry2
  //      .
  //      .
  //      .

  else if (*Param_Type_Flag == -DOUBLE_PARAM_VECTOR_TYPE_FLAG)
  {
    Read_Flag = sscanf (Value, "%lg", &Double_Param_Value_);

    if (Read_Flag != 1)
    {
      ug_free (*_Double_Param_Vector);

      *_Double_Param_Vector = NULL;

      *Param_Type_Flag = abs (*Param_Type_Flag);

      return (410);
    }

    (*_Double_Param_Vector)[Vector_Index] = Double_Param_Value_;

    ++Vector_Index;

    if (Vector_Index == *Entries)
      *Param_Type_Flag = abs (*Param_Type_Flag);
  }

  // continue setting int vector parameter of type
  // name number_of_entries
  //      entry1
  //      entry2
  //      .
  //      .
  //      .

  else if (*Param_Type_Flag == -INT_PARAM_VECTOR_TYPE_FLAG)
  {
    Error_Flag = ug_get_int_range_from_string (Value, &Int_Param_Value_1, &Int_Param_Value_2);

    if (Error_Flag)
    {
      ug_free (*_Int_Param_Vector);

      *_Int_Param_Vector = NULL;

      *Param_Type_Flag = abs (*Param_Type_Flag);

      return (Error_Flag);
    }

    Max_Entries = Vector_Index + Int_Param_Value_2 - Int_Param_Value_1 + 1;

    if (Max_Entries > *Entries)
    {
      ug_free (*_Int_Param_Vector);

      *_Int_Param_Vector = NULL;

      *Param_Type_Flag = -(*Param_Type_Flag);

      return (410);
    }

    for (Int_Param_Value_ = Int_Param_Value_1; Int_Param_Value_ <= Int_Param_Value_2; ++Int_Param_Value_)
    {
      (*_Int_Param_Vector)[Vector_Index] = Int_Param_Value_;

      ++Vector_Index;
    }

    if (Vector_Index == *Entries)
      *Param_Type_Flag = abs (*Param_Type_Flag);
  }
  
  // parameter type is unkown

  else
    return (-2);

  return (0);

}

INT_ ug_get_int_range_from_string
 (char Sub_String[],
  INT_ *Int_Param_Value_1,
  INT_ *Int_Param_Value_2)

{

  CHAR_UG_MAX Range_String;

  char *String_ptr;

  INT_ Length, Read_Flag;

  if (strstr (Sub_String, "-"))
  {
    Length = (INT_) strcspn (Sub_String, "-");

    String_ptr = Sub_String;

    if (Length == 0)
      Length = 1 + (INT_) strcspn (String_ptr+1, "-");

    strcpy (Range_String, "");

    strncat (Range_String, String_ptr, Length);

    Read_Flag = sscanf (Range_String, "%i", (int *) Int_Param_Value_1);

    if (Read_Flag != 1)
      return (410);

    String_ptr = strstr (String_ptr+1, "-");

    if (String_ptr)
    {
      ++String_ptr;

      Read_Flag = sscanf (String_ptr, "%i", (int *) Int_Param_Value_2);

      if (Read_Flag != 1)
        return (410);
    }
    else
      *Int_Param_Value_2 = *Int_Param_Value_1;
  }
  else
  {
    Read_Flag = sscanf (Sub_String, "%i", (int *) Int_Param_Value_1);

    if (Read_Flag != 1)
      return (410);

    *Int_Param_Value_2 = *Int_Param_Value_1;
  }

  return (0);

}
