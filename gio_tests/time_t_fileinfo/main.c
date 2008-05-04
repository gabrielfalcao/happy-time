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
#include <time.h>
#include <glib.h>
#include <gio/gio.h>

/*int*/
main (int argc, char **argv)
{
  GFile *file;
  GFileInfo *info;
  GCancellable *cancel;
  time_t time;
  GError *error = NULL;
  g_type_init();
  cancel = g_cancellable_new ();
  file = g_file_new_for_uri ("file:///home/gabriel/Imagens/jazzradio1.jpg");
  info = g_file_query_info (file, "standard::*", G_FILE_QUERY_INFO_NONE,
      cancel, &error);
  guint32 xtime;

  xtime = g_file_info_get_attribute_uint32 (info, "time::created-usec");
  GDate *date;
  date  = g_date_new_julian (xtime);
  g_print ("Dia: %d\n=========", (int) g_date_get_month(date));
  if (error)
    g_printerr ("FUDEU: %s\n", error->message);

  g_print ("O tamanho do arquivo eh: %d", (int) g_file_info_get_size (info));

  return 0;
}
