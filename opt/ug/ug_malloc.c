#include "UG_LIB.h"

/*
 * Memory allocation routines.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_malloc.c,v 1.38 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum 
 */

static int Memory_Monitor = 0;

static CHAR_133 Text;

static LONG_LONG_int *Pointer_Location = NULL;
static LONG_LONG_int *Pointer_Memory = NULL;

static double TM = 0.0;
static double MB = 1000000.0;
static double KB = 1000.0;

static LONG_LONG_int Total_Memory = 0;

static int Found = 0;
static int Inc_Number_of_Pointers = 100;
static int Max_Number_of_Pointers = 0;
static int Number_of_Pointers = 0;
static int Pointer_Index = 0;
static int Total_MB = 0;

void ug_memory_monitor
 (char *Flag)
 
{

/*
 * Turn memory monitor on or off.
 */

  if (strcmp (Flag, "on") == 0)
  {
    if (Memory_Monitor == 1) return;

    Memory_Monitor = 1;
  }
  else
    Memory_Monitor = 0;

  Max_Number_of_Pointers = 0;

  Number_of_Pointers = 0;

  Total_Memory = 0;

  if (Pointer_Location)
    free (Pointer_Location);

  if (Pointer_Memory)
    free (Pointer_Memory);

  Pointer_Location = NULL;

  Pointer_Memory = NULL;

  return;

}

void ug_free
 (void * Pointer)

{

/*
 * Free array space previously allocated using ug_malloc or ug_realloc.
 */

  if (Memory_Monitor)
  {
    if (Pointer)
    {
      Found = 0;

      if (Number_of_Pointers > 0)
      {
        Pointer_Index = 0;

        do
        {
          if (Pointer_Location[Pointer_Index] == (LONG_int) Pointer) Found = 1;

          ++Pointer_Index;
        }
        while (Pointer_Index < Number_of_Pointers && ! Found);

        --Pointer_Index;
      }

      if (! Found)
      {
        TM = (double) (Total_Memory) / MB;

        Total_MB = NINT (TM);

        if (Total_MB >= 10)
          sprintf (Text, "                                        unknown memory free %12i MBytes", Total_MB);

        else
        {
          TM = (double) (Total_Memory) / KB;

          Total_MB = NINT (TM);

          if (Total_MB >= 10)
            sprintf (Text, "                                        unknown memory free %12i KBytes", Total_MB);

          else
          {
            Total_MB = (int) (Total_Memory);

            sprintf (Text, "                                        unknown memory free %12i  Bytes", Total_MB);
          }
        }
      }

      else
      {
        Total_Memory = Total_Memory - Pointer_Memory[Pointer_Index];

        Pointer_Location[Pointer_Index] = 0;

        Pointer_Memory[Pointer_Index] = 0;

        TM = (double) (Total_Memory) / MB;

        Total_MB = NINT (TM);

        if (Total_MB >= 10)
          sprintf (Text, "                                                            %12i MBytes", Total_MB);

        else
        {
          TM = (double) (Total_Memory) / KB;

          Total_MB = NINT (TM);

          if (Total_MB >= 10)
            sprintf (Text, "                                                            %12i KBytes", Total_MB);

          else
          {
            Total_MB = (int) (Total_Memory);

            sprintf (Text, "                                                            %12i  Bytes", Total_MB);
          }
        }
      }

      ug_message (Text);
    }
  }

  if (Pointer)
    free (Pointer);

  return;
}

void *ug_calloc
 (INT_ *Error_Flag,
  size_t Number_of_Elements,
  size_t Size_of_Each)

