#include "UG_LIB.h"

/*
 * Local variables.
 */

static INT_1D *proc_status = NULL;

static INT_ iproc_ = 0;
static INT_ nproc_ = 1;
static INT_ mpmode_ = 0;

/*
 * Parallel processing mode routines.
 *
 * mpmode = 0 = Serial processing mode.
 * mpmode = 1 = Parallel processing mode.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_mp.c,v 1.22 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

static INT_ ug_mp_check_file
 (char[],
  INT_,
  INT_);

static void ug_mp_cleanup
 (void);

static void ug_mp_close_file
 (char[],
  INT_);

static INT_ ug_mp_write_file
 (char[],
  INT_,
  INT_);

INT_ ug_mp_setup
 (INT_ iproc,
  INT_ nproc,
  char TMP_File_Dir[])

{

/*
 * Setup parallel processing mode.
 */

  INT_ Error_Flag;

  iproc_ = 0;
  nproc_ = 1;

  mpmode_ = 0;

  if (nproc == 1)
    return (0);

  if (iproc < 0 || iproc >= nproc)
    return (403);

  if (nproc <= 1)
    return (404);

  Error_Flag = ug_set_tmp_file_dir (TMP_File_Dir);

  if (Error_Flag > 0)
    return (Error_Flag);

  iproc_ = iproc;
  nproc_ = nproc;

  mpmode_ = 1;

  Error_Flag = ug_mp ("setup");

  if (Error_Flag > 0)
    return (Error_Flag);

  proc_status = (INT_1D *) ug_calloc (&Error_Flag, nproc, sizeof (INT_1D));

  if (Error_Flag > 0)
    return (100403);

  return (0);

}

INT_ ug_mp
 (char Task[])

{

/*
 * Perform parallel processing task.
 */

  static INT_ isync = 0;

  INT_ Error_Flag, jproc, jproc1, jproc2, nwait;
  INT_ Return_Flag = 0;

  if (strcmp (Task, "iproc") == 0)
    Return_Flag = iproc_;

  else if (strcmp (Task, "nproc") == 0)
    Return_Flag = nproc_;

  else if (strcmp (Task, "mode") == 0)
    Return_Flag = mpmode_;

  else if (strcmp (Task, "setup") == 0 && mpmode_ == 1)
  {
    Return_Flag = ug_mp_write_file ("active", iproc_, 0);

    if (Return_Flag != 0)
    {
      ug_mp_cleanup ();
      return (405);
    }
  }

  else if (strcmp (Task, "wait") == 0 && mpmode_ == 1)
  {
    ++isync;

    Error_Flag = ug_mp_write_file ("wait", iproc_, isync);

    if (Error_Flag != 0)
    {
      ug_mp_cleanup ();
      return (405);
    }

    proc_status[iproc_] = 1;

    do
    {
      if (iproc_ == 0)
      {
        jproc1 = 1;
        jproc2 = nproc_-1;
      }
      else
      {
        jproc1 = 0;
        jproc2 = 0;
      }

      nwait = jproc2 - jproc1 + 1;

      for (jproc = jproc1; jproc <= jproc2; ++jproc)
      {
        if (proc_status[jproc] == 0)
        {
          Error_Flag = ug_mp_check_file ("active", jproc, 0);

          if (Error_Flag != 0)
          {
            ug_mp_cleanup ();
            return (405);
          }

          Error_Flag = ug_mp_check_file ("wait", jproc, isync);

          if (Error_Flag == 0)
          {
            proc_status[jproc] = 1;

            --nwait;
          }
        }
        else
          --nwait;
      }
    }
    while (nwait > 0);

    Error_Flag = ug_mp_write_file ("done", iproc_, isync);

    if (Error_Flag != 0)
    {
      ug_mp_cleanup ();
      return (405);
    }

    proc_status[iproc_] = 0;

    do
    {
      if (iproc_ == 0)
      {
        jproc1 = 1;
        jproc2 = nproc_-1;
      }
      else
      {
        jproc1 = 0;
        jproc2 = 0;
      }

      nwait = jproc2 - jproc1 + 1;

      for (jproc = jproc1; jproc <= jproc2; ++jproc)
      {
        if (proc_status[jproc] == 1)
        {
          Error_Flag = ug_mp_check_file ("active", jproc, 0);

          if (Error_Flag != 0)
          {
            ug_mp_cleanup ();
            return (405);
          }

          Error_Flag = ug_mp_check_file ("done", jproc, isync);

          if (Error_Flag == 0)
          {
            proc_status[jproc] = 0;

            --nwait;
          }
        }
        else
          --nwait;
      }
    }
    while (nwait > 0);

    Return_Flag = 0;
  }

  else if (strcmp (Task, "cleanup") == 0 && mpmode_ == 1)
  {
    ug_mp_cleanup ();

    Return_Flag = 0;
  }

  else if (strcmp (Task, "end") == 0 && mpmode_ == 1)
  {
    mpmode_ = 0;

    ug_mp_cleanup ();

    ug_free (proc_status);

    Return_Flag = 0;
  }

  return (Return_Flag);

}

static INT_ ug_mp_check_file
 (char Label[],
  INT_ iproc,
  INT_ isync)

{

  FILE *Data_File = NULL;

  CHAR_UG_MAX File_Name;

  INT_ iread;
  INT_ jsync = -1;

  sprintf (File_Name, "UG_MP_%s_%i", Label, (int) iproc);

  Data_File = ug_fopen (File_Name, "r_tmp");

  if (Data_File == NULL)
    return (1);

  if (isync > 0)
  {
    rewind (Data_File);

    iread = ug_fread (&jsync, sizeof (INT_), 1, Data_File);

    iread = iread - 1;
  }
  else
  {
    iread = 0;

    jsync = 0;
  }

  ug_close_dataf (File_Name);

  if (iread < 0)
    return (1);

  if (isync != jsync)
    return (1);

  return (0);

}

static void ug_mp_cleanup
 (void)

{

  ug_mp_close_file ("active", iproc_);
  ug_mp_close_file ("done", iproc_);
  ug_mp_close_file ("wait", iproc_);

  return;

}

static void ug_mp_close_file
 (char Label[],
  INT_ iproc)

{

  CHAR_UG_MAX File_Name;

  sprintf (File_Name, "UG_MP_%s_%i", Label, (int) iproc);

  ug_close_data (File_Name);

  return;

}

static INT_ ug_mp_write_file
 (char Label[],
  INT_ iproc,
  INT_ isync)

{

  FILE *Data_File = NULL;

  CHAR_UG_MAX File_Name;

  INT_ iwrite;

  sprintf (File_Name, "UG_MP_%s_%i", Label, (int) iproc);

  Data_File = ug_fopen (File_Name, "tmp");

  if (Data_File == NULL)
    return (402);

  iwrite = ug_fwrite (&isync, sizeof (INT_), 1, Data_File);

  iwrite = iwrite - 1;

  ug_close_dataf (File_Name);

  if (iwrite < 0)
    return (402);

  return (0);

}
