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
#include <glib.h>
#include <gio/gio.h>

int
main (int argc, char **argv)
{
  GFile *dir;
  GFileEnumerator *fenum;
  GFileInfo *info;
  GError *error = NULL;
  g_type_init();
  dir = g_file_new_for_uri ("file:///home/gabriel/Fotos/Festa%20D%C3%A9ia");
  fenum = g_file_enumerate_children           (dir,
      "*",
      G_FILE_QUERY_INFO_NONE,
      NULL,
      &error);

  if (error)
    g_printerr ("FUDEU: %s\n", error->message);

  while ((info = g_file_enumerator_next_file(fenum, NULL, NULL))){
    g_print ("LISTANDO: %s\n", g_file_info_get_name (info));
    g_object_unref (info);
  }

  return 0;
}
