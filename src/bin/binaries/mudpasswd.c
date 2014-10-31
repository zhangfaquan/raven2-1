/* ************************************************************************
*  file:  passwd.c                                    Part of CircleMud   *
*  Usage: changing passwords of chars in a Diku playerifle                *
*  Copyright (C) 1990, 1991 - see 'license.doc' for complete information. *
*  All Rights Reserved                                                    *
************************************************************************* */

#include "general/conf.h"
#include "general/sysdep.h"

#include "general/db.h"

#define FALSE 0
#define TRUE 1

/* defines for fseek */
#ifndef SEEK_SET
#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2
#endif

int	str_eq(char *s, char *t)
{
   for (; ; ) {
      if (*s == 0 && *t == 0) 
	 return TRUE;
      if (tolower(*s) != tolower(*t)) 
	 return FALSE;
      s++;
      t++;
   }
}


void	pword(char *filename, char *name, char *password)
{
   FILE * fl;
   struct char_file_u buf;
   int	found = FALSE;
   extern char	*crypt();

   if (!(fl = fopen(filename, "r+"))) {
      perror(filename);
      exit(1);
   }

   for (; ; ) {
      fread(&buf, sizeof(buf), 1, fl);
      if (feof(fl))
	 break;

      if (str_eq(name, buf.name)) {
	 found = TRUE;
	 strncpy(buf.pwd, crypt(password, buf.name), MAX_PWD_LENGTH);
	 if (fseek(fl, -1L * sizeof(buf), SEEK_CUR) != 0) 
	    perror("fseek");
	 if (fwrite(&buf, sizeof(buf), 1, fl) != 1) 
	    perror("fwrite");
	 if (fseek(fl, 0L, SEEK_CUR) != 0) 
	    perror("fseek");
      }
   }

   if (found) {
      printf("%s password is now %s\n", name, password);
   } else {
      printf("%s not found\n", name);
   }

   fclose(fl);
}


int
main(int argc, char **argv)
{
   if (argc != 4)
      fprintf(stderr, "Usage: %s playerfile character-name new-password\n", argv[0]);
   else
      pword(argv[1], argv[2], argv[3]);

   return(0);

}

