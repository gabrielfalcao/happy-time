/**
 * Copyright (C) 2008 Gabriel Falcão <gabriel@nacaolivre.org>
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>
int
main(void)
{
  g_type_init ();
  GFile *file;
  GFileOutputStream *ostream;
  GError *error = NULL;
  const gchar *names[] = {"GNU", "Debian", "Gnome", "Glib", "GObject"};
  int NELEMENTS = G_N_ELEMENTS (names);
  int j;
  file = g_file_new_for_path ("./teste.txt");
  ostream = g_file_create (file, G_FILE_CREATE_NONE, NULL, &error);
  if (error)
    g_error ("Fudeu: %s", error->message);
  
  for (j=0;j<NELEMENTS;j++)
    {
      g_output_stream_write (G_OUTPUT_STREAM(ostream), names[j], 3, NULL, &error);
      if (error)
        {
          g_warning ("Error writing: %s", error->message);
          g_error_free (error);
          break;
        }
    }

  
  
  return 0;

}
