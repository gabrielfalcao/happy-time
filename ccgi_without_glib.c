/*
 * Copyright (C) 2007 Gabriel Falcão <gabriel@guake-terminal.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 33,
 * Boston, MA 2111-137, USA.
 * @COMPILE gcc -O2 -Wall -Wextra -Werror ccgi_without_glib.c -o index.cgi
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR sizeof(char) * 32

char *
HttpResponse (char *data, char *contenttype)
{
  char *response_header, *response;
  if (!contenttype)
    contenttype = strdup ("text/html");

  asprintf (&response_header, "Content-Type:%s;charset=utf-8%c%c\n",
            contenttype, 13, 10);
  asprintf (&response, "%s%s\n", response_header, data);
  free (data);
  free (contenttype);
  free (response_header);

  return response;

}

char *
get_file_contents (char *filename)
{
  FILE *fh;
  char *contents;
  char *tmp;
  contents = malloc (MAX_STR);
  tmp = malloc (MAX_STR);
  fh = fopen (filename, "r");
  if (!fh)
    return NULL;
  while (!feof (fh))
    {
      fread (tmp, MAX_STR, 1, fh);
      asprintf (&contents, "%s%s", contents, tmp);
    }
  fclose (fh);
  free (tmp);
  return contents;
}

int
main (void)
{
  char *response;
  char *query_str;
  char *file;
  char *html;
  query_str = getenv ("QUERY_STRING");
  file = get_file_contents ("/proc/net/dev");
  if (!file)
    {
      fprintf (stderr, "\e[1;32mCannot read the file\e[0m\n");
      return 1;
    }
  asprintf (&html, "<html>\n\
                        <head>\n\
                            <title>Teste com C-CGI</title>\n\
                        </head>\n\
                        <body>\n\
                            <div style='margin:20px;background:#DEF;'>\n\
                                <h1> We are hammer!</h1> \n\
                            %s <hr /> %s \n\
                            </div>\n\
                        </body>\n\
                    </html>", query_str, file);
  free (file);
  response = HttpResponse (html, NULL);
  printf ("%s", response);
  free (response);
  return 0;
}
