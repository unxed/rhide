/* Copyright (C) 1996-1998 Robert H�hne, see COPYING.RH for details */
/* This file is part of RHIDE. */
#include <rhutils.h>
#include <ctype.h>
#define uctolower(a)  tolower((unsigned char)a)
#define uctoupper(a)  toupper((unsigned char)a)

char *string_down(char * dest)
{
  if (!dest) return dest;
  while (*dest) { *dest = uctolower(*dest); dest++; }
  return dest;
}

char *string_up(char * dest)
{
  if (!dest) return dest;
  while (*dest) { *dest = uctoupper(*dest); dest++; }
  return dest;
}


