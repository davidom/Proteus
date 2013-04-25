#include "UG_LIB.h"

/*
 * Global variables.
 *
 */

  static INT_ Host_Byte_Order = 0;
  static INT_ File_Format_Byte_Order = 0;


/*
 * Routines to handle file format and host byte ordering.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_fio_byte_order.c,v 1.12 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1999-2012, David L. Marcum
 */

INT_ ug_get_byte_order ()

{
 
/*
 * Set host byte ordering. Return 1 if big endian byte ordering or -1 if little
 * endian byte ordering.
 * 
 */

  FILE *Test_File;

  char *Test_Byte;

  INT_ Test_Value = 0;
  INT_ True_Value = 15;

  if (Host_Byte_Order == 0)
  {
    Test_Byte = (char *) &Test_Value;

    Test_File = ug_tmpfile ();

    if (Test_File)
    {
      fwrite (&True_Value, sizeof (INT_), 1, Test_File);

      rewind (Test_File);

      fread (Test_Byte, 1, 1, Test_File);

      fclose (Test_File);
    }

    if (Test_Value == True_Value)
      Host_Byte_Order = 1234;
    else
      Host_Byte_Order = 4321;
  }

  if (Host_Byte_Order == 1234)
    return (-1);
  else
    return (1);

}

void ug_reverse_byte_order
 (void * Data,
  size_t Size,
  INT_ Number)

{
 
/*
 * Reverse byte order if file format and host byte ordering differ.
 * 
 */

  char *Data_Byte_Ptr;
  char Temp_Data_Byte;

  INT_ Byte_Index, Index, Number_of_Bytes, Reverse_Byte_Index;

  if (Host_Byte_Order != File_Format_Byte_Order)
  {
    Number_of_Bytes = (INT_) Size;

    Data_Byte_Ptr = (char *) Data;

    for (Index = 0; Index < Number; ++Index)
    {
      Reverse_Byte_Index = Number_of_Bytes;

      for (Byte_Index = 0; Byte_Index < Number_of_Bytes/2; ++Byte_Index)
      {
        --Reverse_Byte_Index;

        Temp_Data_Byte = Data_Byte_Ptr[Byte_Index];

        Data_Byte_Ptr[Byte_Index] = Data_Byte_Ptr[Reverse_Byte_Index];

        Data_Byte_Ptr[Reverse_Byte_Index] = Temp_Data_Byte;
      }

      Data_Byte_Ptr += Number_of_Bytes;
    }
  }

  return;
}

void ug_set_byte_order
 (INT_ File_Format)

{
 
/*
 * Set file format and host to big or little endian byte ordering.
 * 
 */

  INT_ Byte_Order;

  if (File_Format < 0)
    File_Format_Byte_Order = 1234;

  else if (File_Format > 0)
    File_Format_Byte_Order = 4321;

  Byte_Order = ug_get_byte_order ();

  return;

}
