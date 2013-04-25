#include "UG_LIB.h"

void ug_date_time (char Current_Date[],
                   char Current_Time[])

/*
 * Get current date and time.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_date_time.c,v 1.3 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

{
  struct tm * time_struct;

  time_t current_time;

  int current_day, current_hour, current_month, current_min, current_sec,
      current_year;

  int offset_hour = 0;
  int offset_min = 0;
  int offset_sec = 0;
  int offset_day = 0;
  int offset_month = 1;
  int offset_year = 1900;

  current_time = time (0);

  time_struct = localtime (&current_time);

  current_hour = offset_hour + time_struct->tm_hour;
  current_min = offset_min + time_struct->tm_min;
  current_sec = offset_sec + time_struct->tm_sec;
  current_day = offset_day + time_struct->tm_mday;
  current_month = offset_month + time_struct->tm_mon;
  current_year = offset_year + time_struct->tm_year;

  if      (current_month == 1)
    sprintf (Current_Date, "%02i Jan %i", current_day, current_year);
  else if (current_month == 2)
    sprintf (Current_Date, "%02i Feb %i", current_day, current_year);
  else if (current_month == 3)
    sprintf (Current_Date, "%02i Mar %i", current_day, current_year);
  else if (current_month == 4)
    sprintf (Current_Date, "%02i Apr %i", current_day, current_year);
  else if (current_month == 5)
    sprintf (Current_Date, "%02i May %i", current_day, current_year);
  else if (current_month == 6)
    sprintf (Current_Date, "%02i Jun %i", current_day, current_year);
  else if (current_month == 7)
    sprintf (Current_Date, "%02i Jul %i", current_day, current_year);
  else if (current_month == 8)
    sprintf (Current_Date, "%02i Aug %i", current_day, current_year);
  else if (current_month == 9)
    sprintf (Current_Date, "%02i Sep %i", current_day, current_year);
  else if (current_month == 10)
    sprintf (Current_Date, "%02i Oct %i", current_day, current_year);
  else if (current_month == 11)
    sprintf (Current_Date, "%02i Nov %i", current_day, current_year);
  else
    sprintf (Current_Date, "%02i Dec %i", current_day, current_year);

  sprintf (Current_Time, "%02i:%02i:%02i", 
           current_hour, current_min, current_sec);

  return;

}
