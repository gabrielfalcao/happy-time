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
#include <dbus/dbus-glib-bindings.h>
int
main (void)
{
  DBusGConnection *connection;
  GError *error;
  DBusGProxy *proxy;
  char **name_list;
  char **name_list_ptr;

  g_type_init ();

  error = NULL;
  connection = dbus_g_bus_get (DBUS_BUS_SESSION, &error);
  if (connection == NULL)
    {
      g_printerr ("Failed to open connection to bus: %s\n", error->message);
      g_error_free (error);
      exit (1);
    }

  /* Create a proxy object for the "bus driver" (name "org.freedesktop.DBus") */

  proxy = dbus_g_proxy_new_for_name (connection,
                                     "org.gnome.ScreenSaver",
                                     "/org/gnome/ScreenSaver", "org.gnome.ScreenSaver");
  /* Call ListNames method, wait for reply */
  error = NULL;
  if (!dbus_g_proxy_call (proxy, "Lock", &error, G_TYPE_INVALID,
                          G_TYPE_STRV, &name_list, G_TYPE_INVALID))
    {
      /* Just do demonstrate remote exceptions versus regular GError */
      if (error->domain == DBUS_GERROR
          && error->code == DBUS_GERROR_REMOTE_EXCEPTION)
        g_printerr ("Caught remote method exception %s: %s",
                    dbus_g_error_get_name (error), error->message);
      else
        g_printerr ("Error: %s\n", error->message);
      g_error_free (error);
      exit (1);
    }

  /* Print the results */

  g_print ("Names on the message bus:\n");

  for (name_list_ptr = name_list; *name_list_ptr; name_list_ptr++)
    {
      g_print ("  %s\n", *name_list_ptr);
    }
  g_strfreev (name_list);

  g_object_unref (proxy);

  return 0;
}
