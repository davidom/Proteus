#include "UG_LIB.h"

void ug_cpu_message
 (char * Label)
{

/*
 * Write CPU time since last call to standard output.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_cpu_message.c,v 1.14 2013/03/16 19:11:06 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_61 Text;

  static float old_CPU_Time = -1.0;

  float CPU_Time, fc0, new_CPU_Time, sys_clocks_per_sec;

  fc0 = 0.0;

  if (strcmp (Label, "reset") == 0)
  {
    old_CPU_Time = -1.0;

    return;
  }

  sys_clocks_per_sec = (float) CLOCKS_PER_SEC;

  new_CPU_Time = (float) clock () / sys_clocks_per_sec;

  if (old_CPU_Time < fc0)
    old_CPU_Time = new_CPU_Time;

  CPU_Time = new_CPU_Time - old_CPU_Time;
  CPU_Time = MAX (CPU_Time, fc0);

  old_CPU_Time = new_CPU_Time;

  if (strcmp (Label, "") == 0)
    return;

  if (CPU_Time < 60)
    sprintf (Text, "%s CPU Time          =%10.3f seconds", Label, CPU_Time);
  else if (CPU_Time < 3600)
    sprintf (Text, "%s CPU Time          =%10.3f minutes", Label, CPU_Time/60);
  else
    sprintf (Text, "%s CPU Time          =%10.3f hours", Label, CPU_Time/3600);

  ug_message (Text);

  return;

}
