/* Copyright (C) 1996-1998 Robert H�hne, see COPYING.RH for details */
/* This file is part of RHIDE. */
#include <rhutils.h>
#define Uses_stdio
#define Uses_glob
#define Uses_alloca
#define Uses_string
#define Uses_stdlib
#include <compatlayer.h>

#ifdef USE_REGPARM
#define A(n) __attribute__ (( regparm(n) ))
#else
#define A(n)
#endif

const char * GetVariable(const char *variable);
const char * GetVariable(const char *variable,int use_env);

static void _handle_newline();
void (*handle_newline)() = _handle_newline;
int expand_variable_names = 1;
char **vars = NULL;
int var_count = 0;
static int var_size = 0;
/* set this nonzero to print to stderr, how the specs
   are expanded */
int debug_specs = 0;

static void _handle_newline()
{
}

static void
_add_variable(char **&_vars, int & _var_count, int & _var_size,
              const char *variable, const char *contents)
{
  /* Special case when contents==NULL, remove that variable */
  if (!contents)
  {
    int i;
    for (i=_var_count;i>0;i--)
    {
      int j=(i-1)*2;
      if (strcmp(_vars[j],variable) == 0)
      {
        string_free(_vars[j]);
        string_free(_vars[j+1]);
        if (i<_var_count)
        {
          memcpy(_vars+j,_vars+j+2,(_var_count-i)*sizeof(char *)*2);
        }
        _var_count--;
        break;
      }
    }
    return;
  }
  if (_var_count == _var_size)
  {
    _var_size += 16;
    _vars = (char **)realloc(_vars,_var_size*2*sizeof(char *));
  }
  _var_count++;
  string_dup(_vars[_var_count*2-2],variable);
  string_dup(_vars[_var_count*2-1],contents);
}

static void
_set_variable(char **_vars, int _var_count, const char *variable,
              const char *contents)
{
  int i;
  for (i=_var_count;i>0;i--)
  {
    int j = (i-1)*2;
    if (strcmp(_vars[j],variable) == 0)
    {
      string_free(_vars[j+1]);
      _vars[j+1] = string_dup(contents);
      return;
    }
  }
}

static const char *
_get_variable(char **_vars, int _var_count, const char *variable)
{
  int i;
  for (i=_var_count;i>0;i--)
  {
    int j=(i-1)*2;
    if (strcmp(_vars[j],variable) == 0)
      return _vars[j+1];
  }
  return NULL;
}

void insert_variable(const char *variable,const char *contents)
{
  _add_variable(vars, var_count, var_size, variable, contents);
}

void set_variable(const char *variable, const char *contents)
{
  _set_variable(vars, var_count, variable, contents);
}

static int internal_var_count = 0;
static int internal_var_size = 0;
static char **internal_vars = NULL;

static void add_internal_var(const char *name,const char *value)
{
  _add_variable(internal_vars, internal_var_count, internal_var_size,
                name, value);
}

static void remove_internal_var(const char *name)
{
  _add_variable(internal_vars, internal_var_count, internal_var_size,
                name, NULL);
}

static const char *
get_internal_var(const char *name)
{
  return _get_variable(internal_vars, internal_var_count, name);
}

static void set_internal_var(const char *name,const char *value)
{
  if (!get_internal_var(name))
    add_internal_var(name, value);
  else
    _set_variable(internal_vars, internal_var_count, name, value);
}

const char *RHGetVariable(const char *variable)
{
  return GetVariable(variable,1);
}

const char * GetVariable(const char *variable,int use_env)
{
  const char *ival = get_internal_var(variable);
  if (!ival)
  {
    ival = _get_variable(vars, var_count, variable);
    if (!ival && use_env)
      ival = getenv(variable);
  }
  if (debug_specs)
  {
    fprintf(stderr,"GetVariable(%s,%d) ==> %s\n",variable,use_env,
            ival?ival:"NULL");
  }
  return ival;
}

#ifdef TVCompf_djgpp
extern char **environ;
#endif

typedef A(1) char *(*string_function)(char *);

typedef struct
{
  char *name;
  int name_len;
  string_function function;
} string_function_rec;