{
  
/*
 * Allocate array space and set each element to zero.
 */

  size_t Size;

  void *Pointer;

  Size = Size_of_Each * Number_of_Elements;

  Pointer = NULL;

  if (Size > 0)
    Pointer = calloc (Number_of_Elements, Size_of_Each);

  if (Size != 0 && Pointer == NULL)
    ++(*Error_Flag);

  if (Memory_Monitor)
  {
    if (Pointer)
    {
      Found = 0;

      if (Number_of_Pointers > 0)
      {
        Pointer_Index = 0;

        do
        {
          if (Pointer_Memory[Pointer_Index] == 0) Found = 1;

          ++Pointer_Index;
        }
        while (Pointer_Index < Number_of_Pointers && ! Found);

        --Pointer_Index;
      }

      if (! Found)
      {
        ++Number_of_Pointers;

        Pointer_Index = Number_of_Pointers - 1;

        if (Number_of_Pointers > Max_Number_of_Pointers)
        {
          Max_Number_of_Pointers = Max_Number_of_Pointers
                                 + Inc_Number_of_Pointers;

          if (Pointer_Location == NULL)
            Pointer_Location = (LONG_LONG_int *) malloc (sizeof (LONG_LONG_int)
                                                * Max_Number_of_Pointers);
          else
            Pointer_Location = (LONG_LONG_int *) realloc (Pointer_Location,
                                                  sizeof (LONG_LONG_int)
                                                * Max_Number_of_Pointers);

          if (Pointer_Memory == NULL)
            Pointer_Memory = (LONG_LONG_int *) malloc (sizeof (LONG_LONG_int)
                                              * Max_Number_of_Pointers);
          else
            Pointer_Memory = (LONG_LONG_int *) realloc (Pointer_Memory,
                                                sizeof (LONG_LONG_int)
                                              * Max_Number_of_Pointers);

          if (Pointer_Location == NULL || Pointer_Memory == NULL)
          {
            ug_memory_monitor ("off");

            ug_message ("                                                            MemoryMonitor ERROR");
          }
        }
      }

      if (Memory_Monitor)
      {
        Total_Memory = Total_Memory + (LONG_LONG_int) Size;

        Pointer_Location[Pointer_Index] = (LONG_int) Pointer;

        Pointer_Memory[Pointer_Index] = (LONG_LONG_int) Size;

        Total_MB = NINT (TM);

        if (Total_MB >= 10)
          sprintf (Text, "                                                            %12i MBytes", Total_MB);

        else
        {
          TM = (double) (Total_Memory) / KB;

          Total_MB = NINT (TM);

          if (Total_MB >= 10)
            sprintf (Text, "                                                            %12i KBytes", Total_MB);

          else
          {
            Total_MB = (int) (Total_Memory);

            sprintf (Text, "                                                            %12i  Bytes", Total_MB);
          }
        }

        ug_message (Text);
      }
    }
  }

  return (Pointer);

}

void *ug_malloc
 (INT_ *Error_Flag,
  size_t Size)

{
  
/*
 * Allocate array space.
 */

  void *Pointer;

  Pointer = NULL;

  if (Size > 0)
    Pointer = malloc (Size);

  if (Size != 0 && Pointer == NULL)
    ++(*Error_Flag);

  if (Memory_Monitor)
  {
    if (Pointer)
    {
      Found = 0;

      if (Number_of_Pointers > 0)
      {
        Pointer_Index = 0;

        do
        {
          if (Pointer_Memory[Pointer_Index] == 0) Found = 1;

          ++Pointer_Index;
        }
        while (Pointer_Index < Number_of_Pointers && ! Found);

        --Pointer_Index;
      }

      if (! Found)
      {
        ++Number_of_Pointers;

        Pointer_Index = Number_of_Pointers - 1;

        if (Number_of_Pointers > Max_Number_of_Pointers)
        {
          Max_Number_of_Pointers = Max_Number_of_Pointers
                                 + Inc_Number_of_Pointers;

          if (Pointer_Location == NULL)
            Pointer_Location = (LONG_LONG_int *) malloc (sizeof (LONG_LONG_int)
                                                * Max_Number_of_Pointers);
          else
            Pointer_Location = (LONG_LONG_int *) realloc (Pointer_Location,
                                                  sizeof (LONG_LONG_int)
                                                * Max_Number_of_Pointers);

          if (Pointer_Memory == NULL)
            Pointer_Memory = (LONG_LONG_int *) malloc (sizeof (LONG_LONG_int)
                                              * Max_Number_of_Pointers);
          else
            Pointer_Memory = (LONG_LONG_int *) realloc (Pointer_Memory,
                                                sizeof (LONG_LONG_int)
                                              * Max_Number_of_Pointers);

          if (Pointer_Location == NULL || Pointer_Memory == NULL)
          {
            ug_memory_monitor ("off");

            ug_message ("                                                            MemoryMonitor ERROR");
          }
        }
      }

      if (Memory_Monitor)
      {
        Total_Memory = Total_Memory + (LONG_LONG_int) Size;

        Pointer_Location[Pointer_Index] = (LONG_int) Pointer;

        Pointer_Memory[Pointer_Index] = (LONG_LONG_int) Size;

        Total_MB = NINT (TM);

        if (Total_MB >= 10)
          sprintf (Text, "                                                            %12i MBytes", Total_MB);

        else
        {
          TM = (double) (Total_Memory) / KB;

          Total_MB = NINT (TM);

          if (Total_MB >= 10)
            sprintf (Text, "                                                            %12i KBytes", Total_MB);

          else
          {
            Total_MB = (int) (Total_Memory);

            sprintf (Text, "                                                            %12i  Bytes", Total_MB);
          }
        }

        ug_message (Text);
      }
    }
  }

  return (Pointer);

}

void *ug_realloc
 (INT_ * Error_Flag,
  void * Pointer,
  size_t Size)
  
