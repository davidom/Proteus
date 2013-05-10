#include "UG_LIB.h"

#ifdef _WIN32
#ifndef S_ISDIR
#define S_ISDIR(stat_mode) (((stat_mode) & _S_IFMT) == _S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(stat_mode) (((stat_mode) & _S_IFMT) == _S_IFREG)
#endif
#endif

INT_ ug_check_file (const char *path, INT_ m)
{

/*
 * Check if file exists.
 *
 * If path = "set_ug_check_file_nrepeat" then set number of times to
 * repeat check.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_check_file.c,v 1.14 2012/12/12 17:54:27 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  static INT_ nrepeat_ = 1000;

  ug_stat_struct stat_struct;

  INT_ i, n;
  INT_ err = 0;

  if (err) return (-1);

  if (m > 1)
    nrepeat_ = m;

  n = (m) ? nrepeat_: 0;

  i = 0;

  do
  {
    err = ug_stat (path, &stat_struct);

    ++i;
  }
  while (i < n && err < 0);

  if (err < 0) return (-1);

  return (0);

}

INT_ ug_check_file_isdir (const char *path)
{ 
  ug_stat_struct stat_struct;
  CHAR_UG_MAX dpath;

  INT_ i, err;

  strcpy (dpath, path);

  err = ug_stat (dpath, &stat_struct);

  i = (err == 0 && S_ISDIR (stat_struct.st_mode)) ? 0: -1;

  return (i);
}

INT_ ug_check_file_isreg (const char *path)
{ 
  ug_stat_struct stat_struct;
  CHAR_UG_MAX dpath;

  INT_ i, err;

  strcpy (dpath, path);

  err = ug_stat (dpath, &stat_struct);

  i = (err == 0 && S_ISREG (stat_struct.st_mode)) ? 0: -1;

  return (i);
}