static A(1)
char *string_function_strip(char *);
static A(1)
char *string_function_addsuffix(char *);
static A(1)
char *string_function_addprefix(char *);
static A(1)
char *string_function_notdir(char *);
static A(1)
char *string_function_dir(char *);
static A(1)
char *string_function_subst(char *);
static A(1)
char *string_function_suffix(char *);
static A(1)
char *string_function_nosuffix(char *);
static A(1)
char *string_function_setsuffix(char *);
static A(1)
char *string_function_sort(char *);
static A(1)
char *string_function_foreach(char *);
static A(1)
char *string_function_shell(char *);
static A(1)
char *string_function_wildcard(char *);
static A(1)
char *string_function_word(char *);

static string_function_rec string_function_list[] =
{
#define SF(x) {#x,sizeof(#x)-1,string_function_##x}
  SF(strip),
  SF(addsuffix),
  SF(addprefix),
  SF(notdir),
  SF(dir),
  SF(subst),
  SF(suffix),
  SF(nosuffix),
  SF(setsuffix),
  SF(sort),
  SF(foreach),
  SF(shell),
  SF(wildcard),
  SF(word),
  {NULL,0,NULL}
#undef SF
};

char *find_close_brace(char *open_brace)
{
  int brace_count = 1;
  open_brace++;
  while (brace_count > 0 && *open_brace)
  {
    if (*open_brace == '\\')
    {
      open_brace += 2;
      continue;
    }
    if (*open_brace == ')') brace_count--;
    else if (*open_brace == '(') brace_count++;
    open_brace++;
  }
  if (brace_count != 0) return NULL;
  return open_brace-1;
}

char *find_next_comma(char *arg)
{
  while (*arg)
  {
    if (*arg == '\\')
    {
      arg += 2;
      continue;
    }
    if (*arg == ',') return arg;
    if (*arg == '(')
    {
      arg = find_close_brace(arg+1);
      if (!arg) return NULL;
    }
    arg++;
  }
  return NULL;
}

static
external_token_func external_expand_token;

static A(1)
char *expand_tokens(char *tokens);

static A(1)
char *expand_variable(char *__token)
{
  char *token;

  // At first expand the variable name itself, because it
  // can be a computed variable like $(RHIDE_OS_LIBS_$(RHIDE_OS))
  // which can be computed at runtime depending on the value
  // of $(RHIDE_OS)
  // if the expand_variable_names == 0, do not the above
  char *end;
  if (expand_variable_names && ((end = find_close_brace(__token+2)) != NULL))
  {
    char *_token;
    *end = 0;
    _token = expand_tokens(__token+2);
    *end = ')';
    token = (char *)alloca(strlen(_token)+4);
    strcpy(token,"$(");
    strcat(token,_token);
    strcat(token,")");
    string_free(_token);
  }
  else
  {
    token = (char *)alloca(strlen(__token)+1);
    strcpy(token,__token);
  }
  end = token + strlen(token) - 1;
  char *retval;
  const char *variable;
  *end = 0;
  variable = GetVariable(token+2);
  *end = ')';
  if (!variable)
  {
    retval = NULL;
    goto END;
  }
  string_dup(end,variable);
  retval = expand_tokens(end);
  string_free(end);
END:
  if (debug_specs)
  {
    fprintf(stderr,"expand_variable(%s) ==> %s\n",__token,
            retval?retval:"NULL");
  }
  return retval;
}

static A(1)
char *check_for_string_function(char *token)
{
  int i=0;
  while (string_function_list[i].name)
  {
    int name_len = string_function_list[i].name_len;
    if (strncmp(token+2,string_function_list[i].name,name_len) == 0
        && token[2+name_len] == ' ')
    {
      char *start,*end,*expanded;
      start = token + 3 + name_len;
      end = token + strlen(token)-1;
      *end = 0;
#if 0
/* I cannot expand the argument here, because at least the
   foreach function expects the last argument unexpanded. If
   I would expand it here, it would become probably empty.
   That means, any string function is responsible for expanding
   the arguments */
      expanded = expand_tokens(start);
      *end = ')';
      end = string_function_list[i].function(expanded);
      string_free(expanded);
      return end;
#else
      expanded = string_function_list[i].function(start);
      *end = ')';
      return expanded;
#endif
    }
    i++;
  }
  return NULL;
}

static int _call_extern_first = 0;