{

/*
 * Reallocate array space previously allocated using ug_malloc or ug_realloc.
 */

  void *New_Pointer;

  New_Pointer = NULL;

  if (Memory_Monitor)
  {
    if (Pointer)
    {
      Found = 0;

      if (Number_of_Pointers > 0)
      {
        Pointer_Index = 0;

        do
        {
          if (Pointer_Location[Pointer_Index] == (LONG_int) Pointer) Found = 1;

          ++Pointer_Index;
        }
        while (Pointer_Index < Number_of_Pointers && ! Found);

        --Pointer_Index;
      }
    }
  }

  if (Size > 0)
  {
    if (Pointer)
      New_Pointer = realloc (Pointer, Size);
    else
      New_Pointer = malloc (Size);
  }

  if (Size != 0 && New_Pointer == NULL)
  {
    ++(*Error_Flag);

    if (Pointer)
      New_Pointer = Pointer;
  }

  if (Memory_Monitor)
  {
    if (New_Pointer)
    {
      if (Pointer)
      {
        if (! Found)
        {
          Total_MB = NINT (TM);

          if (Total_MB >= 10)
            sprintf (Text, "                                     unknown memory realloc %12i MBytes", Total_MB);

          else
          {
            TM = (double) (Total_Memory) / KB;

            Total_MB = NINT (TM);

            if (Total_MB >= 10)
              sprintf (Text, "                                     unknown memory realloc %12i KBytes", Total_MB);

            else
            {
              Total_MB = (int) (Total_Memory);

              sprintf (Text, "                                     unknown memory realloc %12i  Bytes", Total_MB);
            }
          }
        }

        else
        {
          Total_Memory = Total_Memory + (LONG_LONG_int) Size
                       - Pointer_Memory[Pointer_Index];

          Pointer_Location[Pointer_Index] = (LONG_int) New_Pointer;

          Pointer_Memory[Pointer_Index] = (LONG_LONG_int) Size;

          Total_MB = NINT (TM);

          if (Total_MB >= 10)
            sprintf (Text, "                                                            %12i MBytes", Total_MB);

          else
          {
            TM = (double) (Total_Memory) / KB;

            Total_MB = NINT (TM);

            if (Total_MB >= 10)
              sprintf (Text, "                                                            %12i KBytes", Total_MB);

            else
            {
              Total_MB = (int) (Total_Memory);

              sprintf (Text, "                                                            %12i  Bytes", Total_MB);
            }
          }
        }

        ug_message (Text);
      }
      else
      {
        Found = 0;

        if (Number_of_Pointers > 0)
        {
          Pointer_Index = 0;

          do
          {
            if (Pointer_Memory[Pointer_Index] == 0) Found = 1;

            ++Pointer_Index;
          }
          while (Pointer_Index < Number_of_Pointers && ! Found);

          --Pointer_Index;
        }

        if (! Found)
        {
          ++Number_of_Pointers;

          Pointer_Index = Number_of_Pointers - 1;

          if (Number_of_Pointers > Max_Number_of_Pointers)
          {
            Max_Number_of_Pointers = Max_Number_of_Pointers
                                   + Inc_Number_of_Pointers;

            if (Pointer_Location == NULL)
              Pointer_Location = (LONG_LONG_int *) malloc (sizeof (LONG_LONG_int)
                                                  * Max_Number_of_Pointers);
            else
              Pointer_Location = (LONG_LONG_int *) realloc (Pointer_Location,
                                                    sizeof (LONG_LONG_int)
                                                 * Max_Number_of_Pointers);

            if (Pointer_Memory == NULL)
              Pointer_Memory = (LONG_LONG_int *) malloc (sizeof (LONG_LONG_int)
                                                * Max_Number_of_Pointers);
            else
              Pointer_Memory = (LONG_LONG_int *) realloc (Pointer_Memory,
                                                  sizeof (LONG_LONG_int)
                                                * Max_Number_of_Pointers);

            if (Pointer_Location == NULL || Pointer_Memory == NULL)
            {
              ug_memory_monitor ("off");

              ug_message ("                                                            MemoryMonitor ERROR");
            }
          }
        }

        if (Memory_Monitor)
        {
          Total_Memory = Total_Memory + (LONG_LONG_int) Size;

          Pointer_Location[Pointer_Index] = (LONG_int) New_Pointer;

          Pointer_Memory[Pointer_Index] = (LONG_LONG_int) Size;

          Total_MB = NINT (TM);

          if (Total_MB >= 10)
            sprintf (Text, "                                                            %12i MBytes", Total_MB);

          else
          {
            TM = (double) (Total_Memory) / KB;

            Total_MB = NINT (TM);

            if (Total_MB >= 10)
              sprintf (Text, "                                                            %12i KBytes", Total_MB);

            else
            {
              Total_MB = (int) (Total_Memory);

              sprintf (Text, "                                                            %12i  Bytes", Total_MB);
            }
          }

          ug_message (Text);
        }
      }
    }
  }

  return (New_Pointer);

}
