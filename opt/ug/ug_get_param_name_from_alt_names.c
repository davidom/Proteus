#include "UG_LIB.h"

INT_ ug_get_param_name_from_alt_names
 (char Name[],
  char Alt_Name_List[],
  char Param_Name[])

{

/*
 * Get corresponding UG parameter name from a list of alternative names.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_param_name_from_alt_names.c,v 1.10 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  char *Name_String_ptr;

  CHAR_25 Param_Name_Prefix;

  CHAR_UG_MAX Alt_Name, Name_String, TMP_Name;

  INT_ Found_Flag, Length;

  Found_Flag = 0;

  strcpy (Alt_Name, "-");
  strcat (Alt_Name, Name);

  if (strcmp (Name, Param_Name) == 0)
    Found_Flag = 1;

  else if (strcmp (Alt_Name, Param_Name) == 0)
    Found_Flag = 1;

  else if (strstr (Param_Name, "@"))
  {
    Length = strcspn (Param_Name, "@");

    if (Length > 24 || strlen (Param_Name) > 28)
      return (-1);

    strcpy (Param_Name_Prefix, "");
    strncat (Param_Name_Prefix, Param_Name, Length);

    if (strcmp (Name, Param_Name_Prefix) == 0)
      Found_Flag = 1;
  }

  else if (strstr (Alt_Name_List, ","))
  {
    strcpy (Name_String, Alt_Name_List);

    do
    {
      Length = (INT_) strcspn (Name_String, ",");

      strcpy (Alt_Name, "");

      strncat (Alt_Name, Name_String, Length);

      if (strcmp (Alt_Name, Param_Name) == 0)
        Found_Flag = 1; 

      else
      {
        strcpy (TMP_Name, Alt_Name);
        strcpy (Alt_Name, "-");
        strcat (Alt_Name, TMP_Name);

        if (strcmp (Alt_Name, Param_Name) == 0)
          Found_Flag = 1; 
      }

      if (! Found_Flag)
      {
        Name_String_ptr = strstr (Name_String, ",");

        if (Name_String_ptr)
        {
          ++Name_String_ptr;

          strcpy (Name_String, Name_String_ptr);
        }
      }

      else
        Name_String_ptr = NULL;
    }
    while (Name_String_ptr && ! Found_Flag);
  }

  return (Found_Flag);

}
