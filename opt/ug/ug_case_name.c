#include "UG_LIB.h"

static CHAR_UG_MAX Saved_Case_Name;

static INT_ Case_Name_Flag = 1;

void ug_case_name
 (char Case_Name[])

{

/*
 * Set or get case name.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_case_name.c,v 1.10 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  if (Case_Name_Flag == 2)
    strcpy (Case_Name, Saved_Case_Name);

  else
  {
    Case_Name_Flag = 2;

    strcpy (Saved_Case_Name, "DEBUG");

    if (Case_Name != NULL)
    {
      if (strcmp (Case_Name, "") != 0)
        strcpy (Saved_Case_Name, Case_Name);
    }
  }

  return;

}

void ug_set_case_name
 (char Case_Name[])

{

/*
 * Set case name.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_case_name.c,v 1.10 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  Case_Name_Flag = 2;

  strcpy (Saved_Case_Name, Case_Name);

  return;

}