static A(1)
char *expand_token(char *token)
{
  char *retval = NULL;
  if (_call_extern_first && external_expand_token)
    retval = external_expand_token(token,expand_tokens);
  if (!retval)
    retval = check_for_string_function(token);
  if (!retval)
    retval = expand_variable(token);
  if (!_call_extern_first && !retval && external_expand_token)
    retval = external_expand_token(token,expand_tokens);
  if (!retval) retval = string_dup("");
  if (debug_specs)
  {
    fprintf(stderr,"expand_token(%s) ==> %s\n",token,retval);
  }
  return retval;
}

static A(1)
char *expand_tokens(char *__tokens)
{
  char *start;
  char *end,c;
  char *tokens = __tokens;
  char *_tokens=NULL;
  char *expanded;
  start = tokens;
  while (*start)
  {
    if (*start == '\n')
      handle_newline();
    if (start[0] == '$' && start[1] == '(')
    {
      *start = 0;
      string_cat(_tokens,tokens);
      *start = '$';
      end = find_close_brace(start+1);
      if (!end)
      {
        string_cat(_tokens,start);
        goto END;
      }
      end++;
      c = *end;
      *end = 0;
      expanded = expand_token(start);
      string_cat(_tokens,expanded);
      string_free(expanded);
      *end = c;
      start = tokens = end;
    }
    else if (start[0] == '$' && start[1] == '$')
    {
      start++;
      *start = 0;
      string_cat(_tokens,tokens);
      start++;
      tokens = start;
    }
    else start++;
  }
  string_cat(_tokens,tokens);
END:
  if (debug_specs)
  {
    fprintf(stderr,"expand_tokens(%s) ==> %s\n",__tokens,
            _tokens?_tokens:"NULL");
  }
  return _tokens;
}

char *expand_spec(const char *spec,external_token_func ext_func,
                  int call_extern_first)
{
  char *retval;
  int __call_extern_first = _call_extern_first;
  _call_extern_first = call_extern_first;
  char *tokens = string_dup(spec);
  external_token_func old_func = external_expand_token;
  external_expand_token = ext_func;
  retval = expand_tokens(tokens);
  string_free(tokens);
  external_expand_token = old_func;
  _call_extern_first = __call_extern_first;
  return retval;
}

static A(1)
char *string_function_shell(char *_arg)
{
  #ifndef TVComp_BCPP
  char *arg = expand_tokens(_arg);
  char *retval = NULL;
  char *err_file = open_stderr();
  FILE *pipe = popen(arg,"r");
  if (pipe)
  {
    char buf[1024];
    int count;
    while ((count = fread(buf,1,1023,pipe)))
    {
      char *tmp = buf;
      while (count--)
      {
        if (*tmp == '\n' || *tmp == '\r')
          *tmp = ' ';
        tmp++;
      }
      *tmp = 0;
      string_cat(retval,buf);
    }
    pclose(pipe);
  }
  string_free(arg);
  if (!retval)
    retval = string_dup("");
  else
  {
    int len;
    if (retval[len = strlen(retval)-1] == ' ')
      retval[len] = 0;
  }
  close_stderr();
  remove(err_file);
  return retval;
  #else
  return (char*)"";
  //$todo: implement
  #endif
}

static A(1)
char *string_function_foreach(char *arg)
{
  char *arg1;
  char *arg2;
  char *arg3;
  char *retval = NULL;
  char *tok;
  char *expanded;
  arg3 = find_next_comma(arg);
  if (!arg3)
    return NULL;
  *arg3 = 0;
  arg1 = expand_tokens(arg);
  *arg3 = ',';
  arg3 = find_next_comma(arg2 = (arg3+1));
  if (!arg3)
  {
    string_free(arg1);
    return NULL;
  }
  *arg3 = 0;
  arg2 = expand_tokens(arg2);
  *arg3 = ',';
  arg3++;
  tok = arg2;
  while (1)
  {
    while (*tok && *tok == ' ') tok++;
    if (!*tok) break;
    char *_tok = tok;
    while (*_tok && *_tok != ' ') _tok++;
    char c = *_tok;
    *_tok = 0;
    set_internal_var(arg1,tok);
    expanded = expand_tokens(arg3);
    if (expanded && *expanded)
    {
      if (retval)
        string_cat(retval," ");
      string_cat(retval,expanded);
    }
    string_free(expanded);
    *_tok = c;
    tok = _tok;
  }
  remove_internal_var(arg1);
  string_free(arg1);
  string_free(arg2);
  if (!retval)
    retval = string_dup("");
  return retval;
}

