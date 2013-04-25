#include "UG_LIB.h"

INT_ ug_get_param_name_from_string
 (char Param_String[],
  char Name[],
  char Value[])

{

/*
 * Get UG parameter name from a given string.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_param_name_from_string.c,v 1.14 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  char *String_ptr;

  INT_ Length, Read_Flag;

  // initialize parameter strings

  strcpy (Name, "");
  strcpy (Value, "");

  // check if parameter string is a comment line

  if (strstr (Param_String, "#"))
    return (-1);

  // set name and value for parameter string type "name=value"

  if (strstr (Param_String, "="))
  {
    Length = (INT_) strcspn (Param_String, "=");

    if (Length)
    {
      strcpy (Name, "");
      strncat (Name, Param_String, Length);

      String_ptr = strstr (Param_String, "=");

      Length = (INT_) strlen (String_ptr);

      if (Length > 1)
      {
        ++String_ptr;

        strcpy (Value, "");
        strcat (Value, String_ptr);
      }
      else
        return (-5); // parameter string has no value 
    }
    else
      return (-2); // name is not set - parameter string contains only "=" or "=value"
  }

  // set name and value for parameter string type "name value"

  else
  {
    Read_Flag = sscanf (Param_String, "%s %s", Name, Value);

    if (Read_Flag <= 0)
      return (-1); // parameter string is a blank line

    else if (Read_Flag > 2)
      return (-2); // unknown error

    else if (Read_Flag == 1)
      return (-4); // parameter string has no value 
  }

  return (0);

}
