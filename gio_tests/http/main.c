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

void
copy_progress_callback (goffset current_num_bytes, goffset total_num_bytes,
                        gpointer user_data)
{
  g_print ("\e[0;31mCopiado %li de %li...\e[0m\n", (long int) current_num_bytes, (long int) total_num_bytes);
}

int
main (int argc, char **argv)
{
  GFile *file, *dest;
  GError *err = NULL;
  gboolean ok;
  GCancellable *cancel;
  GFileInputStream *data;

  g_type_init ();
  cancel = g_cancellable_new ();
  file =
    g_file_new_for_uri ("http://gnu.gabrielfalcao.com/ccgi_without_glib.c");
  if (!file)
    {
      g_printerr ("FUDEU! O Arquivo não abre...\n");
      return 1;
    }
  data = g_file_read (file, cancel, &err);
  if (err)
    {
      g_printerr ("FUDEU DENOVO:: %s\n", err->message);
      exit (2);
    }
  dest = g_file_new_for_uri ("file:///home/gabriel/Desktop/ccgi.c");

  g_cancellable_reset (cancel);
  ok =
    g_file_copy (file, dest, G_FILE_COPY_OVERWRITE, cancel,
                 copy_progress_callback, NULL, &err);

  if (!ok)
    {
      if (err)
        g_printerr ("AE:: %s", err->message);
      else
        g_printerr ("FUdeu mas nao sei pq...\n");
    }
  else
    {
      g_print ("COPIADO COM SUCESSO!\n");
    }

  return 0;
}