static A(1)
char *string_function_strip(char *arg)
{
  char *retval;
  char *tmp;
  char quote_char = '\'';
  int in_quote = 0;
  retval = expand_tokens(arg);
  tmp = retval;
  while (*tmp && *tmp == ' ') tmp++;
  strcpy(retval,tmp);
  if (!*retval) return retval;
  tmp =retval;
  while (*tmp)
  {
    if (in_quote)
    {
      if (*tmp == quote_char) in_quote = 0;
      tmp++;
      continue;
    }
    if (*tmp == '\'' || *tmp == '"')
    {
      in_quote = 1;
      quote_char = *tmp;
      tmp++;
      continue;
    }
    if (tmp[0] == ' ' && tmp[1] == ' ')
      strcpy(tmp,tmp+1);
    else tmp++;
  }
  tmp = retval + strlen(retval) - 1;
  while (*tmp == ' ') tmp--;
  tmp++;
  *tmp = 0;
  return retval;
}

static int search(char **items, int count, char *key, int &index)
{
  int h = count - 1;
  int res = 0;
  index = 0;
  while( index <= h )
  {
    int i = (index +  h) >> 1;
    int c = strcmp(items[i], key);
    if( c < 0 )
      index = i + 1;
    else
    {
      h = i - 1;
      if( c == 0 )
      {
        res = 1;
        index = i;
      }
    }
  }
  return res;
}

static void add_string(char **&strings, int &strings_count, char *string)
{
  int index=0;
  if (strings_count && search(strings, strings_count, string, index))
    return;
  strings_count++;
  strings = (char **)realloc(strings, strings_count*sizeof(char *));
  memmove(strings+index+1, strings+index, (strings_count-index-1)*sizeof(char *));
  strings[index] = string_dup(string);
}

static A(1)
char *string_function_sort(char *_arg)
{
  char *arg = expand_tokens(_arg);
  char *retval = NULL;
  char *tok;
  char **strings = NULL;
  int strings_count = 0;
  int i;
  for (tok = strtok(arg," ");tok;tok=strtok(NULL," "))
  {
    add_string(strings, strings_count, tok);
  }
  for (i=0;i<strings_count;i++)
  {
    if (retval)
      string_cat(retval," ");
    string_cat(retval,strings[i]);
    string_free(strings[i]);
  }
  if (!retval)
    retval = string_dup("");
  string_free(arg);
  if (strings)
    free(strings);
  return retval;
}

static A(1)
char *string_function_suffix(char *_arg)
{
  char *retval=NULL;
  char *arg = expand_tokens(_arg);
  char *tok;
  for (tok = strtok(arg," ");tok;tok=strtok(NULL," "))
  {
    char *dot = strrchr(tok,'.');
    if (!dot)
      continue;
    if (retval)
      string_cat(retval," ");
    string_cat(retval,dot);
  }
  if (!retval)
    string_dup(retval,"");
  string_free(arg);
  return retval;
}

static A(1)
char *string_function_nosuffix(char *_arg)
{
  char *retval = NULL;
  char *arg = expand_tokens(_arg);
  char *dot = strrchr(arg,'.');
  if (dot)
  {
    char *slash = strrchr(arg, '/');
    if (dot > slash)
      *dot = 0;
  }
  retval = string_dup(arg);
  string_free(arg);
  return retval;
}

static A(1)
char *string_function_setsuffix(char *arg)
{
  char *retval = NULL;
  char *arg1,*arg2,*dot,*tmp;
  tmp = find_next_comma(arg);
  if (!tmp) return NULL;
  *tmp = 0;
  arg1 = expand_tokens(arg);
  *tmp = ',';
  tmp++;
  arg2 = expand_tokens(tmp);
  dot = strrchr(arg2,'.');
  if (dot)
  {
    if (strrchr(arg2,'/') < dot)
      *dot = 0;
  }
  retval = string_dup(arg2);
  string_cat(retval,arg1);
  string_free(arg1);
  string_free(arg1);
  return retval;
}

