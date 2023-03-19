/*============================================================================
  Copyright (c) 2017-2021 MPEXS Project. All rights reserved.

  This file contains Original Code and/or Modifications of Original Code
  as defined in and that are subject to the MPEXS License (the 'License').
  You may not use this file except in compliance with the License.
  Please obtain a copy of the License at
  https://wiki.kek.jp/display/mpexs/End+User+License+Agreement
  and read it before using this file.

  The Original Code and all software distributed under the License are
  distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
  EXPRESS OR IMPLIED, AND MPEXS HEREBY DISCLAIMS ALL SUCH WARRANTIES,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
  Please see the License for the specific language governing rights and
  limitations under the License.
============================================================================*/
#ifndef __GETOPT_H__
#define __GETOPT_H__

#ifdef __cplusplus
extern "C" {
#endif

extern int opterr;		  /* if error message should be printed */
extern int optind;		  /* index into parent argv vector */
extern int optopt;		  /* character checked for validity */
extern int optreset;		/* reset getopt */
extern char *optarg;		/* argument associated with option */

struct option
{
  const char *name;
  int has_arg;
  int *flag;
  int val;
};

#define no_argument        0
#define required_argument  1
#define optional_argument  2

int getopt(int, char**, const char*);
int getopt_long(int, char**, const char*, const struct option*, int*);

#ifdef __cplusplus
}
#endif

#endif
