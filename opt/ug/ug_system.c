#include "UG_LIB.h"

/*
 * Routines for various system commands that may vary between systems.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_system.c,v 1.24 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

// system routines all are renamed as
// ug_SystemRoutine => SystemRoutine
// or
// ug_SystemRoutine => _SystemRoutine

void (*ext_signal_cleanup) (void) = NULL;

#if defined (_WIN32)

INT_ ug_chdir (const char path[]) { return ((INT_) _chdir (path)); }

INT_ ug_chmod (const char *path, INT_ mode) { return ((INT_) _chmod (path, (int) mode)); }

INT_ ug_execv (const char *path, char *const argv[]) { return ((INT_) _execv (path, (const char *const) argv)); }

INT_ ug_fileno (FILE *stream) { return ((INT_) _fileno (stream)); }

INT_ ug_fstat (INT_ id, ug_stat_struct *buffer) { return ((INT_) _fstat (id, buffer)); }

char *ug_getcwd (char *buffer, size_t size) { return (_getcwd (buffer, size)); }

char *ug_getenv (const char *name) { return (getenv (name)); }

INT_ ug_getpid (void) { return ((INT_) _getpid ()); }

INT_ ug_mkdir (const char *path, INT_ mode)
{
  if (_mkdir (path))
  {
    if (errno != EEXIST)
      return (-1);
  }
  return (ug_chmod (path, (int) mode));
}

INT_ ug_pclose (FILE *stream) { return ((INT_) _pclose (stream)); }

FILE *ug_popen (const char *command, const char *mode) { return (_popen (command, mode)); }

INT_ ug_rmdir (const char *path)
{
  if (rmdir (path))
  {
    if (errno == ENOENT)
      return (-1);
    else
      return (-2);
  }

  return (0);
}

INT_ ug_remove (const char *path)
{
  if (remove (path))
  {
    if (errno == ENOENT)
      return (-1);
    else
      return (-2);
  }

  return (0);
}

INT_ ug_rename (const char *oldname, const char *newname)
{
  if (rename (oldname, newname))
  {
    if (errno == ENOENT)
      return (-1);
    else
      return (-2);
  }

  return (0);
}

INT_ ug_stat (const char *path, ug_stat_struct *buffer) { return ((INT_) _stat (path, buffer)); }

INT_ ug_system (const char *command) { return ((INT_) system (command)); }

void ug_srandom (unsigned seed) { srand (seed); return; }

long ug_random (void) { return (rand ()); }

INT_ ug_unlink (const char *path) { return ((INT_) _unlink (path)); }

// system related routines

char *ug_get_binary_file_mode (const char *mode)
{
  static CHAR_UG_MAX bmode;

  if (strstr (mode, "b"))
    strcpy (bmode, mode);

  else
  {
    strcpy (bmode, "");
    strncat (bmode, mode, strcspn (mode, "+"));

    strcat (bmode, "b");

    if (strstr (mode, "+")) strcat (bmode, "+");
  }

  return (bmode);
}

char *ug_get_home_dir (void) { return (ug_getenv ("USERPROFILE")); }

void ug_get_sys_info (char machine_type[], char os_release[], char os_type[])
{
  FILE *pipe;
  char *env;
  CHAR_UG_MAX current, previous;
  int i, n;

  strcpy (machine_type, "i386");
  strcpy (os_release, "4.0.0");
  strcpy (os_type, "WIN");

  env = ug_getenv ("PROCESSOR_ARCHITECTURE");

  if (env && strstr (env, "64"))
    strcpy (machine_type, "x86_64");
  else
  {
    env = ug_getenv ("PROCESSOR_IDENTIFIER");

    if (env && strstr (env, "64"))
      strcpy (machine_type, "x86_64");
    else
      strcpy (machine_type, "i386");
  }

  pipe = ug_popen ("VER", "r");

  if (pipe)
  {
    strcpy (current, "");

    i = 0;

    do
    {
      strcpy (previous, current);

      n = fscanf (pipe, "%s", current);

      if (i && n == 1 && strcmp (previous, "[Version") == 0) n = 2;

      ++i;
    }
    while (i < 5 && n == 1);

    ug_pclose (pipe);

    if (n == 2)
    {
      n = strcspn (current, "]");

      strcpy (os_release, "");
      strncat (os_release, current, n);
    }
    else
      strcpy (os_release, "4.0.0");
  }

  return;
}

char *ug_get_tmpdir (void)
{
  char *tmp;
             tmp = ug_getenv ("TMP");
  if (! tmp) tmp = ug_getenv ("TEMP");
  if (! tmp) tmp = ug_getenv ("APPDATA");
  if (! tmp) tmp = ug_getenv ("LOCALAPPDATA");
  if (! tmp) tmp = ug_getenv ("USERPROFILE");
  if (! tmp) tmp = NULL;
  return (tmp);
}

FILE *ug_open_tmpdir (char *path)
{
  static FILE *stream;

  strcpy (path, _mktemp (path));

  stream = fopen (path, "w+Db");

  if (stream) { if (ug_fileno (stream) == -1) return (NULL); }

  return (stream);
}

void ug_signal (void)
{
  signal (SIGABRT, ug_signal_handler);
  signal (SIGFPE, ug_signal_handler);
  signal (SIGILL, ug_signal_handler);
  signal (SIGSEGV, ug_signal_handler);
  signal (SIGTERM, ug_signal_handler);
  return;
}

char *ug_signal_message (int signal)
{
       if (signal == SIGABRT) return ("*** abnormal termination ***");
  else if (signal == SIGFPE) return ("*** floating-point exception ***");
  else if (signal == SIGILL) return ("*** illegal instruction ***");
  else if (signal == SIGSEGV) return ("*** segmentation violation (illegal memory access) ***");
  else if (signal == SIGTERM) return ("*** software generated termination request ***");
  else return ("*** unknown system signal ***");
}

#else

INT_ ug_chdir (const char path[]) { return ((INT_) chdir (path)); }

INT_ ug_chmod (const char *path, INT_ mode) { return ((INT_) chmod (path, (mode_t) mode)); }

INT_ ug_execv (const char *path, char *const argv[]) { return ((INT_) execv (path, argv)); }

INT_ ug_fileno (FILE *stream) { return ((INT_) fileno (stream)); }

INT_ ug_fstat (INT_ id, ug_stat_struct *buffer) { return ((INT_) fstat (id, buffer)); }

char *ug_getcwd (char *buffer, size_t size) { return (getcwd (buffer, size)); }

char *ug_getenv (const char *name) { return (getenv (name)); }

INT_ ug_mkdir (const char *path, INT_ mode)
{
  if (mkdir (path, (mode_t) mode))
  {
    if (errno != EEXIST)
      return (-1);
  }

  return (0);
}

INT_ ug_getpid (void) { return ((INT_) getpid ()); }

INT_ ug_pclose (FILE *stream) { return ((INT_) pclose (stream)); }

FILE *ug_popen (const char *command, const char *mode) { return (popen (command, mode)); }

INT_ ug_remove (const char *path)
{
  if (remove (path))
  {
    if (errno == ENOENT)
      return (-1);
    else
      return (-2);
  }

  return (0);
}

INT_ ug_rename (const char *oldname, const char *newname)
{
  if (rename (oldname, newname))
  {
    if (errno == ENOENT)
      return (-1);
    else
      return (-2);
  }

  return (0);
}

INT_ ug_rmdir (const char *path)
{
  if (rmdir (path))
  {
    if (errno == ENOENT)
      return (-1);
    else
      return (-2);
  }

  return (0);
}

INT_ ug_stat (const char *path, ug_stat_struct *buffer) { return ((INT_) stat (path, buffer)); }

INT_ ug_system (const char *command) { return ((INT_) system (command)); }

void ug_srandom (unsigned seed) { srand (seed); return; }

long ug_random (void) { return (rand ()); }

INT_ ug_unlink (const char *path) { return ((INT_) unlink (path)); }

// system related routines

char *ug_get_binary_file_mode (const char *mode)
{
  static CHAR_UG_MAX bmode;
  strcpy (bmode, mode);
  return (bmode);
}

char *ug_get_home_dir (void) { return (ug_getenv ("HOME")); }

void ug_get_sys_info (char machine_type[], char os_release[], char os_type[])
{
  struct utsname *uname_struct;

  INT_ err=0;

  uname_struct = (struct utsname *) ug_malloc (&err, sizeof (struct utsname));

  if (err == 0) err = uname (uname_struct);

  if (err == 0)
  {
    strcpy (machine_type, uname_struct->machine);
    strcpy (os_release, uname_struct->release);
    strcpy (os_type, uname_struct->sysname);

    free (uname_struct);

    uname_struct = NULL;
  }

  else
  {
    strcpy (machine_type, "unknown");
    strcpy (os_release, "unknown");
    strcpy (os_type, "unknown");
  }

  return;
}

char *ug_get_tmpdir (void)
{
  char *tmp;
  tmp = ug_getenv ("TMPDIR");
  if (! tmp)  return ("/tmp");
  return (tmp);
}

FILE *ug_open_tmpdir (char *path)
{
  int id;

  id = mkstemp (path);

  if (id == -1)
    return (NULL);
  else
    return (fdopen (id, "wb+"));
}

void ug_signal (void)
{
  signal (SIGABRT, ug_signal_handler);
  signal (SIGFPE, ug_signal_handler);
  signal (SIGILL, ug_signal_handler);
  signal (SIGSEGV, ug_signal_handler);
  signal (SIGTERM, ug_signal_handler);
  signal (SIGHUP, ug_signal_handler);
  signal (SIGINT, ug_signal_handler);
  signal (SIGQUIT, ug_signal_handler);
  signal (SIGTRAP, ug_signal_handler);
  signal (SIGKILL, ug_signal_handler);
  signal (SIGBUS, ug_signal_handler);
  signal (SIGSYS, ug_signal_handler);
  signal (SIGPIPE, ug_signal_handler);
  signal (SIGALRM, ug_signal_handler);
  signal (SIGXCPU, ug_signal_handler);
  signal (SIGXFSZ, ug_signal_handler);
  signal (SIGVTALRM, ug_signal_handler);
  signal (SIGPROF, ug_signal_handler);
  signal (SIGUSR1, ug_signal_handler);
  signal (SIGUSR2, ug_signal_handler);
  return;
}

char *ug_signal_message (int sig)
{
       if (sig == SIGABRT) return ("*** abnormal termination ***");
  else if (sig == SIGFPE) return ("*** floating-point exception ***");
  else if (sig == SIGILL) return ("*** illegal instruction ***");
  else if (sig == SIGSEGV) return ("*** segmentation violation (illegal memory access) ***");
  else if (sig == SIGTERM) return ("*** software generated termination request ***");
  else if (sig == SIGHUP) return ("*** terminal line hangup ***");
  else if (sig == SIGINT) return ("*** interrupt program (CTRL+C) ***");
  else if (sig == SIGQUIT) return ("*** quit program ***");
  else if (sig == SIGTRAP) return ("*** trace trap ***");
  else if (sig == SIGKILL) return ("*** kill program ***");
  else if (sig == SIGBUS) return ("*** bus error ***");
  else if (sig == SIGSYS) return ("*** non-existent system call invoked ***");
  else if (sig == SIGPIPE) return ("*** write on a pipe with no reader ***");
  else if (sig == SIGALRM) return ("*** real-time timer expired ***");
  else if (sig == SIGXCPU) return ("*** cpu time limit exceeded ***");
  else if (sig == SIGXFSZ) return ("*** file size limit exceeded ***");
  else if (sig == SIGVTALRM) return ("*** virtual time alarm ***");
  else if (sig == SIGPROF) return ("*** profiling timer alarm ***");
  else if (sig == SIGUSR1) return ("*** User defined signal 1 ***");
  else if (sig == SIGUSR2) return ("*** User defined signal 2 ***");
  else return ("*** unknown system signal ***");
}
#endif

char *ug_get_cwd (void)
{
  static CHAR_UG_MAX buffer;
  return (ug_getcwd (&(buffer[0]), sizeof (buffer)));
}

INT_ ug_get_fullpath (const char *path, char *fullpath)
{
  char *cwd;

  CHAR_UG_MAX owd;

  if (! path) return (-1);

  cwd = ug_get_cwd ();

  strcpy (owd, cwd);

  if (ug_chdir (path)) return (-2);

  cwd = ug_get_cwd ();

  if (ug_chdir (owd)) return (415);

  strcpy (fullpath, cwd);

  return (0);
}

void ug_register_signal_cleanup (void (*ext_signal_cleanup_routine) (void))
{
  ext_signal_cleanup = ext_signal_cleanup_routine;
  return;
}

INT_ ug_remove_dir (const char *path) { return ((INT_) ug_rmdir (path)); }

INT_ ug_remove_file (const char *path) { return ((INT_) ug_remove (path)); }

INT_ ug_rename_file (const char *oldname, const char *newname) { return ((INT_) ug_rename (oldname, newname)); }

void ug_signal_handler (int sig)
{
  CHAR_UG_MAX Text;
  sprintf (Text, "*** FATAL SYSTEM ERROR SIGNAL %i ***", sig);
  ug_error_message (Text);
  ug_error_message (ug_signal_message (sig));
  ug_free_tmp_files ();
  if (ext_signal_cleanup) (*ext_signal_cleanup) ();
  exit (sig);
}

void ug_splitpath (const char *path, char *drive, char *dname, char *fname, char *ext)
{
  char *fname_ptr, *tmp_ptr;

  CHAR_UG_MAX _drive, _dname, _fname, _ext, tmp;

  INT_ n;

  strcpy (_dname, path);

  ug_splitdir (_dname, _drive);

  if (strstr (_dname, UG_PATH_SEP))
  {
    tmp_ptr = _dname;

    do
    {
      fname_ptr = tmp_ptr;

      tmp_ptr = (strlen (fname_ptr) > 1) ? strstr (fname_ptr+1, UG_PATH_SEP): NULL;
    }
    while (tmp_ptr);

    strcpy (_fname, fname_ptr+1);

    n = (INT_) strlen (_dname) - (INT_) strlen (fname_ptr);
    n = MAX (n, 1);

    strcpy (tmp, "");

    strncat (tmp, _dname, n);

    strcpy (_dname, tmp);
  }

  else
  {
    strcpy (_fname, _dname);

    strcpy (_dname, "");
  }

  if (strcmp (_dname, "") == 0 && strcmp (_fname, ".") == 0)
  {
    strcpy (_fname, "");
    strcpy (_dname, ug_get_cwd ());

    ug_splitdir (_dname, _drive);
  }

  if (strcmp (_dname, "") == 0 && strcmp (_fname, "..") == 0)
  {
    strcpy (_fname, "");
    strcpy (_dname, ug_get_cwd ());
    strcat (_dname, UG_PATH_SEP);
    strcat (_dname, "..");

    ug_splitdir (_dname, _drive);
  }

  if (strcmp (_fname, "") && ug_check_file_isdir (path) == 0)
  {
    strcpy (tmp, _drive);
    strcat (tmp, _dname);
    strcat (tmp, UG_PATH_SEP);
    strcat (tmp, _fname);

    strcpy (_dname, tmp);
    strcpy (_fname, "");
  }

  else if (strcmp (_dname, "") == 0 && ug_check_file_isreg (path) == 0)
    strcpy (_dname, ".");

  if (strcmp (_dname, "") && ug_check_file_isdir (_dname) == 0)
  {
    ug_get_fullpath (_dname, tmp);

    strcpy (_dname, tmp);

    ug_splitdir (_dname, _drive);
  }

  if (drive) strcpy (drive, _drive);
  if (dname) strcpy (dname, _dname);

  ug_splitname (_fname, _ext);

  if (fname) strcpy (fname, _fname);

  if (ext) strcpy (ext, _ext);

  return;
}

void ug_splitdir (char *dname, char *drive)
{
#if defined (UG_PATH_DRIVE_SEP)
  char *tmp_ptr;

  CHAR_UG_MAX tmp;

  INT_ n;

  tmp_ptr = strstr (dname, UG_PATH_DRIVE_SEP);

  if (tmp_ptr)
  {
    n = (INT_) strcspn (dname, UG_PATH_DRIVE_SEP) + 1;

    strncat (drive, dname, n);

    if ((INT_) strlen (dname) > n)
      strcpy (tmp, tmp_ptr+1);
    else
      strcpy (tmp, "");

    strcpy (dname, tmp);
  }
  else
#endif
    strcpy (drive, "");

  return;

}

void ug_splitname (char *fname, char *ext)
{
  char *ext_ptr, *tmp_ptr;

  CHAR_UG_MAX tmp;

  INT_ n;

  if (strstr (fname, UG_PATH_EXT_SEP))
  {
    tmp_ptr = fname;

    do
    {
      ext_ptr = tmp_ptr;

      tmp_ptr = (strlen (ext_ptr) > 1) ? strstr (ext_ptr+1, UG_PATH_EXT_SEP): NULL;
    }
    while (tmp_ptr);

    n = (INT_) strlen (fname) - (INT_) strlen (ext_ptr);

    if (n)
    {
      strcpy (ext, ext_ptr);

      strcpy (tmp, "");
      strncat (tmp, fname, n);
      strcpy (fname, tmp);
    }
    else
    {
      strcpy (ext, "");

      strcpy (fname, ext_ptr);
    }
  }
  else
    strcpy (ext, "");

  return;

}
