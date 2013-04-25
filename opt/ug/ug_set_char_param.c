#include "UG_LIB.h"

INT_ ug_set_char_param
 (char _Char_Param_Name[],
  char _Char_Param_Value[],
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Set the value of a UG type char parameter.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_set_char_param.c,v 1.13 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX Current_Char_Param_Value;

  INT_ Index;
  INT_ Message_Flag=0;

  if (UG_Param_Struct_Ptr == NULL)
    return (-2);

  Index = ug_get_char_param_ (_Char_Param_Name, Current_Char_Param_Value, UG_Param_Struct_Ptr);

  if (Index < 0)
    return (-1);

  ug_set_char_param_ (Index, Message_Flag, _Char_Param_Value, UG_Param_Struct_Ptr);

  return (1);
}

void ug_set_char_param_
 (INT_ Index,
  INT_ Message_Flag,
  char _Char_Param_Value[],
  UG_Param_Struct * UG_Param_Struct_Ptr)

{
  CHAR_41 Char_Param_Text;
  CHAR_133 Text;

  if (UG_Param_Struct_Ptr == NULL)
    return;

  if (strcmp (_Char_Param_Value, UG_Param_Struct_Ptr->Char_Param[Index]))
  {
    strcpy (UG_Param_Struct_Ptr->Char_Param[Index], _Char_Param_Value);

    if (Message_Flag)
    {
      strcpy (Char_Param_Text, "");
      strncat (Char_Param_Text, UG_Param_Struct_Ptr->Char_Param[Index], 39);

      sprintf (Text, "UG PARAM : %-24s    = %-39s", UG_Param_Struct_Ptr->Char_Param_Name[Index], Char_Param_Text);
      ug_message (Text);
    }
  }

  return;
}
