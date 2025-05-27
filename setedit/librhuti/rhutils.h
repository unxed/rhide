/* Copyright (C) 1996-1998 Robert H�hne, see COPYING.RH for details */
/* This file is part of RHIDE. */
#ifndef __rhutils_h
#define __rhutils_h

#ifndef __cplusplus
#error You can use these functions only in C++ programs!!
#endif

/* return a malloced string 'src'. When scr=NULL return malloced "" */
char *string_dup(const char *src);
/* the same as above, but store the result also in 'dest' */
char *string_dup(char *&dest, const char *src);
/* convert 'str' to lowercase */
char *string_down(char *str);
/* convert 'str' to uppercase */
char *string_up(char *str);

/* expand_spec expands the spec 'spec' which can be $(VARIABLE) and
   or $(function [args]). Additionally you can pass a function which
   is called when the builtin specs do not match to define your own
   specs and/or special variables. The 'ext_func' function is called
   in the second argument with the internal function of
   'expand_spec' which does the real expansion so you can use in your
   code also references to the standard expansion code.

   If you set the last argument to nonzero, the external function
   is called before any builtin function, otherwise it is called
   last.
   */
typedef char *(*token_func)(char *);
typedef char *(*external_token_func)(const char *, token_func);
/* This returns a malloced string ever!!
   Currently it knows about the following string_functions. (For
   a description of them please look in the GNU Make doc, because
   the functionallity is the same (new functions have a small comment):

  $(strip TEXT)
  $(addsuffix suffix,TEXT)
  $(addprefix prefix,TEXT)
  $(notdir filename)
  $(dir filename)
  $(subst from,to,TEXT)
  $(suffix filename)
  $(nosuffix filename) this removes the suffix (if any) from 'filename'
  $(setsuffix suffix,filename) thsi replaces (or add) the 'suffix' for 'filename'
  $(sort TEXT)
  $(foreach variable,list,command)
  $(shell command)
  $(wildcard WILDCARD)
  $(word nr,TEXT)
*/
char *expand_spec(const char *spec, external_token_func ext_func,
                  int expand_extern_first = 0);

/*
  Set this to a function, which is called for each newline '\n'
  character.
*/
extern void (*handle_newline)();

/*
  Set this variable to 0, if variable-names itself should not
  be expanded, i.e. treat $(var$(other)) as a variable with the
  name 'var$(other)', otherwise $(other) is expande to lets say
  'foo' and $(var$(other)) becomes to $(varfoo) */
extern int expand_variable_names;

/* Some helper functions for 'expand_spec' which expand _ONLY_ variables,
   and no string functions or external specs. The first simply calls the
   second with 'use_env=1' and the 'use_env' of the second functions can
   be used to disable the search in the real environment for the wanted
   variable by setting it to zero. */
//const char * GetVariable(const char *variable);
//const char * GetVariable(const char *variable,int use_env);
void set_variable(const char *variable, const char *contents);

/* Array of strings with the internal variables with the organization
   vars[i]   - variable name
   vars[i+1] - variable contents
*/
extern char **vars;
/* The count of the variables (Note that this is the variable count and
   _NOT_ the string count in 'vars') */
extern int var_count;

/* set this to nonzero to print to stderr debug information about expanding
   the specs */
extern int debug_specs;

/* Add a variable 'variable' with contents 'contents'. If 'contents==NULL',
   then the variable is removed. */
void insert_variable(const char *variable, const char *contents);

/* Some helper functions: 'open_brace' points to an opening brace
   or parenthesis and return the matching closing character or
   NULL if not found */
char *find_close_brace(char *open_brace);
/* Find the next comma in the string where it knows about quoting characters
   '\'' and '"' or return NULL if no comma was found */
char *find_next_comma(char *arg);

/* Split a given filename in its parts given in the format string 'fmt'
   similar to scanf. The results are stored in the paramters you have
   to pass in '...' (for each format spec one 'char *'). Possible format
   codes are:

  %D - drive + directory (drive of course only on DJGPP)
  %d - directory
  %f - filename + suffix
  %F - filename without suffix
  %e - suffix
*/
void split_fname_fmt(const char *filename, const char *fmt, ...);

/* Append 'src' to 'dest' and return 'dest' (malloced or realloced
   strings) */
char *string_cat(char *&dest, const char *src);
/* The same as above, but with variable count of arguments.
   _NOTE_: The last argument must be NULL!!!! */
char *string_cat(char *&, const char *src1, const char *src2, ...);
/* Free 'str' and set it to NULL */
void string_free(char *&str);
/* Remove any directory part in 'name' and when 'with_suffix==0' also
   the suffix */
void BaseName(char *name, int with_suffix = 1);
/* The same as above, but store the result in the malloced 'bname' */
void BaseName(const char *name, char *&bname, int with_suffix = 1);
/* Convert any '\' to '/' in 'name' */
void BackslashToSlash(char * name);
/* split the given filename 'fname' in its components and return them
   malloc'd. If the filename contained a drive letter and colon, it will
   be part of 'dir'. */
void split_fname(const char *fname, char *&dir, char *&name, char *&ext);
/* The same as above, except that the drive letter is stored in 'drive'.
   If there is no drive letter, 'drive' will be zero. 'dir' will contain
   only the directory and not (if any) drive and colon. */
void split_fname(const char *fname, char &drive, char *&dir, char *&name,
                 char *&ext);
/* Expands an given name 'name' to an absolute filename relative to the
   current directory. If it was
   already absolute nothing is done. If 'new_alloc==0' the 'name' is
   not realloced */
void FExpand(char * & name,int new_alloc=1);

/*
   I moved it to compat layer library. We had 2 copies!
   Very usefull function from DJGPP's libc, which "fixes" the path 'in'
   and placing it in 'out'. If 'in' is a relative filename it is made
   absolute to the current directory. Example:

     char Out[256];
     __fixpath("/foo/foo1/../.././foo",Out);

   will return "/foo" in 'Out'
#ifndef TVCompf_djgpp
void __fixpath(const char *in, char *out);
#endif
*/

/* Some functions for redirecting stdout and stderr. They cannot
   be called nested!!! */
/* redirect stderr to a file and return the name of that file */
char * open_stderr(void);
char * open_stderr(int *nherr);
/* the same for stdout */
char * open_stdout(void);
/* close the redirected stderr */
void close_stderr(void);
/* the same for stdout */
void close_stdout(void);
/* SET: that's for stderr and stdout redirected to the same file */
char *open_stderr_out(void);
char *open_stderr_out(int *nherr); // Without dup2
void close_stderr_out(void);
/* SET: can be used to shut-up leak reporters */
void op_cl_std_clean_up(void);

/* Return an unique filename in $TMPDIR which starts with 'before'.
   Store the result also in 'retval' if it is != NULL. This function
   creates the file to make sure that it is really unique. That means
   the application is responsible for deleting this file */
char *unique_name(char *before,char *retval = (char *)0);

/* This functions makes ret_path relative to ref_path
   It assumes that ref_path is an absolute path which can
   optionally have a slash appended */
int AbsToRelPath(char *ref_path,char *& ret_path, const char *subst);
#endif /* __rhutils_h */


