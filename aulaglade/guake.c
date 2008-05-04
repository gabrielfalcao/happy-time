/*
 * Copyright (C) 2007 Gabriel Falcão <root@gabrielfalcao.com>
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#define GUAKE_GLADE_FILE "glade/guake.glade"
#define PREFS_GLADE_FILE "glade/prefs.glade"
#define ABOUT_GLADE_FILE "glade/about.glade"

/*void
some_signal_handler_func (GtkWidget * widget, gpointer user_data)
{
  // do something useful here
}*/

int
main (int argc, char *argv[])
{
  GladeXML *xml;

  gtk_init (&argc, &argv);
  xml = glade_xml_new (GUAKE_GLADE_FILE, NULL, NULL);
  glade_xml_signal_autoconnect (xml);

  gtk_main ();

  return 0;
}
