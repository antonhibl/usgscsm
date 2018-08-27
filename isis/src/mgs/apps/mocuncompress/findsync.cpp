/*
NOTICE

The software accompanying this notice (the "Software") is provided to you
free of charge to facilitate your use of the data collected by the Mars
Orbiter Camera (the "MOC Data").  Malin Space Science Systems ("MSSS")
grants to you (either as an individual or entity) a personal,
non-transferable, and non-exclusive right (i) to use and reproduce the
Software solely for the purpose of accessing the MOC Data; (ii) to modify
the source code of the Software as necessary to maintain or adapt the
Software to run on alternate computer platforms; and (iii) to compile, use
and reproduce the modified versions of the Software solely for the purpose
of accessing the MOC Data.  In addition, you may distribute the Software,
including any modifications thereof, solely for use with the MOC Data,
provided that (i) you must include this notice with all copies of the
Software to be distributed; (ii) you may not remove or alter any
proprietary notices contained in the Software; (iii) you may not charge any
third party for the Software; and (iv) you will not export the Software
without the appropriate United States and foreign government licenses.

You acknowledge that no title to the intellectual property in the Software
is transferred to you.  You further acknowledge that title and full
ownership rights to the Software will remain the exclusive property of MSSS
or its suppliers, and you will not acquire any rights to the Software
except as expressly set forth above.  The Software is provided to you AS
IS.  MSSS MAKES NO WARRANTY, EXPRESS OR IMPLIED, WITH RESPECT TO THE
SOFTWARE, AND SPECIFICALLY DISCLAIMS THE IMPLIED WARRANTIES OF
NON-INFRINGEMENT OF THIRD PARTY RIGHTS, MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE.  SOME JURISDICTIONS DO NOT ALLOW THE EXCLUSION OR
LIMITATION OF INCIDENTAL OR CONSEQUENTIAL DAMAGES, SO SUCH LIMITATIONS OR
EXCLUSIONS MAY NOT APPLY TO YOU.

Your use or reproduction of the Software constitutes your agreement to the
terms of this Notice.  If you do not agree with the terms of this notice,
promptly return or destroy all copies of the Software in your possession.

Copyright (C) 1999 Malin Space Science Systems.  All Rights Reserved.
*/
//static char *sccsid = "@(#)findsync.c  1.1 10/04/99";
#include "fs.h"

static int maxdelta = 64;

static int delta_ok(uint8 *p)
{
  int i, delta, md = 0;

  for(i = 1; i < 32; i++) {
    delta = p[i] - p[i+1];
    if(delta < 0) delta = -delta;
    if(delta > md) md = delta;
  }
  return md <= maxdelta;
}

/* locate the first sync line following the location pointed to by p.
   Note that a valid sync line must start with the 16-bit sync pattern
   and that the uncompressed pixels following must pass a test based
   on the maximum delta value pixel-to-pixel, which is forced to be <=
   maxdelta. */
uint8 *findsync(uint8 *p, int len, uint16 sync)
{
  uint16 s;

  p += 2; /* skip the previous sync value and search forward for the next */
  len -= 2;
  while(len--) {
    s = *p | (*(p + 1) << 8);
    if(s == sync && delta_ok(p + 2)) {
      return p;
    }
    else p += 1;
  }
  return 0;
}