static A(2)
char *string_function_add_pre_or_suffix(char *arg,int prefix)
{
  char *retval=NULL;
  char *tmp;
  char *arg1,*arg2;
  char *tok;
  tmp = find_next_comma(arg);
  if (!tmp) return NULL;
  *tmp = 0;
  arg1 = expand_tokens(arg);
  *tmp = ',';
  tmp++;
  arg2 = expand_tokens(tmp);
  for (tok = strtok(arg2," ");tok;tok = strtok(NULL," "))
  {
    if (retval) string_cat(retval," ");
    if (prefix)
    {
      string_cat(retval,arg1);
      string_cat(retval,tok);
    }
    else
    {
      string_cat(retval,tok);
      string_cat(retval,arg1);
    }
  }
  string_free(arg1);
  string_free(arg2);
  if (!retval) // arg2 may be consist of only ws's or empty
  {
    string_dup(retval,"");
  }
  return retval;
}

static A(1)
char *string_function_addprefix(char *arg)
{
  return string_function_add_pre_or_suffix(arg,1);
}

static A(1)
char *string_function_addsuffix(char *arg)
{
  return string_function_add_pre_or_suffix(arg,0);
}

static A(1)
char *string_function_notdir(char *_arg)
{
  char *retval;
  char *arg = expand_tokens(_arg);
  BaseName(arg,retval);
  string_free(arg);
  return retval;
}

static A(1)
char *string_function_dir(char *_arg)
{
  char *dir,*name,*ext,*arg = expand_tokens(_arg);
  split_fname(arg,dir,name,ext);
  string_free(name);
  string_free(ext);
  string_free(arg);
  return dir;
}

static A(1)
char *string_function_subst(char *arg)
{
  char *arg1,*arg2,*arg3;
  char *retval=NULL;
  char *tmp,*temp;
  int l_arg1;
  tmp = find_next_comma(arg);
  if (!tmp) return NULL;
  arg3 = find_next_comma(tmp+1);
  if (!arg3) return NULL;
  *tmp = 0;
  arg1 = expand_tokens(arg);
  l_arg1 = strlen(arg1);
  *tmp = ',';
  tmp++;
  *arg3 = 0;
  arg2 = expand_tokens(tmp);
  *arg3 = ',';
  tmp = arg3+1;
  arg3 = expand_tokens(tmp);
  temp = arg3;
  while (l_arg1 && (tmp = strstr(temp,arg1)) != NULL)
  {
    char c = *tmp;
    *tmp = 0;
    string_cat(retval,temp);
    string_cat(retval,arg2);
    *tmp = c;
    temp = tmp + l_arg1;
  }
  if (*temp) string_cat(retval,temp);
  string_free(arg1);
  string_free(arg2);
  string_free(arg3);
  return retval;
}

#ifdef TVOSf_Linux
#include <sys/stat.h>
#endif

static A(1)
char *string_function_wildcard(char *arg)
{
  size_t cnt, i, first = 1;
  glob_t glob_results;
  char *retval = NULL;
  char *wildcard = expand_tokens(arg);
  if (glob(wildcard, GLOB_NOSORT, 0, &glob_results)
      == 0)
  {
    cnt = glob_results.gl_pathc;
    for (i=0; i<cnt; i++)
    {
      #ifdef TVOSf_Linux
      /* glob() on linux returns the filename, if it contains no
      meta characters even when it does not exist */
      struct stat st;
      if (stat(glob_results.gl_pathv[i], &st))
         continue;
      #endif
      if (!first)
        string_cat(retval, " ");
      string_cat(retval, glob_results.gl_pathv[i]);
      first = 0;
    }
    globfree(&glob_results);
  }
  else
    retval = string_dup("");
  string_free(wildcard);
  return retval;
}

static A(1)
char *string_function_word(char *_arg)
{
  int nr;
  char *arg = expand_tokens(_arg);
  char *retval = string_dup("");
  char *arg2 = find_next_comma(arg);
  char *tok;
  if (!arg2)
    goto end;
  if (sscanf(arg, "%d", &nr) != 1)
    goto end;
  if (nr < 1)
    goto end;
  arg2++;
  for (tok = strtok(arg2," ");tok;tok = strtok(NULL," "))
  {
    nr--;
    if (!nr)
    {
      string_cat(retval, tok);
      goto end;
    }
  }
end:
  string_free(arg);
  return retval;
}




