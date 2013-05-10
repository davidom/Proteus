#include "UG_LIB.h"

INT_ ug_set_int_param
 (char _Int_Param_Name[],
  INT_ _Int_Param_Value,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{

/*
 * Set the value of a UG type int parameter.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_set_int_param.c,v 1.18 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  char *Param_Name_Suffix = NULL;

  INT_ Index;
  INT_ Message_Flag=0;

  INT_ Current_Int_Param_Value;

  if (UG_Param_Struct_Ptr == NULL)
    return (-2);

  Index = ug_get_int_param_ (_Int_Param_Name, &Current_Int_Param_Value, UG_Param_Struct_Ptr);

  if (Index < 0)
    return (-1);

  Param_Name_Suffix = strstr (_Int_Param_Name, "@");

  if (Param_Name_Suffix)
  {
    if (strcmp (Param_Name_Suffix, "@def") == 0)
      _Int_Param_Value = UG_Param_Struct_Ptr->Def_Int_Param[Index];
    else if (strcmp (Param_Name_Suffix, "@max") == 0)
      _Int_Param_Value = UG_Param_Struct_Ptr->Max_Int_Param[Index];
    else if (strcmp (Param_Name_Suffix, "@min") == 0)
      _Int_Param_Value = UG_Param_Struct_Ptr->Min_Int_Param[Index];
  }

  ug_set_int_param_ (Index, Message_Flag, _Int_Param_Value, UG_Param_Struct_Ptr);

  return (1);

}

void ug_set_int_param_
 (INT_ Index,
  INT_ Message_Flag,
  INT_ _Int_Param_Value,
  UG_Param_Struct * UG_Param_Struct_Ptr)

{
  CHAR_133 Text;

  if (UG_Param_Struct_Ptr == NULL)
    return;

  if (_Int_Param_Value != UG_Param_Struct_Ptr->Int_Param[Index])
  {
    if (UG_Param_Struct_Ptr->Max_Int_Param[Index] > UG_Param_Struct_Ptr->Min_Int_Param[Index])
      UG_Param_Struct_Ptr->Int_Param[Index] = MAX (MIN (_Int_Param_Value, UG_Param_Struct_Ptr->Max_Int_Param[Index]),
                                                                                UG_Param_Struct_Ptr->Min_Int_Param[Index]);
    else
      UG_Param_Struct_Ptr->Int_Param[Index] = _Int_Param_Value;

    if (UG_Param_Struct_Ptr->Max_Int_Param[Index] > UG_Param_Struct_Ptr->Min_Int_Param[Index])
      UG_Param_Struct_Ptr->Int_Param[Index] = MAX (MIN (UG_Param_Struct_Ptr->Int_Param[Index],
                                                           UG_Param_Struct_Ptr->Max_Int_Param[Index]),
                                                           UG_Param_Struct_Ptr->Min_Int_Param[Index]);

    if (Message_Flag)
    {
      sprintf (Text, "UG PARAM : %-24s    = %-10i", UG_Param_Struct_Ptr->Int_Param_Name[Index], (int) UG_Param_Struct_Ptr->Int_Param[Index]);
      ug_message (Text);
    }
  }

  return;
}

