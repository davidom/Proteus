#include "UG_LIB.h"

INT_ ug_rerun (INT_ mmsg, const int argc, char *const argv[])
{

/*
 * Set or get case name.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_rerun.c,v 1.13 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  char **argv2;
  char *path_list;

  CHAR_256 *items = NULL;

  CHAR_UG_MAX arch_dir, arch_mode32_suffix, arch_mode64_suffix, 
              base_arch_type, base_sys_arch_type,
              def_arch_mode_suffix, def_arch_type,
              exe_dir, exe_drive, exe_name, exe_ext,
              name, new_ver_dir, path, sys_dir, text, tmp, ver_dir;

  INT_ arch_mode, debug_mode, err, i, j, n, argc2, prof_mode, purify_mode;

  // allocate a new argument vector
  // will be of the same length or less than the input argument vector

  err = 0;

  argv2 = (char **) ug_malloc (&err, (argc+2) * sizeof (*argv));

  if (err)
    return (100411);

  // get architecture directory naming information

  err = ug_get_arch_info (arch_mode32_suffix, arch_mode64_suffix,
                          base_arch_type, base_sys_arch_type,
                          def_arch_mode_suffix, def_arch_type); 

  if (err)
  {
    ug_free (argv2);
    return (err);
  }

  // default architecture and system options

  arch_mode = 0;
  debug_mode = 0;
  prof_mode = 0;
  purify_mode = 0;

  strcpy (new_ver_dir, "");

  // loop over input argument vectors

  j = 0;

  for (i = 1; i < argc; ++i)
  {
    // 32-bit mode option

    if (strcmp (argv[i], "-32") == 0)
    {
      if (strcmp (def_arch_mode_suffix, arch_mode32_suffix))
        arch_mode = 32;
    }

    // 64-bit mode option

    else if (strcmp (argv[i], "-64") == 0)
    {
      if (strcmp (def_arch_mode_suffix, arch_mode64_suffix))
        arch_mode = 64;
    }

    // debug mode option

    else if (strcmp (argv[i], "-debug") == 0)
      debug_mode = 1;

    // prof mode option

    else if (strcmp (argv[i], "-prof") == 0)
      prof_mode = 1;

    // purify mode option

    else if (strcmp (argv[i], "-purify") == 0)
      purify_mode = 1;

    // system version directory option

    else if (strcmp (argv[i], "-v") == 0)
    {
      ++i;

      strcpy (new_ver_dir, argv[i]);
    }

    // save all arguments not related to architecture or system version
    // directory in a new argument vector

    else
    {
      ++j;

      argv2[j] = argv[i];
    }
  }

  // set number of entries in new argument vector

  argc2 = j+1;

  // if none of the architecture or system directory related arguments were set
  // then exit without error

  if (arch_mode == 0 && debug_mode == 0 && prof_mode == 0 && purify_mode == 0 && strcmp (new_ver_dir, "") == 0)
  {
    ug_free (argv2);

    return (0);
  }

  // split the path name for the current executable into separate strings for
  // the drive (Windows only), directory, executable name, and extension

  strcpy (path, argv[0]);

  ug_splitpath (path, exe_drive, exe_dir, exe_name, exe_ext);

  // if directory name is empty then set it to the full path name of the
  // current directory

  if (strcmp (exe_dir, "") == 0)
  {
    strcpy (path, ug_get_cwd ());

    ug_splitpath (path, exe_drive, exe_dir, NULL, NULL);
  }

  // build derived path

  strcpy (path, exe_drive);
  strcat (path, exe_dir);
  strcat (path, UG_PATH_SEP);
  strcat (path, exe_name);
  strcat (path, exe_ext);

  // if the file does not exist at the derived path then check if it exists
  // in a directory included in the PATH environment variable

  if (ug_check_file_isreg (path))
  {
    strcpy (name, exe_name);
    strcat (name, exe_ext);

    path_list = ug_getenv ("PATH");

    err = ug_find_file_in_path_list (name, path_list, path);

#if defined (UG_EXE_EXT)
    if (err && strcmp (exe_ext, "") == 0)
    {
      strcpy (name, exe_name);
      strcat (name, UG_EXE_EXT);

      err = ug_find_file_in_path_list (name, path_list, path);
    }
#endif

    if (err)
      return (422);

    ug_splitpath (path, exe_drive, exe_dir, exe_name, exe_ext);
  }

  // strip prof mode extension from executable name

  if (strstr (exe_name, ".p")) ug_splitname (exe_name, tmp);

  // strip debug mode extension from executable name

  if (strstr (exe_name, ".g")) ug_splitname (exe_name, tmp);

  // strip purify mode extension from executable name

  if (strstr (exe_name, ".purify")) ug_splitname (exe_name, tmp);

  // output message header

  if (mmsg)
  {
    ug_message (" ");
    sprintf (text, "UG RERUN : RERUN WITH ANOTHER VERSION OF %s", exe_name);
    ug_message (text);
    ug_message (" ");
  }

  // set 32-bit architecture directory name

  if (arch_mode == 32)
  {
    if (strcmp (def_arch_mode_suffix, arch_mode64_suffix) == 0)
    {
      strcpy (arch_dir, base_arch_type);
      strcat (arch_dir, arch_mode32_suffix);
    }
  }

  // set 64-bit architecture directory name

  else if (arch_mode == 64)
  {
    if (strcmp (def_arch_mode_suffix, arch_mode32_suffix) == 0)
    {
      strcpy (arch_dir, base_arch_type);
      strcat (arch_dir, arch_mode64_suffix);
    }
  }

  // set default (32 or 64-bit) architecture directory name

  else
    strcpy (arch_dir, def_arch_type);

  // add on bin directory

  strcat (arch_dir, UG_PATH_SEP);
  strcat (arch_dir, "bin");

  // add prof mode extension to executable name

  if (prof_mode) strcat (exe_name, ".p");

  // add debug mode extension to executable name

  if (debug_mode) strcat (exe_name, ".g");

  // add purify mode extension to executable name

  if (purify_mode) strcat (exe_name, ".pure");

  // add executable extension on executable name

#if defined (UG_EXE_EXT)
  strcat (exe_name, UG_EXE_EXT);
#endif

  // get a list of items from the executable directory path that represent each
  // subdirectory name

  err = ug_get_items_from_list (exe_dir, UG_PATH_SEP, &items, &n);

  if (err || n < 4)
  {
    ug_free (argv2);
    ug_free (items);
    if (err < 0)
      return (417);
    else
      return (err);
  }

  // set the system root directory staring with the drive

  // the assumed structure is
  // ..._system_root_dir_/_system_version_dir_/_arch_directory_/bin/file.exe
  // for example .../simsys/MacOSX-x86/bin/ug.exe

  strcpy (sys_dir, exe_drive);

  for (i = 0; i <= n-4; ++i)
  {
    strcat (sys_dir, UG_PATH_SEP);
    strcat (sys_dir, items[i]);
  }

  // replace the system version subdirectory name

  strcpy (ver_dir, items[n-3]);

  // free the list

  ug_free (items);

  // save output message

  if (mmsg)
    strcpy (text, "         :");

  // if a new version directory was specified then
  // set and check full system version directory path

  if (strcmp (new_ver_dir, ""))
  {
    // check if new version directory is a full path name

    strcpy (path, new_ver_dir);

    if (ug_check_file_isdir (path))
    {
      // save output message

      if (mmsg)
      {
        strcat (text, " ");
        strcat (text, path);
      }

      // check if new version directory is a subdirectory of the
      // current system root directory

      strcpy (path, sys_dir);
      strcat (path, UG_PATH_SEP);
      strcat (path, new_ver_dir);

      if (ug_check_file_isdir (path))
      {
        // save output message

        if (mmsg)
        {
          strcat (text, " ");
          strcat (text, path);
        }

        // check if new version directory is a subdirectory of the 
        // home directory

        strcpy (path, ug_get_home_dir());
        strcat (path, UG_PATH_SEP);
        strcat (path, new_ver_dir);

        if (ug_check_file_isdir (path))
        {
          // output message

          if (mmsg)
          {
            strcat (text, " ");
            strcat (text, path);

            ug_message ("UG RERUN : unable to find any of the following directories");
            ug_message (text);
          }

          // exit if none of these directories actually exists

          ug_free (argv2);
          return (418);
        }
      }
    }
  }

  // set and check full system version directory path (this should never fail)

  else
  {
    strcpy (path, sys_dir);
    strcat (path, UG_PATH_SEP);
    strcat (path, ver_dir);

    if (ug_check_file_isdir (path))
    {
      // exit if this directory doesn't exist

      ug_free (argv2);
      return (419);
    }
  }

  // add on architecture directory and exectuable file name to the full 
  // system version directory path

  strcat (path, UG_PATH_SEP);
  strcat (path, arch_dir);
  strcat (path, UG_PATH_SEP);
  strcat (path, exe_name);

  // exit if new exectuable doesn't exist

  if (ug_check_file_isreg (path))
  {
    // output message

    if (mmsg)
    {
      sprintf (text, "UG RERUN : unable to find file %s", path);
      ug_message (text);
    }

    ug_free (argv2);
    return (420);
  }

  // set first argument of new argument vector to the full path name of the new
  // executable

  argv2[0] = &path[0];

  // set last argument of new argument vector to NULL to signify the end

  argv2[argc2] = NULL;

  // output message

  if (mmsg)
  {
    strcpy (text, "UG RERUN :");

    for (j = 0; j < argc2; ++j)
    {
      strcat (text, " ");
      strcat (text, argv2[j]);
    }

    ug_message (text);
  }

  // rerun the executable with new argument vector

  ug_execv (path, argv2);

  // if no error occurrs then this code is never returned to

  // if an error occurred then free new argument vector and return with error

  ug_free (argv2);

  return (421);
}
